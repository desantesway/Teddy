#include "KeyStar2D.h"

#include <chrono>

KeyStar2D::KeyStar2D() 
	: Layer("Example"), m_CameraController(1920.0f / 1080.0f, true)
{

}

void KeyStar2D::OnAttach()
{
	TED_PROFILE_FUNCTION();

	m_SustainTexture = Teddy::Texture2D::Create("assets/textures/piano/pedal/sus.png");
}

void KeyStar2D::OnDetach()
{
	TED_PROFILE_FUNCTION();
}

void KeyStar2D::OnUpdate(Teddy::Timestep ts)
{
	TED_PROFILE_FUNCTION();

	m_CameraController.OnUpdate(ts);

	Teddy::Renderer2D::ResetStats();

	{
		TED_PROFILE_SCOPE("Renderer Prep");
		Teddy::RenderCommand::SetClearColor({ 1.0f, 0.1f, 0.1f, 1 });
		Teddy::RenderCommand::Clear();
	}

	{

		static float rotation = 0.0f;
		rotation += ts.GetSeconds() * 50.0f;

		TED_PROFILE_SCOPE("Renderer Draw (CPU)");
		Teddy::Renderer2D::BeginScene(m_CameraController.GetCamera());
	
		Teddy::Renderer2D::DrawQuad({ 0.0f, -1.0f }, { 1.0f, 1.0f }, m_SquareColor);
		Teddy::Renderer2D::DrawRotatedQuad({ 1.0f, 0.0f }, { 0.25f, 1.0f }, rotation, {0.2f, 0.8f, 0.1f, 1.0f});
		Teddy::Renderer2D::DrawQuad({ -5.0f, -5.0f, -0.1f }, { 10.0f, 10.0f }, m_SustainTexture, {0.1f, 0.1f, 0.9f, 1.0f});
		Teddy::Renderer2D::DrawRotatedQuad({ -15.0f, -5.0f, -0.1f }, { 10.0f, 10.0f }, 45.0f, m_SustainTexture, { 0.1f, 0.1f, 0.9f, 1.0f });
		Teddy::Renderer2D::EndScene();

		Teddy::Renderer2D::BeginScene(m_CameraController.GetCamera());

		for(float y= -5.0f; y < 5.0f; y += 0.1f)
		{
			for (float x = -5.0f; x < 5.0f; x += 0.1f)
			{
				glm::vec4 color = {(x + 5.0f) / 10.0f, 0.3f, (y + 5.0f) / 10.0f, 1.0f };
				Teddy::Renderer2D::DrawQuad({ x, y }, { 0.09f, 0.09f }, color);
			}
		}

		Teddy::Renderer2D::EndScene();
	}
}

void KeyStar2D::OnEvent(Teddy::Event& event)
{
	m_CameraController.OnEvent(event);

}

void KeyStar2D::OnImGuiRender() 
{
	TED_PROFILE_FUNCTION();
	ImGui::Begin("Settings");

	auto stats = Teddy::Renderer2D::GetStats();
	ImGui::Text("Renderer2D Stats:");
	ImGui::Text("Draw Calls: %d", stats.DrawCalls);
	ImGui::Text("Quads: %d", stats.QuadCount);
	ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
	ImGui::Text("Indices: %d", stats.GetTotalIndexCount());
	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));

	ImGui::End();
}