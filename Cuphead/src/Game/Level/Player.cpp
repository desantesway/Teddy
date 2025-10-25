#include "Player.h"

#include <Teddy.h>

#include "LevelCategories.h"

namespace Cuphead
{

	Player::~Player()
	{
		for (auto& it : m_ActiveProjectiles)
		{
			m_Scene->DestroyEntity(it);
		}
		m_ActiveProjectiles.clear();

		for (auto& it : m_ProjectileExplosion)
		{
			m_Scene->DestroyEntity(it);
		}	
		m_ProjectileExplosion.clear();
	}

	void Player::OnUpdate(Teddy::Timestep ts)
	{
		if (m_Health > 0)
		{
			DeleteCookie(ts);

			if (m_ZHeld)
				m_ZHeld = Teddy::Input::IsKeyPressed(Teddy::Key::Z);

			if (m_ShiftHeld)
				m_ShiftHeld = Teddy::Input::IsKeyPressed(Teddy::Key::LShift);

			if (!m_DashReset)
				m_DashReset = m_Grounded;

			if (!m_ParryReset)
				m_ParryReset = m_Grounded;

			HUDAnimation(ts);

			if (m_State != PlayerState::Parrying)
			{
				auto& sensor = m_Entity.GetComponent<Teddy::Sensor2DComponent>();
				if (sensor.Sensors.contains("ParryHitBox"))
				{
					m_Scene->DeleteSensor(sensor.Sensors["ParryHitBox"]);
					sensor.Sensors.erase("ParryHitBox");
				}
			}

			FlashPlayer(ts);

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
			case PlayerState::Hit:
				Move(ts);
				Hitting(ts);
				BlockMove();
				break;
			case PlayerState::ParryHit:
				Move(ts);
				ParryHitting(ts);
				BlockMove();
				break;
			case PlayerState::Idle:
				Idle();
				BlockMove();
				break;
			default:
				break;
			}
		}
		else
		{
			if (m_State != PlayerState::Dead)
				StartDeath();
			else
				Dying(ts);
		}
		
		Shoot(ts);
	}

	void Player::LaunchProjectile()
	{
		if (m_Projectile == ProjectileType::Lobber)
		{
			m_Damage = 11.6f;
			m_EXDamage = 28.0f;

			static int lobberCount = 0;

			auto ent = m_Scene->CreateEntity("Lobber #" + std::to_string(lobberCount));
			auto& sprite = ent.AddComponent<Teddy::SpriteAnimationComponent>(0.05f, 0.05f, 0.05f);
			sprite.Loop = false;
			sprite.Textures = m_LobberTextures;
			auto& atlas = ent.AddComponent<Teddy::SpriteAtlasComponent>(0,0, 223, 189);
			
			sprite.PlayableIndicies = { 12, 13, 14 };
			
			auto& aA = ent.AddComponent<Teddy::SpriteAnimationAtlasComponent>();
			aA.Index = 12;
			
			auto& transform = ent.GetComponent<Teddy::TransformComponent>();
			transform.Scale *= 1.5f;
			if (m_DirectionRight)
				transform.Translation = m_Entity.GetComponent<Teddy::TransformComponent>().Translation + glm::vec3(0.5f, 0.0f, 0.102f); // TODO: change based on hand position
			else
				transform.Translation = m_Entity.GetComponent<Teddy::TransformComponent>().Translation + glm::vec3(-0.5f, 0.0f, 0.102f);
			
			auto& rb = ent.AddComponent<Teddy::Rigidbody2DComponent>();
			rb.FixedRotation = true;
			rb.Type = Teddy::Rigidbody2DComponent::BodyType::Dynamic;
			rb.Velocity = { m_DirectionRight ? 5.0f : -5.0f, 0.5f };

			auto& sensor = ent.AddComponent<Teddy::Sensor2DComponent>();
			sensor.Sensors["ProjectileSensor"] = Teddy::Sensor2DComponent::SensorData({ 0.0f, 0.0f }, { 0.2f, 0.2f }, 0.0f, false);
			
			auto& filter = ent.AddComponent<Teddy::CollisionFilter2DComponent>();
			filter.CategoryBits = LevelCategories::PROJECTILE;
			filter.MaskBits = LevelCategories::ENEMY;
			
			m_Scene->RefreshBody(ent);
			
			m_ActiveProjectiles.push_back(ent);

			//auto exp = m_Scene->CreateEntity("Lobber Explosion");
			//auto& expSprite = exp.AddComponent<Teddy::SpriteAnimationComponent>(0.025f, 0.025f, 0.025f);
			//expSprite.Textures = m_LobberTextures;
			//expSprite.Loop = false;
			//auto& expAtlas = exp.AddComponent<Teddy::SpriteAtlasComponent>(0, 0, 223, 189);
			//
			//expSprite.PlayableIndicies = { 23, 24, 25, 26, 27 };
			//auto& expAA = exp.AddComponent<Teddy::SpriteAnimationAtlasComponent>();
			//expAA.Index = 23;
			//
			//auto& expTransform = exp.GetComponent<Teddy::TransformComponent>();
			//expTransform.Scale *= 1.25f;
			//expTransform.Translation = transform.Translation - glm::vec3(0.2f, 0.175f, 0.5f); // TODO: see this, it has a weird offset depending on player location
			//
			//m_ProjectileExplosion.push_back(exp);
			//
			//lobberCount++;
		}
		else if (m_Projectile == ProjectileType::Roundabout)
		{
			m_Damage = 8.5f;
			m_EXDamage = 5.5f;
		}
	}

	void Player::Shoot(Teddy::Timestep ts)
	{
		std::vector<Teddy::Entity> newActive;
		for (auto& ent : m_ActiveProjectiles)
		{
			auto& transform = ent.GetComponent<Teddy::TransformComponent>();
			auto& aA = ent.GetComponent<Teddy::SpriteAnimationAtlasComponent>();
			if (transform.Translation.x >= 5.5f || transform.Translation.x <= -5.5f ||
				transform.Translation.y <= -3.0f || aA.Index == 11)
			{
				m_Scene->DestroyEntity(ent);
			}
			else
			{
				auto& aA = ent.GetComponent<Teddy::SpriteAnimationAtlasComponent>();
				auto& sprite = ent.GetComponent<Teddy::SpriteAnimationComponent>();
				if (sprite.PlayableIndicies.size() < 4 && aA.Index >= 14)
				{
					sprite.PlayableIndicies = { 15, 16, 17, 18, 19, 20, 21 };
					aA.Index = 15;
		
					sprite.Loop = true;
				}
		
				if (ent && ent.HasComponent<Teddy::Rigidbody2DComponent>())
					newActive.push_back(ent);
			}
		}
		m_ActiveProjectiles = newActive;

		std::vector<Teddy::Entity> newExplosions;
		for (auto& exp : m_ProjectileExplosion)
		{
			auto& aA = exp.GetComponent<Teddy::SpriteAnimationAtlasComponent>();
			if (aA.Index >= 27)
			{
				m_Scene->DestroyEntity(exp);
			}
			else
			{
				newExplosions.push_back(exp);
			}
		}
		m_ProjectileExplosion = newExplosions;

		if (!m_Shooting) return;

		m_ShootTimer += ts;

		auto& aA = m_Entity.GetComponent<Teddy::SpriteAnimationAtlasComponent>();

		if (m_State == PlayerState::Crouching || m_State == PlayerState::Idle)
		{
			if ((aA.Index == 10 || aA.Index == 95) && m_Entity.GetComponent<Teddy::SpriteAnimationComponent>().Reverse == false)
			{
				if (!m_Shot && m_ShootTimer >= m_ShootingRestTime + m_ShootingActiveTime * 2)
				{
					LaunchProjectile();
					m_Shot = true;
					m_ShootTimer = 0.0f;
				}
			}
			else
			{
				m_Shot = false;
			}
		}

		if (m_State == PlayerState::Dashing || m_State == PlayerState::Jumping || m_State == PlayerState::Falling ||
			m_State == PlayerState::Dropping || m_State == PlayerState::Parrying || m_State == PlayerState::Running )
		{
			if (m_ShootTimer >= m_ShootingRestTime + m_ShootingActiveTime * 2)
			{
				m_ShootTimer = 0.0f;
				LaunchProjectile();
			}
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
		{
			count = 0;
			lastIndex = 0;
			StartIdle();
			return;
		}

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
		if (m_LeftPressed && m_RightPressed)
		{
			if(m_State != PlayerState::Dashing) m_Entity.GetComponent<Teddy::Rigidbody2DComponent>().SetVelocityX(0.0f);
			if (m_State == PlayerState::Running) StartIdle();
		}
		else if (m_LeftPressed)
			StartRun(false);
		else if(m_RightPressed)
			StartRun(true);
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

		m_HealthTextures = assets.LoadMultiple<Teddy::Texture2D>({
			"assets/Textures/Cuphead/Cuphead_Health_146x225_2048x2048_0.png"
			});

		m_HealthHudTextures = assets.LoadMultiple<Teddy::Texture2D>({
			"assets/Textures/Hud/Hud_Health_78x32_256x256_0.png"
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
		sensor.Sensors["GroundSensor"] = { { 0.0f, -0.75f }, { 0.34f, 0.1f }, 0.0f, true };
		sensor.Sensors["HitBox"] = { { 0.0f, -0.25f }, { 0.25f, 0.45f }, 0.0f, true };

		auto& filter = m_Entity.AddComponent<Teddy::CollisionFilter2DComponent>();
		filter.CategoryBits = LevelCategories::PLAYER;
		filter.MaskBits = LevelCategories::INVISIBLEWALLS | LevelCategories::CLOUDPLATFORMON | LevelCategories::ENEMY;

		m_State = PlayerState::Intro1;

		InitPlayerHUD();
		InitProjectiles();
	}

	void Player::InitProjectiles()
	{
		auto& assets = Teddy::AssetManager::Get();

		m_LobberTextures = assets.LoadMultiple<Teddy::Texture2D>({
			"assets/Textures/Weapons/Lobber/Lobber_223x189_1024x1024_0.png",
			"assets/Textures/Weapons/Lobber/Lobber_223x189_1024x1024_1.png",
			});

		// load EX and others
	}

	void Player::InitPlayerHUD()
	{
		m_HealthHUD = m_Scene->CreateEntity("Player HUD");

		auto& sprite = m_HealthHUD.AddComponent<Teddy::SpriteAnimationComponent>(0.1f, 0.05f, 0.1f);
		sprite.Textures = m_HealthHudTextures;
		sprite.Pause = true;
		m_HealthHUD.AddComponent<Teddy::SpriteAtlasComponent>(0, 4, 78, 32);
		auto& transform = m_HealthHUD.GetComponent<Teddy::TransformComponent>();
		transform.Translation = glm::vec3(-3.875f, -2.15f, 3.2f);
		transform.Scale = glm::vec3(0.225f, 0.225f, 1.0f);
	}

	void Player::StartIdle()
	{
		if (m_State == PlayerState::Hit || m_State == PlayerState::Idle) return;

		if (!m_Entity.HasComponent<Teddy::SpriteAnimationAtlasComponent>())
			return;

		auto& sprite = m_Entity.GetComponent<Teddy::SpriteAnimationComponent>();
		sprite.Textures = m_MovementTextures;
		sprite.PingPong = true;
		sprite.Loop = true;
		if (m_Shooting)
		{
			sprite.FinalFrameTime = m_ShootingRestTime;
			sprite.FrameTime = m_ShootingActiveTime;
			sprite.InitialFrameTime = m_ShootingActiveTime;
			sprite.PlayableIndicies = { 9, 10, 11 };
		}
		else
		{
			sprite.FinalFrameTime = 0.1f;
			sprite.FrameTime = 0.05f;
			sprite.InitialFrameTime = 0.1f;
			sprite.PlayableIndicies = { 84, 85, 86, 87, 88 };
		}

		auto& atlas = m_Entity.GetComponent<Teddy::SpriteAtlasComponent>();
		atlas.SpriteWidth = 347;
		atlas.SpriteHeight = 192;

		auto& indicies = m_Entity.GetComponent<Teddy::SpriteAnimationAtlasComponent>();
		indicies.GenerateFrames(sprite, atlas);
		indicies.Index = *sprite.PlayableIndicies.begin();

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
		m_Scene->DeleteSensor(sensor.Sensors["HitBox"]);
		sensor.Sensors["HitBox"] = { { 0.0f, -0.25f }, { 0.25f, 0.45f }, 0.0f, true };
		m_Scene->RefreshSensor(m_Entity, sensor.Sensors["HitBox"]);

		m_State = PlayerState::Idle;
	}

	void Player::Idle()
	{
		auto& sprite = m_Entity.GetComponent<Teddy::SpriteAnimationComponent>();
		if (m_Shooting && *sprite.PlayableIndicies.begin() == 84) // TODO: fix atlas x difference
		{
			sprite.FinalFrameTime = m_ShootingRestTime;
			sprite.FrameTime = m_ShootingActiveTime;
			sprite.InitialFrameTime = m_ShootingActiveTime;
			sprite.PlayableIndicies = { 9, 10, 11 };
		}
		else if (!m_Shooting && *sprite.PlayableIndicies.begin() == 9)
		{
			sprite.FinalFrameTime = 0.1f;
			sprite.FrameTime = 0.05f;
			sprite.InitialFrameTime = 0.1f;
			sprite.PlayableIndicies = { 84, 85, 86, 87, 88 };
		}
	}

	void Player::DeleteCookie(Teddy::Timestep ts)
	{
		static bool fadeOut = false;

		if (!m_CookieDeleted && m_Cookie)
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
				m_CookieDeleted = true;
				m_Scene->DestroyEntity(m_Cookie);
				m_Cookie = {};
			}
		}
	}

	void Player::BreakCookie()
	{
		if (m_CookieCreated)
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

		m_CookieCreated = true;
	}

	void Player::LoadIntro()
	{
		if (m_IntroLoaded) return;

		m_IntroLoaded = true;

		if (!m_Entity.HasComponent<Teddy::SpriteAnimationAtlasComponent>())
			return;

		static std::random_device rd;
		static std::mt19937 gen(rd());
		static std::uniform_int_distribution<> distr(0, 2); // TODO
		int choice = distr(gen);

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
		float velocity = 0;
		if (m_LeftPressed)
		{
			m_Moving = true;
			velocity = -3.75f;
			m_Entity.GetComponent<Teddy::Rigidbody2DComponent>().SetVelocityX(velocity);
		}
		else if (m_RightPressed)
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
		if (m_State == PlayerState::Hit || m_State == PlayerState::Dashing) return;

		if ((isRight && m_LeftPressed) || (!isRight && m_RightPressed)) return;

		m_DirectionRight = isRight;
		m_Entity.GetComponent<Teddy::TransformComponent>().Scale.x = m_DirectionRight ? 1.75f : -1.75f;

		if ( m_State == PlayerState::Jumping || !m_Grounded || m_State == PlayerState::Running || m_State == PlayerState::Crouching) return;

		auto& sprite = m_Entity.GetComponent<Teddy::SpriteAnimationComponent>();
		sprite.Textures = m_MovementTextures;
		sprite.PingPong = false;
		sprite.Loop = true;
		sprite.Reverse = true;

		auto& atlas = m_Entity.GetComponent<Teddy::SpriteAtlasComponent>();
		atlas.SpriteWidth = 347;
		atlas.SpriteHeight = 192;

		auto& indicies = m_Entity.GetComponent<Teddy::SpriteAnimationAtlasComponent>();
		indicies.GenerateFrames(sprite, atlas);

		sprite.FinalFrameTime = 0.05f;
		sprite.FrameTime = 0.05f;
		sprite.InitialFrameTime = 0.05f;
		
		sprite.PlayableIndicies.clear();
		if (m_Shooting)
		{
			for (int i = 52; i < 68; i++)
				sprite.PlayableIndicies.push_back(i);
		}
		else
		{
			for (int i = 68; i < 84; i++)
				sprite.PlayableIndicies.push_back(i);
		}
		indicies.Index = *sprite.PlayableIndicies.begin();

		auto& transform = m_Entity.GetComponent<Teddy::TransformComponent>();

		if (m_DirectionRight)
			transform.Scale = glm::vec3(1.75f);
		else
			transform.Scale = glm::vec3(-1.75f, 1.75f, 1.0f);

		auto& sensor = m_Entity.GetComponent<Teddy::Sensor2DComponent>();
		sensor.Sensors["HitBox"] = { { 0.0f, -0.25f }, { 0.25f, 0.45f }, 0.0f, true, sensor.Sensors["HitBox"].RuntimeFixture };
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
		else
		{
			auto& sprite = m_Entity.GetComponent<Teddy::SpriteAnimationComponent>();
			auto& indicies = m_Entity.GetComponent<Teddy::SpriteAnimationAtlasComponent>();
			if (m_Shooting && *sprite.PlayableIndicies.begin() == 68)
			{
				sprite.PlayableIndicies.clear();
				for (int i = 52; i < 68; i++)
					sprite.PlayableIndicies.push_back(i);
				indicies.Index = indicies.Index - 16;
			}
			else if(!m_Shooting && *sprite.PlayableIndicies.begin() == 52)
			{
				sprite.PlayableIndicies.clear();
				for (int i = 68; i < 84; i++)
					sprite.PlayableIndicies.push_back(i);
				indicies.Index = indicies.Index + 16;
			}
		}
	}

	void Player::StartFall()
	{			
		if (m_Health < 0 || m_State == PlayerState::Dead || m_State == PlayerState::Hit || m_State == PlayerState::Falling || m_State == PlayerState::Jumping || m_Grounded ||
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
		sensor.Sensors["HitBox"] = { { 0.0f, 0.0f }, { 0.3f, 0.3f }, 0.0f, true, sensor.Sensors["HitBox"].RuntimeFixture };
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

			if (m_DownPressed)
			{
				m_State = PlayerState::DoneJumping;
				StartCrouch();
			}
			else if (m_RightPressed && !m_LeftPressed)
			{
				m_State = PlayerState::AnimationDone;
				StartRun(true);
			}
			else if (m_LeftPressed && !m_RightPressed)
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
		if (m_State == PlayerState::Hit || m_State == PlayerState::Jumping || m_State == PlayerState::Dead ||
			m_State == PlayerState::Falling || !m_Grounded || m_State == PlayerState::Dashing) return;

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
		sensor.Sensors["HitBox"] = { { 0.0f, 0.0f }, { 0.3f, 0.3f }, 0.0f, true, sensor.Sensors["HitBox"].RuntimeFixture };
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
		if (m_State == PlayerState::Hit || m_State == PlayerState::Crouching || !m_Grounded || m_State == PlayerState::Jumping || m_State == PlayerState::Dashing) return;

		auto& sprite = m_Entity.GetComponent<Teddy::SpriteAnimationComponent>();
		sprite.Textures = m_MovementTextures;
		sprite.PingPong = true;
		sprite.Loop = true;
		sprite.Reverse = false;

		auto& atlas = m_Entity.GetComponent<Teddy::SpriteAtlasComponent>();
		atlas.SpriteWidth = 347;
		atlas.SpriteHeight = 192;

		auto& indicies = m_Entity.GetComponent<Teddy::SpriteAnimationAtlasComponent>();
		indicies.GenerateFrames(sprite, atlas);

		sprite.PlayableIndicies.clear();
		
		sprite.FinalFrameTime = 0.05f;
		sprite.FrameTime = 0.05f;
		sprite.InitialFrameTime = 0.05f;
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
		sensor.Sensors["HitBox"] = { { 0.0f, -0.5f }, { 0.4f, 0.25f }, 0.0f, true, sensor.Sensors["HitBox"].RuntimeFixture };
		m_Scene->RefreshSensor(m_Entity, sensor.Sensors["HitBox"]);

		m_State = PlayerState::Crouching;
	}

	void Player::Crouching() // TODO: direction change animation while crouching
	{
		static bool running = false;

		auto& animationAtlas = m_Entity.GetComponent<Teddy::SpriteAnimationAtlasComponent>();
		auto& sprite = m_Entity.GetComponent<Teddy::SpriteAnimationComponent>();
		if (m_StartCrouch)
		{
			if (animationAtlas.Index <= 101)
			{
				auto& animation = m_Entity.GetComponent<Teddy::SpriteAnimationComponent>();
				animation.Loop = true;
				animation.PingPong = true;
				animation.Reverse = true;
				if (m_Shooting)
				{
					animation.FinalFrameTime = m_ShootingRestTime;
					animation.FrameTime = m_ShootingActiveTime;
					animation.InitialFrameTime = m_ShootingActiveTime;
					animation.PlayableIndicies = { 94, 95, 96 };
					animationAtlas.Index = 96;
				}
				else
				{
					animationAtlas.Index = 101;
					animation.PlayableIndicies = { 97, 98, 99, 100, 101 };
					animation.FinalFrameTime = 0.1f;
					animation.FrameTime = 0.05f;
					animation.InitialFrameTime = 0.1f;
				}

				m_StartCrouch = false;
			}
		}
		else if (m_Shooting && *sprite.PlayableIndicies.begin() == 97)
		{
			auto& animation = m_Entity.GetComponent<Teddy::SpriteAnimationComponent>();
			animation.FinalFrameTime = m_ShootingRestTime;
			animation.FrameTime = m_ShootingActiveTime;
			animation.InitialFrameTime = m_ShootingActiveTime;
			animation.PlayableIndicies = { 94, 95, 96 };
			animationAtlas.Index = 96;
		}
		else if (!m_Shooting && *sprite.PlayableIndicies.begin() == 94)
		{
			auto& animation = m_Entity.GetComponent<Teddy::SpriteAnimationComponent>();
			animationAtlas.Index = 101;
			animation.PlayableIndicies = { 97, 98, 99, 100, 101 };
			animation.FinalFrameTime = 0.1f;
			animation.FrameTime = 0.05f;
			animation.InitialFrameTime = 0.1f;
		}

		if (!m_DownPressed)
		{
			if (m_RightPressed)
			{
				m_State = PlayerState::AnimationDone;
				StartRun(true);
			}
			else if(m_LeftPressed)
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
		if (m_State == PlayerState::Hit || m_State == PlayerState::Dashing || !m_DashReset || m_ShiftHeld) return;

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
		sensor.Sensors["HitBox"] = { { m_DirectionRight ? 0.25f : -0.25f, -0.25f }, { 0.4f, 0.45f }, 0.0f, true, sensor.Sensors["HitBox"].RuntimeFixture };
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
				if (m_DownPressed)
				{
					m_State = PlayerState::AnimationDone;
					StartCrouch();
				}
				else if (m_RightPressed)
				{
					m_State = PlayerState::AnimationDone;
					StartRun(true);
				}
				else if (m_LeftPressed)
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

	void Player::StartParry() // TODO: observe if the parry animation happens until he is on the ground// if hit change to pink animation + lil jump
	{
		if (m_ZHeld || !m_ParryReset || m_State == PlayerState::Parrying || m_Grounded || m_State == PlayerState::Hit) return;

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
		for (int i = 36; i < 44; i++)
			sprite.PlayableIndicies.push_back(i);
		indicies.Index = indicies.Index + 36;

		auto& sensor = m_Entity.GetComponent<Teddy::Sensor2DComponent>();
		m_Scene->DeleteSensor(sensor.Sensors["ParryHitBox"]);
		sensor.Sensors["ParryHitBox"] = { { 0.0f, 0.0f }, { 0.45f, 0.45f }, 0.0f, true };
		m_Scene->RefreshSensor(m_Entity, sensor.Sensors["ParryHitBox"]);

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
			m_LeftPressed = true;
			StartRun(false);
			return true;
		case Teddy::Key::D:
		case Teddy::Key::Right:
			m_RightPressed = true;
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
			m_DownPressed = true;
			StartCrouch();
			return true;
		case Teddy::Key::LShift:
			StartDash();
			return true;
		case Teddy::Key::X:
			m_Shooting = true;
			return true;
		default:
			break;
		}

		return false;
	}

	bool Player::OnKeyReleased(Teddy::KeyReleasedEvent& e)
	{
		switch (e.GetKeyCode())
		{
		case Teddy::Key::A:
		case Teddy::Key::Left:
			m_LeftPressed = false;
			return true;
		case Teddy::Key::D:
		case Teddy::Key::Right:
			m_RightPressed = false;
			return true;
		case Teddy::Key::Down:
		case Teddy::Key::S:
			m_DownPressed = false;
			return true;
		case Teddy::Key::X:
			m_Shooting = false;
			break;
		default:
			break;
		}

		return false;
	}

	void Player::OnEvent(Teddy::Event& event)
	{
		if (m_State != PlayerState::Intro1 && m_State != PlayerState::Intro2 && m_State != PlayerState::Intro0 && m_Health > 0)
		{
			Teddy::EventDispatcher dispatcher(event);
			dispatcher.Dispatch<Teddy::KeyPressedEvent>(TED_BIND_EVENT_FN(Player::OnKeyPressed));
			dispatcher.Dispatch<Teddy::KeyReleasedEvent>(TED_BIND_EVENT_FN(Player::OnKeyReleased));
		}
	}

	void Player::StartHit()
	{
		if (m_State == PlayerState::Hit) return;
		
		auto& sprite = m_Entity.GetComponent<Teddy::SpriteAnimationComponent>();
		sprite.Textures = m_HealthTextures;
		sprite.PingPong = false;
		sprite.Loop = false;
		sprite.Reverse = false;

		sprite.FinalFrameTime = 0.05f;
		sprite.FrameTime = 0.05f;
		sprite.InitialFrameTime = 0.05f;

		auto& atlas = m_Entity.GetComponent<Teddy::SpriteAtlasComponent>();
		atlas.SpriteWidth = 146;
		atlas.SpriteHeight = 225;

		auto& indicies = m_Entity.GetComponent<Teddy::SpriteAnimationAtlasComponent>();
		indicies.GenerateFrames(sprite, atlas);

		auto& transform = m_Entity.GetComponent<Teddy::TransformComponent>();
		if (transform.Scale.x >= 0)
			transform.Scale = glm::vec3(2.0f);
		else
			transform.Scale = glm::vec3(-2.0f, 2.0f, 1.0f);

		sprite.PlayableIndicies.clear();
		if (m_Grounded)
		{
			for (int i = 24; i < 29; i++)
				sprite.PlayableIndicies.push_back(i);
		}
		else
		{
			for (int i = 29; i < 34; i++)
				sprite.PlayableIndicies.push_back(i);
		}
		indicies.Index = *sprite.PlayableIndicies.begin();

		m_State = PlayerState::Hit;
		m_Hitting = true;
	}

	void Player::Hitting(Teddy::Timestep ts)
	{
		static float timer = 0;
		timer += ts.GetSeconds();
		auto& indicies = m_Entity.GetComponent<Teddy::SpriteAnimationAtlasComponent>();
		if (indicies.Index == 28 || indicies.Index == 34 || timer > 0.5f)
		{
			timer = 0;
			m_Hitting = false;
			m_HitTolerance = true;
			m_State = PlayerState::AnimationDone;
			if(m_Health <= 0)
				StartDeath();
			else if (m_Grounded)
				StartIdle();
			else
				StartFall();
		}
		else if (m_Grounded && indicies.Index >= 29)
		{
			auto& sprite = m_Entity.GetComponent<Teddy::SpriteAnimationComponent>();
			indicies.Index -= 6;
			for (int i = 24; i < 29; i++)
				sprite.PlayableIndicies.push_back(i);
		}
		else if (!m_Grounded && indicies.Index < 29)
		{
			auto& sprite = m_Entity.GetComponent<Teddy::SpriteAnimationComponent>();
			indicies.Index += 6;
			for (int i = 29; i < 34; i++)
				sprite.PlayableIndicies.push_back(i);
		}
	}

	bool Player::FloorHit()
	{
		if (!m_Hitting)
		{
			bool ret = Hit(0.0f);
			auto& body = m_Entity.GetComponent<Teddy::Rigidbody2DComponent>();
			body.SetVelocityY(25.0f);
			return ret;
		}
	}

	bool Player::NormalHit()
	{
		return Hit(15.0f);
	}

	void Player::Dying(Teddy::Timestep ts)
	{
		auto& transform = m_Entity.GetComponent<Teddy::TransformComponent>();
		transform.Translation.y += 1.5f * ts.GetSeconds();
	}

	void Player::StartDeath()
	{
		if (m_Health > 0 || !m_FirstDeath || m_State == PlayerState::Dead) return;
		
		m_State = PlayerState::Dead;

		auto& sprite = m_Entity.GetComponent<Teddy::SpriteAnimationComponent>();
		sprite.Textures = m_HealthTextures;
		sprite.PingPong = false;
		sprite.Loop = true;
		sprite.Reverse = false;

		sprite.FinalFrameTime = 0.05f;
		sprite.FrameTime = 0.05f;
		sprite.InitialFrameTime = 0.05f;

		auto& atlas = m_Entity.GetComponent<Teddy::SpriteAtlasComponent>();
		atlas.SpriteWidth = 146;
		atlas.SpriteHeight = 225;

		auto& indicies = m_Entity.GetComponent<Teddy::SpriteAnimationAtlasComponent>();
		indicies.GenerateFrames(sprite, atlas);

		auto& transform = m_Entity.GetComponent<Teddy::TransformComponent>();
		if (transform.Scale.x >= 0)
			transform.Scale = glm::vec3(2.0f);
		else
			transform.Scale = glm::vec3(-2.0f, 2.0f, 1.0f);

		sprite.PlayableIndicies.clear();
		for (int i = 0; i < 24; i++)
			sprite.PlayableIndicies.push_back(i);
		indicies.Index = 0;

		m_FirstDeath = false;
	}

	void Player::StartParryHit()
	{
		if (m_State != PlayerState::Parrying) return;

		auto& sprite = m_Entity.GetComponent<Teddy::SpriteAnimationComponent>();
		sprite.Textures = m_JumpTextures;
		sprite.PingPong = false;
		sprite.Loop = false;
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
		for (int i = 44; i < 52; i++)
			sprite.PlayableIndicies.push_back(i);
		indicies.Index = 44;

		auto& sensor = m_Entity.GetComponent<Teddy::Sensor2DComponent>();
		sensor.Sensors["HitBox"] = { { 0.0f, 0.0f }, { 0.3f, 0.3f }, 0.0f, true, sensor.Sensors["HitBox"].RuntimeFixture };
		m_Scene->RefreshSensor(m_Entity, sensor.Sensors["HitBox"]);

		m_State = PlayerState::ParryHit;
	}

	void Player::ParryHitting(Teddy::Timestep ts)
	{
		if (!m_Grounded)
		{
			auto& indicies = m_Entity.GetComponent<Teddy::SpriteAnimationAtlasComponent>();
			if (indicies.Index == 51)
			{
				auto& sprite = m_Entity.GetComponent<Teddy::SpriteAnimationComponent>();
				sprite.PlayableIndicies.clear();
				for (int i = 0; i < 8; i++)
					sprite.PlayableIndicies.push_back(i);
				indicies.Index = 0;
				m_State = PlayerState::Falling;
			}
		}
		else
		{
			if (m_DownPressed)
			{
				m_State = PlayerState::DoneJumping;
				StartCrouch();
			}
			else if (m_RightPressed && !m_LeftPressed)
			{
				m_State = PlayerState::AnimationDone;
				StartRun(true);
			}
			else if (m_LeftPressed && !m_RightPressed)
			{
				m_State = PlayerState::AnimationDone;
				StartRun(false);
			}
			else
				StartIdle();
		}
	}

	void Player::HUDAnimation(Teddy::Timestep ts)
	{
		static bool firstTime = true;
		static float elapsed = 0.0f;
		if (m_HitAnimation)
		{
			elapsed += ts;
			auto& transform = m_HealthHUD.GetComponent<Teddy::TransformComponent>();
			if (firstTime)
			{
				transform.Scale = glm::vec3(0.4f, 0.4f, 1.0f);
			}
			else
			{
				static constexpr float velocity = 1.0f;
				static constexpr float duration = 0.5f;
				static constexpr glm::vec3 startScale = glm::vec3(0.4f, 0.4f, 1.0f);
				static constexpr glm::vec3 endScale = glm::vec3(0.225f, 0.225f, 1.0f); 

				float t = glm::clamp(elapsed / duration, 0.0f, 1.0f);

				float easeOutT = 1.0f - ((1.0f - t) * (1.0f - t) * (1.0f - t));

				transform.Scale = glm::mix(startScale, endScale, easeOutT);
			}

			firstTime = false;

			if (transform.Scale.x <= 0.225f)
			{
				transform.Scale = glm::vec3(0.225f, 0.225f, 1.0f);
				m_HitAnimation = false;
				firstTime = true;
				elapsed = 0.0f;
			}
		}
	}

	void Player::UpdateHUD()
	{
		m_HitAnimation = true;
		if (m_Health <= 0)
		{
			auto& sprite = m_HealthHUD.GetComponent<Teddy::SpriteAnimationComponent>();
			sprite.Pause = true;
			auto& atlas = m_HealthHUD.GetComponent<Teddy::SpriteAtlasComponent>();
			atlas.X = 0;
			atlas.Y = 2;
		}
		else if (m_Health == 1)
		{
			auto& sprite = m_HealthHUD.GetComponent<Teddy::SpriteAnimationComponent>();
			sprite.PlayableIndicies = { 15, 16 };
			sprite.Pause = false;
			auto& aA = m_HealthHUD.GetComponent<Teddy::SpriteAnimationAtlasComponent>().Index = 15;
		}
		else
		{
			auto& atlas = m_HealthHUD.GetComponent<Teddy::SpriteAtlasComponent>();
			atlas.X = 4 - m_Health;
			atlas.Y = 4;
		}
	}

	bool Player::Hit(float velocity)
	{
		if (!m_Hitting)
		{
			if (!m_HitTolerance)
			{
				auto& body = m_Entity.GetComponent<Teddy::Rigidbody2DComponent>();
				if(body.GetGravityScale() != 7.5f) 
					body.SetGravityScale(7.5f);
				body.SetVelocityY(velocity);

				m_Health--;
				UpdateHUD();
				StartHit();
				return true;
			}
		}
		return false;
	}

	void Player::FlashPlayer(Teddy::Timestep ts)
	{
		static bool doneFlashing = false;
		static float timer = 0.0f;
		if (m_HitTolerance && !doneFlashing)
		{
			timer += ts.GetSeconds();
			static float timerReset = 0.0f;
			timerReset += ts.GetSeconds();
			auto& color = m_Entity.GetComponent<Teddy::SpriteAnimationComponent>().Color;
			if (color.a <= 0.25 && timerReset > 0.1f)
			{
				color = glm::vec4(1.0f);
				timerReset = 0.0f;
			}
			else if (color.a > 0.25 && timerReset > 0.25f)
			{
				color = glm::vec4(0.25f);
				timerReset = 0.0f;
			}

			if (timer > 1.0f)
			{
				auto& color = m_Entity.GetComponent<Teddy::SpriteAnimationComponent>().Color;
				color = glm::vec4(1.0f);
				timer = 0.0f;
				doneFlashing = true;
			}
		}
		else if (doneFlashing) // hit delay after flashing
		{
			timer += ts.GetSeconds();
			if (timer > 0.5f)
			{
				timer = 0.0f;
				m_HitTolerance = false;
				doneFlashing = false;
			}
		}
	}

	bool Player::IsGroundSensor(b2ShapeId shape)
	{
		b2ShapeId sensorShape = *static_cast<b2ShapeId*>(m_Entity.GetComponent<Teddy::Sensor2DComponent>().Sensors["GroundSensor"].RuntimeFixture);
		return B2_ID_EQUALS(sensorShape, shape);
	}

	bool Player::IsProjectile(b2ShapeId shape)
	{
		for (auto& proj : m_ActiveProjectiles)
		{
			auto& sensor = proj.GetComponent<Teddy::Sensor2DComponent>();
			for (auto& [name, sensorShape] : sensor.Sensors)
			{
				b2ShapeId senShape = *static_cast<b2ShapeId*>(sensorShape.RuntimeFixture);
				if(B2_ID_EQUALS(senShape, shape))
					return true;
			}
		}

		return false;
	}

	void Player::ProjectileImpact(b2ShapeId shape)
	{
		for (auto& proj : m_ActiveProjectiles)
		{
			auto& sensor = proj.GetComponent<Teddy::Sensor2DComponent>();
			for (auto& [_, sensorShape] : sensor.Sensors)
			{
				b2ShapeId senShape = *static_cast<b2ShapeId*>(sensorShape.RuntimeFixture);
				if (B2_ID_EQUALS(senShape, shape))
				{
					auto& sprite = proj.GetComponent<Teddy::SpriteAnimationComponent>();
					sprite.PlayableIndicies = { 6, 7, 8, 9, 10, 11 };
					sprite.Loop = false;
					sprite.PingPong = false;
					sprite.Reverse = false;
		
					auto& aA = proj.GetComponent<Teddy::SpriteAnimationAtlasComponent>();
					aA.Index = 6;
		
					auto& body = proj.GetComponent<Teddy::Rigidbody2DComponent>();
					body.SetVelocity(0.0f, 0.0f);
					body.SetGravityScale(0.0f);
				}
			}
		}
	}

	bool Player::IsHitBox(b2ShapeId shape)
	{
		b2ShapeId sensorShape = *static_cast<b2ShapeId*>(m_Entity.GetComponent<Teddy::Sensor2DComponent>().Sensors["HitBox"].RuntimeFixture);
		return B2_ID_EQUALS(sensorShape, shape);
	}

	bool Player::IsParry(b2ShapeId shape)
	{
		auto& sensor = m_Entity.GetComponent<Teddy::Sensor2DComponent>();
		if (m_State == PlayerState::Parrying && sensor.Sensors.contains("ParryHitBox"))
		{
			if (sensor.Sensors["ParryHitBox"].RuntimeFixture)
			{
				b2ShapeId sensorShape = *static_cast<b2ShapeId*>(sensor.Sensors["ParryHitBox"].RuntimeFixture);
				return B2_ID_EQUALS(sensorShape, shape);
			}
			
		}
		return false;
	}

	void Player::ParryHit()
	{
		if (m_State == PlayerState::Parrying)
		{
			auto& body = m_Entity.GetComponent<Teddy::Rigidbody2DComponent>();
			body.SetVelocityY(15.0f);

			auto& sensor = m_Entity.GetComponent<Teddy::Sensor2DComponent>();
			if (sensor.Sensors.contains("ParryHitBox"))
			{
				m_Scene->DeleteSensor(sensor.Sensors["ParryHitBox"]);
				sensor.Sensors.erase("ParryHitBox");
			}

			StartParryHit();
		}
	}
}