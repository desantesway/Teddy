#include "TeddyPch.h"
#include "Teddy/Renderer/Renderer2D.h"

#include "Teddy/Renderer/VertexArray.h"
#include "Teddy/Renderer/Shader.h"
#include "Teddy/Renderer/RenderCommand.h"

#include "Teddy/Renderer/UniformBuffer.h"

#include "Teddy/Renderer/MSDFData.h"
#include "Teddy/Core/AssetManager.h"

#include <glm/gtc/matrix_transform.hpp>

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
	};

	template<typename T>
	struct BashVertexResource : VertexResource
	{
		uint32_t IndexCount = 0;
		T* VertexBufferBase = nullptr;
		T* VertexBufferPtr = nullptr;
	};

	struct Renderer2DData
	{ 
		static const uint32_t MaxQuads = 20000;
		static const uint32_t MaxLines = 20000;
		static const uint32_t MaxVertices = MaxQuads * 4;
		static const uint32_t MaxIndices = MaxQuads * 6;
		static const uint32_t MaxLineIndices = MaxLines * 2;
		static const uint32_t MaxTextureSlots = 32; // TODO: See the maximum number of texture slots for the target platform
		static const uint32_t MaxFontSlots = 32;

		Ref<Texture2D> WhiteTexture;

		BashRenderResource<LineVertex> LineResources;

		BashRenderResource<QuadVertex> QuadResources;

		BashRenderResource<CircleVertex> CircleResources;
		BashVertexResource<CircleVertex> CircleLineResources;

		BashRenderResource<TextVertex> TextResources;

		float LineWidth = 2.0f;

		std::array<Ref<Texture2D>, MaxTextureSlots> TextureSlots;
		uint32_t TextureSlotIndex = 1; // 0 is reserved for white texture

		std::array<Ref<Texture2D>, MaxFontSlots> FontAtlasSlots;
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

		// Adding shaders
		auto& assets = AssetManager::Get();
		
		s_Data.QuadResources.Shader = assets.Load<Shader>("Quad Shader", "../Teddy/assets/shaders/Renderer2D_Quad.glsl");
		s_Data.CircleResources.Shader = assets.Load<Shader>("Circle Shader", "../Teddy/assets/shaders/Renderer2D_Circle.glsl");
		s_Data.LineResources.Shader = assets.Load<Shader>("Line Shader", "../Teddy/assets/shaders/Renderer2D_Line.glsl");
		s_Data.TextResources.Shader = assets.Load<Shader>("Text Shader", "../Teddy/assets/shaders/Renderer2D_Text.glsl");

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

	void Renderer2D::BeginScene(const EditorCamera& camera)
	{
		TED_PROFILE_CAT(InstrumentorCategory::Rendering);

		s_Data.CameraBuffer.ViewProjection = camera.GetViewProjection();
		s_Data.CameraUniformBuffer->SetData(&s_Data.CameraBuffer, sizeof(Renderer2DData::CameraData));

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
	}

	void Renderer2D::Flush()
	{
		TED_PROFILE_CAT(InstrumentorCategory::Rendering);

		if (s_Data.QuadResources.IndexCount)
		{
			uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.QuadResources.VertexBufferPtr - (uint8_t*)s_Data.QuadResources.VertexBufferBase);
			s_Data.QuadResources.VertexBuffer->SetData(s_Data.QuadResources.VertexBufferBase, dataSize);

			for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++)
				s_Data.TextureSlots[i]->Bind(i);

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
				s_Data.FontAtlasSlots[i]->Bind(i);

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
		const float& texIndex, const float& tilingFactor, int entityID)
	{
		TED_PROFILE_CAT(InstrumentorCategory::Rendering);

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

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, 
		const SpriteRendererComponent& sprite, float rotation, int entityID)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, sprite, rotation, entityID);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, 
		const SpriteRendererComponent& sprite, float rotation, int entityID)
	{
		TED_PROFILE_CAT(InstrumentorCategory::Rendering);

		glm::mat4 transform;

		if (rotation)
		{
			transform = glm::translate(glm::mat4(1.0f), position)
				* glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f })
				* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		}
		else
		{
			transform = glm::translate(glm::mat4(1.0f), position)
				* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		}

		DrawQuad(transform, sprite, entityID);
	}

	void Renderer2D::DrawQuad(const glm::mat4& transform, const SpriteRendererComponent& sprite, int entityID)
	{
		TED_PROFILE_CAT(InstrumentorCategory::Rendering);

		if (sprite.Texture != nullptr)
		{
			float textureIndex = 0.0f;
			for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++)
			{
				if (*s_Data.TextureSlots[i] == *sprite.Texture)
				{
					textureIndex = (float)i;
					break;
				}
			}

			if (textureIndex == 0.0f)
			{
				if (s_Data.QuadResources.IndexCount >= Renderer2DData::MaxIndices) NextBatch();

				textureIndex = (float)s_Data.TextureSlotIndex;
				s_Data.TextureSlots[s_Data.TextureSlotIndex] = sprite.Texture;
				s_Data.TextureSlotIndex++;
			}

			SetQuad(transform, sprite.Color, textureIndex, sprite.TilingFactor, entityID);

		}
		else
		{
			if (s_Data.QuadResources.IndexCount >= Renderer2DData::MaxIndices) NextBatch();

			const float textureIndex = 0.0f;

			SetQuad(transform, sprite.Color, textureIndex, sprite.TilingFactor, entityID);
		}
	}
	
	// TODO: outline with freetype
	// TODO: Rotation in the center + letter rotation
	void Renderer2D::DrawString(const TextParams& textParams, const TransformComponent& textQuad, 
		Ref<Font> font, const glm::mat4& transform, int entityID)
	{
		TED_PROFILE_CAT(InstrumentorCategory::Rendering);
		
		glm::mat4 bgTransform = transform * textQuad.GetTransform();
		bgTransform[3].z -= 0.01f;
		if (textParams.BackgroundColor.a != 0)
		{
			DrawQuad(bgTransform, {textParams.BackgroundColor}, entityID);
		}

		glm::mat4 scale = glm::mat4(1.0f);

		glm::mat4 translate = glm::translate(glm::mat4(1.0f), glm::vec3(-textParams.OutlineThickness / 2, textParams.OutlineThickness / 2, 1.0f));

		const auto& fontGeometry = font->GetMSDFData()->FontGeometry;
		const auto& metrics = fontGeometry.getMetrics();
		Ref<Texture2D> fontAtlas = font->GetAtlasTexture();

		float atlasIndex = 0.0f;
		for (uint32_t i = 0; i < s_Data.FontAtlasSlotIndex; i++)
		{
			if (*s_Data.FontAtlasSlots[i] == *fontAtlas)
			{
				atlasIndex = (float)i;
				break;
			}
		}

		if (atlasIndex == 0.0f)
		{

			atlasIndex = (float)s_Data.FontAtlasSlotIndex;
			s_Data.FontAtlasSlots[s_Data.FontAtlasSlotIndex] = fontAtlas;
			s_Data.FontAtlasSlotIndex++;
		}
		
		double x = 0.0;
		double fsScale = 1.0 / (metrics.ascenderY - metrics.descenderY);
		double y = 0.0;
		const float spaceGlyphAdvance = fontGeometry.getGlyph(' ')->getAdvance();

		for (size_t i = 0; i < textParams.TextString.size(); i++)
		{
			if (s_Data.TextResources.IndexCount >= Renderer2DData::MaxIndices)
			{
				NextBatch();

				atlasIndex = (float)s_Data.FontAtlasSlotIndex;
				s_Data.FontAtlasSlots[s_Data.FontAtlasSlotIndex] = fontAtlas;
				s_Data.FontAtlasSlotIndex++;
			}

			char character = textParams.TextString[i];

			if (character == '\r')
				continue;

			if (character == '\n')
			{
				x = 0;
				y -= fsScale * metrics.lineHeight + textParams.LineSpacing;
				continue;
			}

			if (character == ' ')
			{
				float advance = spaceGlyphAdvance;
				if (i < textParams.TextString.size() - 1)
				{
					char nextCharacter = textParams.TextString[i + 1];
					double dAdvance;
					fontGeometry.getAdvance(dAdvance, character, nextCharacter);
					advance = (float)dAdvance;
				}

				x += fsScale * advance + textParams.Kerning;
				continue;
			}

			if (character == '\t')
			{
				x += 4.0f * (fsScale * spaceGlyphAdvance + textParams.Kerning);
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
			s_Data.TextResources.VertexBufferPtr->Position = transform * glm::vec4(quadMin, 0.0f, 1.0f);
			s_Data.TextResources.VertexBufferPtr->Color = textParams.Color;
			s_Data.TextResources.VertexBufferPtr->TexCoord = texCoordMin;
			s_Data.TextResources.VertexBufferPtr->OutlineColor = textParams.OutlineColor;
			s_Data.TextResources.VertexBufferPtr->OutlineThickness = textParams.OutlineThickness;
			s_Data.TextResources.VertexBufferPtr->AtlasIndex = atlasIndex;
			s_Data.TextResources.VertexBufferPtr->EntityID = entityID;
			s_Data.TextResources.VertexBufferPtr++;

			s_Data.TextResources.VertexBufferPtr->Position = transform * glm::vec4(quadMin.x, quadMax.y, 0.0f, 1.0f);
			s_Data.TextResources.VertexBufferPtr->Color = textParams.Color;
			s_Data.TextResources.VertexBufferPtr->TexCoord = { texCoordMin.x, texCoordMax.y };
			s_Data.TextResources.VertexBufferPtr->OutlineColor = textParams.OutlineColor;
			s_Data.TextResources.VertexBufferPtr->OutlineThickness = textParams.OutlineThickness;
			s_Data.TextResources.VertexBufferPtr->AtlasIndex = atlasIndex;
			s_Data.TextResources.VertexBufferPtr->EntityID = entityID;
			s_Data.TextResources.VertexBufferPtr++;

			s_Data.TextResources.VertexBufferPtr->Position = transform * glm::vec4(quadMax, 0.0f, 1.0f);
			s_Data.TextResources.VertexBufferPtr->Color = textParams.Color;
			s_Data.TextResources.VertexBufferPtr->TexCoord = texCoordMax;
			s_Data.TextResources.VertexBufferPtr->OutlineColor = textParams.OutlineColor;
			s_Data.TextResources.VertexBufferPtr->OutlineThickness = textParams.OutlineThickness;
			s_Data.TextResources.VertexBufferPtr->AtlasIndex = atlasIndex;
			s_Data.TextResources.VertexBufferPtr->EntityID = entityID;
			s_Data.TextResources.VertexBufferPtr++;

			s_Data.TextResources.VertexBufferPtr->Position = transform * glm::vec4(quadMax.x, quadMin.y, 0.0f, 1.0f);
			s_Data.TextResources.VertexBufferPtr->Color = textParams.Color;
			s_Data.TextResources.VertexBufferPtr->TexCoord = { texCoordMax.x, texCoordMin.y };
			s_Data.TextResources.VertexBufferPtr->OutlineColor = textParams.OutlineColor;
			s_Data.TextResources.VertexBufferPtr->OutlineThickness = textParams.OutlineThickness;
			s_Data.TextResources.VertexBufferPtr->AtlasIndex = atlasIndex;
			s_Data.TextResources.VertexBufferPtr->EntityID = entityID;
			s_Data.TextResources.VertexBufferPtr++;

			s_Data.TextResources.IndexCount += 6;
			s_Data.Stats.QuadCount++;

			if (i < textParams.TextString.size() - 1)
			{
				double advance = glyph->getAdvance();
				char nextCharacter = textParams.TextString[i + 1];
				fontGeometry.getAdvance(advance, character, nextCharacter);

				x += fsScale * advance + textParams.Kerning;
			}
		}

	}

	void Renderer2D::DrawString(const TextComponent& component, const glm::mat4& transform, int entityID)
	{
		DrawString(
			TextParams
			{
				component.TextString,
				component.Color,
				component.Kerning,
				component.LineSpacing,
				component.BackgroundColor,
				component.OutlineColor,
				component.OutlineThickness
			},
			component.TextQuad,
			component.FontAsset,
			transform,
			entityID
		);
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