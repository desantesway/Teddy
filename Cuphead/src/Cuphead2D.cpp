#include "Cuphead2D.h"

#include <chrono>

#include "Platform/OpenGL/OpenGLShader.h"
#include "Teddy/Renderer/Shader.h"

#include <imgui.h>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Teddy/Scene/SceneSerializer.h"
#include "Teddy/Utils/PlatformUtils.h"

Cuphead2D::Cuphead2D()
	: Layer("Main Layer")
{

}

void Cuphead2D::OnAttach()
{
	TED_PROFILE_FUNCTION();

	m_ActiveScene = Teddy::CreateRef<Teddy::Scene>();

    auto camEntt = m_ActiveScene->CreateEntity("Camera");
	camEntt.GetComponent<Teddy::TransformComponent>().Translation = { 0.0f, 0.0f, 9.0f };
    auto& cam = camEntt.AddComponent<Teddy::CameraComponent>();
    cam.Camera.SetProjectionType(Teddy::SceneCamera::ProjectionType::Perspective);
	cam.Camera.SetViewportSize(1920, 1080);

	auto background = m_ActiveScene->CreateEntity("Red Square");
    auto& sprite = background.AddComponent<Teddy::SpriteRendererComponent>();
	sprite.IsBackground = true;
    sprite.Texture = Teddy::AssetManager::Get().Load<Teddy::Texture2D>("Background", "assets/Textures/SpriteAtlasTexture-Title_Assets (Group 1)-2048x1024-fmt10.png");

    m_ActiveScene->OnRuntimeStart();
}

void Cuphead2D::OnDetach()
{
	TED_PROFILE_FUNCTION();
}

void Cuphead2D::OnUpdate(Teddy::Timestep ts)
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

        m_ActiveScene->AlwaysOnUpdate();
        
        m_ActiveScene->OnUpdateRuntime(ts);
    }
}

void Cuphead2D::OnEvent(Teddy::Event& event)
{
    m_ActiveScene->OnEvent(event);
}

void Cuphead2D::OnImGuiRender()
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