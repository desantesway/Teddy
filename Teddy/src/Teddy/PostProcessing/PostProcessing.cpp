#include "Teddy/PostProcessing/PostProcessing.h"

#include "Teddy/Renderer/RenderCommand.h"
#include "Teddy/Renderer/VertexArray.h"
#include "Teddy/Renderer/Shader.h"

#include "Teddy/Core/AssetManager.h"

namespace Teddy
{
	struct QuadVertex
	{
		glm::vec3 Position;
		glm::vec2 TexCoord;
	};

	struct FramebufferData
	{
		Ref<VertexArray> VertexArray;
		Ref<VertexBuffer> VertexBuffer;

		Ref<Shader> Shader;
		std::string ShaderName = "PostProcessing";

		float LineWidth = 2.0f;

		glm::vec4 QuadVertexPositions[4];
	};

	static FramebufferData s_Data;

	void PostProcessing::Init()
	{
		TED_PROFILE_CAT(InstrumentorCategory::Rendering);

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

	// TODO: Shader hot reload
	// TODO: Shader with multiple effects
	void PostProcessing::Apply(Ref<Framebuffer> framebuffer)
	{
		TED_PROFILE_FUNCTION();

		RenderCommand::DisableDepth();
		RenderCommand::SetClearColor({ 1.0f, 1.0f, 1.0f, 1 });
		RenderCommand::ClearColor();

		s_Data.Shader->Bind();
		s_Data.VertexArray->Bind();

		RenderCommand::DrawFramebufferTexture(framebuffer->GetColorAttachmentRendererID());
	}
}
