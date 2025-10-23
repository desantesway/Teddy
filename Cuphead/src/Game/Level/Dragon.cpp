#include "Dragon.h"

#include <Teddy.h>

#include "LevelCategories.h"

namespace Cuphead
{
	void Dragon::OnUpdate(Teddy::Timestep ts)
	{
		Hitting(ts);

		if (m_State == DragonState::Idle && m_Phase == 1)
		{
			m_PeashotTimer += ts;
			if (m_PeashotTimer >= 2.5f && (m_Entity.GetComponent<Teddy::SpriteAnimationAtlasComponent>().Index == 7 || m_Entity.GetComponent<Teddy::SpriteAnimationAtlasComponent>().Index == 8))
			{
				m_PeashotTimer = 0.0f;
				StartPeashot();
			}
		}

		switch (m_State)
		{
		case DragonState::Peashot:
			Peashot(ts);
			break;
		case DragonState::Idle:
			Idle();
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

	void Dragon::Idle()
	{

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
					return true;
			}
		}
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

	void Dragon::Peashot(Teddy::Timestep ts)
	{
		auto& aA = m_Entity.GetComponent<Teddy::SpriteAnimationAtlasComponent>();
		auto& sprite = m_Entity.GetComponent<Teddy::SpriteAnimationComponent>();

		static int shot = 0;
		const int maxShots = 3; // TODO: randomizer with like 70/30 for 2/3
		static float timer = 0.0f;
		timer += ts;
		
		if (sprite.PlayableIndicies.size() > 10)
		{
			if (shot <= maxShots)
			{
				if (timer >= 2.5f)
				{
					TED_CORE_INFO("PIUPIU");
					shot++;
					timer = 0.0f;
				}
			}
			else if (aA.Index == 8)
			{
				sprite.PlayableIndicies = { 24, 25, 26, 27, 28, 29, 30, 31 };
			}
		} 
		else if (aA.Index <= 7)
		{
			// start shot
			static bool inPosition = false;
			if (inPosition && aA.Index == 7)
			{
				aA.Index = 8;
				sprite.Loop = true;
				sprite.PlayableIndicies = { 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23 };
				shot = 1;
				timer = 0.0f;
				inPosition = false;
				TED_CORE_INFO("PIU PIU");
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
}