#include "TeddyPch.h"
#include "Teddy/Renderer/Renderer2D.h"

#include "Teddy/Renderer/VertexArray.h"
#include "Teddy/Renderer/Shader.h"
#include "Teddy/Renderer/RenderCommand.h"

#include "Teddy/Renderer/UniformBuffer.h"

#include "Teddy/Renderer/MSDFData.h"
#include "Teddy/Core/AssetManager.h"

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtc/type_ptr.hpp>

// TODO: For 3D implement instancing instead of batching
namespace Teddy 
{

	struct LineVertex
	{
		glm::vec3 Position;
		glm::vec4 Color;

		// Editor-only
		int EntityID;
	};

	struct QuadVertex
	{
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 TexCoord;
		float TexIndex;
		float TilingFactor;

		// Editor-only
		int EntityID;
	};

	struct CircleVertex
	{
		glm::vec3 WorldPosition;
		glm::vec3 LocalPosition;
		glm::vec4 Color;
		float Thickness;
		float Fade;

		// Editor-only
		int EntityID;
	};

	struct TextVertex
	{
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 TexCoord;

		glm::vec4 OutlineColor;
		float OutlineThickness;
		float AtlasIndex;

		// Editor-only
		int EntityID;
	};

	struct VertexResource
	{
		Ref<VertexArray> VertexArray;
		Ref<VertexBuffer> VertexBuffer;
	};

	struct RenderResource : VertexResource
	{
		Ref<Shader> Shader;
	};

	template<typename T>
	struct BashRenderResource : RenderResource
	{
		uint32_t IndexCount = 0;
		T* VertexBufferBase = nullptr;
		T* VertexBufferPtr = nullptr;
		std::string Name = "";
	};

	template<typename T>
	struct BashVertexResource : VertexResource
	{
		uint32_t IndexCount = 0;
		T* VertexBufferBase = nullptr;
		T* VertexBufferPtr = nullptr;
		std::string Name = "";
	};

	struct Renderer2DData
	{ 
		static const uint32_t MaxQuads = 20000;
		static const uint32_t MaxLines = 20000;
		static const uint32_t MaxVertices = MaxQuads * 4;
		static const uint32_t MaxIndices = MaxQuads * 6;
		static const uint32_t MaxLineIndices = MaxLines * 2;
		static const uint32_t MaxTextureSlots = 32;

		uint32_t TextureSlotsCapacity = 0;

		Ref<Texture2D> WhiteTexture;

		BashRenderResource<LineVertex> LineResources;
		BashRenderResource<QuadVertex> QuadResources;
		BashRenderResource<CircleVertex> CircleResources;
		BashRenderResource<TextVertex> TextResources;
		BashVertexResource<CircleVertex> CircleLineResources;

		float LineWidth = 2.0f;

		std::array<Weak<Texture2D>, MaxTextureSlots> TextureSlots;
		uint32_t TextureSlotIndex = 1; // 0 is reserved for white texture

		std::array<Weak<Texture2D>, MaxTextureSlots> FontAtlasSlots;
		uint32_t FontAtlasSlotIndex = 0;

		glm::vec4 QuadVertexPositions[4];

		Renderer2D::Statistics Stats;

		struct CameraData
		{
			glm::mat4 ViewProjection;
		};

		CameraData CameraBuffer;
		Ref<UniformBuffer> CameraUniformBuffer;
	};

	static Renderer2DData s_Data;

	Teddy::Renderer2D::ShaderType Teddy::Renderer2D::m_LastShaderDrawn = Teddy::Renderer2D::ShaderType::None;

	void Renderer2D::Init()
	{
		TED_PROFILE_CAT(InstrumentorCategory::Rendering);
		// Quad
		s_Data.QuadResources.VertexArray = VertexArray::Create();

		s_Data.QuadResources.VertexBuffer = VertexBuffer::Create(s_Data.MaxVertices * sizeof(QuadVertex));
		s_Data.QuadResources.VertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float4, "a_Color" },
			{ ShaderDataType::Float2, "a_TexCoord" },
			{ ShaderDataType::Float, "a_TexIndex" },
			{ ShaderDataType::Float, "a_TilingFactor" },
			{ ShaderDataType::Float, "a_EntityID" }
			});
		s_Data.QuadResources.VertexArray->AddVertexBuffer(s_Data.QuadResources.VertexBuffer);

		s_Data.QuadResources.VertexBufferBase = new QuadVertex[s_Data.MaxVertices];

		uint32_t* quadIndices = new uint32_t[s_Data.MaxIndices];

		uint32_t offset = 0;
		for (uint32_t i = 0; i < s_Data.MaxIndices; i += 6)
		{
			quadIndices[i + 0] = offset + 0;
			quadIndices[i + 1] = offset + 1;
			quadIndices[i + 2] = offset + 2;

			quadIndices[i + 3] = offset + 2;
			quadIndices[i + 4] = offset + 3;
			quadIndices[i + 5] = offset + 0;

			offset += 4;
		}

		Ref<IndexBuffer> quadIB = IndexBuffer::Create(quadIndices, s_Data.MaxIndices);
		s_Data.QuadResources.VertexArray->SetIndexBuffer(quadIB);
		delete[] quadIndices;

		// Circle
		s_Data.CircleResources.VertexArray = VertexArray::Create();
		s_Data.CircleResources.VertexBuffer = VertexBuffer::Create(s_Data.MaxVertices * sizeof(CircleVertex));

		s_Data.CircleResources.VertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "a_WorldPosition" },
			{ ShaderDataType::Float3, "a_LocalPosition" },
			{ ShaderDataType::Float4, "a_Color"         },
			{ ShaderDataType::Float,  "a_Thickness"     },
			{ ShaderDataType::Float,  "a_Fade"          },
			{ ShaderDataType::Int,    "a_EntityID"      }
			});

		s_Data.CircleResources.VertexArray->AddVertexBuffer(s_Data.CircleResources.VertexBuffer);
		s_Data.CircleResources.VertexArray->SetIndexBuffer(quadIB); // Use quad IB
		s_Data.CircleResources.VertexBufferBase = new CircleVertex[s_Data.MaxVertices];

		// Circle Line
		s_Data.CircleLineResources.VertexArray = VertexArray::Create();
		s_Data.CircleLineResources.VertexBuffer = VertexBuffer::Create(s_Data.MaxVertices * sizeof(CircleVertex));

		s_Data.CircleLineResources.VertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "a_WorldPosition" },
			{ ShaderDataType::Float3, "a_LocalPosition" },
			{ ShaderDataType::Float4, "a_Color"         },
			{ ShaderDataType::Float,  "a_Thickness"     },
			{ ShaderDataType::Float,  "a_Fade"          },
			{ ShaderDataType::Int,    "a_EntityID"      }
			});

		s_Data.CircleLineResources.VertexArray->AddVertexBuffer(s_Data.CircleLineResources.VertexBuffer);
		s_Data.CircleLineResources.VertexArray->SetIndexBuffer(quadIB); // Use quad IB
		s_Data.CircleLineResources.VertexBufferBase = new CircleVertex[s_Data.MaxVertices];

		// Line
		s_Data.LineResources.VertexArray = VertexArray::Create();
		s_Data.LineResources.VertexBuffer = VertexBuffer::Create(s_Data.MaxVertices * sizeof(LineVertex));

		s_Data.LineResources.VertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "a_Position"		},
			{ ShaderDataType::Float4, "a_Color"         },
			{ ShaderDataType::Int,    "a_EntityID"      }
			});

		s_Data.LineResources.VertexArray->AddVertexBuffer(s_Data.LineResources.VertexBuffer);
		s_Data.LineResources.VertexArray->SetIndexBuffer(quadIB); // Use quad IB
		s_Data.LineResources.VertexBufferBase = new LineVertex[s_Data.MaxVertices];

		// Text
		s_Data.TextResources.VertexArray = VertexArray::Create();

		s_Data.TextResources.VertexBuffer = VertexBuffer::Create(s_Data.MaxVertices * sizeof(TextVertex));
		s_Data.TextResources.VertexBuffer->SetLayout({
			{ ShaderDataType::Float3,	"a_Position"		},
			{ ShaderDataType::Float4,	"a_Color"			},
			{ ShaderDataType::Float2,	"a_TexCoord"		},
			{ ShaderDataType::Float4,	"a_OutlineColor"	},
			{ ShaderDataType::Float,	"a_OutlineThickness"},
			{ ShaderDataType::Float,	"a_AtlasIndex"		},
			{ ShaderDataType::Int,		"a_EntityID"		}
			});

		s_Data.TextResources.VertexArray->AddVertexBuffer(s_Data.TextResources.VertexBuffer);
		s_Data.TextResources.VertexArray->SetIndexBuffer(quadIB);
		s_Data.TextResources.VertexBufferBase = new TextVertex[s_Data.MaxVertices];

		// White texture
		uint32_t whiteTextureData = 0xffffffff;
		s_Data.WhiteTexture = AssetManager::Get().Load<Texture2D>(whiteTextureData, TextureSpecification());
		s_Data.WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

		// TODO: API abscraction
		GLint maxFragmentUnits = 0;
		glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &maxFragmentUnits);
		s_Data.TextureSlotsCapacity = std::min<uint32_t>((uint32_t)maxFragmentUnits, Renderer2DData::MaxTextureSlots);

		// Adding shaders
		auto& assets = AssetManager::Get();
		
		s_Data.QuadResources.Name = "Quad Resources";
		s_Data.QuadResources.Shader = assets.Load<Shader>(s_Data.QuadResources.Name, "../Teddy/assets/shaders/Renderer2D_Quad.glsl");
		s_Data.CircleResources.Name = "Circle Shader";
		s_Data.CircleResources.Shader = assets.Load<Shader>(s_Data.CircleResources.Name, "../Teddy/assets/shaders/Renderer2D_Circle.glsl");
		s_Data.LineResources.Name = "Line Shader";
		s_Data.LineResources.Shader = assets.Load<Shader>(s_Data.LineResources.Name, "../Teddy/assets/shaders/Renderer2D_Line.glsl");
		s_Data.TextResources.Name = "Text Shader";
		s_Data.TextResources.Shader = assets.Load<Shader>(s_Data.TextResources.Name, "../Teddy/assets/shaders/Renderer2D_Text.glsl");

		// Set all texture slots to 0
		s_Data.TextureSlots[0] = s_Data.WhiteTexture;

		s_Data.QuadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f};
		s_Data.QuadVertexPositions[1] = {  0.5f, -0.5f, 0.0f, 1.0f };
		s_Data.QuadVertexPositions[2] = {  0.5f, 0.5f, 0.0f, 1.0f };
		s_Data.QuadVertexPositions[3] = { -0.5f, 0.5f, 0.0f, 1.0f };

		s_Data.CameraUniformBuffer = UniformBuffer::Create(sizeof(Renderer2DData::CameraData), 0);
	}

	void Renderer2D::Shutdown()
	{
		TED_PROFILE_CAT(InstrumentorCategory::Rendering);

		delete[] s_Data.QuadResources.VertexBufferBase;
	}

	void Renderer2D::BeginScene(const Camera& camera, const glm::mat4& transform)
	{
		TED_PROFILE_CAT(InstrumentorCategory::Rendering);

		s_Data.CameraBuffer.ViewProjection = camera.GetProjection() * glm::inverse(transform);
		s_Data.CameraUniformBuffer->SetData(&s_Data.CameraBuffer, sizeof(Renderer2DData::CameraData));

		StartBatch();
	}

	template<typename T>
	void Renderer2D::ReloadShader(BashRenderResource<T>& resource)
	{
		auto& assets = AssetManager::Get();

		std::string shaderPath = assets.AssetNeedsToReload<Shader>(resource.Shader->GetPath(), true);

		if (shaderPath != "")
		{
			resource.Shader = nullptr;
			assets.RemoveExpired<Shader>(resource.Name);
			resource.Shader = assets.Load<Shader>(resource.Name, shaderPath, true);
		}
	}

	void Renderer2D::BeginScene(const EditorCamera& camera)
	{
		TED_PROFILE_CAT(InstrumentorCategory::Rendering);

		s_Data.CameraBuffer.ViewProjection = camera.GetViewProjection();
		s_Data.CameraUniformBuffer->SetData(&s_Data.CameraBuffer, sizeof(Renderer2DData::CameraData));

		if (AssetManager::Get().IsHotReloading<Shader>())
		{
			ReloadShader<QuadVertex>(s_Data.QuadResources);
			ReloadShader<CircleVertex>(s_Data.CircleResources);
			ReloadShader<LineVertex>(s_Data.LineResources);
			ReloadShader<TextVertex>(s_Data.TextResources);
		}

		StartBatch();
	}

	void Renderer2D::EndScene()
	{
		TED_PROFILE_CAT(InstrumentorCategory::Rendering);

		Flush();
	}

	void Renderer2D::StartBatch()
	{
		TED_PROFILE_CAT(InstrumentorCategory::Rendering);

		s_Data.QuadResources.IndexCount = 0;
		s_Data.QuadResources.VertexBufferPtr = s_Data.QuadResources.VertexBufferBase;

		s_Data.CircleResources.IndexCount = 0;
		s_Data.CircleResources.VertexBufferPtr = s_Data.CircleResources.VertexBufferBase;

		s_Data.TextResources.IndexCount = 0;
		s_Data.TextResources.VertexBufferPtr = s_Data.TextResources.VertexBufferBase;

		s_Data.LineResources.IndexCount = 0;
		s_Data.LineResources.VertexBufferPtr = s_Data.LineResources.VertexBufferBase;

		s_Data.CircleLineResources.IndexCount = 0;
		s_Data.CircleLineResources.VertexBufferPtr = s_Data.CircleLineResources.VertexBufferBase;

		s_Data.TextureSlotIndex = 1;
		s_Data.FontAtlasSlotIndex = 0;

		// DEBUG: Uncomment this if for some reason you change from weak to ref s_Data.TextureSlots/s_Data.FontAtlasSlots
		//for (uint32_t i = 1; i < Renderer2DData::MaxTextureSlots; ++i)
		//	s_Data.TextureSlots[i].reset();
		//for (uint32_t i = 0; i < Renderer2DData::MaxFontSlots; ++i)
		//	s_Data.FontAtlasSlots[i].reset();
	}

	void Renderer2D::Flush()
	{
		TED_PROFILE_CAT(InstrumentorCategory::Rendering);

		if (s_Data.QuadResources.IndexCount)
		{
			uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.QuadResources.VertexBufferPtr - (uint8_t*)s_Data.QuadResources.VertexBufferBase);
			s_Data.QuadResources.VertexBuffer->SetData(s_Data.QuadResources.VertexBufferBase, dataSize);

			for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++)
				s_Data.TextureSlots[i].lock()->Bind(i);

			s_Data.QuadResources.Shader->Bind();
			RenderCommand::DrawIndexed(s_Data.QuadResources.VertexArray, s_Data.QuadResources.IndexCount);

			s_Data.Stats.DrawCalls++;
		}

		if (s_Data.CircleResources.IndexCount)
		{
			uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.CircleResources.VertexBufferPtr - (uint8_t*)s_Data.CircleResources.VertexBufferBase);
			s_Data.CircleResources.VertexBuffer->SetData(s_Data.CircleResources.VertexBufferBase, dataSize);

			s_Data.CircleResources.Shader->Bind();
			RenderCommand::DrawIndexed(s_Data.CircleResources.VertexArray, s_Data.CircleResources.IndexCount);
			s_Data.Stats.DrawCalls++;
		}

		if (s_Data.TextResources.IndexCount)
		{
			uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.TextResources.VertexBufferPtr - (uint8_t*)s_Data.TextResources.VertexBufferBase);
			s_Data.TextResources.VertexBuffer->SetData(s_Data.TextResources.VertexBufferBase, dataSize);

			for (uint32_t i = 0; i < s_Data.FontAtlasSlotIndex; i++)
				s_Data.FontAtlasSlots[i].lock()->Bind(i);

			s_Data.TextResources.Shader->Bind();
			RenderCommand::DrawIndexed(s_Data.TextResources.VertexArray, s_Data.TextResources.IndexCount);
			s_Data.Stats.DrawCalls++;
		}

		if (s_Data.CircleLineResources.IndexCount)
		{
			uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.CircleLineResources.VertexBufferPtr - (uint8_t*)s_Data.CircleLineResources.VertexBufferBase);
			s_Data.CircleLineResources.VertexBuffer->SetData(s_Data.CircleLineResources.VertexBufferBase, dataSize);

			s_Data.CircleResources.Shader->Bind();
			RenderCommand::DisableDepth();
			RenderCommand::DrawIndexed(s_Data.CircleLineResources.VertexArray, s_Data.CircleLineResources.IndexCount);
			RenderCommand::EnableDepth();
			s_Data.Stats.DrawCalls++;
		}

		if (s_Data.LineResources.IndexCount)
		{
			uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.LineResources.VertexBufferPtr - (uint8_t*)s_Data.LineResources.VertexBufferBase);
			s_Data.LineResources.VertexBuffer->SetData(s_Data.LineResources.VertexBufferBase, dataSize);

			s_Data.LineResources.Shader->Bind();
			RenderCommand::SetLineWidth(s_Data.LineWidth);
			RenderCommand::DrawLines(s_Data.LineResources.VertexArray, s_Data.LineResources.IndexCount);
			s_Data.Stats.DrawCalls++;
		}
	}

	void Renderer2D::NextBatch()
	{
		Flush();
		StartBatch();
	}

	void Renderer2D::SetQuad(const glm::mat4& transform, const glm::vec4& color,
		const float& texIndex, const float& tilingFactor, 
		const SpriteAtlasComponent& atlas, const float& atlasWidth, const float& atlasHeight,
		int entityID)
	{
		TED_PROFILE_CAT(InstrumentorCategory::Rendering);

		if (m_LastShaderDrawn != ShaderType::Quad)
		{
			if (m_LastShaderDrawn != ShaderType::None)
				Flush();
			m_LastShaderDrawn = ShaderType::Quad;
		}

		constexpr size_t quadVertexCount = 4;
		glm::vec2 textureCoords[] = {
			{ (atlas.X * atlas.SpriteWidth) / atlasWidth,		(atlas.Y * atlas.SpriteHeight) / atlasHeight },
			{ ((atlas.X + 1) * atlas.SpriteWidth) / atlasWidth,	(atlas.Y * atlas.SpriteHeight) / atlasHeight },
			{ ((atlas.X + 1) * atlas.SpriteWidth) / atlasWidth,	((atlas.Y + 1) * atlas.SpriteHeight) / atlasHeight },
			{ (atlas.X * atlas.SpriteWidth) / atlasWidth,		((atlas.Y + 1) * atlas.SpriteHeight) / atlasHeight }
		};

		for (size_t i = 0; i < quadVertexCount; i++)
		{
			s_Data.QuadResources.VertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[i];
			s_Data.QuadResources.VertexBufferPtr->Color = color;
			s_Data.QuadResources.VertexBufferPtr->TexCoord = textureCoords[i];
			s_Data.QuadResources.VertexBufferPtr->TexIndex = texIndex;
			s_Data.QuadResources.VertexBufferPtr->TilingFactor = tilingFactor;
			s_Data.QuadResources.VertexBufferPtr->EntityID = entityID;
			s_Data.QuadResources.VertexBufferPtr++;
		}

		s_Data.QuadResources.IndexCount += 6;

		s_Data.Stats.QuadCount++;
	}

	void Renderer2D::SetQuad(const glm::mat4& transform, const glm::vec4& color,
		const float& texIndex, const float& tilingFactor, int entityID)
	{
		TED_PROFILE_CAT(InstrumentorCategory::Rendering);

		if (m_LastShaderDrawn != ShaderType::Quad)
		{
			if (m_LastShaderDrawn != ShaderType::None)
				Flush();
			m_LastShaderDrawn = ShaderType::Quad;
		}

		constexpr size_t quadVertexCount = 4;
		constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

		for (size_t i = 0; i < quadVertexCount; i++)
		{
			s_Data.QuadResources.VertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[i];
			s_Data.QuadResources.VertexBufferPtr->Color = color;
			s_Data.QuadResources.VertexBufferPtr->TexCoord = textureCoords[i];
			s_Data.QuadResources.VertexBufferPtr->TexIndex = texIndex;
			s_Data.QuadResources.VertexBufferPtr->TilingFactor = tilingFactor;
			s_Data.QuadResources.VertexBufferPtr->EntityID = entityID;
			s_Data.QuadResources.VertexBufferPtr++;
		}

		s_Data.QuadResources.IndexCount += 6;

		s_Data.Stats.QuadCount++;
	}

	void Renderer2D::DrawLine(const glm::vec3& p0, glm::vec3& p1, const glm::vec4& color, int entityID)
	{
		TED_PROFILE_CAT(InstrumentorCategory::Rendering);

		if (s_Data.LineResources.IndexCount >= Renderer2DData::MaxLineIndices)
			NextBatch();

		s_Data.LineResources.VertexBufferPtr->Position = p0;
		s_Data.LineResources.VertexBufferPtr->Color = color;
		s_Data.LineResources.VertexBufferPtr->EntityID = entityID;
		s_Data.LineResources.VertexBufferPtr++;

		s_Data.LineResources.VertexBufferPtr->Position = p1;
		s_Data.LineResources.VertexBufferPtr->Color = color;
		s_Data.LineResources.VertexBufferPtr->EntityID = entityID;
		s_Data.LineResources.VertexBufferPtr++;

		s_Data.LineResources.IndexCount += 2;
	}

	void Renderer2D::DrawRect(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, int entityID)
	{
		TED_PROFILE_CAT(InstrumentorCategory::Rendering);

		glm::vec3 p0 = glm::vec3(position.x - size.x * 0.5f, position.y - size.y * 0.5f, position.z);
		glm::vec3 p1 = glm::vec3(position.x + size.x * 0.5f, position.y - size.y * 0.5f, position.z);
		glm::vec3 p2 = glm::vec3(position.x + size.x * 0.5f, position.y + size.y * 0.5f, position.z);
		glm::vec3 p3 = glm::vec3(position.x - size.x * 0.5f, position.y + size.y * 0.5f, position.z);

		DrawLine(p0, p1, color, entityID);
		DrawLine(p1, p2, color, entityID);
		DrawLine(p2, p3, color, entityID);
		DrawLine(p3, p0, color, entityID);
	}

	void Renderer2D::DrawRect(const glm::mat4& transform, const glm::vec4& color, int entityID)
	{
		TED_PROFILE_CAT(InstrumentorCategory::Rendering);

		glm::vec3 lineVertices[4];
		for (size_t i = 0; i < 4; i++)
			lineVertices[i] = transform * s_Data.QuadVertexPositions[i];

		DrawLine(lineVertices[0], lineVertices[1], color, entityID);
		DrawLine(lineVertices[1], lineVertices[2], color, entityID);
		DrawLine(lineVertices[2], lineVertices[3], color, entityID);
		DrawLine(lineVertices[3], lineVertices[0], color, entityID);
	}

	void Renderer2D::DrawCircle(const glm::mat4& transform, const glm::vec4& color, float thickness /*= 1.0f*/, float fade /*= 0.005f*/, int entityID /*= -1*/)
	{
		TED_PROFILE_CAT(InstrumentorCategory::Rendering);

		if (m_LastShaderDrawn != ShaderType::Circle)
		{
			if (m_LastShaderDrawn != ShaderType::None)
				Flush();
			m_LastShaderDrawn = ShaderType::Circle;
		}

		if (s_Data.CircleResources.IndexCount >= Renderer2DData::MaxIndices)
			NextBatch();

		for (size_t i = 0; i < 4; i++)
		{
			s_Data.CircleResources.VertexBufferPtr->WorldPosition = transform * s_Data.QuadVertexPositions[i];
			s_Data.CircleResources.VertexBufferPtr->LocalPosition = s_Data.QuadVertexPositions[i] * 2.0f;
			s_Data.CircleResources.VertexBufferPtr->Color = color;
			s_Data.CircleResources.VertexBufferPtr->Thickness = thickness;
			s_Data.CircleResources.VertexBufferPtr->Fade = fade;
			s_Data.CircleResources.VertexBufferPtr->EntityID = entityID;
			s_Data.CircleResources.VertexBufferPtr++;
		}
		s_Data.CircleResources.IndexCount += 6;

		s_Data.Stats.QuadCount++;
	}

	void Renderer2D::DrawCircleLine(const glm::mat4& transform, const glm::vec4& color, float thickness /*= 1.0f*/, float fade /*= 0.005f*/, int entityID /*= -1*/)
	{
		TED_PROFILE_CAT(InstrumentorCategory::Rendering);

		if (s_Data.CircleLineResources.IndexCount >= Renderer2DData::MaxIndices)
			NextBatch();

		for (size_t i = 0; i < 4; i++)
		{
			s_Data.CircleLineResources.VertexBufferPtr->WorldPosition = transform * s_Data.QuadVertexPositions[i];
			s_Data.CircleLineResources.VertexBufferPtr->LocalPosition = s_Data.QuadVertexPositions[i] * 2.0f;
			s_Data.CircleLineResources.VertexBufferPtr->Color = color;
			s_Data.CircleLineResources.VertexBufferPtr->Thickness = thickness;
			s_Data.CircleLineResources.VertexBufferPtr->Fade = fade;
			s_Data.CircleLineResources.VertexBufferPtr->EntityID = entityID;
			s_Data.CircleLineResources.VertexBufferPtr++;
		}
		s_Data.CircleLineResources.IndexCount += 6;

		s_Data.Stats.QuadCount++;
	}

	void Renderer2D::DrawQuad(TransformComponent& transform, const SpriteRendererComponent& sprite,
		const Camera& camera, const TransformComponent& cameraTransform, int entityID)
	{
		SpriteAtlasComponent atlas;

		if (sprite.Texture)
		{
			atlas = { 0, 0, (float)sprite.Texture->GetWidth(), (float)sprite.Texture->GetHeight() };
		}
		else
			atlas = { 0, 0, 1, 1 };

		DrawQuad(transform, sprite, camera, cameraTransform, atlas, entityID);
	}

	void Renderer2D::DrawQuad(TransformComponent& transform, const SpriteRendererComponent& sprite, 
		const Camera& camera, const TransformComponent& cameraTransform,
		const SpriteAtlasComponent& atlas, int entityID)
	{
		TransformComponent newTransform = transform;
		if (sprite.IsBackground)
		{
			float width, height;
			camera.GetWidthAndHeight(width, height);
			newTransform.Translation += glm::vec3(cameraTransform.Translation.x, cameraTransform.Translation.y, 0.0f);
			newTransform.Scale *= glm::vec3(width * cameraTransform.Scale.x, height * cameraTransform.Scale.y, 1.0f);
			if (camera.GetProjectionType() == SceneCamera::ProjectionType::Perspective)
			{
				newTransform.Scale *= glm::vec3(cameraTransform.Translation.z / cameraTransform.Scale.z, 
					cameraTransform.Translation.z / cameraTransform.Scale.z, 1.0f);
			}
		}

		DrawQuad(newTransform, sprite, atlas, entityID);
	}

	void Renderer2D::DrawQuad(TransformComponent& transform, const SpriteRendererComponent& sprite, int entityID)
	{
		TED_PROFILE_CAT(InstrumentorCategory::Rendering);

		SpriteAtlasComponent atlas;

		if (sprite.Texture)
		{
			atlas = {0, 0, (float)sprite.Texture->GetWidth(), (float)sprite.Texture->GetHeight() };
		}
		else 
			atlas = { 0, 0, 1, 1 };

		DrawQuad(transform, sprite, atlas, entityID);
	}

	void Renderer2D::DrawQuad(TransformComponent& transform, const SpriteRendererComponent& sprite,
		const SpriteAtlasComponent& atlas, int entityID)
	{
		TED_PROFILE_CAT(InstrumentorCategory::Rendering);

		if (sprite.Texture)
		{
			if (sprite.OriginalAspectRatio)
			{
				float expectedRatio = (float)atlas.SpriteWidth / (float)atlas.SpriteHeight;
				float ratio = (float)transform.Scale.x / (float)transform.Scale.y;

				if (ratio != expectedRatio)
				{
					transform.Scale.x = transform.Scale.y * expectedRatio;
				}
			}

			float textureIndex = 0.0f;
			for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++)
			{
				if (*s_Data.TextureSlots[i].lock() == *sprite.Texture)
				{
					textureIndex = (float)i;
					break;
				}
			}

			if (textureIndex == 0.0f)
			{
				if (s_Data.QuadResources.IndexCount >= Renderer2DData::MaxIndices ||
					s_Data.TextureSlotIndex + s_Data.FontAtlasSlotIndex >= s_Data.TextureSlotsCapacity) NextBatch();

				textureIndex = (float)s_Data.TextureSlotIndex;
				s_Data.TextureSlots[s_Data.TextureSlotIndex] = sprite.Texture;
				s_Data.TextureSlotIndex++;
			}

			float width = 1;
			float height = 1;
			if (sprite.Texture)
			{
				width = (float)sprite.Texture->GetWidth();
				height = (float)sprite.Texture->GetHeight();
			}				
			
			SetQuad(transform.GetTransform(), sprite.Color, textureIndex, sprite.TilingFactor,
				atlas, width, height, entityID);

		}
		else
		{
			if (s_Data.QuadResources.IndexCount >= Renderer2DData::MaxIndices) NextBatch();

			const float textureIndex = 0.0f;

			SetQuad(transform.GetTransform(), sprite.Color, textureIndex, sprite.TilingFactor, entityID);
		}
	}
	
	// TODO: outline with freetype
	// TODO: Rotation in the center + letter rotation
	void Renderer2D::DrawString(const TextComponent& component, TransformComponent& transform, int entityID)
	{
		TED_PROFILE_CAT(InstrumentorCategory::Rendering);
		
		if (m_LastShaderDrawn != ShaderType::Text)
		{
			if (m_LastShaderDrawn != ShaderType::None)
				Flush();
			m_LastShaderDrawn = ShaderType::Text;
		}

		TransformComponent textQuad{ transform };
		switch (component.TextAlignment) // TODO: More alignment options
		{
			case TextComponent::AlignmentType::Center:
			{
				textQuad.Translation -= glm::vec3(component.TextQuad.Scale.x * transform.Scale.x / 2,
					component.TextQuad.Scale.y * transform.Scale.y / 2,
					0.0f);
				break;
			}
			default:
				break;
		}

		if (component.BackgroundColor.a != 0)
		{ // TODO: Fix this
			TransformComponent bgQuad{ textQuad };
			bgQuad.Translation += component.TextQuad.Translation - glm::vec3(0.0f, 0.0f, 0.01f);
			bgQuad.Rotation += component.TextQuad.Rotation;
			bgQuad.Scale *= component.TextQuad.Scale;
			DrawQuad(bgQuad, { component.BackgroundColor}, entityID);
		}

		const auto& fontGeometry = component.FontAsset->GetMSDFData()->FontGeometry;
		const auto& metrics = fontGeometry.getMetrics();
		Ref<Texture2D> fontAtlas = component.FontAsset->GetAtlasTexture();

		float atlasIndex = 0.0f;
		for (uint32_t i = 0; i < s_Data.FontAtlasSlotIndex; i++)
		{
			if (*s_Data.FontAtlasSlots[i].lock() == *fontAtlas)
			{
				atlasIndex = (float)i;
				break;
			}
		}

		if (atlasIndex == 0.0f)
		{
			if (s_Data.TextResources.IndexCount >= Renderer2DData::MaxIndices ||
				s_Data.TextureSlotIndex + s_Data.FontAtlasSlotIndex >= s_Data.TextureSlotsCapacity) NextBatch();

			atlasIndex = (float)s_Data.FontAtlasSlotIndex;
			s_Data.FontAtlasSlots[s_Data.FontAtlasSlotIndex] = fontAtlas;
			s_Data.FontAtlasSlotIndex++;
		}
		
		double x = 0.0;
		double fsScale = 1.0 / (metrics.ascenderY - metrics.descenderY);
		double y = 0.0;
		const float spaceGlyphAdvance = fontGeometry.getGlyph(' ')->getAdvance();

		const glm::mat4& textTransform = textQuad.GetTransform();

		for (size_t i = 0; i < component.TextString.size(); i++)
		{
			if (s_Data.TextResources.IndexCount >= Renderer2DData::MaxIndices)
			{
				NextBatch();

				atlasIndex = (float)s_Data.FontAtlasSlotIndex;
				s_Data.FontAtlasSlots[s_Data.FontAtlasSlotIndex] = fontAtlas;
				s_Data.FontAtlasSlotIndex++;
			}

			char character = component.TextString[i];

			if (character == '\r')
				continue;

			if (character == '\n')
			{
				x = 0;
				y -= fsScale * metrics.lineHeight + component.LineSpacing;
				continue;
			}

			if (character == ' ')
			{
				float advance = spaceGlyphAdvance;
				if (i < component.TextString.size() - 1)
				{
					char nextCharacter = component.TextString[i + 1];
					double dAdvance;
					fontGeometry.getAdvance(dAdvance, character, nextCharacter);
					advance = (float)dAdvance;
				}

				x += fsScale * advance + component.Kerning;
				continue;
			}

			if (character == '\t')
			{
				x += 4.0f * (fsScale * spaceGlyphAdvance + component.Kerning);
				continue;
			}

			auto glyph = fontGeometry.getGlyph(character);

			if (!glyph)
				glyph = fontGeometry.getGlyph('?');
			if (!glyph)
				return;

			double al, ab, ar, at;
			glyph->getQuadAtlasBounds(al, ab, ar, at);
			glm::vec2 texCoordMin((float)al, (float)ab);
			glm::vec2 texCoordMax((float)ar, (float)at);

			double pl, pb, pr, pt;
			glyph->getQuadPlaneBounds(pl, pb, pr, pt);
			glm::vec2 quadMin((float)pl, (float)pb);
			glm::vec2 quadMax((float)pr, (float)pt);

			quadMin *= fsScale, quadMax *= fsScale;
			quadMin += glm::vec2(x, y);
			quadMax += glm::vec2(x, y);

			float texelWidth = 1.0f / fontAtlas->GetWidth();
			float texelHeight = 1.0f / fontAtlas->GetHeight();

			texCoordMin *= glm::vec2(texelWidth, texelHeight);
			texCoordMax *= glm::vec2(texelWidth, texelHeight);

			// Add index + atlas array
			s_Data.TextResources.VertexBufferPtr->Position = textTransform * glm::vec4(quadMin, 0.0f, 1.0f);
			s_Data.TextResources.VertexBufferPtr->Color = component.Color;
			s_Data.TextResources.VertexBufferPtr->TexCoord = texCoordMin;
			s_Data.TextResources.VertexBufferPtr->OutlineColor = component.OutlineColor;
			s_Data.TextResources.VertexBufferPtr->OutlineThickness = component.OutlineThickness;
			s_Data.TextResources.VertexBufferPtr->AtlasIndex = atlasIndex;
			s_Data.TextResources.VertexBufferPtr->EntityID = entityID;
			s_Data.TextResources.VertexBufferPtr++;

			s_Data.TextResources.VertexBufferPtr->Position = textTransform * glm::vec4(quadMin.x, quadMax.y, 0.0f, 1.0f);
			s_Data.TextResources.VertexBufferPtr->Color = component.Color;
			s_Data.TextResources.VertexBufferPtr->TexCoord = { texCoordMin.x, texCoordMax.y };
			s_Data.TextResources.VertexBufferPtr->OutlineColor = component.OutlineColor;
			s_Data.TextResources.VertexBufferPtr->OutlineThickness = component.OutlineThickness;
			s_Data.TextResources.VertexBufferPtr->AtlasIndex = atlasIndex;
			s_Data.TextResources.VertexBufferPtr->EntityID = entityID;
			s_Data.TextResources.VertexBufferPtr++;

			s_Data.TextResources.VertexBufferPtr->Position = textTransform * glm::vec4(quadMax, 0.0f, 1.0f);
			s_Data.TextResources.VertexBufferPtr->Color = component.Color;
			s_Data.TextResources.VertexBufferPtr->TexCoord = texCoordMax;
			s_Data.TextResources.VertexBufferPtr->OutlineColor = component.OutlineColor;
			s_Data.TextResources.VertexBufferPtr->OutlineThickness = component.OutlineThickness;
			s_Data.TextResources.VertexBufferPtr->AtlasIndex = atlasIndex;
			s_Data.TextResources.VertexBufferPtr->EntityID = entityID;
			s_Data.TextResources.VertexBufferPtr++;

			s_Data.TextResources.VertexBufferPtr->Position = textTransform * glm::vec4(quadMax.x, quadMin.y, 0.0f, 1.0f);
			s_Data.TextResources.VertexBufferPtr->Color = component.Color;
			s_Data.TextResources.VertexBufferPtr->TexCoord = { texCoordMax.x, texCoordMin.y };
			s_Data.TextResources.VertexBufferPtr->OutlineColor = component.OutlineColor;
			s_Data.TextResources.VertexBufferPtr->OutlineThickness = component.OutlineThickness;
			s_Data.TextResources.VertexBufferPtr->AtlasIndex = atlasIndex;
			s_Data.TextResources.VertexBufferPtr->EntityID = entityID;
			s_Data.TextResources.VertexBufferPtr++;

			s_Data.TextResources.IndexCount += 6;
			s_Data.Stats.QuadCount++;

			if (i < component.TextString.size() - 1)
			{
				double advance = glyph->getAdvance();
				char nextCharacter = component.TextString[i + 1];
				fontGeometry.getAdvance(advance, character, nextCharacter);

				x += fsScale * advance + component.Kerning;
			}
		}

	}

	float Renderer2D::GetLineWidth()
	{
		return s_Data.LineWidth;
	}

	void Renderer2D::SetLineWidth(float width)
	{
		s_Data.LineWidth = width;
	}

	void Renderer2D::ResetStats()
	{
		memset(&s_Data.Stats, 0, sizeof(Statistics));
	}

	Renderer2D::Statistics Renderer2D::GetStats()
	{
		return s_Data.Stats;
	}
}