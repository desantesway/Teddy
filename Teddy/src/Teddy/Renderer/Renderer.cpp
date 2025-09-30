#include "TeddyPch.h"
#include "Teddy/Renderer/Renderer.h"
#include "Teddy/Renderer/Renderer2D.h"

#include "Teddy/PostProcessing/PostProcessing.h"

namespace Teddy 
{

	Scope<Renderer::SceneData> Renderer::s_SceneData = CreateScope<Renderer::SceneData>();

	void Renderer::Init()
	{
		TED_PROFILE_FUNCTION();

		RenderCommand::Init();
		Renderer2D::Init();
		PostProcessing::Init();
	}

	void Renderer::Shutdown()
	{
		Renderer2D::Shutdown();
	}

	void Renderer::OnWindowResize(uint32_t width, uint32_t height) 
	{
		RenderCommand::SetViewport(0, 0, width, height);
	}

	void Renderer::EndScene()
	{
	}

	void Renderer::Submit(const Ref<Shader>& shader,
		const Ref<VertexArray>& vertexArray,
		const glm::mat4& transform)
	{
		TED_PROFILE_FUNCTION();

		shader->Bind();
		shader->SetMat4("u_ViewProjection", s_SceneData->ViewProjectionMatrix);
		shader->SetMat4("u_Transform", transform);

		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}

}