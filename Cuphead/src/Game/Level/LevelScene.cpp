#include "LevelScene.h"

#include <Teddy.h>

#include "LevelCategories.h"

namespace Cuphead
{
	Teddy::Ref<Teddy::Scene> LevelScene::Init(unsigned int character)
	{
		m_Scene = Teddy::CreateRef<Teddy::Scene>();
		
		auto camEntt = m_Scene->CreateEntity("Main Menu Camera");
		camEntt.GetComponent<Teddy::TransformComponent>().Translation = { 0.0f, 0.0f, 9.0f };
		auto& cam = camEntt.AddComponent<Teddy::CameraComponent>();
		cam.Camera.SetProjectionType(Teddy::SceneCamera::ProjectionType::Perspective);

		auto& window = Teddy::Application::Get().GetWindow();
		m_Scene->OnViewportResize(window.GetWidth(), window.GetHeight());
		cam.Camera.GetWidthAndHeight(m_WorldWidth, m_WorldHeight);

		auto floor = m_Scene->CreateEntity("Level Floor");  // Turn this into a sensor later // or contact and when on contact give damage to player
		//auto& floorSprite = floor.AddComponent<Teddy::SpriteRendererComponent>();
		auto& floorCollider = floor.AddComponent<Teddy::BoxCollider2DComponent>();
		floorCollider.EnableSensorEvents = true;
		floorCollider.Friction = 0.0f;
		floor.AddComponent<Teddy::Rigidbody2DComponent>();
		auto& floorTransform = floor.GetComponent<Teddy::TransformComponent>();
		floorTransform.Translation = glm::vec3(0.0f, -3.4f, 2.0f);
		floorTransform.Scale = glm::vec3(15.0f, 1.0f, 1.0f);
		auto& filter = floor.AddComponent<Teddy::CollisionFilter2DComponent>();
		filter.CategoryBits = LevelCategories::INVISIBLEWALLS;
		filter.MaskBits = LevelCategories::PLAYER | LevelCategories::PLAYERGHOST;

		auto lWall = m_Scene->CreateEntity("Level Left Wall"); 
		//lWall.AddComponent<Teddy::SpriteRendererComponent>();
		auto& body = lWall.AddComponent<Teddy::BoxCollider2DComponent>();
		body.Friction = 0.0f;
		lWall.AddComponent<Teddy::Rigidbody2DComponent>();
		auto& lWallTransform = lWall.GetComponent<Teddy::TransformComponent>();
		lWallTransform.Translation = glm::vec3(-5.75f,0.0f, 2.0f);
		lWallTransform.Scale = glm::vec3(1.0f, 15.0f, 1.0f);
		auto& lWallfilter = lWall.AddComponent<Teddy::CollisionFilter2DComponent>();
		lWallfilter.CategoryBits = LevelCategories::INVISIBLEWALLS;
		lWallfilter.MaskBits = LevelCategories::PLAYER | LevelCategories::PLAYERGHOST;

		auto rWall = m_Scene->CreateEntity("Level Right Wall");
		//rWall.AddComponent<Teddy::SpriteRendererComponent>();
		auto& rBody = rWall.AddComponent<Teddy::BoxCollider2DComponent>();
		rBody.Friction = 0.0f;
		rWall.AddComponent<Teddy::Rigidbody2DComponent>();
		auto& rWallTransform = rWall.GetComponent<Teddy::TransformComponent>();
		rWallTransform.Translation = glm::vec3(5.75f, 0.0f, 2.0f);
		rWallTransform.Scale = glm::vec3(1.0f, 15.0f, 1.0f);
		auto& rWallfilter = rWall.AddComponent<Teddy::CollisionFilter2DComponent>();
		rWallfilter.CategoryBits = LevelCategories::INVISIBLEWALLS;
		rWallfilter.MaskBits = LevelCategories::PLAYER | LevelCategories::PLAYERGHOST;

		m_Clouds.Init(m_Scene);

		InitPhase1();

		if (character == 1)
			m_Player.InitCuphead(m_Scene);
		else
			m_Player.InitMugman(m_Scene);

		return m_Scene;
	}

	void LevelScene::InitPhase1()
	{
		InitPhase1Background();
		InitPhase1Foreground();
		m_Clouds.InitPhase1();
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
			spireSprite.Pause = true;
			std::vector<std::string> paths;
			for (int i = 0; i <= 35; i++)
				paths.push_back("assets/Textures/Dragon/Spire/Spire_402x1026_2048x2048_" + std::to_string(i) + ".png");

			spireSprite.Textures = assets.LoadMultiple<Teddy::Texture2D>(paths);
			m_Background.Spire.AddComponent<Teddy::SpriteAtlasComponent>(0, 0, 402, 1026);
			auto& spireTransform = m_Background.Spire.GetComponent<Teddy::TransformComponent>();
			spireTransform.Translation = glm::vec3(0.0f, -0.5f, 1.0f);
			spireTransform.Scale *= 9.5f;
		}

		// Background cloud 1
		{ 
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

		// Background cloud 3
		{
			m_Background.Cloud3Right = m_Scene->CreateEntity("Phase 1 Cloud 3 Right");
			auto& cloud3Sprite = m_Background.Cloud3Right.AddComponent<Teddy::SpriteRendererComponent>();
			cloud3Sprite.Texture = assets.Load<Teddy::Texture2D>("assets/Textures/Dragon/Background/clouds_2047x374_2048x2048_0.png", Teddy::Boolean::True);
			m_Background.Cloud3Right.AddComponent<Teddy::SpriteAtlasComponent>(0, 2, 2047, 374);
			auto& cloud3Transform = m_Background.Cloud3Right.GetComponent<Teddy::TransformComponent>();
			cloud3Transform.Translation = glm::vec3(-10.0f, -1.5f, 0.7f);
			cloud3Transform.Scale *= 6.0f;

			m_Background.Cloud3Left = m_Scene->CreateEntity("Phase 1 Cloud 3 Left");
			auto& cloud3LeftSprite = m_Background.Cloud3Left.AddComponent<Teddy::SpriteRendererComponent>();
			cloud3LeftSprite.Texture = assets.Load<Teddy::Texture2D>("assets/Textures/Dragon/Background/clouds_2047x374_2048x2048_0.png", Teddy::Boolean::True);
			m_Background.Cloud3Left.AddComponent<Teddy::SpriteAtlasComponent>(0, 2, 2047, 374);
			auto& cloud3LeftTransform = m_Background.Cloud3Left.GetComponent<Teddy::TransformComponent>();
			cloud3LeftTransform.Translation = glm::vec3(-45.0f, -1.5f, 0.7f);
			cloud3LeftTransform.Scale *= 6.0f;
		}

		// Background cloud 4
		{
			m_Background.Cloud4Right = m_Scene->CreateEntity("Phase 1 Cloud 4 Right");
			auto& cloud4Sprite = m_Background.Cloud4Right.AddComponent<Teddy::SpriteRendererComponent>();
			cloud4Sprite.Texture = assets.Load<Teddy::Texture2D>("assets/Textures/Dragon/Background/clouds_2047x374_2048x2048_0.png", Teddy::Boolean::True);
			m_Background.Cloud4Right.AddComponent<Teddy::SpriteAtlasComponent>(0, 3, 2047, 374);
			auto& cloud4Transform = m_Background.Cloud4Right.GetComponent<Teddy::TransformComponent>();
			cloud4Transform.Translation = glm::vec3(0.0f, -3.0f, 0.8f);
			cloud4Transform.Scale *= 5.0f;

			m_Background.Cloud4Left = m_Scene->CreateEntity("Phase 1 Cloud 4 Left");
			auto& cloud4LeftSprite = m_Background.Cloud4Left.AddComponent<Teddy::SpriteRendererComponent>();
			cloud4LeftSprite.Texture = assets.Load<Teddy::Texture2D>("assets/Textures/Dragon/Background/clouds_2047x374_2048x2048_0.png", Teddy::Boolean::True);
			m_Background.Cloud4Left.AddComponent<Teddy::SpriteAtlasComponent>(0, 3, 2047, 374);
			auto& cloud4LeftTransform = m_Background.Cloud4Left.GetComponent<Teddy::TransformComponent>();
			cloud4LeftTransform.Translation = glm::vec3(-27.5f, -3.0f, 0.8f);
			cloud4LeftTransform.Scale *= 5.0f;
		}

		// Background cloud 5
		{
			m_Background.Cloud5Right = m_Scene->CreateEntity("Phase 1 Cloud 5 Right");
			auto& cloud5Sprite = m_Background.Cloud5Right.AddComponent<Teddy::SpriteRendererComponent>();
			cloud5Sprite.Texture = assets.Load<Teddy::Texture2D>("assets/Textures/Dragon/Background/clouds_2047x374_2048x2048_0.png", Teddy::Boolean::True);
			m_Background.Cloud5Right.AddComponent<Teddy::SpriteAtlasComponent>(0, 4, 2047, 374);
			auto& cloud5Transform = m_Background.Cloud5Right.GetComponent<Teddy::TransformComponent>();
			cloud5Transform.Translation = glm::vec3(0.0f, -2.5f, 0.9f);
			cloud5Transform.Scale *= 4.0f;

			m_Background.Cloud5Left = m_Scene->CreateEntity("Phase 1 Cloud 5 Left");
			auto& cloud5LeftSprite = m_Background.Cloud5Left.AddComponent<Teddy::SpriteRendererComponent>();
			cloud5LeftSprite.Texture = assets.Load<Teddy::Texture2D>("assets/Textures/Dragon/Background/clouds_2047x374_2048x2048_0.png", Teddy::Boolean::True);
			m_Background.Cloud5Left.AddComponent<Teddy::SpriteAtlasComponent>(0, 4, 2047, 374);
			auto& cloud5LeftTransform = m_Background.Cloud5Left.GetComponent<Teddy::TransformComponent>();
			cloud5LeftTransform.Translation = glm::vec3(-17.5f, -2.5f, 0.9f);
			cloud5LeftTransform.Scale *= 4.0f;
		}
	}

	void LevelScene::InitPhase1Foreground()
	{
		// one bellow dragon, 2 above dragon
		auto& assets = Teddy::AssetManager::Get();

		// Foreground cloud 1
		{
			m_Foreground.Cloud1Right = m_Scene->CreateEntity("Phase 1 Foreground Cloud 1 Right");
			auto& cloud1Sprite = m_Foreground.Cloud1Right.AddComponent<Teddy::SpriteRendererComponent>();
			cloud1Sprite.Texture = assets.Load<Teddy::Texture2D>("assets/Textures/Dragon/Foreground/clouds_1561x198_2048x2048_0.png", Teddy::Boolean::True);
			m_Foreground.Cloud1Right.AddComponent<Teddy::SpriteAtlasComponent>(0, 0, 1561, 198);
			auto& cloud1Transform = m_Foreground.Cloud1Right.GetComponent<Teddy::TransformComponent>();
			cloud1Transform.Translation = glm::vec3(13.75f, -2.75f, 1.1f);
			cloud1Transform.Scale *= 1.75f;

			m_Foreground.Cloud1Left = m_Scene->CreateEntity("Phase 1 Foreground Cloud 1 Left");
			auto& cloud1LeftSprite = m_Foreground.Cloud1Left.AddComponent<Teddy::SpriteRendererComponent>();
			cloud1LeftSprite.Texture = assets.Load<Teddy::Texture2D>("assets/Textures/Dragon/Foreground/clouds_1561x198_2048x2048_0.png", Teddy::Boolean::True);
			m_Foreground.Cloud1Left.AddComponent<Teddy::SpriteAtlasComponent>(0, 0, 1561, 198);
			auto& cloud1LeftTransform = m_Foreground.Cloud1Left.GetComponent<Teddy::TransformComponent>();
			cloud1LeftTransform.Translation = glm::vec3(0.0f, -2.75f, 1.1f);
			cloud1LeftTransform.Scale *= 1.75f;
		}

		// Foreground cloud 2
		{
			m_Foreground.Cloud2Right = m_Scene->CreateEntity("Phase 1 Foreground Cloud 2 Right");
			auto& cloud2Sprite = m_Foreground.Cloud2Right.AddComponent<Teddy::SpriteRendererComponent>();
			cloud2Sprite.Texture = assets.Load<Teddy::Texture2D>("assets/Textures/Dragon/Foreground/clouds_1561x198_2048x2048_0.png", Teddy::Boolean::True);
			m_Foreground.Cloud2Right.AddComponent<Teddy::SpriteAtlasComponent>(0, 1, 1561, 198);
			auto& cloud2Transform = m_Foreground.Cloud2Right.GetComponent<Teddy::TransformComponent>();
			cloud2Transform.Translation = glm::vec3(11.25f, -2.375f, 2.1f);
			cloud2Transform.Scale *= 1.5f;

			m_Foreground.Cloud2Left = m_Scene->CreateEntity("Phase 1 Foreground Cloud 2 Left");
			auto& cloud2LeftSprite = m_Foreground.Cloud2Left.AddComponent<Teddy::SpriteRendererComponent>();
			cloud2LeftSprite.Texture = assets.Load<Teddy::Texture2D>("assets/Textures/Dragon/Foreground/clouds_1561x198_2048x2048_0.png", Teddy::Boolean::True);
			m_Foreground.Cloud2Left.AddComponent<Teddy::SpriteAtlasComponent>(0, 1, 1561, 198);
			auto& cloud2LeftTransform = m_Foreground.Cloud2Left.GetComponent<Teddy::TransformComponent>();
			cloud2LeftTransform.Translation = glm::vec3(0.0f, -2.375f, 2.1f);
			cloud2LeftTransform.Scale *= 1.5f;
		}

		// Foreground cloud 3
		{
			m_Foreground.Cloud3Right = m_Scene->CreateEntity("Phase 1 Foreground Cloud 3 Right");
			auto& cloud3Sprite = m_Foreground.Cloud3Right.AddComponent<Teddy::SpriteRendererComponent>();
			cloud3Sprite.Texture = assets.Load<Teddy::Texture2D>("assets/Textures/Dragon/Foreground/clouds_1561x198_2048x2048_0.png", Teddy::Boolean::True);
			m_Foreground.Cloud3Right.AddComponent<Teddy::SpriteAtlasComponent>(0, 2, 1561, 198);
			auto& cloud3Transform = m_Foreground.Cloud3Right.GetComponent<Teddy::TransformComponent>();
			cloud3Transform.Translation = glm::vec3(8.75f, -2.3f, 3.1f);
			cloud3Transform.Scale *= 1.25f;

			m_Foreground.Cloud3Left = m_Scene->CreateEntity("Phase 1 Foreground Cloud 3 Left");
			auto& cloud3LeftSprite = m_Foreground.Cloud3Left.AddComponent<Teddy::SpriteRendererComponent>();
			cloud3LeftSprite.Texture = assets.Load<Teddy::Texture2D>("assets/Textures/Dragon/Foreground/clouds_1561x198_2048x2048_0.png", Teddy::Boolean::True);
			m_Foreground.Cloud3Left.AddComponent<Teddy::SpriteAtlasComponent>(0, 2, 1561, 198);
			auto& cloud3LeftTransform = m_Foreground.Cloud3Left.GetComponent<Teddy::TransformComponent>();
			cloud3LeftTransform.Translation = glm::vec3(0.0f, -2.3f, 3.1f);
			cloud3LeftTransform.Scale *= 1.25f;
		}
	}

	void LevelScene::OnUpdate(Teddy::Timestep ts)
	{
		static bool increasingSpeed = false;
		if (m_Player.IsIntroDone())
		{
			static float timer = 0.0f;
			timer += ts;
			m_MovementSpeed = m_MovementVelocity * ts;
			m_MovementVelocity = 0.25f * timer;
			increasingSpeed = true;
			if (m_MovementVelocity > 1.1f)
			{
				m_MovementVelocity = 1.1f;
				increasingSpeed = false;
			}

			m_Clouds.SetMovementSpeed(m_MovementSpeed);
		}
		else
		{
			m_MovementSpeed = 0.0f;
			increasingSpeed = false;
		}
		if (increasingSpeed)
		{
			auto& spireSprite = m_Background.Spire.GetComponent<Teddy::SpriteAnimationComponent>();

			constexpr float maxFrameTime = 0.3f;
			constexpr float minFrameTime = 0.125f;

			float frameT = maxFrameTime - (m_MovementVelocity / 1.1f) * (maxFrameTime - minFrameTime);
			spireSprite.FrameTime = std::max(frameT, minFrameTime);
			spireSprite.InitialFrameTime = spireSprite.FrameTime;
			spireSprite.FinalFrameTime = spireSprite.FrameTime;
			spireSprite.Pause = false;
		}

		m_Player.OnUpdate(ts);
		m_Clouds.SetPlayerY(m_Player.GetPosition().y);

		switch (m_Phase)
		{
		case 1:
			OnUpdatePhase1();
			break;
		default:
			break;
		}

		m_Clouds.OnUpdate(ts);
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

		// Background Cloud1
		{
			auto& cloud1Transform = m_Background.Cloud1Right.GetComponent<Teddy::TransformComponent>();
			cloud1Transform.Translation.x += m_MovementSpeed * 0.8f;

			auto& cloud1LeftTransform = m_Background.Cloud1Left.GetComponent<Teddy::TransformComponent>();
			cloud1LeftTransform.Translation.x += m_MovementSpeed * 0.8f;

			if (cloud1Transform.Translation.x >= 25.0f)
			{
				cloud1Transform.Translation.x = -10.0f;
				cloud1LeftTransform.Translation.x = -45.0f;
			}
		}

		// Background Cloud 2
		{
			auto& cloud2Transform = m_Background.Cloud2Right.GetComponent<Teddy::TransformComponent>();
			cloud2Transform.Translation.x += m_MovementSpeed * 0.75f;

			auto& cloud2LeftTransform = m_Background.Cloud2Left.GetComponent<Teddy::TransformComponent>();
			cloud2LeftTransform.Translation.x += m_MovementSpeed * 0.75f;

			if (cloud2Transform.Translation.x >= 17.5f)
			{
				cloud2Transform.Translation.x = -5.0f;
				cloud2LeftTransform.Translation.x = -27.5f;
			}

		}

		// Background Cloud 3
		{
			auto& cloud3Transform = m_Background.Cloud3Right.GetComponent<Teddy::TransformComponent>();
			cloud3Transform.Translation.x += m_MovementSpeed * 0.6f;
			auto& cloud3LeftTransform = m_Background.Cloud3Left.GetComponent<Teddy::TransformComponent>();
			cloud3LeftTransform.Translation.x += m_MovementSpeed * 0.6f;
			if (cloud3Transform.Translation.x >= 25.0f)
			{
				cloud3Transform.Translation.x = -10.0f;
				cloud3LeftTransform.Translation.x = -45.0f;
			}
		}

		// Background Cloud 4
		{
			auto& cloud4Transform = m_Background.Cloud4Right.GetComponent<Teddy::TransformComponent>();
			cloud4Transform.Translation.x += m_MovementSpeed * 0.5f;
			auto& cloud4LeftTransform = m_Background.Cloud4Left.GetComponent<Teddy::TransformComponent>();
			cloud4LeftTransform.Translation.x += m_MovementSpeed * 0.5f;
			if (cloud4Transform.Translation.x >= 27.5f)
			{
				cloud4Transform.Translation.x = 0.0f;
				cloud4LeftTransform.Translation.x = -27.5f;
			}
		}

		// Background Cloud 5
		{
			auto& cloud5Transform = m_Background.Cloud5Right.GetComponent<Teddy::TransformComponent>();
			cloud5Transform.Translation.x += m_MovementSpeed * 0.25f;
			auto& cloud5LeftTransform = m_Background.Cloud5Left.GetComponent<Teddy::TransformComponent>();
			cloud5LeftTransform.Translation.x += m_MovementSpeed * 0.25f;
			if (cloud5Transform.Translation.x >= 17.5f)
			{
				cloud5Transform.Translation.x = 0.0f;
				cloud5LeftTransform.Translation.x = -17.5f;
			}
		}

		// Foreground Cloud 1
		{
			auto& cloud1Transform = m_Foreground.Cloud1Right.GetComponent<Teddy::TransformComponent>();
			cloud1Transform.Translation.x -= m_MovementSpeed * 0.6f;
			auto& cloud1LeftTransform = m_Foreground.Cloud1Left.GetComponent<Teddy::TransformComponent>();
			cloud1LeftTransform.Translation.x -= m_MovementSpeed * 0.6f;
			if (cloud1LeftTransform.Translation.x <= -13.75f)
			{
				cloud1Transform.Translation.x = 13.75f;
				cloud1LeftTransform.Translation.x = 0.0f;
			}
		}

		// Foreground Cloud 2
		{
			auto& cloud2Transform = m_Foreground.Cloud2Right.GetComponent<Teddy::TransformComponent>();
			cloud2Transform.Translation.x -= m_MovementSpeed * 1.5f;
			auto& cloud2LeftTransform = m_Foreground.Cloud2Left.GetComponent<Teddy::TransformComponent>();
			cloud2LeftTransform.Translation.x -= m_MovementSpeed * 1.5f;
			if (cloud2LeftTransform.Translation.x <= -11.25f)
			{
				cloud2Transform.Translation.x = 11.25f;
				cloud2LeftTransform.Translation.x = 0.0f;
			}
		}

		// Foreground Cloud 3
		{
			auto& cloud3Transform = m_Foreground.Cloud3Right.GetComponent<Teddy::TransformComponent>();
			cloud3Transform.Translation.x -= m_MovementSpeed * 2.0f;
			auto& cloud3LeftTransform = m_Foreground.Cloud3Left.GetComponent<Teddy::TransformComponent>();
			cloud3LeftTransform.Translation.x -= m_MovementSpeed * 2.0f;
			if (cloud3LeftTransform.Translation.x <= -8.75f)
			{
				cloud3Transform.Translation.x = 8.75f;
				cloud3LeftTransform.Translation.x = 0.0f;
			}
		}

		//m_Clouds.OnUpdatePhase1(ts);
	}

	void LevelScene::StartIntro()
	{
		m_StartIntro = true;

		m_Player.StartIntro();
	}

	void LevelScene::LoadIntro()
	{
		m_Player.LoadIntro();
	}
	
	bool LevelScene::OnContactBegin(Teddy::ContactBeginEvent& e)
	{
		b2ShapeId playerCollider = *static_cast<b2ShapeId*>(m_Player.GetEntity().GetComponent<Teddy::BoxCollider2DComponent>().RuntimeFixture);
		if (B2_ID_EQUALS(e.GetShapeA(), playerCollider))
		{
			m_Clouds.CloudContactBegin(e.GetShapeB());
			return true;
		}
		else if (B2_ID_EQUALS(e.GetShapeB(), playerCollider))
		{
			m_Clouds.CloudContactBegin(e.GetShapeA());
			return true;
		}

		return false;
	}

	bool LevelScene::OnContactEnd(Teddy::ContactEndEvent& e)
	{
		b2ShapeId playerCollider = *static_cast<b2ShapeId*>(m_Player.GetEntity().GetComponent<Teddy::BoxCollider2DComponent>().RuntimeFixture);
		if (B2_ID_EQUALS(e.GetShapeA(), playerCollider))
		{
			m_Clouds.CloudContactEnd(e.GetShapeB());
			return true;
		}
		else if (B2_ID_EQUALS(e.GetShapeB(), playerCollider))
		{
			m_Clouds.CloudContactEnd(e.GetShapeA());
			return true;
		}

		return false;
	}

	bool LevelScene::OnSensorBegin(Teddy::SensorBeginEvent& e)
	{
		b2ShapeId playerSensor = *static_cast<b2ShapeId*>(m_Player.GetEntity().GetComponent<Teddy::Sensor2DComponent>().Sensors["GroundSensor"].RuntimeFixture);
		if (B2_ID_EQUALS(e.GetSensorShape(), playerSensor) || B2_ID_EQUALS(e.GetVisitorShape(), playerSensor))
		{
			m_Player.SetGrounded(true);
			return true;
		}

		return false;
	}

	bool LevelScene::OnSensorEnd(Teddy::SensorEndEvent& e)
	{
		if (m_Player.GetEntity().GetComponent<Teddy::Sensor2DComponent>().Sensors["GroundSensor"].RuntimeFixture)
		{
			b2ShapeId playerSensor = *static_cast<b2ShapeId*>(m_Player.GetEntity().GetComponent<Teddy::Sensor2DComponent>().Sensors["GroundSensor"].RuntimeFixture);
			if (B2_ID_EQUALS(e.GetSensorShape(), playerSensor) || B2_ID_EQUALS(e.GetVisitorShape(), playerSensor))
			{
				m_Player.SetGrounded(false);
				return true;
			}
		}

		return false;
	}

	void LevelScene::OnEvent(Teddy::Event& event)
	{
		m_Player.OnEvent(event);

		Teddy::EventDispatcher dispatcher(event);
		dispatcher.Dispatch<Teddy::SensorBeginEvent>(TED_BIND_EVENT_FN(LevelScene::OnSensorBegin));
		dispatcher.Dispatch<Teddy::SensorEndEvent>(TED_BIND_EVENT_FN(LevelScene::OnSensorEnd));
		dispatcher.Dispatch<Teddy::ContactBeginEvent>(TED_BIND_EVENT_FN(LevelScene::OnContactBegin));
		dispatcher.Dispatch<Teddy::ContactEndEvent>(TED_BIND_EVENT_FN(LevelScene::OnContactEnd));
	}
}