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

	{
		TED_PROFILE_SCOPE("Renderer Prep");
		Teddy::RenderCommand::SetClearColor({ 1.0f, 0.1f, 0.1f, 1 });
		Teddy::RenderCommand::Clear();
	}

	{
		TED_PROFILE_SCOPE("Renderer Draw (CPU)");
		Teddy::Renderer2D::BeginScene(m_CameraController.GetCamera());
	
		Teddy::Renderer2D::DrawQuad({ 0.0f, -1.0f }, { 1.0f, 1.0f }, m_SquareColor);
		Teddy::Renderer2D::DrawQuad({ 1.0f, 0.0f }, { 0.25f, 1.0f }, {0.9f, 0.3f, 0.1f, 1.0f});
		Teddy::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 10.0f, 10.0f }, m_SustainTexture);
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
	ImGui::Text("ChangeColor");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));

	ImGui::End();
}