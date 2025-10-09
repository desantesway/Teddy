#include "LevelScene.h"

#include <Teddy.h>

namespace Cuphead
{
	Teddy::Ref<Teddy::Scene> LevelScene::Init(unsigned int character)
	{
		m_Scene = Teddy::CreateRef<Teddy::Scene>(true);

		auto camEntt = m_Scene->CreateEntity("Main Menu Camera");
		camEntt.GetComponent<Teddy::TransformComponent>().Translation = { 0.0f, 0.0f, 9.0f };
		auto& cam = camEntt.AddComponent<Teddy::CameraComponent>();
		cam.Camera.SetProjectionType(Teddy::SceneCamera::ProjectionType::Perspective);

		auto& window = Teddy::Application::Get().GetWindow();
		m_Scene->OnViewportResize(window.GetWidth(), window.GetHeight());

		auto& assets = Teddy::AssetManager::Get();

		auto background = m_Scene->CreateEntity("Main Menu Background");
		auto& sprite = background.AddComponent<Teddy::SpriteRendererComponent>();
		sprite.IsBackground = true;
		sprite.Texture = assets.Load<Teddy::Texture2D>("assets/Textures/SpriteAtlasTexture-Dragon_Background-2048x2048-fmt12 #0710017.png", Teddy::Boolean::True);

		background.AddComponent<Teddy::SpriteAtlasComponent>(0, 0, 1520, 780);
		//background.GetComponent<Teddy::TransformComponent>().Scale *= 1.1f;

		return m_Scene;
	}
}