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

	auto& assets = Teddy::AssetManager::Get();

	// Cuphead Animation
    auto cupheadAnimation = m_ActiveScene->CreateEntity("Animation Title");
    cupheadAnimation.AddComponent<Teddy::SpriteAtlasComponent>(0, 2, 1013, 552);
    auto& spriteAnimation = cupheadAnimation.AddComponent<Teddy::SpriteAnimationComponent>(0.125f, 0.05f, 0.25f); // TODO: intial frame is being ran twice?
    spriteAnimation.PingPong = true;
    spriteAnimation.Textures = assets.LoadMultiple<Teddy::Texture2D>(
        {
            "assets/textures/CupAndMugMan_1013x552_2048x2048_0.png",
            "assets/textures/CupAndMugMan_1013x552_2048x2048_1.png",
            "assets/textures/CupAndMugMan_1013x552_1013x552_2.png"
        });
    auto& animationTransform = cupheadAnimation.GetComponent<Teddy::TransformComponent>();
    animationTransform.Translation = glm::vec3(0.2f, -0.55f, 1.0f);
    animationTransform.Scale = glm::vec3(0.0f, 0.75f, 1.0f);
    spriteAnimation.IsBackground = true;
    
    // Text
	auto textEntity = m_ActiveScene->CreateEntity("Title text");
    auto& text = textEntity.AddComponent<Teddy::TextComponent>();
    text.FontAsset = assets.Load<Teddy::Font>("assets/Fonts/CupheadVogue-ExtraBold.otf", Teddy::Boolean::True );
    text.SetString("Press Any Button");
    text.Color = glm::vec4(233.0f/255.0f, 193.0f /255.0f, 80.0f /255.0f, 1.0f);
    text.TextAlignment = Teddy::TextComponent::AlignmentType::Center;
    auto& textTransform = textEntity.GetComponent<Teddy::TransformComponent>();
    textTransform.Scale *= 0.41;
    textTransform.Translation += glm::vec3(0.0f, -2.60f, 1.5f);

	// Background
	auto background = m_ActiveScene->CreateEntity("Background");
    auto& sprite = background.AddComponent<Teddy::SpriteRendererComponent>();
	sprite.IsBackground = true;
    sprite.Texture = assets.Load<Teddy::Texture2D>("Background", "assets/Textures/cuttedSpriteAtlasTexture-Title_Assets (Group 1)-2048x1024-fmt10.png");

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