#include "KeyStar2D.h"

KeyStar2D::KeyStar2D() 
	: Layer("Example"), m_CameraController(1920.0f / 1080.0f, true)
{

}

void KeyStar2D::OnAttach()
{
	m_SustainTexture = Teddy::Texture2D::Create("assets/textures/piano/pedal/sus.png");
}

void KeyStar2D::OnDetach()
{
}

void KeyStar2D::OnUpdate(Teddy::Timestep ts)
{
	m_CameraController.OnUpdate(ts);

	Teddy::RenderCommand::SetClearColor({ 0.9f, 0.1f, 0.1f, 1 });
	Teddy::RenderCommand::Clear();

	Teddy::Renderer2D::BeginScene(m_CameraController.GetCamera());
	
	Teddy::Renderer2D::DrawQuad({ 0.0f, -1.0f }, { 1.0f, 1.0f }, m_SquareColor);
	Teddy::Renderer2D::DrawQuad({ 1.0f, 0.0f }, { 0.25f, 1.0f }, {0.9f, 0.3f, 0.1f, 1.0f});
	Teddy::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 10.0f, 10.0f }, m_SustainTexture);
	Teddy::Renderer2D::EndScene();
	
}

void KeyStar2D::OnEvent(Teddy::Event& event)
{
	m_CameraController.OnEvent(event);

}
void KeyStar2D::OnImGuiRender() 
{
	ImGui::Begin("Settings");
	ImGui::Text("ChangeColor");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
	ImGui::End();
}