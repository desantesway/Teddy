#include "Player.h"

#include <Teddy.h>

#include "LevelCategories.h"
//TODO:  parry object //
namespace Cuphead
{
	void Player::OnUpdate(Teddy::Timestep ts)
	{
		DeleteCookie(ts);

		if(m_ZHeld)
			m_ZHeld = Teddy::Input::IsKeyPressed(Teddy::Key::Z);

		if (m_ShiftHeld)
			m_ShiftHeld = Teddy::Input::IsKeyPressed(Teddy::Key::LShift);

		if (!m_DashReset)
			m_DashReset = m_Grounded;

		if (!m_ParryReset)
			m_ParryReset = m_Grounded;

		if (m_State != PlayerState::Parrying)
		{
			auto& sensor = m_Entity.GetComponent<Teddy::Sensor2DComponent>();
			if (sensor.Sensors.contains("ParryHitBox"))
			{
				m_Scene->DeleteSensor(sensor.Sensors["ParryHitBox"]);
				sensor.Sensors.erase("ParryHitBox");
			}
		}

		TED_CORE_INFO("m_Health {}", m_Health);

		switch (m_State)
		{
		case PlayerState::Intro0:
			Intro0();
			break;
		case PlayerState::Intro1:
			Intro1();
			break;
		case PlayerState::Intro2:
			Intro2();
			break;
		case PlayerState::Running:
			Move(ts);
			Running();
			BlockMove();
			break;
		case PlayerState::Crouching:
			Crouching();
			break;
		case PlayerState::Jumping:
			Move(ts);
			Jumping(ts);
			BlockMove();
			break;
		case PlayerState::Dashing:
			Dashing(ts);
			break;
		case PlayerState::Falling:
			Move(ts);
			Falling();
			BlockMove();
			break;
		case PlayerState::Dropping:
			Dropping(ts);
			break;
		case PlayerState::Parrying:
			Move(ts);
			Parrying();
			BlockMove();
			break;
		case PlayerState::Idle:
			BlockMove();
			break;
		default:
			break;
		}
	}

	void Player::Intro0()
	{
		static int count = 0;
		static int lastIndex = 0; 
		int index = m_Entity.GetComponent<Teddy::SpriteAnimationAtlasComponent>().Index;
		if (count == 0 && index != lastIndex && index >= 18)
		{
			auto& sprite = m_Entity.GetComponent<Teddy::SpriteAnimationComponent>();
			sprite.PingPong = true;
			sprite.Loop = true;
			sprite.Reverse = true;
			sprite.FrameTime = 0.04f;
			sprite.InitialFrameTime = 0.05f;
			sprite.FinalFrameTime = 0.05f;
			auto& indicies = m_Entity.GetComponent<Teddy::SpriteAnimationAtlasComponent>();
			sprite.PlayableIndicies.clear();
			for (int i = 11; i < 19; i++)
				sprite.PlayableIndicies.push_back(i);
			indicies.Index = 18;
			count++;
		}
		else if (count <= 2 && index >= 18 && index != lastIndex)
		{
			if (count == 2)
			{
				auto& sprite = m_Entity.GetComponent<Teddy::SpriteAnimationComponent>();
				sprite.PingPong = false;
				sprite.Loop = false;
				sprite.Reverse = false;
				sprite.FrameTime = 0.05f;
				sprite.InitialFrameTime = 0.05f;
				sprite.FinalFrameTime = 0.05f;
				auto& indicies = m_Entity.GetComponent<Teddy::SpriteAnimationAtlasComponent>();
				sprite.PlayableIndicies.clear();
				for (int i = 18; i < 28; i++)
					sprite.PlayableIndicies.push_back(i);
				indicies.Index = 18;
			}
			count++;
		}
		else if (index == 27)
			StartIdle();

		lastIndex = index;
	}

	void Player::Intro1()
	{
		int index = m_Entity.GetComponent<Teddy::SpriteAnimationAtlasComponent>().Index;
		auto& sprite = m_Entity.GetComponent<Teddy::SpriteAnimationComponent>();
		auto& transform = m_Entity.GetComponent<Teddy::TransformComponent>();
		static float x = transform.Translation.x;
		if (index == 73)
		{
			StartIdle();
		}
		if (index >= 64)
		{
			transform.Translation.x = x;
			m_Scene->RefreshBody(m_Entity);
		}
		else if (index >= 58)
		{
			transform.Translation.x = x - 0.075f;
			m_Scene->RefreshBody(m_Entity);
		}
	}

	void Player::Intro2()
	{
		int index = m_Entity.GetComponent<Teddy::SpriteAnimationAtlasComponent>().Index;
		if (index == 106)
			StartIdle();
		if (index == 95)
			BreakCookie();
	}

	void Player::BlockMove()
	{
		bool leftPressed = Teddy::Input::IsKeyPressed(Teddy::Key::A) || Teddy::Input::IsKeyPressed(Teddy::Key::Left);
		bool rightPressed = Teddy::Input::IsKeyPressed(Teddy::Key::D) || Teddy::Input::IsKeyPressed(Teddy::Key::Right);

		if (leftPressed && rightPressed)
		{
			if(m_State != PlayerState::Dashing) m_Entity.GetComponent<Teddy::Rigidbody2DComponent>().SetVelocityX(0.0f);
			if (m_State == PlayerState::Running) StartIdle();
			return;
		}
		else
		{
			if (leftPressed)
				StartRun(false);
			if (rightPressed)
				StartRun(true);
		}
	}

	void Player::SetGrounded(bool grounded)
	{
		m_Grounded = grounded; 
		StartFall(); 
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
		transform.Translation = glm::vec3(-4.0f, 0.5f, 2.0f);
		transform.Scale = glm::vec3(1.75f, 1.75f, 1.0f);

		auto& aA = m_Entity.GetComponent<Teddy::SpriteAnimationAtlasComponent>();

		// Physics
		auto& body = m_Entity.AddComponent<Teddy::Rigidbody2DComponent>();
		body.Type = Teddy::Rigidbody2DComponent::BodyType::Dynamic;
		body.FixedRotation = true;
		body.GravityScale = 7.5f;
		auto& box = m_Entity.AddComponent<Teddy::BoxCollider2DComponent>();
		box.Offset = { 0.0f, -0.25f };
		box.Size = { 0.2f, 0.3f };
		box.EnableContactEvents = true;
		auto& sensor = m_Entity.AddComponent<Teddy::Sensor2DComponent>();
		sensor.Sensors["GroundSensor"] = { { 0.0f, -0.75f }, { 0.34f, 0.1f }, 0.0f };
		sensor.Sensors["HitBox"] = { { 0.0f, -0.25f }, { 0.25f, 0.45f }, 0.0f };

		auto& filter = m_Entity.AddComponent<Teddy::CollisionFilter2DComponent>();
		filter.CategoryBits = LevelCategories::PLAYER;
		filter.MaskBits = LevelCategories::INVISIBLEWALLS | LevelCategories::CLOUDPLATFORMON | LevelCategories::ENEMY;

		m_State = PlayerState::Intro1;
	}

	void Player::StartIdle()
	{
		if (m_State == PlayerState::Idle) return;

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

		auto& sensor = m_Entity.GetComponent<Teddy::Sensor2DComponent>();
		sensor.Sensors["HitBox"] = { { 0.0f, -0.25f }, { 0.25f, 0.45f }, 0.0f };
		m_Scene->RefreshSensor(m_Entity, sensor.Sensors["HitBox"]);

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
		sprite.Reverse = false;
		sprite.TextureIndex = 1;
		auto& atlas = m_Cookie.AddComponent<Teddy::SpriteAtlasComponent>(8, 4, 212, 270);

		auto& indicies = m_Cookie.GetComponent<Teddy::SpriteAnimationAtlasComponent>();
		indicies.Index = 107;
		
		sprite.PlayableIndicies = {107, 108, 109, 110, 111, 112, 113};

		auto& transform = m_Cookie.GetComponent<Teddy::TransformComponent>();
		transform.Translation = glm::vec3(0.5f, -0.1f, -0.001f);

		transform.Translation += m_Entity.GetComponent<Teddy::TransformComponent>().Translation;
		transform.Scale = glm::vec3(2.0f, 2.0f, 1.0f);

		created = true;
	}

	void Player::LoadIntro()
	{
		static bool loaded = false;

		if (loaded) return;

		loaded = true;

		if (!m_Entity.HasComponent<Teddy::SpriteAnimationAtlasComponent>())
			return;

		static std::random_device rd;
		static std::mt19937 gen(rd());
		static std::uniform_int_distribution<> distr(0, 2);
		static int choice = distr(gen);

		auto& sprite = m_Entity.GetComponent<Teddy::SpriteAnimationComponent>();
		sprite.Textures = m_IntroTextures;
		sprite.PingPong = false;
		sprite.Loop = false;
		sprite.Reverse = false;
		sprite.Pause = true;
		sprite.Timer = 0.0f;
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
			transform.Scale = glm::vec3(1.75f, 1.75f, 1.0f) * 1.4f;
			m_State = PlayerState::Intro0;
			boxCollider.Offset = { 0.0f, -0.4f };
			break;
		}
		case 1:
		{
			for (int i = 28; i < 74; i++)
				sprite.PlayableIndicies.push_back(i);
			indicies.Index = 28;
			transform.Scale = glm::vec3(1.75f, 1.75f, 1.0f) * 1.3f;
			m_State = PlayerState::Intro1;
			boxCollider.Offset = { 0.0f, -0.35f };
			break;
		}
		case 2:
		{
			for (int i = 74; i < 107; i++)
				sprite.PlayableIndicies.push_back(i);
			indicies.Index = 74;
			transform.Scale = glm::vec3(1.75f, 1.75f, 1.0f) * 1.3f;
			m_State = PlayerState::Intro2;
			boxCollider.Offset = { 0.0f, -0.35f };
			break;
		}
		default:
			for (int i = 28; i < 74; i++)
				sprite.PlayableIndicies.push_back(i);
			indicies.Index = 28;
			transform.Scale = glm::vec3(1.75f, 1.75f, 1.0f) * 1.3f;
			m_State = PlayerState::Intro1;
			boxCollider.Offset = { 0.0f, -0.35f };
			break;
		}

		m_Scene->RefreshBody(m_Entity);
	}

	void Player::StartIntro()
	{
		auto& sprite = m_Entity.GetComponent<Teddy::SpriteAnimationComponent>();
		sprite.Pause = false;
	}

	void Player::Move(Teddy::Timestep ts)
	{
		bool leftPressed = Teddy::Input::IsKeyPressed(Teddy::Key::A) || Teddy::Input::IsKeyPressed(Teddy::Key::Left);
		bool rightPressed = Teddy::Input::IsKeyPressed(Teddy::Key::D) || Teddy::Input::IsKeyPressed(Teddy::Key::Right);

		float velocity = 0;
		if (leftPressed)
		{
			m_Moving = true;
			velocity = -3.75f;
			m_Entity.GetComponent<Teddy::Rigidbody2DComponent>().SetVelocityX(velocity);
		}
		else if (rightPressed)
		{
			m_Moving = true;
			velocity = 3.75f;
			m_Entity.GetComponent<Teddy::Rigidbody2DComponent>().SetVelocityX(velocity);
		}
		else
		{
			m_Moving = false;
			m_Entity.GetComponent<Teddy::Rigidbody2DComponent>().SetVelocityX(velocity);
		}
	}

	void Player::StartRun(bool isRight)
	{
		if (m_State == PlayerState::Dashing) return;

		if ((isRight && (Teddy::Input::IsKeyPressed(Teddy::Key::A) || Teddy::Input::IsKeyPressed(Teddy::Key::Left))) ||
			(!isRight && (Teddy::Input::IsKeyPressed(Teddy::Key::D) || Teddy::Input::IsKeyPressed(Teddy::Key::Right))))
			return;

		m_DirectionRight = isRight;
		m_Entity.GetComponent<Teddy::TransformComponent>().Scale.x = m_DirectionRight ? 1.75f : -1.75f;

		if ( m_State == PlayerState::Jumping || !m_Grounded || m_State == PlayerState::Running || m_State == PlayerState::Crouching) return;

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

		auto& sensor = m_Entity.GetComponent<Teddy::Sensor2DComponent>();
		sensor.Sensors["HitBox"] = { { 0.0f, -0.25f }, { 0.25f, 0.45f }, 0.0f };
		m_Scene->RefreshSensor(m_Entity, sensor.Sensors["HitBox"]);

		m_Moving = true;
		m_State = PlayerState::Running;
	}

	void Player::Running()
	{
		if(!m_Moving)
		{
			StartIdle();
		}
	}

	void Player::StartFall()
	{			
		if (m_State == PlayerState::Falling || m_State == PlayerState::Jumping || m_Grounded ||
			m_State == PlayerState::Dashing || m_State == PlayerState::Intro0 || m_State == PlayerState::Intro1 || m_State == PlayerState::Intro2) return;

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

		auto& sensor = m_Entity.GetComponent<Teddy::Sensor2DComponent>();
		sensor.Sensors["HitBox"] = { { 0.0f, 0.0f }, { 0.3f, 0.3f }, 0.0f };
		m_Scene->RefreshSensor(m_Entity, sensor.Sensors["HitBox"]);

		if(m_State != PlayerState::Dropping)
			m_State = PlayerState::Falling;
	}

	void Player::Falling()
	{
		if (m_Grounded)
		{
			auto& filter = m_Entity.GetComponent<Teddy::CollisionFilter2DComponent>();
			auto& box = m_Entity.GetComponent<Teddy::BoxCollider2DComponent>();
			if (filter.CategoryBits != LevelCategories::PLAYER)
			{
				filter.CategoryBits = LevelCategories::PLAYER;
				filter.SetFilterCategory(box, filter.CategoryBits);
			}

			if (Teddy::Input::IsKeyPressed(Teddy::Key::S) || Teddy::Input::IsKeyPressed(Teddy::Key::Down))
			{
				m_State = PlayerState::DoneJumping;
				StartCrouch();
			}
			else if ((Teddy::Input::IsKeyPressed(Teddy::Key::D) || Teddy::Input::IsKeyPressed(Teddy::Key::Right)) && !(Teddy::Input::IsKeyPressed(Teddy::Key::A) || Teddy::Input::IsKeyPressed(Teddy::Key::Left)))
			{
				m_State = PlayerState::AnimationDone;
				StartRun(true);
			}
			else if ((Teddy::Input::IsKeyPressed(Teddy::Key::A) || Teddy::Input::IsKeyPressed(Teddy::Key::Left)) && !(Teddy::Input::IsKeyPressed(Teddy::Key::D) || Teddy::Input::IsKeyPressed(Teddy::Key::Right)))
			{
				m_State = PlayerState::AnimationDone;
				StartRun(false);
			}
			else
				StartIdle();
		}
	}

	void Player::StartJump()
	{
		if (m_State == PlayerState::Jumping || m_State == PlayerState::Falling || !m_Grounded || m_State == PlayerState::Dashing) return;

		if (m_ZHeld) return;

		auto& sprite = m_Entity.GetComponent<Teddy::SpriteAnimationComponent>();
		sprite.Textures = m_JumpTextures;
		sprite.PingPong = false;
		sprite.Loop = true;
		sprite.Reverse = false;

		sprite.FinalFrameTime = 0.04f;
		sprite.FrameTime = 0.04f;
		sprite.InitialFrameTime = 0.04f;

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

		auto& sensor = m_Entity.GetComponent<Teddy::Sensor2DComponent>();
		sensor.Sensors["HitBox"] = { { 0.0f, 0.0f }, { 0.3f, 0.3f }, 0.0f };
		m_Scene->RefreshSensor(m_Entity, sensor.Sensors["HitBox"]);

		m_ZHeld = true;
		m_StartJump = true;
		m_State = PlayerState::Jumping;
	}

	void Player::Jumping(Teddy::Timestep ts)
	{
		auto& body = m_Entity.GetComponent<Teddy::Rigidbody2DComponent>();

		bool running = false;
		static bool falling = false;
		static bool keyHeld = false;
		static float time = 0.0f;
		time += ts.GetSeconds();

		static constexpr float maxJumpHoldTime = 0.25f;
		static constexpr float minJumpHoldTime = 0.1f;
		static constexpr float jumpHoldForce = 70.0f;

		if (m_StartJump)
		{
			body.SetVelocity(0.0f, 12.5f);
			keyHeld = true;
			m_StartJump = false;
			time = 0.0f;
		}

		if (time <= maxJumpHoldTime && keyHeld && m_ZHeld && body.GetVelocity().y > 0)
		{
			if (time > minJumpHoldTime)
			{
				body.SetVelocityY(body.GetVelocity().y + ts * jumpHoldForce);
				keyHeld = true;
			}
		}
		else
		{
			keyHeld = false;
			m_State = PlayerState::Falling;
		}
	}

	void Player::StartCrouch()
	{
		if (m_State == PlayerState::Crouching || !m_Grounded || m_State == PlayerState::Jumping || m_State == PlayerState::Dashing) return;

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

		m_StartCrouch = true; 

		auto& sensor = m_Entity.GetComponent<Teddy::Sensor2DComponent>();
		sensor.Sensors["HitBox"] = { { 0.0f, -0.5f }, { 0.4f, 0.25f }, 0.0f };
		m_Scene->RefreshSensor(m_Entity, sensor.Sensors["HitBox"]);

		m_State = PlayerState::Crouching;
	}

	void Player::Crouching()
	{
		static bool running = false;

		auto& animationAtlas = m_Entity.GetComponent<Teddy::SpriteAnimationAtlasComponent>();
		if (m_StartCrouch && animationAtlas.Index <= 101)
		{
			auto& animation = m_Entity.GetComponent<Teddy::SpriteAnimationComponent>();
			animation.Loop = true;
			animation.PingPong = true;
			animation.Reverse = true;
			animationAtlas.Index = 101;
			animation.PlayableIndicies = { 97, 98, 99, 100, 101 };
			animation.FinalFrameTime = 0.1f;
			animation.FrameTime = 0.05f;
			animation.InitialFrameTime = 0.1f;

			m_StartCrouch = false;
		}

		if (Teddy::Input::IsKeyPressed(Teddy::Key::S) || Teddy::Input::IsKeyPressed(Teddy::Key::Down))
		{
			m_Entity.GetComponent<Teddy::Rigidbody2DComponent>().SetVelocity(0.0f, 0.0f);
		}
		else
		{
			if (Teddy::Input::IsKeyPressed(Teddy::Key::D) || Teddy::Input::IsKeyPressed(Teddy::Key::Right))
			{
				m_State = PlayerState::AnimationDone;
				StartRun(true);
			}
			else if(Teddy::Input::IsKeyPressed(Teddy::Key::A) || Teddy::Input::IsKeyPressed(Teddy::Key::Left))
			{
				m_State = PlayerState::AnimationDone;
				StartRun(false);
			}
			else
				StartIdle();
		}
	}

	void Player::StartDash()
	{
		if (m_State == PlayerState::Dashing || !m_DashReset || m_ShiftHeld) return;

		auto& sprite = m_Entity.GetComponent<Teddy::SpriteAnimationComponent>();
		sprite.Textures = m_MovementTextures;
		sprite.PingPong = false;
		sprite.Loop = false;
		sprite.Reverse = true;

		sprite.FinalFrameTime = 0.05f;
		sprite.FrameTime = 0.05f;
		sprite.InitialFrameTime = 0.05f;

		auto& atlas = m_Entity.GetComponent<Teddy::SpriteAtlasComponent>();
		atlas.SpriteWidth = 347;
		atlas.SpriteHeight = 192;

		auto& indicies = m_Entity.GetComponent<Teddy::SpriteAnimationAtlasComponent>();
		indicies.GenerateFrames(sprite, atlas);

		if (m_Grounded)
		{
			sprite.PlayableIndicies = { 110, 111 };
			indicies.Index = 111;
		}
		else
		{
			sprite.PlayableIndicies = { 112, 113 };
			indicies.Index = 112;
		}

		auto& transform = m_Entity.GetComponent<Teddy::TransformComponent>();
		if (transform.Scale.x >= 0)
			transform.Scale = glm::vec3(1.75f);
		else
			transform.Scale = glm::vec3(-1.75f, 1.75f, 1.0f);

		m_StartDash = true;
		m_DashReset = false;
		m_ShiftHeld = true;

		auto& sensor = m_Entity.GetComponent<Teddy::Sensor2DComponent>();
		sensor.Sensors["HitBox"] = { { m_DirectionRight ? 0.25f : -0.25f, -0.25f }, { 0.4f, 0.45f }, 0.0f };
		m_Scene->RefreshSensor(m_Entity, sensor.Sensors["HitBox"]);

		m_State = PlayerState::Dashing;
	}

	void Player::Dashing(Teddy::Timestep ts) // TODO: fix sprite position
	{
		static float timer = 0.0f;
		static float initialDistance = 0;
		timer += ts.GetSeconds();
		if (m_StartDash)
		{
			float velocity = m_DirectionRight ? 10.0f : -10.0f;
			m_Entity.GetComponent<Teddy::Rigidbody2DComponent>().SetGravityScale(0.0f);
			m_Entity.GetComponent<Teddy::Rigidbody2DComponent>().SetVelocity(velocity, 0.0f);
			m_StartDash = false;
			timer = 0.0f;
			initialDistance = m_Entity.GetComponent<Teddy::TransformComponent>().Translation.x;
		}
		else if (m_Entity.GetComponent<Teddy::SpriteAnimationComponent>().PlayableIndicies.size() < 4)
		{
			auto& sprite = m_Entity.GetComponent<Teddy::SpriteAnimationComponent>();
			sprite.PingPong = false;
			sprite.Loop = false;
			sprite.Reverse = true;
			sprite.InitialFrameTime = 1.0f;
			auto& indicies = m_Entity.GetComponent<Teddy::SpriteAnimationAtlasComponent>();

			if (m_Grounded)
			{
				sprite.PlayableIndicies = { 111, 110, 120, 121, 122, 123, 124, 125 };
				indicies.Index = 125;
			}
			else
			{
				sprite.PlayableIndicies = { 113, 112, 114, 115, 116, 117, 118, 119 };
				indicies.Index = 119;
			}
		}
		else if (abs(initialDistance - m_Entity.GetComponent<Teddy::TransformComponent>().Translation.x) >= 2.75f || 
			(abs(initialDistance - m_Entity.GetComponent<Teddy::TransformComponent>().Translation.x) < 1.0f && timer > 0.5f) || timer > 0.3f)
		{
			m_Entity.GetComponent<Teddy::Rigidbody2DComponent>().SetGravityScale(7.5f);
			m_Entity.GetComponent<Teddy::SpriteAnimationComponent>().InitialFrameTime = 0.5f;
			if(m_Grounded)
			{
				m_Entity.GetComponent<Teddy::Rigidbody2DComponent>().SetVelocity(0.0f, 0.0f);
				if (Teddy::Input::IsKeyPressed(Teddy::Key::S) || Teddy::Input::IsKeyPressed(Teddy::Key::Down))
				{
					m_State = PlayerState::AnimationDone;
					StartCrouch();
				}
				else if (Teddy::Input::IsKeyPressed(Teddy::Key::D) || Teddy::Input::IsKeyPressed(Teddy::Key::Right))
				{
					m_State = PlayerState::AnimationDone;
					StartRun(true);
				}
				else if (Teddy::Input::IsKeyPressed(Teddy::Key::A) || Teddy::Input::IsKeyPressed(Teddy::Key::Left))
				{
					m_State = PlayerState::AnimationDone;
					StartRun(false);
				}
				else
					StartIdle();
			}
			else
			{
				m_State = PlayerState::AnimationDone;
				StartFall();
			}
		}
		else
		{
			auto& sprite = m_Entity.GetComponent<Teddy::SpriteAnimationComponent>();
			auto& indicies = m_Entity.GetComponent<Teddy::SpriteAnimationAtlasComponent>();
			if (m_Grounded)
			{
				sprite.PlayableIndicies = { 111, 110, 120, 121, 122, 123, 124, 125 };
				if(indicies.Index > 111 && indicies.Index < 114)
					indicies.Index = indicies.Index - 2;
				else if(indicies.Index < 120)
					indicies.Index = indicies.Index + 6;
			}
			else
			{
				sprite.PlayableIndicies = { 113, 112, 114, 115, 116, 117, 118, 119 };
				if (indicies.Index < 112)
					indicies.Index = indicies.Index + 2;
				else if (indicies.Index >= 120)
					indicies.Index = indicies.Index - 6;
			}
		}
		
	}

	void Player::StartParry() // TODO: observer if the parry animation happens until he is on the ground// if hit change to pink animation + lil jump
	{
		if (m_ZHeld || !m_ParryReset || m_State == PlayerState::Parrying || m_Grounded) return;

		auto& sprite = m_Entity.GetComponent<Teddy::SpriteAnimationComponent>();
		sprite.Textures = m_JumpTextures;
		sprite.PingPong = false;
		sprite.Loop = true;
		sprite.Reverse = false;

		sprite.FinalFrameTime = 0.025f;
		sprite.FrameTime = 0.025f;
		sprite.InitialFrameTime = 0.025f;

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
		for (int i = 36; i < 44; i++)
			sprite.PlayableIndicies.push_back(i);
		indicies.Index = indicies.Index + 36;

		auto& sensor = m_Entity.GetComponent<Teddy::Sensor2DComponent>();
		sensor.Sensors["ParryHitBox"] = { { 0.0f, 0.0f }, { 0.4f, 0.4f }, 0.0f };

		m_State = PlayerState::Parrying;
		m_ParryReset = false;
	}

	void Player::Parrying()
	{
		Falling();
	}

	void Player::StartDrop()
	{
		if (m_State != PlayerState::Crouching) return;

		auto& filter = m_Entity.GetComponent<Teddy::CollisionFilter2DComponent>();
		filter.CategoryBits = LevelCategories::PLAYERGHOST;
		
		filter.SetFilterCategory(m_Entity.GetComponent<Teddy::BoxCollider2DComponent>(), filter.CategoryBits);
		
		m_State = PlayerState::Dropping;

		m_Dropping = true;

		StartFall();
	}

	void Player::Dropping(Teddy::Timestep ts)
	{
		static float initialDistance = 0.0f;
		static float timer = 0.0f;
		timer += ts;

		if(m_Dropping)
		{
			m_Dropping = false;
			initialDistance = m_Entity.GetComponent<Teddy::TransformComponent>().Translation.y;
			timer = 0.0f;
		}
		else if (abs(initialDistance - m_Entity.GetComponent<Teddy::TransformComponent>().Translation.y) > 0.5f)
		{
			auto& filter = m_Entity.GetComponent<Teddy::CollisionFilter2DComponent>();
			filter.CategoryBits = LevelCategories::PLAYER;

			filter.SetFilterCategory(m_Entity.GetComponent<Teddy::BoxCollider2DComponent>(), filter.CategoryBits);
			m_State = PlayerState::Falling;
		}
		else if (timer > 0.25f)
		{
			auto& filter = m_Entity.GetComponent<Teddy::CollisionFilter2DComponent>();
			filter.CategoryBits = LevelCategories::PLAYER;

			filter.SetFilterCategory(m_Entity.GetComponent<Teddy::BoxCollider2DComponent>(), filter.CategoryBits);
			m_State = PlayerState::Crouching;
		}
	}

	bool Player::OnKeyPressed(Teddy::KeyPressedEvent& e)
	{
		switch (e.GetKeyCode())
		{
		case Teddy::Key::A:
		case Teddy::Key::Left:
			StartRun(false);
			return true;
		case Teddy::Key::D:
		case Teddy::Key::Right:
			StartRun(true);
			return true;
		case Teddy::Key::Z:
			if (m_State != PlayerState::Falling && m_State != PlayerState::Crouching)
				StartJump();
			else if (m_State == PlayerState::Falling)
				StartParry();
			else
				StartDrop();
			return true;
		case Teddy::Key::Down:
		case Teddy::Key::S:
			StartCrouch();
			return true;
		case Teddy::Key::LShift:
			StartDash();
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

	void Player::FloorHit(bool isContacting)
	{
		if (isContacting)
		{
			if (!m_Hitting)
			{
				m_Hitting = true;
				m_Health--;
			}
		}
		else if (!isContacting)
		{
			m_Hitting = false;
		}
	}
}