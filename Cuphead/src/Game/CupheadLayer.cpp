#include "CupheadLayer.h"

#include <chrono>

#include "Platform/OpenGL/OpenGLShader.h"
#include "Teddy/Renderer/Shader.h"

#include <imgui.h>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Teddy/Scene/SceneSerializer.h"
#include "Teddy/Utils/PlatformUtils.h"

CupheadLayer::CupheadLayer()
	: Layer("Main Layer")
{

}

void CupheadLayer::OnAttach()
{
	TED_PROFILE_FUNCTION();

	m_ActiveScene = Teddy::CreateRef<Teddy::Scene>();

    auto camEntt = m_ActiveScene->CreateEntity("Camera");
	camEntt.GetComponent<Teddy::TransformComponent>().Translation = { 0.0f, 0.0f, 9.0f };
    auto& cam = camEntt.AddComponent<Teddy::CameraComponent>();
    cam.Camera.SetProjectionType(Teddy::SceneCamera::ProjectionType::Perspective);
	cam.Camera.SetViewportSize(1920, 1080);

    auto cupheadAnimation = m_ActiveScene->CreateEntity("Animation Title");
    auto& spriteAnimation = cupheadAnimation.AddComponent<Teddy::SpriteRendererComponent>();
    spriteAnimation.Texture = Teddy::AssetManager::Get().Load<Teddy::Texture2D>("Cuphead Title Animation", "assets/Textures/CupAndMugMan_Title_Animation_Atlas1.png");
    auto& animationTransform = cupheadAnimation.GetComponent<Teddy::TransformComponent>();
    animationTransform.Translation = glm::vec3(0, 0, 1.0f);
    //animationTransform.Scale = glm::vec3(16.1f, 7.5f, 1.0f);

	auto background = m_ActiveScene->CreateEntity("Background");
    auto& sprite = background.AddComponent<Teddy::SpriteRendererComponent>();
	sprite.IsBackground = true;
    sprite.Texture = Teddy::AssetManager::Get().Load<Teddy::Texture2D>("Background", "assets/Textures/cuttedSpriteAtlasTexture-Title_Assets (Group 1)-2048x1024-fmt10.png");

    m_ActiveScene->OnRuntimeStart();
}

void CupheadLayer::OnDetach()
{
	TED_PROFILE_FUNCTION();
}

void CupheadLayer::OnUpdate(Teddy::Timestep ts)
{
	TED_PROFILE_FUNCTION();

	Teddy::Renderer2D::ResetStats();

    {
        TED_PROFILE_SCOPE("Renderer Prep");

        Teddy::RenderCommand::SetClearColor({ 0.0f, 0.0f, 0.0f, 1.0f });
        Teddy::RenderCommand::Clear();
    }

    {
        TED_PROFILE_SCOPE("Renderer Draw (CPU)");

        m_ActiveScene->AlwaysOnUpdate();
        m_ActiveScene->OnUpdateRuntime(ts);
    }
}

void CupheadLayer::OnEvent(Teddy::Event& event)
{
    m_ActiveScene->OnEvent(event);
}

void CupheadLayer::OnImGuiRender()
{
	TED_PROFILE_FUNCTION();
}