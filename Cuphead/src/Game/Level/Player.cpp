#include "Player.h"

#include <Teddy.h>

namespace Cuphead
{
	void Player::OnUpdate(Teddy::Timestep ts)
	{
		DeleteCookie(ts);

		if(m_ZHeld)
			m_ZHeld = Teddy::Input::IsKeyPressed(Teddy::Key::Z);

		switch (m_State)
		{
		case PlayerState::Intro0:
			
			if (m_Entity.GetComponent<Teddy::SpriteAnimationAtlasComponent>().Index == 27)
				StartIdle();
			break;
		case PlayerState::Intro1:
			if (m_Entity.GetComponent<Teddy::SpriteAnimationAtlasComponent>().Index == 73)
				StartIdle();
			break;
		case PlayerState::Intro2:
			if (m_Entity.GetComponent<Teddy::SpriteAnimationAtlasComponent>().Index == 106)
				StartIdle();
			if (m_Entity.GetComponent<Teddy::SpriteAnimationAtlasComponent>().Index == 95)
				BreakCookie();
			break;
		case PlayerState::Running:
			Running();
			break;
		case PlayerState::Crouching:
			Crouching();
			break;
		case PlayerState::Jumping:
			Jumping(ts);
			break;
		case PlayerState::Idle:
			break;
		default:
			break;
		}
	}

	void Player::Crouching()
	{
		static bool running = false;
		static bool transition = true;
		
		auto& animationAtlas = m_Entity.GetComponent<Teddy::SpriteAnimationAtlasComponent>();
		if (transition && animationAtlas.Index == 98)
		{
			auto& animation = m_Entity.GetComponent<Teddy::SpriteAnimationComponent>();
			animation.Loop = true;
			animation.PingPong = true;
			animation.Reverse = true;
			animationAtlas.Index = 98;
			animation.PlayableIndicies = { 97, 98, 99, 100, 101 };
			animation.FinalFrameTime = 0.1f;
			animation.FrameTime = 0.05f;
			animation.InitialFrameTime = 0.1f;

			transition = false;
		}

		if (Teddy::Input::IsKeyPressed(Teddy::Key::S) || Teddy::Input::IsKeyPressed(Teddy::Key::Down))
		{
			m_Entity.GetComponent<Teddy::Rigidbody2DComponent>().SetVelocity(0.0f, 0.0f);
		}
		else
		{
			if (Teddy::Input::IsKeyPressed(Teddy::Key::A) || Teddy::Input::IsKeyPressed(Teddy::Key::Left))
			{
				running = true;
			}

			if (Teddy::Input::IsKeyPressed(Teddy::Key::D) || Teddy::Input::IsKeyPressed(Teddy::Key::Right))
			{
				running = true;
			}

			transition = true;
			if (running)
			{
				m_State = PlayerState::AnimationDone;
				StartRunning();
			}
			else
				StartIdle();
		}
	}

	void Player::Jumping(Teddy::Timestep ts)
	{
		auto& body = m_Entity.GetComponent<Teddy::Rigidbody2DComponent>();

		bool running = false;
		bool crouching = false;
		static bool falling = false;
		static bool keyHeld = false;
		static float time = 0.0f;
		time += ts.GetSeconds();

		static constexpr float maxJumpHoldTime = 0.45f;
		static constexpr float jumpHoldForce = 35.0f;

		if (time < maxJumpHoldTime && keyHeld && m_ZHeld && body.GetVelocity().y > 0)
		{
			body.SetVelocityY(body.GetVelocity().y + jumpHoldForce * ts.GetSeconds());
			keyHeld = true;
		}
		else
			keyHeld = false;

		if (Teddy::Input::IsKeyPressed(Teddy::Key::S) || Teddy::Input::IsKeyPressed(Teddy::Key::Down))
		{
			crouching = true;
		}

		if (Teddy::Input::IsKeyPressed(Teddy::Key::A) || Teddy::Input::IsKeyPressed(Teddy::Key::Left))
		{
			body.SetVelocityX(-5.0f);
			running = true;
		}

		if (Teddy::Input::IsKeyPressed(Teddy::Key::D) || Teddy::Input::IsKeyPressed(Teddy::Key::Right))
		{
			body.SetVelocityX(5.0f);
			running = true;
		}

		if(!running)
			body.SetVelocityX(0.0f);

		if (m_Jump)
		{
			body.SetVelocity(0.0f, 12.5f);
			keyHeld = true;
			m_Jump = false;
			time = 0.0f;
		}
		else
		{				
			if (m_Grounded)
			{
				if(falling)
				{
					if (crouching)
					{
						m_State = PlayerState::DoneJumping;
						StartCrouching();
					}
					else if (running)
					{
						m_State = PlayerState::DoneJumping;
						StartRunning();
					}
					else
						StartIdle();
					falling = false;
					body.SetVelocity(0.0f, 0.0f);
				}
			}
			else
				falling = true;
		}
	}

	void Player::Running()
	{
		if (m_DirectionRight)
		{
			if (Teddy::Input::IsKeyPressed(Teddy::Key::D) || Teddy::Input::IsKeyPressed(Teddy::Key::Right))
			{
				m_Entity.GetComponent<Teddy::Rigidbody2DComponent>().SetVelocityX(5.0f);
			}
			else
			{
				m_Entity.GetComponent<Teddy::Rigidbody2DComponent>().SetVelocityX(0.0f);
				StartIdle();
			}
		}
		else
		{
			if (Teddy::Input::IsKeyPressed(Teddy::Key::A) || Teddy::Input::IsKeyPressed(Teddy::Key::Left))
			{
				m_Entity.GetComponent<Teddy::Rigidbody2DComponent>().SetVelocityX(-5.0f);
			}
			else
			{
				m_Entity.GetComponent<Teddy::Rigidbody2DComponent>().SetVelocityX(0.0f);
				StartIdle();
			}
		}
	}

	void Player::LoadCupheadTextures()
	{
		auto& assets = Teddy::AssetManager::Get();

		m_MovementTextures = assets.LoadMultiple<Teddy::Texture2D>({
			"assets/Textures/Cuphead/Movement/Cuphead_Movement_347x192_2048x2048_0.png",
			"assets/Textures/Cuphead/Movement/Cuphead_Movement_347x192_2048x2048_1.png",
			"assets/Textures/Cuphead/Movement/Cuphead_Movement_347x192_2048x2048_2.png"
			});

		m_IntroTextures = assets.LoadMultiple<Teddy::Texture2D>({
			"assets/Textures/Cuphead/Intros/Cuphead_Intros_212x270_2048x2048_0.png",
			"assets/Textures/Cuphead/Intros/Cuphead_Intros_212x270_2048x2048_1.png"
			});

		m_JumpTextures = assets.LoadMultiple<Teddy::Texture2D>({
			"assets/Textures/Cuphead/Movement/Cuphead_Jump_288x152_2048x2048_0.png"
			});
	}

	void Player::InitCuphead(Teddy::Ref<Teddy::Scene> scene)
	{
		m_Scene = scene;
		m_Entity = m_Scene->CreateEntity("Cuphead Player");

		LoadCupheadTextures();

		InitPlayer();
	}

	void Player::InitMugman(Teddy::Ref<Teddy::Scene> scene)
	{
		m_Scene = scene;
		m_Entity = m_Scene->CreateEntity("Mugman Player");

		LoadCupheadTextures();

		InitPlayer();
	}

	void Player::InitPlayer()
	{
		auto& sprite = m_Entity.AddComponent<Teddy::SpriteAnimationComponent>(0.1f, 0.05f, 0.1f);
		sprite.Textures = m_MovementTextures;
		m_Entity.AddComponent<Teddy::SpriteAtlasComponent>(0, 0, 347, 192);
		auto& transform = m_Entity.GetComponent<Teddy::TransformComponent>();
		transform.Translation = glm::vec3(0.0f, 0.0f, 2.0f);
		transform.Scale = glm::vec3(1.75f, 1.75f, 1.0f);

		sprite.PlayableIndicies = { 84, 85, 86, 87, 88 };
		sprite.PingPong = true;
		sprite.Loop = true;
		m_Entity.GetComponent<Teddy::SpriteAnimationAtlasComponent>().Index = 84;

		// Physics
		auto& body = m_Entity.AddComponent<Teddy::Rigidbody2DComponent>();
		body.Type = Teddy::Rigidbody2DComponent::BodyType::Dynamic;
		body.FixedRotation = true;
		body.GravityScale = 10.0f;
		auto& box = m_Entity.AddComponent<Teddy::BoxCollider2DComponent>();
		box.Offset = { 0.0f, -0.25f };
		box.Size = { 0.2f, 0.3f };

		auto& sensor = m_Entity.AddComponent<Teddy::Sensor2DComponent>();
		sensor.Sensors["GroundSensor"] = { { 0.0f, -0.75f }, { 0.3f, 0.1f }, 0.0f };
	}

	void Player::StartIdle()
	{
		if (!m_Entity.HasComponent<Teddy::SpriteAnimationAtlasComponent>())
			return;

		auto& sprite = m_Entity.GetComponent<Teddy::SpriteAnimationComponent>();
		sprite.Textures = m_MovementTextures;
		sprite.PingPong = true;
		sprite.Loop = true;
		sprite.PlayableIndicies = { 84, 85, 86, 87, 88 };
		sprite.FinalFrameTime = 0.1f;
		sprite.FrameTime = 0.05f;
		sprite.InitialFrameTime = 0.1f;

		auto& atlas = m_Entity.GetComponent<Teddy::SpriteAtlasComponent>();
		atlas.SpriteWidth = 347;
		atlas.SpriteHeight = 192;

		auto& indicies = m_Entity.GetComponent<Teddy::SpriteAnimationAtlasComponent>();
		indicies.GenerateFrames(sprite, atlas);
		indicies.Index = 84;

		auto& transform = m_Entity.GetComponent<Teddy::TransformComponent>();

		if (m_DirectionRight)
			transform.Scale = glm::vec3(1.75f);
		else
			transform.Scale = glm::vec3(-1.75f, 1.75f, 1.0f);

		if (m_State == PlayerState::Intro1 || m_State == PlayerState::Intro2)
		{
			auto& boxCollider = m_Entity.GetComponent<Teddy::BoxCollider2DComponent>();
			boxCollider.Offset = { 0.0f, -0.25f };
			boxCollider.Size = { 0.2f, 0.3f };
			transform.Translation -= glm::vec3(0.0f, 0.2625f, 0.0f);
			indicies.Index = 88;
			m_Scene->RefreshBody(m_Entity);
		}
		else if (m_State == PlayerState::Intro0)
		{
			auto& boxCollider = m_Entity.GetComponent<Teddy::BoxCollider2DComponent>();
			boxCollider.Offset = { 0.0f, -0.25f };
			boxCollider.Size = { 0.2f, 0.3f };
			transform.Translation -= glm::vec3(0.0f, 0.35f, 0.0f);
			indicies.Index = 87;
			m_Scene->RefreshBody(m_Entity);
		}
		else if (m_State == PlayerState::Jumping)
		{
			indicies.Index = 88;
		}
		else if (m_State == PlayerState::Crouching)
		{// TODO: Refresh hitbox only
		}

		m_State = PlayerState::Idle;
	}

	void Player::DeleteCookie(Teddy::Timestep ts)
	{
		static bool fadeOut = false;
		static bool done = false;
		if (!done && m_Cookie)
		{
			auto& color = m_Cookie.GetComponent<Teddy::SpriteAnimationComponent>().Color;
			if (m_Cookie.GetComponent<Teddy::SpriteAnimationAtlasComponent>().Index == 113)
			{
				fadeOut = true;
			}
			if (color.a > 0 && fadeOut)
			{
				color -= ts.GetSeconds();
			}
			if (color.a <= 0)
			{
				fadeOut = false;
				done = true;
				m_Scene->DestroyEntity(m_Cookie);
				m_Cookie = {};
			}
		}
	}

	void Player::BreakCookie()
	{
		static bool created = false;
		if (created)
			return;
		m_Cookie = m_Scene->CreateEntity("Cookie");
		auto& sprite = m_Cookie.AddComponent<Teddy::SpriteAnimationComponent>(0.05f, 0.05f, 0.05f);
		sprite.Textures = m_IntroTextures;
		sprite.PingPong = false;
		sprite.Loop = false;

		auto& atlas = m_Cookie.AddComponent<Teddy::SpriteAtlasComponent>();
		atlas.SpriteWidth = 212;
		atlas.SpriteHeight = 270;

		auto& indicies = m_Cookie.GetComponent<Teddy::SpriteAnimationAtlasComponent>();
		indicies.GenerateFrames(sprite, atlas);
		indicies.Index = 107;
		
		sprite.PlayableIndicies = {107, 108, 109, 110, 111, 112, 113};

		auto& transform = m_Cookie.GetComponent<Teddy::TransformComponent>();
		transform.Translation = glm::vec3(0.2f, -0.15f, 0.01f);

		transform.Translation += m_Entity.GetComponent<Teddy::TransformComponent>().Translation;

		created = true;
	}

	void Player::StartIntro() // TODO: see timing of intro
	{
		if (!m_Entity.HasComponent<Teddy::SpriteAnimationAtlasComponent>())
			return;

		static std::random_device rd;
		static std::mt19937 gen(rd());
		static std::uniform_int_distribution<> distr(0, 2);

		int choice = distr(gen); // TODO: Fix intro 1, x axis is shaken in the atlas
		choice = 0;
		auto& sprite = m_Entity.GetComponent<Teddy::SpriteAnimationComponent>();
		sprite.Textures = m_IntroTextures;
		sprite.PingPong = false;
		sprite.Loop = false;
		sprite.Reverse = false;
		
		auto& atlas = m_Entity.GetComponent<Teddy::SpriteAtlasComponent>();
		atlas.SpriteWidth = 212;
		atlas.SpriteHeight = 270;

		auto& indicies = m_Entity.GetComponent<Teddy::SpriteAnimationAtlasComponent>();
		indicies.GenerateFrames(sprite, atlas);

		sprite.PlayableIndicies.clear();

		auto& transform = m_Entity.GetComponent<Teddy::TransformComponent>();
		auto& boxCollider = m_Entity.GetComponent<Teddy::BoxCollider2DComponent>();
		auto& boxBody = m_Entity.GetComponent<Teddy::Rigidbody2DComponent>();

		switch (choice)
		{
		case 0:
		{
			for (int i = 0; i < 28; i++)
				sprite.PlayableIndicies.push_back(i);
			indicies.Index = 0;
			transform.Scale *= 1.4f;
			m_State = PlayerState::Intro0;
			boxCollider.Offset = { 0.0f, -0.4f };
			break;
		}
		case 1:
		{
			for (int i = 28; i < 74; i++)
				sprite.PlayableIndicies.push_back(i);
			indicies.Index = 28;
			transform.Scale *= 1.3f;
			m_State = PlayerState::Intro1;
			boxCollider.Offset = { 0.0f, -0.35f };
			break;
		}
		case 2:
		{
			for (int i = 74; i < 107; i++)
				sprite.PlayableIndicies.push_back(i);
			indicies.Index = 74;
			transform.Scale *= 1.3f;
			m_State = PlayerState::Intro2;
			boxCollider.Offset = { 0.0f, -0.35f };
			break;
		}
		default:
			for (int i = 28; i < 74; i++)
				sprite.PlayableIndicies.push_back(i);
			indicies.Index = 28;
			transform.Scale *= 1.3f;
			m_State = PlayerState::Intro1;
			boxCollider.Offset = { 0.0f, -0.35f };
			break;
		}

		 m_Scene->RefreshBody(m_Entity);
	}

	// TODO: Verify hit box
	void Player::StartRunning()
	{
		m_Entity.GetComponent<Teddy::TransformComponent>().Scale.x = m_DirectionRight ? 1.75f : -1.75f;

		if (m_State == PlayerState::Running || m_State == PlayerState::Jumping || !m_Grounded || m_State == PlayerState::Crouching) return;

		auto& sprite = m_Entity.GetComponent<Teddy::SpriteAnimationComponent>();
		sprite.Textures = m_MovementTextures;
		sprite.PingPong = false;
		sprite.Loop = true;
		sprite.Reverse = true;

		sprite.FinalFrameTime = 0.05f;
		sprite.FrameTime = 0.05f;
		sprite.InitialFrameTime = 0.05f;

		auto& atlas = m_Entity.GetComponent<Teddy::SpriteAtlasComponent>();
		atlas.SpriteWidth = 347;
		atlas.SpriteHeight = 192;

		auto& indicies = m_Entity.GetComponent<Teddy::SpriteAnimationAtlasComponent>();
		indicies.GenerateFrames(sprite, atlas);

		sprite.PlayableIndicies.clear();
		for (int i = 68; i < 84; i++)
			sprite.PlayableIndicies.push_back(i);
		indicies.Index = 68;

		auto& transform = m_Entity.GetComponent<Teddy::TransformComponent>();

		if (m_DirectionRight)
			transform.Scale = glm::vec3(1.75f);
		else
			transform.Scale = glm::vec3(-1.75f, 1.75f, 1.0f);

		m_State = PlayerState::Running;
	}

	void Player::StartJumping() // TODO: falling without jumping
	{
		if (m_State == PlayerState::Jumping && !m_Grounded) return;

		if (m_ZHeld) return;

		auto& sprite = m_Entity.GetComponent<Teddy::SpriteAnimationComponent>();
		sprite.Textures = m_JumpTextures;
		sprite.PingPong = false;
		sprite.Loop = true;
		sprite.Reverse = false;

		sprite.FinalFrameTime = 0.05f;
		sprite.FrameTime = 0.05f;
		sprite.InitialFrameTime = 0.05f;

		auto& atlas = m_Entity.GetComponent<Teddy::SpriteAtlasComponent>();
		atlas.SpriteWidth = 288;
		atlas.SpriteHeight = 152;

		auto& indicies = m_Entity.GetComponent<Teddy::SpriteAnimationAtlasComponent>();
		indicies.GenerateFrames(sprite, atlas);

		auto& transform = m_Entity.GetComponent<Teddy::TransformComponent>();
		if (transform.Scale.x >= 0)
			transform.Scale = glm::vec3(1.35f);
		else
			transform.Scale = glm::vec3(-1.35f, 1.35f, 1.0f);

		sprite.PlayableIndicies.clear();
		for (int i = 0; i < 8; i++)
			sprite.PlayableIndicies.push_back(i);
		indicies.Index = 0;

		m_ZHeld = true;
		m_Jump = true;
		m_State = PlayerState::Jumping;
	}

	void Player::StartCrouching()
	{
		if (m_State == PlayerState::Crouching || !m_Grounded || m_State == PlayerState::Jumping) return;

		auto& sprite = m_Entity.GetComponent<Teddy::SpriteAnimationComponent>();
		sprite.Textures = m_MovementTextures;
		sprite.PingPong = true;
		sprite.Loop = true;
		sprite.Reverse = false;

		sprite.FinalFrameTime = 0.05f;
		sprite.FrameTime = 0.05f;
		sprite.InitialFrameTime = 0.05f;

		auto& atlas = m_Entity.GetComponent<Teddy::SpriteAtlasComponent>();
		atlas.SpriteWidth = 347;
		atlas.SpriteHeight = 192;

		auto& indicies = m_Entity.GetComponent<Teddy::SpriteAnimationAtlasComponent>();
		indicies.GenerateFrames(sprite, atlas);

		sprite.PlayableIndicies.clear();
		for (int i = 97; i < 110; i++)
			sprite.PlayableIndicies.push_back(i);
		indicies.Index = 109;

		auto& transform = m_Entity.GetComponent<Teddy::TransformComponent>();
		if (transform.Scale.x >= 0)
			transform.Scale = glm::vec3(1.75f);
		else
			transform.Scale = glm::vec3(-1.75f, 1.75f, 1.0f);

		m_State = PlayerState::Crouching;
	}

	bool Player::OnKeyPressed(Teddy::KeyPressedEvent& e)
	{
		switch (e.GetKeyCode())
		{
		case Teddy::Key::A:
		case Teddy::Key::Left:
			m_DirectionRight = false;
			StartRunning();
			return true;
		case Teddy::Key::D:
		case Teddy::Key::Right:
			m_DirectionRight = true;
			StartRunning();
			return true;
		case Teddy::Key::Z:
			StartJumping();
			return true;
		case Teddy::Key::Down:
		case Teddy::Key::S:
			StartCrouching();
			return true;
		default:
			break;
		}

		return false;
	}

	void Player::OnEvent(Teddy::Event& event)
	{
		if (m_State != PlayerState::Intro1 && m_State != PlayerState::Intro2 && m_State != PlayerState::Intro0)
		{
			Teddy::EventDispatcher dispatcher(event);
			dispatcher.Dispatch<Teddy::KeyPressedEvent>(TED_BIND_EVENT_FN(Player::OnKeyPressed));
		}
	}
}