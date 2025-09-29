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

	struct FramebufferData
	{
		Ref<VertexArray> VertexArray;
		Ref<VertexBuffer> VertexBuffer;

		Ref<Shader> Shader;
		std::string ShaderName = "PostProcessing";
	};

	static FramebufferData s_Data;

	PostProcessing::PostProcessing()
	{
		TED_CORE_ASSERT(!s_Instance, "Post Processing already exists!");
		s_Instance = this;
	}

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

		s_Data.VertexArray = VertexArray::Create();

		float squareVertices[4 * 4] = {
			-1.0f, -1.0f, 0.0f, 0.0f,
			 1.0f, -1.0f, 1.0f, 0.0f,
			 1.0f,  1.0f, 1.0f, 1.0f,
			-1.0f,  1.0f, 0.0f, 1.0f
		};

		s_Data.VertexBuffer = VertexBuffer::Create(squareVertices, sizeof(squareVertices));
		s_Data.VertexBuffer->SetLayout({
			{ ShaderDataType::Float2, "a_Pos" },
			{ ShaderDataType::Float2, "a_TexCoord" }
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
	// TODO: Shader with multiple effects
	void PostProcessing::Apply()
	{
		TED_PROFILE_FUNCTION();

		RenderCommand::DisableDepth();
		RenderCommand::SetClearColor({ 1.0f, 1.0f, 1.0f, 1 });
		RenderCommand::ClearColor();

		s_Data.Shader->Bind();
		s_Data.VertexArray->Bind();

		RenderCommand::DrawFramebufferTexture(m_Framebuffer->GetColorAttachmentRendererID());
	}
}
