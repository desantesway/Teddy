#include "SandBox2D.h"

#include <chrono>

#include "Platform/OpenGL/OpenGLShader.h"
#include "Teddy/Renderer/Shader.h"

#include <imgui.h>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Teddy/Scene/SceneSerializer.h"
#include "Teddy/Utils/PlatformUtils.h"

Sandbox2D::Sandbox2D() 
	: Layer("Example")
{

}

void Sandbox2D::OnAttach()
{
	TED_PROFILE_FUNCTION();

	//m_BoardTexture = Teddy::AssetManager::Get().Load<Teddy::Texture2D>("checkerboard", "assets/textures/checkerboard.jpg");

	m_ActiveScene = Teddy::CreateRef<Teddy::Scene>();

	m_EditorCamera = Teddy::EditorCamera(30.0f, 1.778f, 0.1f, 1000.0f);

	auto redSquare = m_ActiveScene->CreateEntity("Red Square");
	redSquare.AddComponent<Teddy::SpriteRendererComponent>(glm::vec4{ 1.0f, 0.5f, 0.5f, 1.0f });
}

void Sandbox2D::OnDetach()
{
	TED_PROFILE_FUNCTION();
}

void Sandbox2D::OnUpdate(Teddy::Timestep ts)
{
	TED_PROFILE_FUNCTION();

	m_EditorCamera.OnUpdate(ts);

	Teddy::Renderer2D::ResetStats();

    {
        TED_PROFILE_SCOPE("Renderer Prep");

        Teddy::RenderCommand::SetClearColor({ 1.0f, 0.1f, 0.1f, 1 });
        Teddy::RenderCommand::Clear();
    }

    {

        TED_PROFILE_SCOPE("Renderer Draw (CPU)");

        m_ActiveScene->OnUpdateEditor(ts, m_EditorCamera);
    }
}

void Sandbox2D::OnEvent(Teddy::Event& event)
{
    m_ActiveScene->OnEvent(event);
}

void Sandbox2D::OnImGuiRender() 
{
	TED_PROFILE_FUNCTION();

    ImGui::Begin("Stats");

    auto stats = Teddy::Renderer2D::GetStats();
    ImGui::Text("Renderer2D Stats:");
    ImGui::Text("Draw Calls: %d", stats.DrawCalls);
    ImGui::Text("Quads: %d", stats.QuadCount);
    ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
    ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

    ImGui::End();

    ImGui::Begin("Colors");

    ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));

	ImGui::End();
}