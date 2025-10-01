#include "MainMenuScene.h"

#include <Teddy.h>

namespace Cuphead
{
	Teddy::Ref<Teddy::Scene> MainMenuScene::Init()
	{
		Teddy::Ref<Teddy::Scene> initialScene = Teddy::CreateRef<Teddy::Scene>();
		initialScene->OnRuntimeStart();

		auto camEntt = initialScene->CreateEntity("Main Menu Camera");
		camEntt.GetComponent<Teddy::TransformComponent>().Translation = { 0.0f, 0.0f, 9.0f };
		auto& cam = camEntt.AddComponent<Teddy::CameraComponent>();
		cam.Camera.SetProjectionType(Teddy::SceneCamera::ProjectionType::Perspective);
		cam.Camera.SetViewportSize(1920, 1080); // TODO: Viewport sharing + other scenes

		// Background
		auto& assets = Teddy::AssetManager::Get();

		auto background = initialScene->CreateEntity("Main Menu Background");
		auto& sprite = background.AddComponent<Teddy::SpriteRendererComponent>();
		sprite.IsBackground = true;
		sprite.Texture = assets.Load<Teddy::Texture2D>("Main Menu Background", "assets/Textures/SpriteAtlasTexture-Slot_Select_BG-2048x1024-fmt10.png");
		
		background.AddComponent<Teddy::SpriteAtlasComponent>(0, 0, 1410, 840);
		background.GetComponent<Teddy::TransformComponent>().Scale *= 1.1f;
		
		// 1410 x 840
		return initialScene;
	}
}