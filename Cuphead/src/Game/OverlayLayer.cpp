#include "OverlayLayer.h"

OverlayLayer::OverlayLayer()
	: Layer("Main Layer")
{

}

void OverlayLayer::OnAttach()
{
	TED_PROFILE_FUNCTION();

	m_ActiveScene = Teddy::CreateRef<Teddy::Scene>();
    m_ActiveScene->OnRuntimeStart();

	auto camEntt = m_ActiveScene->CreateEntity("Camera");
    camEntt.GetComponent<Teddy::TransformComponent>().Translation = { 0.0f, 0.0f, 99.0f };
	auto& cam = camEntt.AddComponent<Teddy::CameraComponent>();
	cam.Camera.SetProjectionType(Teddy::SceneCamera::ProjectionType::Perspective);
	cam.Camera.SetViewportSize(1920, 1080);

	auto& assets = Teddy::AssetManager::Get();

    auto overlay = m_ActiveScene->CreateEntity("Screen fx");
    auto& overlayAnimation = overlay.AddComponent<Teddy::SpriteAnimationComponent>(0.025f, 0.05f, 0.05f);
    auto& overlayTransform = overlay.GetComponent<Teddy::TransformComponent>();
    overlayTransform.Translation = { 0.0f, 0.0f, 98.0f };
    overlayTransform.Scale = {1.0f, 0.012f, 1.0f};
    overlayAnimation.Loop = true;
	overlayAnimation.BlendMode = Teddy::BlendingMode::InvertAlpha;

    std::vector<std::string> overlayScreens;
    for (int i = 0; i <= 126; i++)
    {
		std::string number = std::to_string(i);
		if(number.size() == 4)
			overlayScreens.push_back("assets/textures/cuphead_screen_fx_" + number + ".png");
		else if (number.size() == 3)
			overlayScreens.push_back("assets/textures/cuphead_screen_fx_0" + number + ".png");
		else if (number.size() == 2)
			overlayScreens.push_back("assets/textures/cuphead_screen_fx_00" + number + ".png");
		else if (number.size() == 1)
			overlayScreens.push_back("assets/textures/cuphead_screen_fx_000" + number + ".png");
    }

    overlayAnimation.Textures = assets.LoadMultiple<Teddy::Texture2D>(overlayScreens);
    overlayAnimation.IsBackground = true;
    overlayAnimation.OriginalAspectRatio = true;
}

void OverlayLayer::OnDetach()
{

}

void OverlayLayer::OnUpdate(Teddy::Timestep ts)
{
    TED_PROFILE_FUNCTION();

    {
        TED_PROFILE_SCOPE("Renderer Draw (CPU)");

        m_ActiveScene->AlwaysOnUpdate();
        m_ActiveScene->OnUpdateRuntime(ts);
    }
}

void OverlayLayer::OnEvent(Teddy::Event& event)
{

}

void OverlayLayer::OnImGuiRender()
{

}