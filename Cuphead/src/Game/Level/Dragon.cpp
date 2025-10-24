#include "Dragon.h"

#include <Teddy.h>

#include "LevelCategories.h"

namespace Cuphead
{
	void Dragon::OnUpdate(Teddy::Timestep ts)
	{
		Hitting(ts);

		ClearProjectiles();

		switch (m_State)
		{
		case DragonState::Peashot:
			Peashot(ts);
			PsychicEye(ts);
			break;
		case DragonState::Idle:
			Idle(ts);
			break;
		case DragonState::Intro:
			Intro();
			break;
		default:
			break;
		}
	}
	
	void Dragon::Hitting(Teddy::Timestep ts)
	{
		if (m_Hit)
		{
			static bool increasing = true;
			auto& color = m_Entity.GetComponent<Teddy::SpriteAnimationComponent>().Color;
			if (color.r <= 1.25f)
			{
				if(increasing)
					color += glm::vec4(ts * 7.5f);
				else
				{
					color -= glm::vec4(ts * 7.5f);
					if (color.r <= 1.0f)
					{
						color = glm::vec4(1.0f);
						m_Hit = false;
						increasing = true;
					}
				}
			}
			else 
			{
				color = glm::vec4(1.0f);
				increasing = false;
			}
			color.a = 1.0f;
		}
	}

	void Dragon::Intro()
	{
		auto& aA = m_Entity.GetComponent<Teddy::SpriteAnimationAtlasComponent>();
		static int counter = 0;
		if (counter == 0 && aA.Index >= 25 && aA.Index <= 28)
		{
			auto& sprite = m_Entity.GetComponent<Teddy::SpriteAnimationComponent>();
			sprite.PlayableIndicies = { 25, 26, 27, 28 };
			sprite.Loop = true;

			counter++;
		}
		else if (counter >= 1 && counter <= 4)
		{
			static bool loopCounted = false;
			if (aA.Index == 28)
			{
				if (counter == 4)
				{
					auto& sprite = m_Entity.GetComponent<Teddy::SpriteAnimationComponent>();
					sprite.PlayableIndicies = { 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39 };
					sprite.Loop = false;
					counter++;
				}

				if(!loopCounted)
				{ 
					counter++;
					loopCounted = true;
				}
			}
			else
			{
				loopCounted = false;
			}
		}
		else if (aA.Index >= 38)
		{		
			StartIdle();
			counter = 0;
		}
		
	}

	void Dragon::Init(Teddy::Ref<Teddy::Scene> scene)
	{
		m_Scene = scene;

		LoadTextures();

		m_Entity = m_Scene->CreateEntity("Dragon");
		auto& spriteAnim = m_Entity.AddComponent<Teddy::SpriteAnimationComponent>(0.04f, 0.04f, 0.04f);
		spriteAnim.Pause = true;
		spriteAnim.Loop = false;
		spriteAnim.Textures = m_IntroTextures;
		auto& atlas = m_Entity.AddComponent<Teddy::SpriteAtlasComponent>(0, 0, 1000, 805);

		auto& body = m_Entity.AddComponent<Teddy::Rigidbody2DComponent>();
		body.Type = Teddy::Rigidbody2DComponent::BodyType::Kinematic;

		auto& sensor = m_Entity.AddComponent<Teddy::Sensor2DComponent>();
		sensor.Sensors["BellyHitBox"] = { { - 0.4f, -0.75f }, { 1.0f, 1.0f }, 0.0f, false };
		sensor.Sensors["NeckHitBox"] = { { 0.25f, 0.75f }, { 1.25f, 0.5f }, 45.0f, true };
		sensor.Sensors["HeadHitBox"] = { { -0.5f, 2.0f }, { 1.25f, 0.5f }, 0.0f, true };
		
		auto& filter = m_Entity.AddComponent<Teddy::CollisionFilter2DComponent>();
		filter.CategoryBits = LevelCategories::ENEMY;
		filter.MaskBits = LevelCategories::PLAYER | LevelCategories::PROJECTILE;
	}

	void Dragon::LoadTextures()
	{
		auto& assets = Teddy::AssetManager::Get();

		m_IntroTextures = assets.LoadMultiple<Teddy::Texture2D>({
			"assets/Textures/Dragon/Entity/Intro/Dragon_Intro_1000x805_2048x2048_0.png",
			"assets/Textures/Dragon/Entity/Intro/Dragon_Intro_1000x805_2048x2048_1.png",
			"assets/Textures/Dragon/Entity/Intro/Dragon_Intro_1000x805_2048x2048_2.png",
			"assets/Textures/Dragon/Entity/Intro/Dragon_Intro_1000x805_2048x2048_3.png",
			"assets/Textures/Dragon/Entity/Intro/Dragon_Intro_1000x805_2048x2048_4.png",
			"assets/Textures/Dragon/Entity/Intro/Dragon_Intro_1000x805_2048x2048_5.png",
			"assets/Textures/Dragon/Entity/Intro/Dragon_Intro_1000x805_2048x2048_6.png",
			"assets/Textures/Dragon/Entity/Intro/Dragon_Intro_1000x805_2048x2048_7.png",
			"assets/Textures/Dragon/Entity/Intro/Dragon_Intro_1000x805_2048x2048_8.png",
			"assets/Textures/Dragon/Entity/Intro/Dragon_Intro_1000x805_2048x2048_9.png"
			});

		m_IdleTextures = assets.LoadMultiple<Teddy::Texture2D>({
			"assets/Textures/Dragon/Entity/Idle/Dragon_Idle_690x800_2048x2048_0.png",
			"assets/Textures/Dragon/Entity/Idle/Dragon_Idle_690x800_2048x2048_1.png",
			"assets/Textures/Dragon/Entity/Idle/Dragon_Idle_690x800_2048x2048_2.png",
			"assets/Textures/Dragon/Entity/Idle/Dragon_Idle_690x800_2048x2048_3.png"
			});

		m_PeashotTextures = assets.LoadMultiple<Teddy::Texture2D>({
			"assets/Textures/Dragon/Entity/Peashot/Dragon_Peashot_690x800_2048x2048_0.png",
			"assets/Textures/Dragon/Entity/Peashot/Dragon_Peashot_690x800_2048x2048_1.png",
			"assets/Textures/Dragon/Entity/Peashot/Dragon_Peashot_690x800_2048x2048_2.png",
			"assets/Textures/Dragon/Entity/Peashot/Dragon_Peashot_690x800_2048x2048_3.png",
			"assets/Textures/Dragon/Entity/Peashot/Dragon_Peashot_690x800_2048x2048_4.png",
			"assets/Textures/Dragon/Entity/Peashot/Dragon_Peashot_690x800_2048x2048_5.png",
			"assets/Textures/Dragon/Entity/Peashot/Dragon_Peashot_690x800_2048x2048_6.png",
			"assets/Textures/Dragon/Entity/Peashot/Dragon_Peashot_690x800_2048x2048_7.png"
			});

		m_PsychicEyeTexture = assets.Load<Teddy::Texture2D>("assets/Textures/Dragon/Projectiles/Dragon_Psychic_151x145_1024x1024_0.png", Teddy::Boolean::True);
	}

	void Dragon::StartIntro()
	{
		auto& sprite = m_Entity.GetComponent<Teddy::SpriteAnimationComponent>();
		sprite.Pause = false;

		m_State = DragonState::Intro;
	}

	void Dragon::LoadIntro()
	{
		auto& transform = m_Entity.GetComponent<Teddy::TransformComponent>();
		transform.Translation = glm::vec3(3.25f, -0.65f, 2.011f);
		transform.Scale = glm::vec3(6.25f, 6.25f, 1.0f);

		m_Entity.GetComponent<Teddy::Rigidbody2DComponent>().SetPosition(transform);

		auto& sprite = m_Entity.GetComponent<Teddy::SpriteAnimationComponent>();
		sprite.PlayableIndicies.clear();
		
		auto& atlas = m_Entity.GetComponent<Teddy::SpriteAtlasComponent>();
		atlas.SpriteWidth = 1000;
		atlas.SpriteHeight = 805;
		
		auto& atlasAnim = m_Entity.GetComponent<Teddy::SpriteAnimationAtlasComponent>();
		atlasAnim.GenerateFrames(sprite, atlas);
		atlasAnim.Index = 0;
	}

	void Dragon::StartIdle()
	{
		auto& sprite = m_Entity.GetComponent<Teddy::SpriteAnimationComponent>();
		sprite.Pause = false;
		sprite.Loop = true;
		sprite.PingPong = false;
		sprite.PlayableIndicies.clear();
		sprite.Textures = m_IdleTextures;

		auto& atlas = m_Entity.GetComponent<Teddy::SpriteAtlasComponent>();
		atlas.SpriteWidth = 690;
		atlas.SpriteHeight = 800;

		auto& atlasAnim = m_Entity.GetComponent<Teddy::SpriteAnimationAtlasComponent>();
		atlasAnim.GenerateFrames(sprite, atlas);
		atlasAnim.Index = 0;

		switch (m_State)
		{
			case DragonState::Peashot:
				atlasAnim.Index = 6;
				break;
			default:
				break;
		}

		auto& transform = m_Entity.GetComponent<Teddy::TransformComponent>();
		transform.Translation = glm::vec3(4.25f, -0.5f, 2.011f);
		m_Entity.GetComponent<Teddy::Rigidbody2DComponent>().SetPosition(transform);

		auto& sensor = m_Entity.GetComponent<Teddy::Sensor2DComponent>();
		m_Scene->DeleteSensor(sensor.Sensors["BellyHitBox"]);
		m_Scene->DeleteSensor(sensor.Sensors["NeckHitBox"]);
		m_Scene->DeleteSensor(sensor.Sensors["HeadHitBox"]);
		sensor.Sensors["BellyHitBox"] = { { -0.4f, -0.75f }, { 1.0f, 1.0f }, 0.0f, false, sensor.Sensors["BellyHitBox"].RuntimeFixture };
		sensor.Sensors["NeckHitBox"] = { { 0.25f, 0.75f }, { 1.25f, 0.5f }, 45.0f, true	, sensor.Sensors["NeckHitBox"].RuntimeFixture };
		sensor.Sensors["HeadHitBox"] = { { -0.5f, 2.0f }, { 1.25f, 0.5f }, 0.0f, true	, sensor.Sensors["HeadHitBox"].RuntimeFixture };
		m_Scene->RefreshSensor(m_Entity, sensor.Sensors["BellyHitBox"]);
		m_Scene->RefreshSensor(m_Entity, sensor.Sensors["NeckHitBox"]);
		m_Scene->RefreshSensor(m_Entity, sensor.Sensors["HeadHitBox"]);

		m_State = DragonState::Idle;
	}

	void Dragon::Idle(Teddy::Timestep ts)
	{
		if (m_Phase == 1)
		{
			m_PeashotTimer += ts;
			if (m_PeashotTimer >= 2.5f && (m_Entity.GetComponent<Teddy::SpriteAnimationAtlasComponent>().Index == 7 || m_Entity.GetComponent<Teddy::SpriteAnimationAtlasComponent>().Index == 8))
			{
				m_PeashotTimer = 0.0f;
				StartPeashot();
			}
		}
	}

	bool Dragon::IsParry(b2ShapeId shape)
	{
		auto& dragSensor = m_Entity.GetComponent<Teddy::Sensor2DComponent>().Sensors;

		for (auto& [name, sensor] : dragSensor)
		{
			if (name == "Parry" && sensor.RuntimeFixture)
			{
				b2ShapeId sensorShape = *static_cast<b2ShapeId*>(sensor.RuntimeFixture);
				if (B2_ID_EQUALS(shape, sensorShape))
					return true;
			}
		}

		for (auto& ent: m_PeashotEntities)
		{
			auto& shotSensor = ent.GetComponent<Teddy::Sensor2DComponent>().Sensors;

			for (auto& [shotName, shotSensor] : shotSensor)
			{
				if (shotName == "Parry" && shotSensor.RuntimeFixture)
				{
					b2ShapeId sensorShape = *static_cast<b2ShapeId*>(shotSensor.RuntimeFixture);
					if (B2_ID_EQUALS(shape, sensorShape))
						return true;
				}
			}
		}
		
		return false;
	}

	bool Dragon::IsSensor(b2ShapeId shape)
	{
		auto& dragSensor = m_Entity.GetComponent<Teddy::Sensor2DComponent>().Sensors;

		for (auto& [_, sensor] : dragSensor)
		{
			if (sensor.RuntimeFixture)
			{
				b2ShapeId sensorShape = *static_cast<b2ShapeId*>(sensor.RuntimeFixture);
				if (B2_ID_EQUALS(shape, sensorShape))
				{
					return true;
				}
			}
		}

		for (auto& ent : m_PeashotEntities)
		{
			auto& shotSensor = ent.GetComponent<Teddy::Sensor2DComponent>().Sensors;

			for (auto& [_, shotSensor] : shotSensor)
			{
				if (shotSensor.RuntimeFixture)
				{
					b2ShapeId sensorShape = *static_cast<b2ShapeId*>(shotSensor.RuntimeFixture);
					if (B2_ID_EQUALS(shape, sensorShape))
						return true;
				}
			}
		}

		return false;
	}

	void Dragon::Hit(int damage)
	{
		if (!m_Hit)
		{
			m_Health -= damage;
			m_Hit = true;	
		}
	}

	void Dragon::StartPeashot()
	{
		auto& sprite = m_Entity.GetComponent<Teddy::SpriteAnimationComponent>();
		sprite.Pause = false;
		sprite.Loop = false;
		sprite.PlayableIndicies.clear();
		sprite.Textures = m_PeashotTextures;

		auto& atlas = m_Entity.GetComponent<Teddy::SpriteAtlasComponent>();
		atlas.SpriteWidth = 690;
		atlas.SpriteHeight = 800;

		auto& atlasAnim = m_Entity.GetComponent<Teddy::SpriteAnimationAtlasComponent>();
		atlasAnim.GenerateFrames(sprite, atlas);
		atlasAnim.Index = 0; 
		sprite.PlayableIndicies = { 0, 1, 2, 3, 4, 5, 6, 7 };

		auto& sensor = m_Entity.GetComponent<Teddy::Sensor2DComponent>();
		sensor.Sensors["BellyHitBox"] = { { -0.4f, -1.25f }, { 1.0f, 1.0f }, 0.0f, false, sensor.Sensors["BellyHitBox"].RuntimeFixture	};
		sensor.Sensors["NeckHitBox"] = { { 0.4f, 0.5f }, { 1.25f, 0.5f }, 45.0f, true	, sensor.Sensors["NeckHitBox"].RuntimeFixture 	};
		sensor.Sensors["HeadHitBox"] = { { 0.0f, 2.0f }, { 1.0f, 0.5f }, 0.0f, true		, sensor.Sensors["HeadHitBox"].RuntimeFixture 	};
		m_Scene->RefreshSensor(m_Entity, sensor.Sensors["BellyHitBox"]);
		m_Scene->RefreshSensor(m_Entity, sensor.Sensors["NeckHitBox"]);
		m_Scene->RefreshSensor(m_Entity, sensor.Sensors["HeadHitBox"]);

		m_State = DragonState::Peashot;
	}

	void Dragon::PsychicEye(Teddy::Timestep ts)
	{
		if (m_Shooting)
		{
			static int count = 0;
			static float timer = 0.0f;
			static glm::vec2 playerPos = m_PlayerPosition;
			static bool canShoot = false;

			timer += ts;

			if(playerPos.x == 0 && playerPos.y == 0) 
				playerPos = m_PlayerPosition;

			if (!m_PsychicEyeEntity)
			{
				m_PsychicEyeEntity = m_Scene->CreateEntity("Psychic Eye");
				auto& sprite = m_PsychicEyeEntity.AddComponent<Teddy::SpriteAnimationComponent>(0.05f, 0.05f, 0.05f);
				sprite.Textures = { m_PsychicEyeTexture };
				sprite.PlayableIndicies = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
				sprite.Loop = true;

				auto& atlas = m_PsychicEyeEntity.AddComponent<Teddy::SpriteAtlasComponent>(0, 0, 151, 145);

				auto& transform = m_PsychicEyeEntity.GetComponent<Teddy::TransformComponent>();
				transform.Translation = m_Entity.GetComponent<Teddy::TransformComponent>().Translation + glm::vec3(-0.5f, 2.0f, 0.1f);
				count = 0;
				timer = 0.0f;
				playerPos = m_PlayerPosition;

				canShoot = false;
			}
			else if (m_PsychicEyeEntity.GetComponent<Teddy::SpriteAnimationAtlasComponent>().Index >= 5)
			{
				if(!canShoot)
					playerPos = m_PlayerPosition;
				canShoot = true;
			}

			if (canShoot && count < 3 && timer >= 0.2f)
			{
				auto ent = m_Scene->CreateEntity("Psychic Eye Projectile");
				auto& sprite = ent.AddComponent<Teddy::SpriteAnimationComponent>(0.05f, 0.05f, 0.05f);
				sprite.Textures = { m_PsychicEyeTexture };
				auto& atlas = ent.AddComponent<Teddy::SpriteAtlasComponent>(0, 0, 151, 145);

				auto& aA = ent.AddComponent<Teddy::SpriteAnimationAtlasComponent>();

				auto& transform = ent.GetComponent<Teddy::TransformComponent>();
				transform.Translation = m_Entity.GetComponent<Teddy::TransformComponent>().Translation + glm::vec3(-0.5f, 2.0f, 0.11f);
				glm::vec2 direction = playerPos - glm::vec2(transform.Translation.x, transform.Translation.y);
				transform.Rotation.z = std::atan2(direction.y, direction.x);

				auto& rb = ent.AddComponent<Teddy::Rigidbody2DComponent>();
				rb.Type = Teddy::Rigidbody2DComponent::BodyType::Kinematic;

				auto& sensor = ent.AddComponent<Teddy::Sensor2DComponent>();
				
				if (count == 2)
				{
					sprite.PlayableIndicies = { 19, 20, 21, 22, 23, 24, 25, 26 };
					aA.Index = 19;
					sensor.Sensors["Parry"] = { { 0.125f, 0.0f }, { 0.25f, 0.45f }, glm::degrees(transform.Rotation.z), true };
					m_Shooting = false;
					m_Scene->DestroyEntity(m_PsychicEyeEntity);
					m_PsychicEyeEntity = {};
					canShoot = false;
				}
				else
				{
					sprite.PlayableIndicies = { 11, 12, 13, 14, 15, 16, 17, 18 };
					aA.Index = 11;
					sensor.Sensors["HitBox"] = { { 0.125f, 0.0f }, { 0.25f, 0.45f }, glm::degrees(transform.Rotation.z), true };
				}

				auto& filter = ent.AddComponent<Teddy::CollisionFilter2DComponent>();
				filter.CategoryBits = LevelCategories::ENEMY;
				filter.MaskBits = LevelCategories::PLAYER;

				m_Scene->RefreshBody(ent);
				rb.SetVelocity(std::cos(transform.Rotation.z) * 5.0f, std::sin(transform.Rotation.z) * 5.0f);

				m_PeashotEntities.push_back(ent);

				count++;

				timer = 0.0f;
			}
			m_PeashotTimer = 0.0f;
		}
	}

	void Dragon::Peashot(Teddy::Timestep ts)
	{
		if (m_Shooting) return;

		auto& aA = m_Entity.GetComponent<Teddy::SpriteAnimationAtlasComponent>();
		auto& sprite = m_Entity.GetComponent<Teddy::SpriteAnimationComponent>();

		static int shot = 0;
		const int maxShots = 3; // TODO: randomizer with like 70/30 for 2/3
		static float timer = 0.0f;
		timer += ts;
		
		if (sprite.PlayableIndicies.size() > 10)
		{
			if (shot < maxShots)
			{
				if (timer >= 0.75f)
				{
					m_Shooting = true;
					PsychicEye(ts);
					shot++;
					timer = 0.0f;
				}
			}
			else if (aA.Index == 8 && timer >= 1.0f)
			{
				sprite.PlayableIndicies = { 24, 25, 26, 27, 28, 29, 30, 31 };
			}
		} 
		else if (aA.Index <= 7)
		{
			static bool inPosition = false;
			if (inPosition && aA.Index == 7)
			{
				aA.Index = 8;
				sprite.Loop = true;
				sprite.PlayableIndicies = { 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23 };
				shot = 1;
				timer = 0.0f;
				inPosition = false;
				m_Shooting = true;
				PsychicEye(ts);
			}
			else
			{
				auto& transform = m_Entity.GetComponent<Teddy::TransformComponent>();
				transform.Translation += glm::vec3(0.0f, ts * 5.0f, 0.0f);
				m_Entity.GetComponent<Teddy::Rigidbody2DComponent>().SetPosition(transform);

				if (transform.Translation.y >= -0.1f)
				{
					transform.Translation.y = 0.1f;
					m_Entity.GetComponent<Teddy::Rigidbody2DComponent>().SetPosition(transform);
					inPosition = true;
				}
			}
		}
		else if (aA.Index >= 24)
		{
			static bool inOriginalPosition = false;
			if (inOriginalPosition && aA.Index == 31)
			{
				if(m_PsychicEyeEntity)
					m_Scene->DestroyEntity(m_PsychicEyeEntity);
				m_PsychicEyeEntity = {};
				m_Shooting = false;

				StartIdle();
				shot = 0;
				timer = 0.0f;
				inOriginalPosition = false;
			}
			else
			{
				auto& transform = m_Entity.GetComponent<Teddy::TransformComponent>();
				transform.Translation -= glm::vec3(0.0f, ts * 5.0f, 0.0f);
				m_Entity.GetComponent<Teddy::Rigidbody2DComponent>().SetPosition(transform);

				sprite.Loop = false;

				if (transform.Translation.y <= -0.5f)
				{
					transform.Translation.y = -0.5f;
					m_Entity.GetComponent<Teddy::Rigidbody2DComponent>().SetPosition(transform);
					inOriginalPosition = true;
				}
			}
		}
	}

	void Dragon::ClearProjectiles()
	{
		std::vector<Teddy::Entity> newPeashots;
		for (auto& ent : m_PeashotEntities)
		{
			auto& transform = ent.GetComponent<Teddy::TransformComponent>();
			if (transform.Translation.x >= 5.5f || transform.Translation.x <= -5.5f ||
				transform.Translation.y <= -3.5f || transform.Translation.y >= 3.5f)
			{
				m_Scene->DestroyEntity(ent);
			}
			else
			{
				newPeashots.push_back(ent);
			}
		}
		m_PeashotEntities = newPeashots;
	}
}