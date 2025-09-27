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

    // Transition Quad
    auto transitionQuadAnimation = m_ActiveScene->CreateEntity("Title Transition Quad");
    m_TransitionQuad = Teddy::CreateRef<Teddy::Entity>(transitionQuadAnimation);
    auto& transitionQuad = m_TransitionQuad->AddComponent<Teddy::SpriteRendererComponent>();
    transitionQuad.Color = glm::vec4(0.0f);
    transitionQuad.IsBackground = true;
    auto& transitionQuadTransform = transitionQuadAnimation.GetComponent<Teddy::TransformComponent>();
    transitionQuadTransform.Translation = glm::vec3(0.0f, 0.0f, 6.0f);
    m_ActiveScene->OnRuntimeStart();

    // Transition Circle
    auto transitionAnimation = m_ActiveScene->CreateEntity("Title Transition");
    m_TransitionCircle = Teddy::CreateRef<Teddy::Entity>(transitionAnimation);
    auto& transitionCircle = m_TransitionCircle->AddComponent<Teddy::CircleRendererComponent>();
    transitionCircle.Color = glm::vec4(0.0f);
    transitionCircle.Thickness = 0.0f;
    auto& transitionTransform = transitionAnimation.GetComponent<Teddy::TransformComponent>();
    transitionTransform.Scale = glm::vec3(7.0f, 7.0f, 1.0f);
    transitionTransform.Translation = glm::vec3(0.0f, 0.0f, 5.0f);

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

    class TitleText : public Teddy::ScriptableEntity
    {
    public:
        void OnCreate()
        {
            auto& textTransform = GetComponent<Teddy::TransformComponent>();
            textTransform.Scale *= 0.41;
            textTransform.Translation += glm::vec3(0.0f, -2.60f, 1.5f);
        }

        void OnDestroy()
        {
        }

        void OnUpdate(Teddy::Timestep ts)
        {
			static float timer = 0.0f;
			timer += ts;

            auto& text = GetComponent<Teddy::TextComponent>();
            if (text.Color.a == 0 && timer > 1)
            {
                timer = 0.0f;
                text.Color.a = 1;
            }
            else if (text.Color.a != 0 && timer > 1)
            {
                timer = 0.0f;
                text.Color.a = 0;
            }
        }
    };

    textEntity.AddComponent<Teddy::NativeScriptComponent>().Bind<TitleText>();

	// Background
	auto background = m_ActiveScene->CreateEntity("Background");
    auto& sprite = background.AddComponent<Teddy::SpriteRendererComponent>();
	sprite.IsBackground = true;
    sprite.Texture = assets.Load<Teddy::Texture2D>("Background", "assets/Textures/cuttedSpriteAtlasTexture-Title_Assets (Group 1)-2048x1024-fmt10.png");
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
        TED_PROFILE_SCOPE("Scene Entities Prep");

        if (m_LoadMainMenu)
            LoadMainMenu(ts);
        else if (m_ProceedToMainMenu)
            StartTransition(ts);
    }

    {
        TED_PROFILE_SCOPE("Renderer Draw (CPU)");

        m_ActiveScene->AlwaysOnUpdate();
        m_ActiveScene->OnUpdateRuntime(ts);
    }
}

void CupheadLayer::LoadMainMenu(Teddy::Timestep ts)
{
    // Load new scene with another black square at 100%
}

void CupheadLayer::StartTransition(Teddy::Timestep ts)
{
	auto& transitionCircle = m_TransitionCircle->GetComponent<Teddy::CircleRendererComponent>();
    auto& transitionQuad = m_TransitionQuad->GetComponent<Teddy::SpriteRendererComponent>();
    if (transitionCircle.Thickness >= 1 && transitionQuad.Color.a >= 1.0f)
		m_LoadMainMenu = true;
    if (!m_Transitioning)
    {
		m_Transitioning = true;
        transitionCircle.Color.a = 1.0f;
    }

	transitionQuad.Color.a += 2.5f * ts;
    transitionCircle.Thickness += 2.0f * ts; 
    
}

bool CupheadLayer::OnKeyPressed(Teddy::KeyPressedEvent& e)
{
	m_ProceedToMainMenu = true;

    return true;
}

void CupheadLayer::OnEvent(Teddy::Event& event)
{
    m_ActiveScene->OnEvent(event);

    Teddy::EventDispatcher dispatcher(event);
    dispatcher.Dispatch<Teddy::KeyPressedEvent>(TED_BIND_EVENT_FN(CupheadLayer::OnKeyPressed));
}

void CupheadLayer::OnImGuiRender()
{
	TED_PROFILE_FUNCTION();
}