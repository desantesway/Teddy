#include "Teddy/PostProcessing/PostProcessing.h"

#include "Teddy/Renderer/RenderCommand.h"
#include "Teddy/Renderer/VertexArray.h"
#include "Teddy/Renderer/Shader.h"

#include "Teddy/Core/AssetManager.h"

namespace Teddy
{
	PostProcessing* PostProcessing::s_Instance = nullptr;
	Ref<Framebuffer> PostProcessing::m_Framebuffer = nullptr;
	FramebufferSpecification PostProcessing::m_FramebufferSpec;

	PostProcessing::PostProcessing()
	{
		TED_CORE_ASSERT(!s_Instance, "Post Processing already exists!");
		s_Instance = this;
	}

	struct PostProcessingEffects
	{
		glm::vec2 Position;
		glm::vec2 TexCoord;
		int ChromaticAberration = 1; // glsl doesnt accept bool
		glm::vec3 ChromaticAberrationOffset = glm::vec3( -1.0f, 1.0f, 1.0f); // (R,G,B)
	};

	struct FramebufferData
	{
		Ref<VertexArray> VertexArray;
		Ref<VertexBuffer> VertexBuffer;

		Ref<Shader> Shader;
		std::string ShaderName = "PostProcessing";

		PostProcessingEffects Effects[4];

		glm::vec4 VertexPositions[4];
	};

	static FramebufferData s_Data;

	void PostProcessing::Init()
	{
		TED_PROFILE_CAT(InstrumentorCategory::Rendering);

		m_FramebufferSpec.Attachments = Teddy::FramebufferAttachmentSpecification({
			Teddy::FramebufferTextureSpecification(Teddy::FramebufferTextureFormat::RGBA8, Teddy::TextureFilterFormat::LINEAR, Teddy::TextureWrapFormat::REPEAT),
			Teddy::FramebufferTextureSpecification(Teddy::FramebufferTextureFormat::RED_INTEGER, Teddy::TextureFilterFormat::LINEAR, Teddy::TextureWrapFormat::REPEAT),
			Teddy::FramebufferTextureSpecification(Teddy::FramebufferTextureFormat::Depth, Teddy::TextureFilterFormat::LINEAR, Teddy::TextureWrapFormat::REPEAT)
			});
		m_FramebufferSpec.Width = 1920;
		m_FramebufferSpec.Height = 1080;
		m_Framebuffer = Framebuffer::Create(m_FramebufferSpec);

		s_Data.VertexPositions[0] = { -1.0f, -1.0f, 0.0f, 1.0f };
		s_Data.VertexPositions[1] = { 1.0f, -1.0f, 0.0f, 1.0f };
		s_Data.VertexPositions[2] = { 1.0f,  1.0f, 0.0f, 1.0f };
		s_Data.VertexPositions[3] = { -1.0f,  1.0f, 0.0f, 1.0f };

		constexpr size_t quadVertexCount = 4;
		constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

		for (size_t i = 0; i < quadVertexCount; i++)
		{
			s_Data.Effects[i].Position = s_Data.VertexPositions[i];
			s_Data.Effects[i].TexCoord = textureCoords[i];
		}

		s_Data.VertexArray = VertexArray::Create();

		s_Data.VertexBuffer = VertexBuffer::Create(4 * sizeof(PostProcessingEffects));
		s_Data.VertexBuffer->SetLayout({
			{ ShaderDataType::Float2,	"a_Pos" },
			{ ShaderDataType::Float2,	"a_TexCoord" },
			{ ShaderDataType::Int,		"a_ChromaticAberration" },
			{ ShaderDataType::Float3,	"a_ChromaticAberrationOffset" },
			});
		s_Data.VertexArray->AddVertexBuffer(s_Data.VertexBuffer);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		Ref<IndexBuffer> squareIB;
		squareIB = IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
		s_Data.VertexArray->SetIndexBuffer(squareIB);

		auto& assets = AssetManager::Get();

		s_Data.Shader = assets.Load<Shader>(s_Data.ShaderName, "../Teddy/assets/shaders/PostProcessing.glsl");
	}

	void PostProcessing::Shutdown()
	{
		TED_PROFILE_FUNCTION();
	}

	void PostProcessing::Resize(uint32_t width, uint32_t height)
	{
		m_Framebuffer->Resize(width, height);
	}

	// TODO: Shader hot reload
	void PostProcessing::Apply()
	{
		TED_PROFILE_FUNCTION();

		RenderCommand::DisableDepth();
		RenderCommand::SetClearColor({ 1.0f, 1.0f, 1.0f, 1 });
		RenderCommand::ClearColor();

		s_Data.VertexBuffer->SetData(s_Data.Effects, sizeof(s_Data.Effects));

		s_Data.Shader->Bind();
		s_Data.VertexArray->Bind();

		RenderCommand::DrawFramebufferTexture(m_Framebuffer->GetColorAttachmentRendererID());
	}

	bool PostProcessing::IsEffectEnabled(Effect effect)
	{
		switch (effect)
		{
		case Effect::ChromaticAberration:
			return s_Data.Effects[0].ChromaticAberration == 1;
		case Effect::None:
			return false;
		default:
			return false;
		}
	}

	void PostProcessing::EnableEffect(Effect effect)
	{
		switch (effect)
		{
		case Effect::ChromaticAberration:
			for (size_t i = 0; i < 4; i++)
				s_Data.Effects[i].ChromaticAberration = 1;
			break;
		case Effect::None:
			break;
		default:
			break;
		}
	}

	void PostProcessing::DisableEffect(Effect effect)
	{
		switch (effect)
		{
		case Effect::ChromaticAberration:
			for (size_t i = 0; i < 4; i++)
				s_Data.Effects[i].ChromaticAberration = 0;
			break;
		case Effect::None:
			break;
		default:
			break;
		}
	}

	void PostProcessing::SetChromaticAberrationOffset(glm::vec3 offset)
	{
		for (size_t i = 0; i < 4; i++)
			s_Data.Effects[i].ChromaticAberrationOffset = offset;
	}

	glm::vec3 PostProcessing::GetChromaticAberrationOffset()
	{
		return s_Data.Effects[0].ChromaticAberrationOffset;
	}
}
