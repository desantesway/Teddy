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
		cam.Camera.GetWidthAndHeight(m_WorldWidth, m_WorldHeight);

		InitPhase1();
		
		return m_Scene;
	}

	void LevelScene::InitPhase1()
	{
		InitPhase1Background();
	}

	void LevelScene::InitPhase1Background()
	{
		auto& assets = Teddy::AssetManager::Get();

		// Background Carousel
		{
			m_Background.LeftBackground = m_Scene->CreateEntity("Main Menu Background");
			auto& sprite = m_Background.LeftBackground.AddComponent<Teddy::SpriteRendererComponent>();
			sprite.IsBackground = true;
			sprite.Texture = assets.Load<Teddy::Texture2D>("assets/Textures/SpriteAtlasTexture-Dragon_Background-2048x2048-fmt12 #0710017.png", Teddy::Boolean::True);
			m_Background.LeftBackground.AddComponent<Teddy::SpriteAtlasComponent>(0, 0, 1520, 780);
			m_Background.LeftBackground.GetComponent<Teddy::TransformComponent>().Translation = glm::vec3(-14.5f, 0.0f, 0.01f);

			m_Background.RightBackground = m_Scene->CreateEntity("Main Menu Background");
			auto& rsprite = m_Background.RightBackground.AddComponent<Teddy::SpriteRendererComponent>();
			rsprite.IsBackground = true;
			rsprite.Texture = assets.Load<Teddy::Texture2D>("assets/Textures/SpriteAtlasTexture-Dragon_Background-2048x2048-fmt12 #0710017.png", Teddy::Boolean::True);
			m_Background.RightBackground.AddComponent<Teddy::SpriteAtlasComponent>(0, 0, 1520, 780);
			m_Background.RightBackground.GetComponent<Teddy::TransformComponent>().Translation = glm::vec3(0.0f, 0.0f, 0.001f);
		}

		// Spire
		{
			m_Background.Spire = m_Scene->CreateEntity("Phase 1 Spire");
			auto& spireSprite = m_Background.Spire.AddComponent<Teddy::SpriteAnimationComponent>(0.125f, 0.125f, 0.125f);

			std::vector<std::string> paths;
			for (int i = 0; i <= 35; i++)
				paths.push_back("assets/Textures/Dragon/Spire/Spire_402x1026_2048x2048_" + std::to_string(i) + ".png");

			spireSprite.Textures = assets.LoadMultiple<Teddy::Texture2D>(paths);
			m_Background.Spire.AddComponent<Teddy::SpriteAtlasComponent>(0, 0, 402, 1026);
			auto& spireTransform = m_Background.Spire.GetComponent<Teddy::TransformComponent>();
			spireTransform.Translation = glm::vec3(0.0f, -0.625f, 1.0f);
			spireTransform.Scale *= 10.0f;
		}

		// Background cloud 1
		{ // faster than cloud 2
			m_Background.Cloud1Right = m_Scene->CreateEntity("Phase 1 Cloud 1 Right");
			auto& cloud1Sprite = m_Background.Cloud1Right.AddComponent<Teddy::SpriteRendererComponent>();
			cloud1Sprite.Texture = assets.Load<Teddy::Texture2D>("assets/Textures/Dragon/Background/clouds_2047x374_2048x2048_0.png", Teddy::Boolean::True);
			m_Background.Cloud1Right.AddComponent<Teddy::SpriteAtlasComponent>(0, 0, 2047, 374);
			auto& cloud1Transform = m_Background.Cloud1Right.GetComponent<Teddy::TransformComponent>();
			cloud1Transform.Translation = glm::vec3(-10.0f, 0.75f, 0.5f);
			cloud1Transform.Scale *= 7.0f;

			m_Background.Cloud1Left = m_Scene->CreateEntity("Phase 1 Cloud 1 Left");
			auto& cloud1LeftSprite = m_Background.Cloud1Left.AddComponent<Teddy::SpriteRendererComponent>();
			cloud1LeftSprite.Texture = assets.Load<Teddy::Texture2D>("assets/Textures/Dragon/Background/clouds_2047x374_2048x2048_0.png", Teddy::Boolean::True);
			m_Background.Cloud1Left.AddComponent<Teddy::SpriteAtlasComponent>(0, 0, 2047, 374);
			auto& cloud1LeftTransform = m_Background.Cloud1Left.GetComponent<Teddy::TransformComponent>();
			cloud1LeftTransform.Translation = glm::vec3(-45.0f, 0.75f, 0.5f);
			cloud1LeftTransform.Scale *= 7.0f;

		}

		// Background cloud 2
		{
			m_Background.Cloud2Right = m_Scene->CreateEntity("Phase 1 Cloud 2 Right");
			auto& cloud2Sprite = m_Background.Cloud2Right.AddComponent<Teddy::SpriteRendererComponent>();
			cloud2Sprite.Texture = assets.Load<Teddy::Texture2D>("assets/Textures/Dragon/Background/clouds_2047x374_2048x2048_0.png", Teddy::Boolean::True);
			m_Background.Cloud2Right.AddComponent<Teddy::SpriteAtlasComponent>(0, 1, 2047, 374);
			auto& cloud2Transform = m_Background.Cloud2Right.GetComponent<Teddy::TransformComponent>();
			cloud2Transform.Translation = glm::vec3(-5.0f, -1.5f, 0.6f);
			cloud2Transform.Scale *= 4.0f;

			m_Background.Cloud2Left = m_Scene->CreateEntity("Phase 1 Cloud 2 Left");
			auto& cloud2LeftSprite = m_Background.Cloud2Left.AddComponent<Teddy::SpriteRendererComponent>();
			cloud2LeftSprite.Texture = assets.Load<Teddy::Texture2D>("assets/Textures/Dragon/Background/clouds_2047x374_2048x2048_0.png", Teddy::Boolean::True);
			m_Background.Cloud2Left.AddComponent<Teddy::SpriteAtlasComponent>(0, 1, 2047, 374);
			auto& cloud2LeftTransform = m_Background.Cloud2Left.GetComponent<Teddy::TransformComponent>();
			cloud2LeftTransform.Translation = glm::vec3(-27.5f, -1.5f, 0.6f);
			cloud2LeftTransform.Scale *= 4.0f;
		}
	}

	void LevelScene::OnUpdate(Teddy::Timestep ts)
	{
		m_MovementSpeed = m_MovementVelocity * ts;

		switch (m_Phase)
		{
		case 1:
			OnUpdatePhase1();
			break;
		default:
			break;
		}
		
	}

	void LevelScene::OnUpdatePhase1()
	{
		// Move Background
		{
			auto& leftTransform = m_Background.LeftBackground.GetComponent<Teddy::TransformComponent>();
			leftTransform.Translation.x += m_MovementSpeed;

			auto& rightTransform = m_Background.RightBackground.GetComponent<Teddy::TransformComponent>();
			rightTransform.Translation.x += m_MovementSpeed;

			if (leftTransform.Translation.x >= 0.0f || rightTransform.Translation.x >= 14.5f)
			{
				leftTransform.Translation.x = -14.5f;
				rightTransform.Translation.x = 0.0f;
			}
		}

		// Cloud1
		{
			auto& cloud1Transform = m_Background.Cloud1Right.GetComponent<Teddy::TransformComponent>();
			cloud1Transform.Translation.x += m_MovementSpeed * 0.9f;

			auto& cloud1LeftTransform = m_Background.Cloud1Left.GetComponent<Teddy::TransformComponent>();
			cloud1LeftTransform.Translation.x += m_MovementSpeed * 0.9f;

			if (cloud1Transform.Translation.x >= 25.0f)
			{
				cloud1Transform.Translation.x = -10.0f;
				cloud1LeftTransform.Translation.x = -45.0f;
			}
		}

		// Cloud 2
		{
			auto& cloud2Transform = m_Background.Cloud2Right.GetComponent<Teddy::TransformComponent>();
			cloud2Transform.Translation.x += m_MovementSpeed * 0.85f;

			auto& cloud2LeftTransform = m_Background.Cloud2Left.GetComponent<Teddy::TransformComponent>();
			cloud2LeftTransform.Translation.x += m_MovementSpeed * 0.85f;

			if (cloud2Transform.Translation.x >= 17.5f)
			{
				cloud2Transform.Translation.x = -5.0f;
				cloud2LeftTransform.Translation.x = -27.5f;
			}

		}
	}
}