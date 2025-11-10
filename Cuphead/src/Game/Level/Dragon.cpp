#include "Dragon.h"

#include <Teddy.h>

#include "LevelCategories.h"
#include "Randomizer.h"

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
		case DragonState::Meteor:
			Meteor(ts);
			break;
		case DragonState::Idle:
			Idle(ts);
			break;
		case DragonState::Intro:
			Intro();
			break;
		case DragonState::Firebubble:
			Firebubble();
			break;
		case DragonState::Firetorch:
			FireTorch(ts);
			break;
		default:
			break;
		}

		Tail(ts);
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

			if (m_TailEntity)
			{
				auto& tailColor = m_TailEntity.GetComponent<Teddy::SpriteAnimationComponent>().Color;
				tailColor = color;
			}

			if (m_Phase3Heads.LeftHead)
			{
				auto& leftHeadColor = m_Phase3Heads.LeftHead.GetComponent<Teddy::SpriteAnimationComponent>().Color;
				leftHeadColor = color;
			}
			if (m_Phase3Heads.RightHead)
			{
				auto& rightHeadColor = m_Phase3Heads.RightHead.GetComponent<Teddy::SpriteAnimationComponent>().Color;
				rightHeadColor = color;
			}
			if (m_Phase3Heads.MidHead)
			{
				auto& midHeadColor = m_Phase3Heads.MidHead.GetComponent<Teddy::SpriteAnimationComponent>().Color;
				midHeadColor = color;
			}
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

		m_PsychicEyeTexture = assets.LoadMultiple<Teddy::Texture2D>({
			"assets/Textures/Dragon/Projectiles/Dragon_Psychic_151x145_512x512_0.png",
			"assets/Textures/Dragon/Projectiles/Dragon_Psychic_151x145_512x512_1.png",
			"assets/Textures/Dragon/Projectiles/Dragon_Psychic_151x145_512x512_2.png"
			}); 

		m_MeteorTextures = assets.LoadMultiple<Teddy::Texture2D>({
			"assets/Textures/Dragon/Entity/Meteor/Dragon_Met_800x800_2048x2048_0.png",
			"assets/Textures/Dragon/Entity/Meteor/Dragon_Met_800x800_2048x2048_1.png",
			"assets/Textures/Dragon/Entity/Meteor/Dragon_Met_800x800_2048x2048_2.png",
			"assets/Textures/Dragon/Entity/Meteor/Dragon_Met_800x800_2048x2048_3.png",
			"assets/Textures/Dragon/Entity/Meteor/Dragon_Met_800x800_2048x2048_4.png",
			"assets/Textures/Dragon/Entity/Meteor/Dragon_Met_800x800_2048x2048_5.png",
			"assets/Textures/Dragon/Entity/Meteor/Dragon_Met_800x800_2048x2048_6.png",
			"assets/Textures/Dragon/Entity/Meteor/Dragon_Met_800x800_2048x2048_7.png"
			});

		m_MeteorProjectileTextures = assets.LoadMultiple<Teddy::Texture2D>({
			"assets/Textures/Dragon/Projectiles/Meteor_240x193_1024x1024_0.png",
			"assets/Textures/Dragon/Projectiles/Meteor_240x193_1024x1024_1.png",
			"assets/Textures/Dragon/Projectiles/Meteor_240x193_1024x1024_2.png"
			});

		m_TailTextures = assets.LoadMultiple<Teddy::Texture2D>({
			"assets/Textures/Dragon/Entity/Tail/Dragon_Tail_303x856_2048x2048_0.png"
			});

		m_DashTextures = assets.LoadMultiple<Teddy::Texture2D>({
			"assets/Textures/Dragon/Entity/Dash/Dragon_Dash_481x127_512x512_0.png",
			"assets/Textures/Dragon/Entity/Dash/Dragon_Dash_481x127_512x512_1.png"
			});

		m_DragonTongueTextures = assets.LoadMultiple<Teddy::Texture2D>({
			"assets/Textures/Dragon/Entity/DragonTongue/Dragon_Tongu_600x750_2048x2048_0.png",
			"assets/Textures/Dragon/Entity/DragonTongue/Dragon_Tongu_600x750_2048x2048_1.png",
			"assets/Textures/Dragon/Entity/DragonTongue/Dragon_Tongu_600x750_2048x2048_2.png",
			"assets/Textures/Dragon/Entity/DragonTongue/Dragon_Tongu_600x750_2048x2048_3.png",
			"assets/Textures/Dragon/Entity/DragonTongue/Dragon_Tongu_600x750_2048x2048_4.png",
			"assets/Textures/Dragon/Entity/DragonTongue/Dragon_Tongu_600x750_2048x2048_5.png",
			"assets/Textures/Dragon/Entity/DragonTongue/Dragon_Tongu_600x750_2048x2048_6.png",
			"assets/Textures/Dragon/Entity/DragonTongue/Dragon_Tongu_600x750_2048x2048_7.png"
			});

		m_TongueTextures = assets.LoadMultiple<Teddy::Texture2D>({
			"assets/Textures/Dragon/Entity/Tongue/Tong_1190x160_2048x2048_0.png",
			"assets/Textures/Dragon/Entity/Tongue/Tong_1190x160_2048x2048_1.png"
			});

		m_SmokeTextures = assets.LoadMultiple<Teddy::Texture2D>({
			"assets/Textures/Dragon/Entity/SmokeFire/Dragon_Smoke_Fire_380x533_2048x2048_0.png",
			"assets/Textures/Dragon/Entity/SmokeFire/Dragon_Smoke_Fire_380x533_2048x2048_1.png",
			"assets/Textures/Dragon/Entity/SmokeFire/Dragon_Smoke_Fire_380x533_2048x2048_2.png"
			});

		m_FireMarcherTextures = assets.LoadMultiple<Teddy::Texture2D>({
			"assets/Textures/Dragon/Projectiles/Dragon_Firemarcher_173x202_2048x2048_0.png"
			});

		m_Phase2DeathTextures = assets.LoadMultiple<Teddy::Texture2D>({
			"assets/Textures/Dragon/Entity/Ph2_Death/Dragon_Death_510x750_2048x2048_0.png",
			"assets/Textures/Dragon/Entity/Ph2_Death/Dragon_Death_510x750_2048x2048_1.png"
			});

		m_Phase3IntroTextures = assets.LoadMultiple<Teddy::Texture2D>({
			"assets/Textures/Dragon/Entity/Ph3_Intro/Dragon_Intro_ph3_1000x1000_2048x2048_0.png",
			"assets/Textures/Dragon/Entity/Ph3_Intro/Dragon_Intro_ph3_1000x1000_2048x2048_1.png",
			"assets/Textures/Dragon/Entity/Ph3_Intro/Dragon_Intro_ph3_1000x1000_2048x2048_2.png",
			"assets/Textures/Dragon/Entity/Ph3_Intro/Dragon_Intro_ph3_1000x1000_2048x2048_3.png",
			"assets/Textures/Dragon/Entity/Ph3_Intro/Dragon_Intro_ph3_1000x1000_2048x2048_4.png",
			"assets/Textures/Dragon/Entity/Ph3_Intro/Dragon_Intro_ph3_1000x1000_2048x2048_5.png",
			"assets/Textures/Dragon/Entity/Ph3_Intro/Dragon_Intro_ph3_1000x1000_2048x2048_6.png",
			"assets/Textures/Dragon/Entity/Ph3_Intro/Dragon_Intro_ph3_1000x1000_2048x2048_7.png",
			"assets/Textures/Dragon/Entity/Ph3_Intro/Dragon_Intro_ph3_1000x1000_2048x2048_8.png",
			"assets/Textures/Dragon/Entity/Ph3_Intro/Dragon_Intro_ph3_1000x1000_2048x2048_9.png"
			});

		m_Phase3IdleBodyTextures = assets.LoadMultiple<Teddy::Texture2D>({
			"assets/Textures/Dragon/Entity/Ph3_Idle/Body/Dragon_Idle_Body_ph3_500x500_2048x2048_0.png"
			});

		std::vector<std::string> paths;
		for (int i = 0; i < 18; i++)
		{
			paths.push_back("assets/Textures/Dragon/Entity/Ph3_Idle/Heads/Dragon_Idle_Head_ph3_900x900_2048x2048_" + std::to_string(i) + ".png");
		}
		m_Phase3IdleHeadTextures = assets.LoadMultiple<Teddy::Texture2D>(paths);

		paths.clear();
		for (int i = 0; i < 14; i++)
		{
			paths.push_back("assets/Textures/Dragon/Entity/Ph3_Attack/Dragon_Attack_ph3_900x900_2048x2048_" + std::to_string(i) + ".png");
		}
		m_Phase3AttackHeadTextures = assets.LoadMultiple<Teddy::Texture2D>(paths);

		m_Phase3FirebubbleTextures = assets.LoadMultiple<Teddy::Texture2D>({
			"assets/Textures/Dragon/Projectiles/Ph3_Firebubble/Dragon_Firebubble_550x500_2048x2048_0.png",
			"assets/Textures/Dragon/Projectiles/Ph3_Firebubble/Dragon_Firebubble_550x500_2048x2048_1.png",
			"assets/Textures/Dragon/Projectiles/Ph3_Firebubble/Dragon_Firebubble_550x500_2048x2048_2.png",
			"assets/Textures/Dragon/Projectiles/Ph3_Firebubble/Dragon_Firebubble_550x500_2048x2048_3.png",
			"assets/Textures/Dragon/Projectiles/Ph3_Firebubble/Dragon_Firebubble_550x500_2048x2048_4.png"
			});

		m_Phase3MiniFirebubbleTextures = assets.LoadMultiple<Teddy::Texture2D>({
			"assets/Textures/Dragon/Projectiles/Dragon_Minifire_78x78_512x512_0.png",
			});

		m_Phase3FiretorchTextures = assets.LoadMultiple<Teddy::Texture2D>({
			"assets/Textures/Dragon/Entity/Ph3_Firetorch/Dragon_FireTorch_900x900_2048x2048_0.png",
			"assets/Textures/Dragon/Entity/Ph3_Firetorch/Dragon_FireTorch_900x900_2048x2048_1.png",
			"assets/Textures/Dragon/Entity/Ph3_Firetorch/Dragon_FireTorch_900x900_2048x2048_2.png",
			"assets/Textures/Dragon/Entity/Ph3_Firetorch/Dragon_FireTorch_900x900_2048x2048_3.png",
			"assets/Textures/Dragon/Entity/Ph3_Firetorch/Dragon_FireTorch_900x900_2048x2048_4.png",
			"assets/Textures/Dragon/Entity/Ph3_Firetorch/Dragon_FireTorch_900x900_2048x2048_5.png",
			"assets/Textures/Dragon/Entity/Ph3_Firetorch/Dragon_FireTorch_900x900_2048x2048_6.png"
			});

		m_Phase3FiretorchProjectileTextures = assets.LoadMultiple<Teddy::Texture2D>({
			"assets/Textures/Dragon/Projectiles/Ph3_Firetorch/Dragon_Firetorch_Projectile_1000x500_2048x2048_0.png",
			"assets/Textures/Dragon/Projectiles/Ph3_Firetorch/Dragon_Firetorch_Projectile_1000x500_2048x2048_1.png",
			"assets/Textures/Dragon/Projectiles/Ph3_Firetorch/Dragon_Firetorch_Projectile_1000x500_2048x2048_2.png"
			});

		m_Phase3DeathTextures = assets.LoadMultiple<Teddy::Texture2D>({
			"assets/Textures/Dragon/Entity/Ph3_Death/Dragon_Ph3_Death_637x872_2048x2048_0.png",
			"assets/Textures/Dragon/Entity/Ph3_Death/Dragon_Ph3_Death_637x872_2048x2048_1.png"
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

	void Dragon::Idle(Teddy::Timestep ts)
	{
		if (m_Phase == 1)
		{
			if (m_Health > 1535)
			{
				Phase1Part1(ts);
			}
			else if (m_Health > 1071)
			{
				Phase1Part2(ts);
			}
			else
			{
				m_Phase = 2;
				m_PhaseStart = true;
			}
		}
		else if (m_Phase == 2)
		{
			if (m_PhaseStart)
			{
				Phase1To2(ts);
			}
			else if (!m_Phase2Start)
			{
				Phase2Start(ts);
			}
			else
			{
				if (m_Health > 561)
				{
					Smoke(ts);
					FireMarchers(ts);
				}
				else
				{
					Phase2Ending();
					m_Phase = 3;
					m_PhaseStart = true;
				}
			}
		}
		else if (m_Phase == 3)
		{
			if (m_PhaseStart)
			{
				Phase2To3(ts);
			}
			else if (m_Phase3Start)
			{
				if(!m_Phase3StartLoop)
					Phase3Start(ts);
				else
					Phase3StartLoop(ts);
			}
			else
			{
				if (m_Health < 0)
				{
					if (m_Phase3Heads.LeftHead)
					{
						StartPhase3Death();
					}

					return;
				}
				if (m_FirebubbleSpitEntity)
				{
					auto& aA = m_FirebubbleSpitEntity.GetComponent<Teddy::SpriteAnimationAtlasComponent>();
					if (aA.Index == 56)
					{
						m_Scene->DestroyEntity(m_FirebubbleSpitEntity);
						m_FirebubbleSpitEntity = Teddy::Entity();
					}
				}
				if (m_FiretorchProjectileEntity)
				{
					auto& aA = m_FiretorchProjectileEntity.GetComponent<Teddy::SpriteAnimationAtlasComponent>();
					if (aA.Index == 17)
					{
						m_Scene->DestroyEntity(m_FiretorchProjectileEntity);
						m_FiretorchProjectileEntity = Teddy::Entity();
					}
				}
				std::vector<FirebubbleProjectile> firebubblesToRemove;
				std::vector<FirebubbleProjectile> newFirebubbles;
				for (auto& bubble : m_Firebubbles)
				{
					auto& aA = bubble.Entity.GetComponent<Teddy::SpriteAnimationAtlasComponent>();
					if (bubble.Health <= 0)
					{
						if (aA.Index == 7)
						{
							firebubblesToRemove.push_back(bubble);
							continue;
						}
					}
					else
					{
						if (aA.Index == 15)
						{
							auto& spriteFB = bubble.Entity.GetComponent<Teddy::SpriteAnimationComponent>();
							spriteFB.Loop = true;
							spriteFB.PlayableIndicies.clear();
							for (int i = 16; i < 40; i++)
								spriteFB.PlayableIndicies.push_back(i);
						}
					}
					newFirebubbles.push_back(bubble);
				}
				m_Firebubbles = newFirebubbles;
				for (auto& bubble : firebubblesToRemove)
				{
					m_Scene->DestroyEntity(bubble.Entity);
				}
				static float timer = 0.0f;
				static float firetorchTimer = 0.0f;
				timer += ts;
				firetorchTimer += ts;
				if (firetorchTimer >= 10.0f)
				{
					auto& aAMidHead = m_Phase3Heads.MidHead.GetComponent<Teddy::SpriteAnimationAtlasComponent>();
					if (aAMidHead.Index == 71)
					{
						StartFireTorch();
						firetorchTimer = 0.0f;
					}
				}
				else if (timer >= 1.25f)
				{
					static int headToAttack = Randomizer::Get().RandomInt(0, 2);
					auto& aALeftHead = m_Phase3Heads.LeftHead.GetComponent<Teddy::SpriteAnimationAtlasComponent>();
					auto& aARightHead = m_Phase3Heads.RightHead.GetComponent<Teddy::SpriteAnimationAtlasComponent>();
					auto& aAMidHead = m_Phase3Heads.MidHead.GetComponent<Teddy::SpriteAnimationAtlasComponent>();
					if (headToAttack == 0 && (aARightHead.Index == 7 || aARightHead.Index == 19))
					{
						timer = 0.0f;
						m_Phase3Heads.SelectedHead = headToAttack;
						StartPhase3Attack();
						headToAttack = Randomizer::Get().RandomInt(0, 2);
					}
					else if (headToAttack == 1 && (aAMidHead.Index == (4 * 9) + 7 || aAMidHead.Index == (4 * 9) + 19))
					{
						timer = 0.0f;
						m_Phase3Heads.SelectedHead = headToAttack;
						StartPhase3Attack();
						headToAttack = Randomizer::Get().RandomInt(0, 2);
					}
					else if (headToAttack == 2 && (aALeftHead.Index == (4 * 9) + 7 || aALeftHead.Index == (4 * 9) + 19))
					{
						timer = 0.0f;
						m_Phase3Heads.SelectedHead = headToAttack;
						StartPhase3Attack();
						headToAttack = Randomizer::Get().RandomInt(0, 2);
					}
				}
			}
		}
	}

	void Dragon::StartPhase3Death()
	{
		m_Scene->DestroyEntity(m_Phase3Heads.LeftHead);
		m_Phase3Heads.LeftHead = Teddy::Entity();
		m_Scene->DestroyEntity(m_Phase3Heads.RightHead);
		m_Phase3Heads.RightHead = Teddy::Entity();
		m_Scene->DestroyEntity(m_Phase3Heads.MidHead);
		m_Phase3Heads.MidHead = Teddy::Entity();

		auto& sprite = m_Entity.GetComponent<Teddy::SpriteAnimationComponent>();
		sprite.Textures = m_Phase3DeathTextures;
		sprite.FrameTime = 0.05f;
		sprite.FinalFrameTime = 0.05f;
		sprite.InitialFrameTime = 0.05f;
		sprite.PlayableIndicies = { 0, 1, 2, 3, 4, 5 , 6, 7 };

		auto& atlas = m_Entity.GetComponent<Teddy::SpriteAtlasComponent>();
		atlas.SpriteWidth = 637;
		atlas.SpriteHeight = 872;

		auto& atlasAnim = m_Entity.GetComponent<Teddy::SpriteAnimationAtlasComponent>();
		atlasAnim.GenerateFrames(sprite, atlas);
		atlasAnim.Index = 0;

		auto& transform = m_Entity.GetComponent<Teddy::TransformComponent>();
		transform.Scale = glm::vec3(6.25f, 6.25f, 1.0f);
		transform.Translation = glm::vec3(-3.5f, -0.5f, 2.011f);

		auto& body = m_Entity.GetComponent<Teddy::Rigidbody2DComponent>();

		body.SetPosition(transform);

		m_State = DragonState::Death;
	}

	void Dragon::StartFireTorch()
	{
		auto& sprite = m_Phase3Heads.MidHead.GetComponent<Teddy::SpriteAnimationComponent>();
		sprite.Textures = m_Phase3FiretorchTextures;
		sprite.FrameTime = 0.05f;
		sprite.FinalFrameTime = 0.05f;
		sprite.InitialFrameTime = 0.05f;
		sprite.PlayableIndicies = {0, 1, 2, 3, 4};
		sprite.Loop = false;

		auto& atlas = m_Phase3Heads.MidHead.GetComponent<Teddy::SpriteAtlasComponent>();
		atlas.SpriteWidth = 900;
		atlas.SpriteHeight = 900;

		auto& atlasAnim = m_Phase3Heads.MidHead.GetComponent<Teddy::SpriteAnimationAtlasComponent>();
		atlasAnim.GenerateFrames(sprite, atlas);

		m_State = DragonState::Firetorch;
	}

	void Dragon::FireTorch(Teddy::Timestep ts)
	{
		if (m_FiretorchProjectileEntity)
		{
			auto& aA = m_FiretorchProjectileEntity.GetComponent<Teddy::SpriteAnimationAtlasComponent>();
			auto& sprite = m_FiretorchProjectileEntity.GetComponent<Teddy::SpriteAnimationComponent>();
			if (aA.Index >= 3 && aA.Index <= 8 && sprite.PlayableIndicies.size() == 9)
			{
				sprite.PlayableIndicies = { 3, 4, 5, 6, 7, 8 };
				sprite.Loop = true;
			}
			else if (aA.Index == 17)
			{
				m_Scene->DestroyEntity(m_FiretorchProjectileEntity);
				m_FiretorchProjectileEntity = Teddy::Entity();
			}
		}

		static float timer = 0.0f;
		timer += ts;
		auto& aA = m_Phase3Heads.MidHead.GetComponent<Teddy::SpriteAnimationAtlasComponent>();
		auto& sprite = m_Phase3Heads.MidHead.GetComponent<Teddy::SpriteAnimationComponent>();
		static bool stop = false;

		if (sprite.PlayableIndicies.size() == 6 && aA.Index == 23)
		{
			auto& spriteMH = m_Phase3Heads.MidHead.GetComponent<Teddy::SpriteAnimationComponent>();
			spriteMH.Textures = m_Phase3IdleHeadTextures;
			spriteMH.Loop = true;
			sprite.FrameTime = 0.04f;
			sprite.FinalFrameTime = 0.04f;
			sprite.InitialFrameTime = 0.04f;

			spriteMH.PlayableIndicies.clear();
			for (int i = (4 * 9); i < (4 * 9 * 2); i++)
				spriteMH.PlayableIndicies.push_back(i);

			auto& atlasMH = m_Phase3Heads.MidHead.GetComponent<Teddy::SpriteAtlasComponent>();
			atlasMH.SpriteHeight = 900;
			atlasMH.SpriteWidth = 900;

			auto& aAMH = m_Phase3Heads.MidHead.GetComponent<Teddy::SpriteAnimationAtlasComponent>();
			aAMH.GenerateFrames(spriteMH, atlasMH);
			aAMH.Index = m_Phase3Heads.LeftHead.GetComponent<Teddy::SpriteAnimationAtlasComponent>().Index - 12;

			timer = 0.0f;

			m_State = DragonState::Idle;
		}
		else if (sprite.PlayableIndicies.size() == 6 && aA.Index == 12)  // loop the fire torch
		{
			sprite.Loop = true;
			sprite.Reverse = false;
			sprite.PlayableIndicies = { 12, 13 };
			timer = 0.0f;
			stop = true;
			CreateFiretorchProjectile();
		}
		else if (timer >= 1.0f && sprite.PlayableIndicies.size() == 2 && aA.Index == 17) // progress to fire from breath in // breath out
		{
			sprite.Loop = false;
			sprite.Reverse = true;
			sprite.PlayableIndicies = { 12, 13, 14, 15, 16, 17 };
			timer = 0.0f;
			stop = false;
		}
		else if (sprite.PlayableIndicies.size() == 4 && aA.Index == 17) // breath in loop
		{
			sprite.Loop = true;
			sprite.PlayableIndicies = { 16, 17 };
			timer = 0.0f;
			stop = false;
		}
		else if (timer >= 2.0f && sprite.PlayableIndicies.size() == 2 && aA.Index == 13) // breath in / get to normal
		{
			if (stop)
			{
				sprite.Loop = false;
				sprite.PlayableIndicies = { 18, 19, 20, 21, 22, 23 };
				timer = 0.0f;
				stop = false;
			}
			else
			{
				sprite.Loop = false;
				sprite.PlayableIndicies = { 14, 15, 16, 17 };
				timer = 0.0f;
			}

			if (m_FiretorchProjectileEntity)
			{
				auto& sprite = m_FiretorchProjectileEntity.GetComponent<Teddy::SpriteAnimationComponent>();
				sprite.PlayableIndicies = { 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17 };
				sprite.Loop = false;
				auto& sensor = m_FiretorchProjectileEntity.GetComponent<Teddy::Sensor2DComponent>();
				m_Scene->DeleteSensor(sensor.Sensors["HitBox"]);
				sensor.Sensors.clear();
			}
		}
		else if (sprite.PlayableIndicies.size() == 7 && aA.Index == 12) // loop the fire torch
		{
			sprite.Loop = true;
			sprite.PlayableIndicies = { 12, 13 };
			timer = 0.0f;
			stop = false;
			CreateFiretorchProjectile();
		}
		else if (timer >= 1.0f && aA.Index == 6) // progress to fire
		{
			sprite.Loop = false;
			sprite.PlayableIndicies = { 6, 7, 8, 9, 10, 11, 12 };
			timer = 0.0f;
			stop = false;
		}
		else if (aA.Index == 4 && sprite.PlayableIndicies.size() > 3) // start first loop
		{
			sprite.Loop = true;
			sprite.PlayableIndicies = { 4, 5, 6 };
			timer = 0.0f;
			stop = false;
		}
	}

	void Dragon::CreateFiretorchProjectile()
	{
		if(m_FiretorchProjectileEntity)
			m_Scene->DestroyEntity(m_FiretorchProjectileEntity);
		m_FiretorchProjectileEntity = m_Scene->CreateEntity("Dragon Firetorch Projectile");
		auto& sprite = m_FiretorchProjectileEntity.AddComponent<Teddy::SpriteAnimationComponent>(0.05f);
		sprite.Textures = m_Phase3FiretorchProjectileTextures;
		sprite.Loop = false;

		auto& atlas = m_FiretorchProjectileEntity.AddComponent<Teddy::SpriteAtlasComponent>(0, 0, 1000, 500);

		sprite.PlayableIndicies = { 0, 1, 2, 3, 4, 5, 6, 7, 8 };

		auto& transform = m_FiretorchProjectileEntity.GetComponent<Teddy::TransformComponent>();
		transform.Scale = glm::vec3(3.5f, 3.5f, 1.0f);
		transform.Translation = glm::vec3(3.0f, 0.65f, 2.02f);

		auto& body = m_FiretorchProjectileEntity.AddComponent<Teddy::Rigidbody2DComponent>();
		body.Type = Teddy::Rigidbody2DComponent::BodyType::Static;

		auto& filter = m_FiretorchProjectileEntity.AddComponent<Teddy::CollisionFilter2DComponent>();
		filter.CategoryBits = LevelCategories::ENEMY;
		filter.MaskBits = LevelCategories::PLAYER;

		auto& sensor = m_FiretorchProjectileEntity.AddComponent<Teddy::Sensor2DComponent>();

		sensor.Sensors["HitBox"] = { { 0.0f, -0.2f }, { 4.0f, 0.4f }, 0.0f, true };

		m_Scene->RefreshBody(m_FiretorchProjectileEntity);
	}

	void Dragon::Phase1Part1(Teddy::Timestep ts)
	{
		m_PeashotTimer += ts;
		if (m_PeashotTimer >= 2.5f && (m_Entity.GetComponent<Teddy::SpriteAnimationAtlasComponent>().Index == 7 || m_Entity.GetComponent<Teddy::SpriteAnimationAtlasComponent>().Index == 8))
		{
			m_PeashotTimer = 0.0f;
			StartPeashot();
		}
	}

	void Dragon::Phase1Part2(Teddy::Timestep ts)
	{
		static bool choosed = false;
		static bool meteorAttack = true;
		m_PeashotTimer += ts;
		if (m_PeashotTimer >= 2.5f)
		{
			if (!choosed)
			{
				if (m_MeteorStart)
				{
					m_MeteorStart = false;
					meteorAttack = true;
					choosed = true;
					m_LaunchThreeMeteors = false;
				}
				else
				{
					meteorAttack = Randomizer::Get().RandomBool(0.6f);
					choosed = true;
					m_LaunchThreeMeteors = true;
				}
			}

			if (meteorAttack && m_Entity.GetComponent<Teddy::SpriteAnimationAtlasComponent>().Index == 0)
			{
				m_PeashotTimer = 0.0f;
				choosed = false;
				m_MeteorsLaunched = 0;
				StartMeteor();
				StartTail();
			}
			else if (!meteorAttack && (m_Entity.GetComponent<Teddy::SpriteAnimationAtlasComponent>().Index == 7 || m_Entity.GetComponent<Teddy::SpriteAnimationAtlasComponent>().Index == 8))
			{
				m_PeashotTimer = 0.0f;
				choosed = false;
				StartPeashot();
				StartTail();
			}
		}
	}

	void Dragon::Phase1To2(Teddy::Timestep ts)
	{
		if (m_Entity.GetComponent<Teddy::CollisionFilter2DComponent>().CategoryBits != 0)
		{
			auto& transform = m_Entity.GetComponent<Teddy::TransformComponent>();
			transform.Translation.x += 2.5f * ts;

			if (transform.Translation.x > 8.0f)
			{
				auto& filter = m_Entity.GetComponent<Teddy::CollisionFilter2DComponent>();
				filter.CategoryBits = 0;
				filter.MaskBits = 0;
				filter.SetFilterCategory(m_Entity.GetComponent<Teddy::Sensor2DComponent>(), filter.CategoryBits);
				filter.SetFilterMask(m_Entity.GetComponent<Teddy::Sensor2DComponent>(), filter.MaskBits);
			}

			auto& body = m_Entity.GetComponent<Teddy::Rigidbody2DComponent>();
			body.SetPosition(transform);
		}
		else
		{
			if (!m_Transitioning)
			{
				auto& sprite = m_Entity.GetComponent<Teddy::SpriteAnimationComponent>();
				sprite.Loop = true;
				sprite.Textures = m_DashTextures;

				auto& atlas = m_Entity.GetComponent<Teddy::SpriteAtlasComponent>();
				atlas.SpriteWidth = 481;
				atlas.SpriteHeight = 127;
				sprite.PlayableIndicies.clear();

				auto& atlasAnim = m_Entity.GetComponent<Teddy::SpriteAnimationAtlasComponent>();
				atlasAnim.GenerateFrames(sprite, atlas);
				atlasAnim.Index = 0;

				auto& transform = m_Entity.GetComponent<Teddy::TransformComponent>();
				transform.Scale = glm::vec3(1.0f, 1.0f, 1.0f);
				transform.Translation.z = 0.9f;

				m_Transitioning = true;
			}

			auto& transform = m_Entity.GetComponent<Teddy::TransformComponent>();
			transform.Translation.x -= 15.0f * ts;
			auto& body = m_Entity.GetComponent<Teddy::Rigidbody2DComponent>();
			body.SetPosition(transform);

			if (transform.Translation.x <= -7.0f)
			{
				m_PhaseStart = false;
				m_Transitioning = false;

				auto& filter = m_Entity.GetComponent<Teddy::CollisionFilter2DComponent>();
				filter.CategoryBits = LevelCategories::ENEMY;
				filter.MaskBits = LevelCategories::PLAYER | LevelCategories::PROJECTILE;
				filter.SetFilterCategory(m_Entity.GetComponent<Teddy::Sensor2DComponent>(), filter.CategoryBits);
				filter.SetFilterMask(m_Entity.GetComponent<Teddy::Sensor2DComponent>(), filter.MaskBits);

				auto& sprite = m_Entity.GetComponent<Teddy::SpriteAnimationComponent>();
				sprite.Textures = m_DragonTongueTextures;
				sprite.PlayableIndicies.clear();
				sprite.Loop = false;

				auto& atlas = m_Entity.GetComponent<Teddy::SpriteAtlasComponent>();
				atlas.SpriteWidth = 600;
				atlas.SpriteHeight = 750;

				auto& atlasAnim = m_Entity.GetComponent<Teddy::SpriteAnimationAtlasComponent>();
				atlasAnim.GenerateFrames(sprite, atlas);
				atlasAnim.Index = 0;

				for (int i = 0; i < 20; i++)
					sprite.PlayableIndicies.push_back(i);

				transform.Translation = glm::vec3(-3.0f, 0.0f, 2.011f);
				transform.Scale = glm::vec3(6.25f, 6.25f, 1.0f);
				auto& body = m_Entity.GetComponent<Teddy::Rigidbody2DComponent>();
				body.SetPosition(transform);

				auto& sensor = m_Entity.GetComponent<Teddy::Sensor2DComponent>();
				m_Scene->DeleteSensor(sensor.Sensors["BellyHitBox"]);
				m_Scene->DeleteSensor(sensor.Sensors["NeckHitBox"]);
				m_Scene->DeleteSensor(sensor.Sensors["HeadHitBox"]);
				sensor.Sensors["NeckHitBox"] = { { -1.0f, -2.25f }, { 1.25f, 0.5f }, 0.0f, true };
				sensor.Sensors["HeadHitBox"] = { { 0.05f, -1.3f }, { 0.15f, 0.75f }, 0.0f, true };
				sensor.Sensors["BellyHitBox"] = { { -1.75f, -0.25f }, { 0.5f,  1.5f}, 0.0f, true };
				m_Scene->RefreshSensor(m_Entity, sensor.Sensors["HeadHitBox"]);
				m_Scene->RefreshSensor(m_Entity, sensor.Sensors["NeckHitBox"]);
				m_Scene->RefreshSensor(m_Entity, sensor.Sensors["BellyHitBox"]);

				if (m_TailEntity)
				{
					m_Scene->DestroyEntity(m_TailEntity);
					m_TailEntity = {};
				}

				if (m_ProjectileEntities.size() > 0)
				{
					for (auto entts : m_ProjectileEntities)
					{
						m_Scene->DestroyEntity(entts);
					}
					m_ProjectileEntities.clear();
				}
			}
		}
	}

	void Dragon::Phase2Start(Teddy::Timestep ts)
	{
		if (m_TongueToLoop)
		{
			auto& aA = m_DragonTongueEntity.GetComponent<Teddy::SpriteAnimationAtlasComponent>();
			if (aA.Index >= 11)
			{
				auto& tongueSprite = m_DragonTongueEntity.GetComponent<Teddy::SpriteAnimationComponent>();
				tongueSprite.Pause = false;
				tongueSprite.Loop = true;
				tongueSprite.FrameTime = 0.1f;
				tongueSprite.FinalFrameTime = 0.1f;
				tongueSprite.InitialFrameTime = 0.1f;

				tongueSprite.PlayableIndicies = { 11, 12, 13 };
				aA.Index = 11;

				m_Phase2Start = true;
				m_TongueToLoop = false;

				// first firemarcher
				auto ent = m_Scene->CreateEntity("Dragon Fire Marcher");
				auto& spriteFM = ent.AddComponent<Teddy::SpriteAnimationComponent>(0.05f);
				spriteFM.Textures = m_FireMarcherTextures;

				ent.AddComponent<Teddy::SpriteAtlasComponent>(0, 0, 173, 202);
				auto& aAFM = ent.GetComponent<Teddy::SpriteAnimationAtlasComponent>();
				spriteFM.PlayableIndicies.clear();
				for (int i = 70; i < 85; i++)
					spriteFM.PlayableIndicies.push_back(i);
				aAFM.Index = 70;

				auto& bodyFM = ent.AddComponent<Teddy::Rigidbody2DComponent>();
				bodyFM.Type = Teddy::Rigidbody2DComponent::BodyType::Kinematic;
				bodyFM.Velocity = glm::vec2(2.5f, 0.0f);

				auto& filterFM = ent.AddComponent<Teddy::CollisionFilter2DComponent>();
				filterFM.CategoryBits = LevelCategories::ENEMY;
				filterFM.MaskBits = LevelCategories::PLAYER;

				auto& sensorFM = ent.AddComponent<Teddy::Sensor2DComponent>();
				sensorFM.Sensors["HitBox"] = { { 0.0f, 0.0f }, { 0.5f, 0.75f }, 0.0f, true };

				m_Scene->RefreshBody(ent);

				auto& transformFM = ent.GetComponent<Teddy::TransformComponent>();
				transformFM.Translation = glm::vec3(-2.5f, -1.5f, 2.013f);
				transformFM.Scale = glm::vec3(2.0f, 2.0f, 1.0f);
				bodyFM.SetPosition(transformFM);

				m_ProjectileEntities.push_back(ent);
			}
			return;
		}

		auto& aA = m_Entity.GetComponent<Teddy::SpriteAnimationAtlasComponent>();
		if (aA.Index >= 19)
		{
			auto& sprite = m_Entity.GetComponent<Teddy::SpriteAnimationComponent>();
			sprite.Loop = true;
			sprite.PlayableIndicies.clear();
			for (int i = 19; i < 33; i++)
				sprite.PlayableIndicies.push_back(i);
			aA.Index = 19;

			class DragonTongueOverlay : public Teddy::ScriptableEntity
			{
			public:
				void OnDestroy() override
				{
					GetScene()->DestroyEntity(m_OverlayEntity);
				}

				void OnCreate() override
				{
					m_OverlayEntity = GetScene()->CreateEntity("Dragon Tongue Overlay");
					auto& sprite = m_OverlayEntity.AddComponent<Teddy::SpriteAnimationComponent>(0.05f);
					sprite.Pause = false;
					sprite.Loop = true;
					sprite.Textures = GetComponent<Teddy::SpriteAnimationComponent>().Textures;

					auto& atlas = m_OverlayEntity.AddComponent<Teddy::SpriteAtlasComponent>(0, 0, 600, 750);
					auto& atlasAnim = m_OverlayEntity.GetComponent<Teddy::SpriteAnimationAtlasComponent>();
					sprite.PlayableIndicies.clear();
					for (int i = 33; i < 47; i++)
						sprite.PlayableIndicies.push_back(i);
					atlasAnim.Index = 34;

					auto& transform = m_OverlayEntity.GetComponent<Teddy::TransformComponent>();
					transform = GetComponent<Teddy::TransformComponent>();
					transform.Translation.z += 0.005f;
				}

				void OnUpdate(Teddy::Timestep ts) override
				{
					auto& aA = GetComponent<Teddy::SpriteAnimationAtlasComponent>();
					auto& overlayA = m_OverlayEntity.GetComponent<Teddy::SpriteAnimationAtlasComponent>();
					overlayA.Index = aA.Index + 14;

					auto& spriteOverlay = m_OverlayEntity.GetComponent<Teddy::SpriteAnimationComponent>();
					spriteOverlay.PlayableIndicies = { overlayA.Index };

					auto& sprite = GetComponent<Teddy::SpriteAnimationComponent>();

					spriteOverlay.Color = sprite.Color;

					spriteOverlay.Pause = sprite.Pause;
				}

				Teddy::Entity m_OverlayEntity;
			};

			m_Entity.AddComponent<Teddy::NativeScriptComponent>().Bind<DragonTongueOverlay>();

			// tongue entity
			m_DragonTongueEntity = m_Scene->CreateEntity("Dragon Tongue");
			auto& tongueSprite = m_DragonTongueEntity.AddComponent<Teddy::SpriteAnimationComponent>(0.05f);
			tongueSprite.Pause = false;
			tongueSprite.Loop = false;
			tongueSprite.Textures = m_TongueTextures;

			auto& atlas = m_DragonTongueEntity.AddComponent<Teddy::SpriteAtlasComponent>(0, 0, 1190, 160);

			tongueSprite.PlayableIndicies = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13 };

			auto& transform = m_DragonTongueEntity.GetComponent<Teddy::TransformComponent>();
			transform.Translation = glm::vec3(0.9f, -2.15f, 2.012f);
			transform.Scale = glm::vec3(1.25f, 1.25f, 1.0f);

			// smoke and fire entity
			m_SmokeEntity = m_Scene->CreateEntity("Dragon Smoke and Fire");
			auto& spriteSmoke = m_SmokeEntity.AddComponent<Teddy::SpriteAnimationComponent>(0.05f);
			spriteSmoke.Textures = m_SmokeTextures;

			m_SmokeEntity.AddComponent<Teddy::SpriteAtlasComponent>(0, 0, 380, 533);

			auto& body = m_SmokeEntity.AddComponent<Teddy::Rigidbody2DComponent>();
			body.Type = Teddy::Rigidbody2DComponent::BodyType::Static;

			auto& filter = m_SmokeEntity.AddComponent<Teddy::CollisionFilter2DComponent>();
			filter.CategoryBits = LevelCategories::ENEMY;
			filter.MaskBits = LevelCategories::PLAYER;

			auto& sensor = m_SmokeEntity.AddComponent<Teddy::Sensor2DComponent>();
			sensor.Sensors["HitBox"] = { { 0.0f, -0.5f }, { 0.25f, 1.25f }, 0.0f, true };

			m_Scene->RefreshBody(m_SmokeEntity);

			auto& transformSmoke = m_SmokeEntity.GetComponent<Teddy::TransformComponent>();
			transformSmoke.Translation = glm::vec3(-3.0f, 1.1f, 2.013f);
			transformSmoke.Scale = glm::vec3(4.0f, 4.0f, 1.0f);
			body.SetPosition(m_SmokeEntity.GetComponent<Teddy::TransformComponent>());

			m_TongueToLoop = true;
			m_ResetFireLoop = true;
		}
	}

	void Dragon::Smoke(Teddy::Timestep ts)
	{
		static bool firstFire = true;
		if (m_ResetFireLoop)
		{
			auto& spriteSmoke = m_SmokeEntity.GetComponent<Teddy::SpriteAnimationComponent>();
			spriteSmoke.Pause = false;
			spriteSmoke.Loop = false;
			spriteSmoke.Textures = m_SmokeTextures;

			auto& atlasAnim = m_SmokeEntity.GetComponent<Teddy::SpriteAnimationAtlasComponent>();
			spriteSmoke.PlayableIndicies.clear();
			for (int i = 21; i < 34; i++)
				spriteSmoke.PlayableIndicies.push_back(i);
			atlasAnim.Index = 21;

			firstFire = true;
			m_ResetFireLoop = false;
		}
		else
		{
			static bool firstFireLoop = false;
			static bool secondFire = false;
			static float timer = 0.0f;
			timer += ts;
			if (firstFire)
			{
				if (!firstFireLoop)
				{
					auto& aA = m_SmokeEntity.GetComponent<Teddy::SpriteAnimationAtlasComponent>();
					if (aA.Index >= 27)
					{
						auto& spriteSmoke = m_SmokeEntity.GetComponent<Teddy::SpriteAnimationComponent>();
						spriteSmoke.Loop = true;
						spriteSmoke.PlayableIndicies = { 27, 28, 29, 30, 31, 32 };
						timer = 0.0f;
						firstFireLoop = true;
					}
				}
				else
				{
					if (timer >= 1.0f) // end loop
					{
						auto& spriteSmoke = m_SmokeEntity.GetComponent<Teddy::SpriteAnimationComponent>();
						spriteSmoke.Loop = false;
						spriteSmoke.PlayableIndicies = { 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39 };
						firstFire = false;
						firstFireLoop = false;
						secondFire = true;
						timer = 0.0f;
					}
				}
			}
			else if (secondFire) // another fire
			{
				auto& aA = m_SmokeEntity.GetComponent<Teddy::SpriteAnimationAtlasComponent>();
				if (aA.Index >= 39)
				{
					auto& spriteSmoke = m_SmokeEntity.GetComponent<Teddy::SpriteAnimationComponent>();
					spriteSmoke.Loop = false;
					spriteSmoke.PlayableIndicies.clear();
					for (int i = 21; i < 40; i++)
						spriteSmoke.PlayableIndicies.push_back(i);
					aA.Index = 21;

					secondFire = false;
				}
			}
			else // smoke
			{
				static bool smokeLoop = false;
				auto& aA = m_SmokeEntity.GetComponent<Teddy::SpriteAnimationAtlasComponent>();
				if (aA.Index >= 39)
				{
					auto& spriteSmoke = m_SmokeEntity.GetComponent<Teddy::SpriteAnimationComponent>();
					spriteSmoke.Loop = false;
					spriteSmoke.PlayableIndicies.clear();
					for (int i = 0; i < 14; i++)
						spriteSmoke.PlayableIndicies.push_back(i);
					aA.Index = 0;
					smokeLoop = false;
				}
				else if (aA.Index <= 20) // smoke playing
				{
					if (!smokeLoop) // smoke loop
					{
						if (aA.Index >= 5)
						{
							auto& spriteSmoke = m_SmokeEntity.GetComponent<Teddy::SpriteAnimationComponent>();
							spriteSmoke.Loop = true;
							spriteSmoke.PlayableIndicies = { 5, 6, 7, 8, 9, 10, 11, 12, 13 };
							timer = 0.0f;
							smokeLoop = true;
						}
					}
					else
					{
						if (aA.Index >= 20)
						{
							firstFire = false;
							firstFireLoop = false;
							secondFire = false;
							m_ResetFireLoop = true;
							smokeLoop = false;
							timer = 0.0f;
						}
						else if (timer >= 1.0f) // end smoke loop
						{
							auto& spriteSmoke = m_SmokeEntity.GetComponent<Teddy::SpriteAnimationComponent>();
							spriteSmoke.Loop = false;
							spriteSmoke.PlayableIndicies.clear();
							for (int i = 0; i < 21; i++)
								spriteSmoke.PlayableIndicies.push_back(i);
							timer = 0.0f;
						}
					}

				}
			}
		}
	}

	void Dragon::FireMarchers(Teddy::Timestep ts)
	{
		if (m_Phase != 2)
		{
			for (auto& ent : m_AttackableEntities) // TODO: it's own function
			{
				auto& transform = ent.Entity.GetComponent<Teddy::TransformComponent>();
				if (ent.ToAttack)
				{
					if (transform.Translation.x > ent.XToAttack)
					{
						auto& body = ent.Entity.GetComponent<Teddy::Rigidbody2DComponent>();
						body.SetVelocity(0.0f, 0.0f);

						auto& sprite = ent.Entity.GetComponent<Teddy::SpriteAnimationComponent>();
						sprite.Loop = false;

						auto& atlasAnim = ent.Entity.GetComponent<Teddy::SpriteAnimationAtlasComponent>();

						sprite.PlayableIndicies.clear();
						for (int i = 36; i < 50; i++)
							sprite.PlayableIndicies.push_back(i);
						atlasAnim.Index = 36;

						auto& transform = ent.Entity.GetComponent<Teddy::TransformComponent>();
						transform.Scale.x = m_PlayerPosition.x < transform.Translation.x ? -transform.Scale.x : transform.Scale.x;

						ent.ToAttack = false;
					}
				}
				else
				{
					auto& atlasAnim = ent.Entity.GetComponent<Teddy::SpriteAnimationAtlasComponent>();
					if (!ent.Attacked && atlasAnim.Index >= 49)
					{
						auto& body = ent.Entity.GetComponent<Teddy::Rigidbody2DComponent>();
						auto& transform = ent.Entity.GetComponent<Teddy::TransformComponent>();
						static constexpr float multiplier = 2.5f;
						body.SetVelocity((m_PlayerPosition.x - transform.Translation.x) * multiplier, (m_PlayerPosition.y - transform.Translation.y) * multiplier);
						body.GravityScale = 1.0f;
						body.SetGravityScale(1.0f);

						auto& sprite = ent.Entity.GetComponent<Teddy::SpriteAnimationComponent>();
						sprite.Loop = true;

						sprite.PlayableIndicies.clear();
						for (int i = 50; i < 58; i++)
							sprite.PlayableIndicies.push_back(i);
						atlasAnim.Index = 50;

						ent.Attacked = true;
					}
				}
			}
			return;
		}

		static float timer = 0.0f;
		timer += ts;

		if (timer >= 0.5f)
		{
			int choice = Randomizer::Get().RandomInt(0, 2);
			if (choice == 0)
				SpawnFireMarcherA();
			else if (choice == 1)
				SpawnFireMarcherB();
			else
				SpawnFireMarcherC();
			timer = 0.0f;
		}

		for (auto& ent : m_AttackableEntities)
		{
			auto& transform = ent.Entity.GetComponent<Teddy::TransformComponent>();
			if (ent.ToAttack)
			{
				if (transform.Translation.x > ent.XToAttack)
				{
					auto& body = ent.Entity.GetComponent<Teddy::Rigidbody2DComponent>();
					body.SetVelocity(0.0f, 0.0f);

					auto& sprite = ent.Entity.GetComponent<Teddy::SpriteAnimationComponent>();
					sprite.Loop = false;

					auto& atlasAnim = ent.Entity.GetComponent<Teddy::SpriteAnimationAtlasComponent>();

					sprite.PlayableIndicies.clear();
					for (int i = 36; i < 50; i++)
						sprite.PlayableIndicies.push_back(i);
					atlasAnim.Index = 36;

					auto& transform = ent.Entity.GetComponent<Teddy::TransformComponent>();
					transform.Scale.x = m_PlayerPosition.x < transform.Translation.x ? -transform.Scale.x : transform.Scale.x;

					ent.ToAttack = false;
				}
			}
			else
			{
				auto& atlasAnim = ent.Entity.GetComponent<Teddy::SpriteAnimationAtlasComponent>();
				if (!ent.Attacked && atlasAnim.Index >= 49)
				{
					auto& body = ent.Entity.GetComponent<Teddy::Rigidbody2DComponent>();
					auto& transform = ent.Entity.GetComponent<Teddy::TransformComponent>();
					static constexpr float multiplier = 2.5f;
					body.SetVelocity((m_PlayerPosition.x - transform.Translation.x) * multiplier, (m_PlayerPosition.y - transform.Translation.y) * multiplier);
					body.GravityScale = 1.0f;
					body.SetGravityScale(1.0f);

					auto& sprite = ent.Entity.GetComponent<Teddy::SpriteAnimationComponent>();
					sprite.Loop = true;
					
					sprite.PlayableIndicies.clear();
					for (int i = 50; i < 58; i++)
						sprite.PlayableIndicies.push_back(i);
					atlasAnim.Index = 50;

					ent.Attacked = true;
				}
			}
		}
	}

	void Dragon::SpawnFireMarcherA()
	{
		auto ent = m_Scene->CreateEntity("Dragon Fire Marcher A");
		auto& spriteFM = ent.AddComponent<Teddy::SpriteAnimationComponent>(0.05f);
		spriteFM.Textures = m_FireMarcherTextures;

		ent.AddComponent<Teddy::SpriteAtlasComponent>(0, 0, 173, 202);
		auto& aAFM = ent.GetComponent<Teddy::SpriteAnimationAtlasComponent>();
		spriteFM.PlayableIndicies.clear();
		for (int i = 0; i < 20; i++)
			spriteFM.PlayableIndicies.push_back(i);
		aAFM.Index = 0;

		auto& bodyFM = ent.AddComponent<Teddy::Rigidbody2DComponent>();
		bodyFM.Type = Teddy::Rigidbody2DComponent::BodyType::Kinematic;
		bodyFM.Velocity = glm::vec2(2.5f, 0.0f);

		auto& filterFM = ent.AddComponent<Teddy::CollisionFilter2DComponent>();
		filterFM.CategoryBits = LevelCategories::ENEMY;
		filterFM.MaskBits = LevelCategories::PLAYER;

		auto& sensorFM = ent.AddComponent<Teddy::Sensor2DComponent>();
		sensorFM.Sensors["HitBox"] = { { 0.0f, -0.5f }, { 0.35f, 0.4f }, 0.0f, true };

		m_Scene->RefreshBody(ent);

		auto& transformFM = ent.GetComponent<Teddy::TransformComponent>();
		transformFM.Translation = glm::vec3(-2.75f, -1.5f, 2.013f);
		transformFM.Scale = glm::vec3(2.0f, 2.0f, 1.0f);
		bodyFM.SetPosition(transformFM);

		m_ProjectileEntities.push_back(ent);
	}

	void Dragon::SpawnFireMarcherB()
	{
		auto ent = m_Scene->CreateEntity("Dragon Fire Marcher B");
		auto& spriteFM = ent.AddComponent<Teddy::SpriteAnimationComponent>(0.05f);
		spriteFM.Textures = m_FireMarcherTextures;

		ent.AddComponent<Teddy::SpriteAtlasComponent>(0, 0, 173, 202);
		auto& aAFM = ent.GetComponent<Teddy::SpriteAnimationAtlasComponent>();
		spriteFM.PlayableIndicies.clear();
		for (int i = 20; i < 36; i++)
			spriteFM.PlayableIndicies.push_back(i);
		aAFM.Index = 20;

		auto& bodyFM = ent.AddComponent<Teddy::Rigidbody2DComponent>();
		bodyFM.Type = Teddy::Rigidbody2DComponent::BodyType::Dynamic;
		bodyFM.Velocity = glm::vec2(2.5f, 0.0f);
		bodyFM.GravityScale = 0.0f;

		auto& filterFM = ent.AddComponent<Teddy::CollisionFilter2DComponent>();
		filterFM.CategoryBits = LevelCategories::ENEMY;
		filterFM.MaskBits = LevelCategories::PLAYER;

		auto& sensorFM = ent.AddComponent<Teddy::Sensor2DComponent>();
		sensorFM.Sensors["HitBox"] = { { 0.0f, -0.5f }, { 0.35f, 0.4f }, 0.0f, true };

		m_Scene->RefreshBody(ent);

		auto& transformFM = ent.GetComponent<Teddy::TransformComponent>();
		transformFM.Translation = glm::vec3(-2.75f, -1.5f, 2.014f);
		transformFM.Scale = glm::vec3(2.0f, 2.0f, 1.0f);
		bodyFM.SetPosition(transformFM);

		m_ProjectileEntities.push_back(ent);
		m_AttackableEntities.push_back({ ent, Randomizer::Get().RandomFloat(-2.0f, 3.0f)});
	}

	void Dragon::SpawnFireMarcherC()
	{
		auto ent = m_Scene->CreateEntity("Dragon Fire Marcher C");
		auto& spriteFM = ent.AddComponent<Teddy::SpriteAnimationComponent>(0.05f);
		spriteFM.Textures = m_FireMarcherTextures;

		ent.AddComponent<Teddy::SpriteAtlasComponent>(0, 0, 173, 202);
		auto& aAFM = ent.GetComponent<Teddy::SpriteAnimationAtlasComponent>();
		spriteFM.PlayableIndicies.clear();
		for (int i = 58; i < 70; i++)
			spriteFM.PlayableIndicies.push_back(i);
		aAFM.Index = 58;

		auto& bodyFM = ent.AddComponent<Teddy::Rigidbody2DComponent>();
		bodyFM.Type = Teddy::Rigidbody2DComponent::BodyType::Kinematic;
		bodyFM.Velocity = glm::vec2(2.5f, 0.0f);

		auto& filterFM = ent.AddComponent<Teddy::CollisionFilter2DComponent>();
		filterFM.CategoryBits = LevelCategories::ENEMY;
		filterFM.MaskBits = LevelCategories::PLAYER;

		auto& sensorFM = ent.AddComponent<Teddy::Sensor2DComponent>();
		sensorFM.Sensors["HitBox"] = { { 0.0f, -0.5f }, { 0.35f, 0.4f }, 0.0f, true };

		m_Scene->RefreshBody(ent);

		auto& transformFM = ent.GetComponent<Teddy::TransformComponent>();
		transformFM.Translation = glm::vec3(-2.75f, -1.5f, 2.013f);
		transformFM.Scale = glm::vec3(2.0f, 2.0f, 1.0f);
		bodyFM.SetPosition(transformFM);

		m_ProjectileEntities.push_back(ent);
	}

	void Dragon::Phase2Ending()
	{
		auto& sprite = m_Entity.GetComponent<Teddy::SpriteAnimationComponent>();
		sprite.Textures = m_Phase2DeathTextures;

		auto& atlas = m_Entity.GetComponent<Teddy::SpriteAtlasComponent>();
		atlas.SpriteWidth = 510;
		atlas.SpriteHeight = 750;

		auto& atlasAnim = m_Entity.GetComponent<Teddy::SpriteAnimationAtlasComponent>();
		atlasAnim.GenerateFrames(sprite, atlas);
		atlasAnim.Index = 0;

		sprite.PlayableIndicies = { 0,1,2,3,4,5,6,7 };
		sprite.Loop = true;

		auto& transform = m_Entity.GetComponent<Teddy::TransformComponent>();
		transform.Translation = glm::vec3(-3.8f, -1.1f, 2.011f);

		auto& body = m_Entity.GetComponent<Teddy::Rigidbody2DComponent>();
		body.SetPosition(transform);

		m_Scene->DestroyScript(m_Entity);
		auto& nsc = m_Entity.GetComponent<Teddy::NativeScriptComponent>();

		class DragonDeathOverlay : public Teddy::ScriptableEntity
		{
		public:
			void OnDestroy() override
			{
				GetScene()->DestroyEntity(m_OverlayEntity);
			}

			void OnCreate() override
			{
				m_OverlayEntity = GetScene()->CreateEntity("Dragon Death Overlay");
				auto& sprite = m_OverlayEntity.AddComponent<Teddy::SpriteAnimationComponent>(0.05f);
				sprite.Pause = false;
				sprite.Loop = true;
				sprite.Textures = GetComponent<Teddy::SpriteAnimationComponent>().Textures;

				auto& atlas = m_OverlayEntity.AddComponent<Teddy::SpriteAtlasComponent>(0, 0, 510, 750);
				auto& atlasAnim = m_OverlayEntity.GetComponent<Teddy::SpriteAnimationAtlasComponent>();
				sprite.PlayableIndicies = { 8, 9, 10, 11, 12, 13, 14, 15 };
				atlasAnim.Index = 0;

				auto& transform = m_OverlayEntity.GetComponent<Teddy::TransformComponent>();
				transform = GetComponent<Teddy::TransformComponent>();
				transform.Translation.z += 0.005f;
				transform.Translation.x -= 0.085f;
			}

			void OnUpdate(Teddy::Timestep ts) override
			{
				auto& aA = GetComponent<Teddy::SpriteAnimationAtlasComponent>();
				auto& overlayA = m_OverlayEntity.GetComponent<Teddy::SpriteAnimationAtlasComponent>();
				overlayA.Index = aA.Index + 8;

				auto& spriteOverlay = m_OverlayEntity.GetComponent<Teddy::SpriteAnimationComponent>();
				spriteOverlay.PlayableIndicies = { overlayA.Index };

				auto& sprite = GetComponent<Teddy::SpriteAnimationComponent>();

				spriteOverlay.Color = sprite.Color;

				spriteOverlay.Pause = sprite.Pause;
			}

			Teddy::Entity m_OverlayEntity;
		};

		nsc.Bind<DragonDeathOverlay>();

		if (m_SmokeEntity)
		{
			m_Scene->DestroyEntity(m_SmokeEntity);
			m_SmokeEntity = {};
		}

		m_AttackableEntities.clear();
	}

	void Dragon::Phase2To3(Teddy::Timestep ts)
	{
		if (m_ProjectileEntities.size() == 0)
		{
			auto& sprite = m_DragonTongueEntity.GetComponent<Teddy::SpriteAnimationComponent>();
			auto& atlasAnim = m_DragonTongueEntity.GetComponent<Teddy::SpriteAnimationAtlasComponent>();
			if (sprite.PlayableIndicies.size() < 14)
			{
				sprite.Loop = false;
				sprite.Reverse = true;

				sprite.PlayableIndicies = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13 };

				sprite.FrameTime = 0.05f;
				sprite.FinalFrameTime = 0.05f;
				sprite.InitialFrameTime = 0.05f;
			}
			else if (atlasAnim.Index == 0)
			{
				m_Scene->DestroyEntity(m_DragonTongueEntity);
				m_DragonTongueEntity = {};
				m_PhaseStart = false;
				m_Phase3Start = true;
			}
		}
		else
		{
			FireMarchers(ts);
		}
	}

	void Dragon::Phase3Start(Teddy::Timestep ts)
	{
		auto& sprite = m_Entity.GetComponent<Teddy::SpriteAnimationComponent>();
		sprite.Textures = m_Phase3IntroTextures;
		sprite.Loop = false;
		sprite.Pause = false;

		auto& atlas = m_Entity.GetComponent<Teddy::SpriteAtlasComponent>();
		atlas.SpriteWidth = 1000;
		atlas.SpriteHeight = 1000;

		auto& atlasAnim = m_Entity.GetComponent<Teddy::SpriteAnimationAtlasComponent>();
		sprite.PlayableIndicies.clear();
		atlasAnim.GenerateFrames(sprite, atlas);
		sprite.PlayableIndicies.pop_back();
		sprite.PlayableIndicies.pop_back();
		atlasAnim.Index = 0;

		m_Scene->DestroyScript(m_Entity);
		m_Entity.RemoveComponent<Teddy::NativeScriptComponent>();

		auto& transform = m_Entity.GetComponent<Teddy::TransformComponent>();
		transform.Translation = glm::vec3(-1.5f, -0.0f, 2.011f);
		transform.Scale = glm::vec3(8.5f, 8.5f, 1.0f);

		auto& body = m_Entity.GetComponent<Teddy::Rigidbody2DComponent>();
		body.SetPosition(transform);

		m_Phase3StartLoop = true;
	}

	void Dragon::Phase3StartLoop(Teddy::Timestep ts)
	{
		static bool started = false;
		static int count = 0;

		if (!started)
		{
			auto& atlasAnim = m_Entity.GetComponent<Teddy::SpriteAnimationAtlasComponent>();
			if (atlasAnim.Index >= 19)
			{
				auto& sprite = m_Entity.GetComponent<Teddy::SpriteAnimationComponent>();
				sprite.PlayableIndicies = { 19, 20, 21, 22, 23, 24, 25, 26, 27 };
				sprite.Loop = true;

				started = true;
				count = 0;
			}
		}
		else
		{
			auto& atlasAnim = m_Entity.GetComponent<Teddy::SpriteAnimationAtlasComponent>();
			static int lastIndex = atlasAnim.Index;
			
			auto& sprite = m_Entity.GetComponent<Teddy::SpriteAnimationComponent>();

			if (atlasAnim.Index == 37)
			{
				// body
				{
					sprite.Textures = m_Phase3IdleBodyTextures;
					sprite.PlayableIndicies = { 0,1,2,3,4,5,6,7,8 };
					sprite.Loop = true;
					sprite.InitialFrameTime = 0.15f;
					sprite.FrameTime = 0.1f;
					sprite.FinalFrameTime = 0.15f;

					auto& atlas = m_Entity.GetComponent<Teddy::SpriteAtlasComponent>();
					atlas.SpriteWidth = 500;
					atlas.SpriteHeight = 500;

					atlasAnim.GenerateFrames(sprite, atlas);
					atlasAnim.Index = 0;

					auto& transform = m_Entity.GetComponent<Teddy::TransformComponent>();
					transform.Scale = glm::vec3(4.5f, 4.5f, 1.0f);
					transform.Translation = glm::vec3(-3.5f, -3.0f, 2.011f);

					auto& body = m_Entity.GetComponent<Teddy::Rigidbody2DComponent>();
					body.SetPosition(transform);

					auto& sensor = m_Entity.GetComponent<Teddy::Sensor2DComponent>();
					m_Scene->DeleteSensor(sensor.Sensors["BellyHitBox"]);
					m_Scene->DeleteSensor(sensor.Sensors["NeckHitBox"]);
					m_Scene->DeleteSensor(sensor.Sensors["HeadHitBox"]);
					sensor.Sensors["BellyHitBox"] = { { 0.0f, 0.0f }, { 1.0f, 1.0f }, 0.0f, false };
					sensor.Sensors["HeadsHitBox"] = { { -0.5f, 3.0f }, { 1.0f, 2.0f }, 0.0f, true };
					m_Scene->RefreshSensor(m_Entity, sensor.Sensors["BellyHitBox"]);
					m_Scene->RefreshSensor(m_Entity, sensor.Sensors["HeadsHitBox"]);
				}
				
				// right head
				{
					m_Phase3Heads.RightHead = m_Scene->CreateEntity("Dragon Phase 3 Right Head");
					auto& spriteRH = m_Phase3Heads.RightHead.AddComponent<Teddy::SpriteAnimationComponent>(0.04f);
					spriteRH.Textures = m_Phase3IdleHeadTextures;
					auto& atlasRH = m_Phase3Heads.RightHead.AddComponent<Teddy::SpriteAtlasComponent>(0, 0, 900, 900);

					spriteRH.PlayableIndicies.clear();
					for (int i = 0; i < (4*9); i++)
						spriteRH.PlayableIndicies.push_back(i);

					auto& transform = m_Phase3Heads.RightHead.GetComponent<Teddy::TransformComponent>();
					transform.Scale = glm::vec3(8.0f, 8.0f, 1.0f);
					transform.Translation = glm::vec3(-3.85f, -0.3f, 2.014f);
				}

				// mid head
				{
					m_Phase3Heads.MidHead = m_Scene->CreateEntity("Dragon Phase 3 Middle Head");
					auto& spriteMH = m_Phase3Heads.MidHead.AddComponent<Teddy::SpriteAnimationComponent>(0.04f);
					spriteMH.Textures = m_Phase3IdleHeadTextures;
					auto& atlasMH = m_Phase3Heads.MidHead.AddComponent<Teddy::SpriteAtlasComponent>(0, 0, 900, 900);

					spriteMH.PlayableIndicies.clear();
					for (int i = (4 * 9); i < (4 * 9 * 2); i++)
						spriteMH.PlayableIndicies.push_back(i);

					auto& aAMH = m_Phase3Heads.MidHead.GetComponent<Teddy::SpriteAnimationAtlasComponent>();
					aAMH.Index = (4 * 9) + 11;

					auto& transformMH = m_Phase3Heads.MidHead.GetComponent<Teddy::TransformComponent>();
					transformMH.Scale = glm::vec3(8.0f, 8.0f, 1.0f);
					transformMH.Translation = glm::vec3(-3.8f, -0.35f, 2.013f);
				}

				// left head
				{
					m_Phase3Heads.LeftHead = m_Scene->CreateEntity("Dragon Phase 3 Left Head");
					auto& spriteMH = m_Phase3Heads.LeftHead.AddComponent<Teddy::SpriteAnimationComponent>(0.04f);
					spriteMH.Textures = m_Phase3IdleHeadTextures;
					auto& atlasMH = m_Phase3Heads.LeftHead.AddComponent<Teddy::SpriteAtlasComponent>(0, 0, 900, 900);

					spriteMH.PlayableIndicies.clear();
					for (int i = (4 * 9); i < (4 * 9 * 2); i++)
						spriteMH.PlayableIndicies.push_back(i);

					auto& aAMH = m_Phase3Heads.LeftHead.GetComponent<Teddy::SpriteAnimationAtlasComponent>();
					aAMH.Index = (4 * 9) + 23;

					auto& transformMH = m_Phase3Heads.LeftHead.GetComponent<Teddy::TransformComponent>();
					transformMH.Scale = glm::vec3(8.0f, 8.0f, 1.0f);
					transformMH.Translation = glm::vec3(-3.75f, -0.25f, 2.012f);
				}

				m_Phase3Start = false;
			}
			else if (sprite.PlayableIndicies.size() < 15 && count >= 4)
			{
				sprite.Loop = false;
				sprite.PlayableIndicies = { 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37 };
				count = 0;
			}
			else if (lastIndex != atlasAnim.Index && atlasAnim.Index == 27)
			{
				count++;
			}

			lastIndex = atlasAnim.Index;
		}
	}

	void Dragon::StartPhase3Attack()
	{
		Teddy::SpriteAnimationComponent* sprite = nullptr;
		Teddy::SpriteAnimationAtlasComponent* animAtlas = nullptr;
		Teddy::SpriteAtlasComponent* atlas = nullptr;

		switch (m_Phase3Heads.SelectedHead)
		{
		case 0:
			sprite = &m_Phase3Heads.RightHead.GetComponent<Teddy::SpriteAnimationComponent>();
			animAtlas = &m_Phase3Heads.RightHead.GetComponent<Teddy::SpriteAnimationAtlasComponent>();
			atlas = &m_Phase3Heads.RightHead.GetComponent<Teddy::SpriteAtlasComponent>();
			if (animAtlas->Index == 7)
			{
				animAtlas->Index = 0;
				sprite->PlayableIndicies.clear();
				for (int i = 0; i< 14; i++)
					sprite->PlayableIndicies.push_back(i);
			}
			else if (animAtlas->Index == 19)
			{
				animAtlas->Index = 14;
				sprite->PlayableIndicies.clear();
				for (int i = 14; i < 28; i++)
					sprite->PlayableIndicies.push_back(i);
			}
			break;
		case 1:
			sprite = &m_Phase3Heads.MidHead.GetComponent<Teddy::SpriteAnimationComponent>();
			animAtlas = &m_Phase3Heads.MidHead.GetComponent<Teddy::SpriteAnimationAtlasComponent>();
			atlas = &m_Phase3Heads.MidHead.GetComponent<Teddy::SpriteAtlasComponent>();
			if (animAtlas->Index == (4 * 9) + 7)
			{
				animAtlas->Index = 28;
				sprite->PlayableIndicies.clear();
				for (int i = 28; i < 42; i++)
					sprite->PlayableIndicies.push_back(i);
			}
			else if (animAtlas->Index == (4 * 9) + 19)
			{
				animAtlas->Index = 42;
				sprite->PlayableIndicies.clear();
				for (int i = 42; i < 56; i++)
					sprite->PlayableIndicies.push_back(i);
			}
			break;
		default:
			sprite = &m_Phase3Heads.LeftHead.GetComponent<Teddy::SpriteAnimationComponent>();
			animAtlas = &m_Phase3Heads.LeftHead.GetComponent<Teddy::SpriteAnimationAtlasComponent>();
			atlas = &m_Phase3Heads.LeftHead.GetComponent<Teddy::SpriteAtlasComponent>();
			if (animAtlas->Index == (4 * 9) + 7)
			{
				animAtlas->Index = 28;
				sprite->PlayableIndicies.clear();
				for (int i = 28; i < 42; i++)
					sprite->PlayableIndicies.push_back(i);
			}
			else if (animAtlas->Index == (4 * 9) + 19)
			{
				animAtlas->Index = 42;
				sprite->PlayableIndicies.clear();
				for (int i = 42; i < 56; i++)
					sprite->PlayableIndicies.push_back(i);
			}
			break;
		}

		sprite->Textures = m_Phase3AttackHeadTextures;
		sprite->Loop = false;

		animAtlas->GenerateFrames(*sprite, *atlas);

		sprite = nullptr;
		animAtlas = nullptr;

		m_State = DragonState::Firebubble;
	}

	void Dragon::CreateFirebubbleSpit(float x, float y)
	{
		m_FirebubbleSpitEntity = m_Scene->CreateEntity("Dragon Firebubble Spit");

		auto& spriteFBS = m_FirebubbleSpitEntity.AddComponent<Teddy::SpriteAnimationComponent>(0.05f);
		spriteFBS.Textures = m_Phase3FirebubbleTextures;
		spriteFBS.Loop = false;
		spriteFBS.PlayableIndicies.clear();
		for (int i = 40; i < 57; i++)
			spriteFBS.PlayableIndicies.push_back(i);

		auto& atlasFBS = m_FirebubbleSpitEntity.AddComponent<Teddy::SpriteAtlasComponent>(0, 0, 550, 500);

		auto& animAtlasFBS = m_FirebubbleSpitEntity.GetComponent<Teddy::SpriteAnimationAtlasComponent>();
		animAtlasFBS.Index = 40;

		auto& transformFBS = m_FirebubbleSpitEntity.GetComponent<Teddy::TransformComponent>();
		transformFBS.Scale = glm::vec3(5.0f, 5.0f, 1.0f);
		transformFBS.Translation = glm::vec3(x, y, 2.02f);
	}

	void Dragon::CreateFirebubble(float x, float y)
	{
		auto ent = m_Scene->CreateEntity("Dragon Firebubble");

		auto& spriteFB = ent.AddComponent<Teddy::SpriteAnimationComponent>(0.05f);
		spriteFB.Textures = m_Phase3FirebubbleTextures;
		spriteFB.Loop = false;
		spriteFB.PlayableIndicies.clear();
		for (int i = 8; i < 16; i++)
			spriteFB.PlayableIndicies.push_back(i);

		auto& atlasFB = ent.AddComponent<Teddy::SpriteAtlasComponent>(0, 0, 550, 500);

		auto& animAtlasFB = ent.GetComponent<Teddy::SpriteAnimationAtlasComponent>();
		animAtlasFB.Index = 8;

		auto& transformFB = ent.GetComponent<Teddy::TransformComponent>();
		transformFB.Scale = glm::vec3(2.5f, 2.5f, 1.0f);
		transformFB.Translation = glm::vec3(x, y, 2.021f);

		auto& bodyFB = ent.AddComponent<Teddy::Rigidbody2DComponent>();
		bodyFB.Type = Teddy::Rigidbody2DComponent::BodyType::Kinematic;
		glm::vec2 tailPos = glm::vec2(transformFB.Translation.x, transformFB.Translation.y); // TODO: do the same for the phase 2
		glm::vec2 direction = glm::normalize(m_PlayerPosition - tailPos);
		float speed = 2.0f;
		bodyFB.Velocity = direction * speed;

		auto& filter = ent.AddComponent<Teddy::CollisionFilter2DComponent>();
		filter.CategoryBits = LevelCategories::ENEMY;
		filter.MaskBits = LevelCategories::PLAYER | LevelCategories::PROJECTILE;

		auto& sensor = ent.AddComponent<Teddy::Sensor2DComponent>();
		sensor.Sensors["HitBox"] = { { 0.0f, 0.0f }, { 0.2f, 0.2f }, 0.0f, false };

		m_Scene->RefreshBody(ent);

		bodyFB.SetPosition(transformFB);

		m_Firebubbles.push_back(ent);
	}

	bool Dragon::IsFirebubble(b2ShapeId shape)
	{
		for (auto& ent : m_Firebubbles)
		{
			auto sensor = ent.Entity.GetComponent<Teddy::Sensor2DComponent>();
			if (sensor.Sensors["HitBox"].RuntimeFixture)
			{
				b2ShapeId sensorShape = *static_cast<b2ShapeId*>(sensor.Sensors["HitBox"].RuntimeFixture);
				if (B2_ID_EQUALS(shape, sensorShape))
					return true;
			}
		}
		return false;
	}

	void Dragon::CreateMiniFirebubbles(glm::vec3& pos)
	{
		constexpr std::array<std::pair<int, int>, 4> directions = { std::make_pair(0, 1), std::make_pair(1, 0), std::make_pair(-1, 0), std::make_pair(0, -1) };
		for (auto& direction : directions)
		{
			auto ent = m_Scene->CreateEntity("Dragon Mini Firebubble");

			auto& spriteFB = ent.AddComponent<Teddy::SpriteAnimationComponent>(0.05f);
			spriteFB.Textures = m_Phase3MiniFirebubbleTextures;
			spriteFB.Loop = true;
			spriteFB.PlayableIndicies.clear();
			for (int i = 0; i < 20; i++)
				spriteFB.PlayableIndicies.push_back(i);

			auto& atlasFB = ent.AddComponent<Teddy::SpriteAtlasComponent>(0, 0, 78, 78);

			auto& body = ent.AddComponent<Teddy::Rigidbody2DComponent>();
			body.Type = Teddy::Rigidbody2DComponent::BodyType::Kinematic;
			float speed = 3.0f;
			body.Velocity = glm::vec2(direction.first * speed, direction.second * speed);

			auto& transformFB = ent.GetComponent<Teddy::TransformComponent>();
			transformFB.Scale = glm::vec3(0.5f, 0.5f, 1.0f);
			transformFB.Translation = glm::vec3(pos.x + 0.05f * direction.first, pos.y + 0.05f * direction.second, 2.022f);

			body.SetPosition(transformFB);

			auto& filter = ent.AddComponent<Teddy::CollisionFilter2DComponent>();
			filter.CategoryBits = LevelCategories::ENEMY;
			filter.MaskBits = LevelCategories::PLAYER;

			auto& sensor = ent.AddComponent<Teddy::Sensor2DComponent>();
			sensor.Sensors["HitBox"] = { { 0.0f, 0.0f }, { 0.1f, 0.1f }, 0.0f, false };

			m_Scene->RefreshBody(ent);

			m_ProjectileEntities.push_back(ent);
		}
	}

	void Dragon::HitFirebubble(b2ShapeId shape, float damage)
	{
		for (auto& bubble : m_Firebubbles)
		{
			auto& sensor = bubble.Entity.GetComponent<Teddy::Sensor2DComponent>();
			if (sensor.Sensors["HitBox"].RuntimeFixture)
			{
				b2ShapeId sensorShape = *static_cast<b2ShapeId*>(sensor.Sensors["HitBox"].RuntimeFixture);
				if (B2_ID_EQUALS(shape, sensorShape))
				{
					bubble.Health -= damage;
					if (bubble.Health <= 0.0f)
					{
						auto& spriteFB = bubble.Entity.GetComponent<Teddy::SpriteAnimationComponent>();
						spriteFB.Loop = false;
						spriteFB.PlayableIndicies.clear();
						for (int i = 0; i < 8; i++)
							spriteFB.PlayableIndicies.push_back(i);
						auto& body = bubble.Entity.GetComponent<Teddy::Rigidbody2DComponent>();
						body.SetVelocity(0.0f, 0.0f);
						CreateMiniFirebubbles(bubble.Entity.GetComponent<Teddy::TransformComponent>().Translation);
						m_Scene->DeleteSensor(sensor.Sensors["HitBox"]);
					}
					break;
				}
			}
		}
	}

	void Dragon::Firebubble()
	{
		Teddy::SpriteAnimationComponent* sprite = nullptr;
		Teddy::SpriteAnimationAtlasComponent* animAtlas = nullptr;
		Teddy::SpriteAtlasComponent* atlas = nullptr;

		switch (m_Phase3Heads.SelectedHead)
		{
		case 0:
			sprite = &m_Phase3Heads.RightHead.GetComponent<Teddy::SpriteAnimationComponent>();
			animAtlas = &m_Phase3Heads.RightHead.GetComponent<Teddy::SpriteAnimationAtlasComponent>();
			atlas = &m_Phase3Heads.RightHead.GetComponent<Teddy::SpriteAtlasComponent>();
			if (animAtlas->Index == 13)
			{
				m_Phase3Heads.Shot = false;
				animAtlas->Index = 20;
				sprite->PlayableIndicies.clear();
				for (int i = 0; i < (4 * 9); i++)
					sprite->PlayableIndicies.push_back(i);
				sprite->Loop = true;
				sprite->Textures = m_Phase3IdleHeadTextures;
				animAtlas->GenerateFrames(*sprite, *atlas);

				m_State = DragonState::Idle;
			}
			else if (animAtlas->Index == 27)
			{
				m_Phase3Heads.Shot = false;
				animAtlas->Index = 32;
				sprite->PlayableIndicies.clear();
				for (int i = 0; i < (4 * 9); i++)
					sprite->PlayableIndicies.push_back(i);
				sprite->Loop = true;
				sprite->Textures = m_Phase3IdleHeadTextures;
				animAtlas->GenerateFrames(*sprite, *atlas);

				m_State = DragonState::Idle;
			}
			else if (animAtlas->Index == 6 && !m_Phase3Heads.Shot)
			{
				CreateFirebubbleSpit(0.0f, 1.25f);
				CreateFirebubble(0.0f, 1.25f);
				m_Phase3Heads.Shot = true;
			}
			else if (animAtlas->Index == 20 && !m_Phase3Heads.Shot)
			{
				CreateFirebubbleSpit(-0.5f, -1.0f);
				CreateFirebubble(-0.5f, -1.0f);
				m_Phase3Heads.Shot = true;
			}
			break;
		case 1:
			sprite = &m_Phase3Heads.MidHead.GetComponent<Teddy::SpriteAnimationComponent>();
			animAtlas = &m_Phase3Heads.MidHead.GetComponent<Teddy::SpriteAnimationAtlasComponent>();
			atlas = &m_Phase3Heads.MidHead.GetComponent<Teddy::SpriteAtlasComponent>();
			if (animAtlas->Index == 41)
			{
				m_Phase3Heads.Shot = false;
				animAtlas->Index = (4 * 9) + 20;
				sprite->PlayableIndicies.clear();
				for (int i = (4 * 9); i < (4 * 9 * 2); i++)
					sprite->PlayableIndicies.push_back(i);
				sprite->Loop = true;
				sprite->Textures = m_Phase3IdleHeadTextures;
				animAtlas->GenerateFrames(*sprite, *atlas);

				m_State = DragonState::Idle;
			}
			else if (animAtlas->Index == 55)
			{
				m_Phase3Heads.Shot = false;
				animAtlas->Index = (4 * 9) + 32;
				sprite->PlayableIndicies.clear();
				for (int i = (4 * 9); i < (4 * 9 * 2); i++)
					sprite->PlayableIndicies.push_back(i);
				sprite->Loop = true;
				sprite->Textures = m_Phase3IdleHeadTextures;
				animAtlas->GenerateFrames(*sprite, *atlas);

				m_State = DragonState::Idle;
			}
			else if (animAtlas->Index == 34 && !m_Phase3Heads.Shot)
			{
				CreateFirebubbleSpit(0.75f, 1.25f);
				CreateFirebubble(0.75f, 1.5f);
				m_Phase3Heads.Shot = true;
			}
			else if (animAtlas->Index == 48 && !m_Phase3Heads.Shot)
			{
				CreateFirebubbleSpit(0.25f, -1.0f);
				CreateFirebubble(0.25f, -1.0f);
				m_Phase3Heads.Shot = true;
			}
			break;
		default:
			sprite = &m_Phase3Heads.LeftHead.GetComponent<Teddy::SpriteAnimationComponent>();
			animAtlas = &m_Phase3Heads.LeftHead.GetComponent<Teddy::SpriteAnimationAtlasComponent>();
			atlas = &m_Phase3Heads.LeftHead.GetComponent<Teddy::SpriteAtlasComponent>();
			if (animAtlas->Index == 41)
			{
				m_Phase3Heads.Shot = false;
				animAtlas->Index = (4 * 9) + 20;
				sprite->PlayableIndicies.clear();
				for (int i = (4 * 9); i < (4 * 9 * 2); i++)
					sprite->PlayableIndicies.push_back(i);
				sprite->Loop = true;
				sprite->Textures = m_Phase3IdleHeadTextures;
				animAtlas->GenerateFrames(*sprite, *atlas);

				m_State = DragonState::Idle;
			}
			else if (animAtlas->Index == 55)
			{
				m_Phase3Heads.Shot = false;
				animAtlas->Index = (4 * 9) + 32;
				sprite->PlayableIndicies.clear();
				for (int i = (4 * 9); i < (4 * 9 * 2); i++)
					sprite->PlayableIndicies.push_back(i);
				sprite->Loop = true;
				sprite->Textures = m_Phase3IdleHeadTextures;
				animAtlas->GenerateFrames(*sprite, *atlas);

				m_State = DragonState::Idle;
			}
			else if (animAtlas->Index == 34 && !m_Phase3Heads.Shot)
			{
				CreateFirebubbleSpit(0.75f, 1.5f);
				CreateFirebubble(0.75f, 1.5f);
				m_Phase3Heads.Shot = true;
			}
			else if (animAtlas->Index == 48 && !m_Phase3Heads.Shot)
			{
				CreateFirebubbleSpit(0.25f, -1.0f);
				CreateFirebubble(0.25f, -1.0f);
				m_Phase3Heads.Shot = true;
			}
			break;
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

		for (auto& ent: m_ProjectileEntities)
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

		if (m_TailEntity)
		{
			auto& tailSensor = m_TailEntity.GetComponent<Teddy::Sensor2DComponent>().Sensors;
			for (auto& [_, sensor] : tailSensor)
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
		}

		if (m_FiretorchProjectileEntity)
		{
			auto& tongueSensor = m_FiretorchProjectileEntity.GetComponent<Teddy::Sensor2DComponent>().Sensors;
			for (auto& [_, sensor] : tongueSensor)
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
		}

		if (m_SmokeEntity)
		{
			auto& smokeSensor = m_SmokeEntity.GetComponent<Teddy::Sensor2DComponent>().Sensors;
			for (auto& [_, sensor] : smokeSensor)
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
		}

		for (auto& ent : m_ProjectileEntities)
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

	void Dragon::DestroyParry(b2ShapeId shape)
	{
		std::vector<Teddy::Entity> newShots;
		
		for (auto& ent : m_ProjectileEntities)
		{
			bool shouldRemove = false;
			auto& sensors = ent.GetComponent<Teddy::Sensor2DComponent>().Sensors;
		
			for (auto& [name, shotSensor] : sensors)
			{
				if (shotSensor.RuntimeFixture)
				{
					b2ShapeId sensorShape = *static_cast<b2ShapeId*>(shotSensor.RuntimeFixture);
					if (B2_ID_EQUALS(shape, sensorShape))
					{
						shouldRemove = true;
						break;
					}
				}
			}
		
			if (shouldRemove)
				m_Scene->DestroyEntity(ent);
			else
				newShots.push_back(ent);
		}
		
		m_ProjectileEntities = newShots;
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
			int maxCount = 2;
			float maxTimer = 0.2f;
			if (m_Health <= 1535)
			{
				maxCount = 3;
				maxTimer = 0.15f;
			}
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

			if (canShoot && count <= maxCount && timer >= maxTimer)
			{
				auto ent = m_Scene->CreateEntity("Psychic Eye Projectile");
				auto& sprite = ent.AddComponent<Teddy::SpriteAnimationComponent>(0.05f, 0.05f, 0.05f);
				sprite.Textures = m_PsychicEyeTexture;
				auto& atlas = ent.AddComponent<Teddy::SpriteAtlasComponent>(0, 0, 151, 145);

				auto& aA = ent.GetComponent<Teddy::SpriteAnimationAtlasComponent>();
				sprite.PlayableIndicies = { 19, 20, 21, 22, 23, 24, 25, 26 };
				aA.Index = 19;

				auto& transform = ent.GetComponent<Teddy::TransformComponent>();
				transform.Translation = m_Entity.GetComponent<Teddy::TransformComponent>().Translation + glm::vec3(-0.5f, 2.0f, 0.11f);
				glm::vec2 direction = playerPos - glm::vec2(transform.Translation.x, transform.Translation.y);
				transform.Rotation.z = std::atan2(direction.y, direction.x);

				auto& rb = ent.AddComponent<Teddy::Rigidbody2DComponent>();
				rb.Type = Teddy::Rigidbody2DComponent::BodyType::Kinematic;

				auto& filter = ent.AddComponent<Teddy::CollisionFilter2DComponent>();
				filter.CategoryBits = LevelCategories::ENEMY;
				filter.MaskBits = LevelCategories::PLAYER;

				class PsychicEye : public Teddy::ScriptableEntity
				{
				public:
					void OnUpdate(Teddy::Timestep ts) override
					{
						m_Timer += ts;
						auto& transform = GetComponent<Teddy::TransformComponent>();
						if (m_Direction)
						{
							transform.Translation += glm::vec3(std::cos(transform.Rotation.z + glm::radians(90.0f)) * ts * m_Velocity,
								std::sin(transform.Rotation.z + glm::radians(90.0f)) * ts * m_Velocity, 0.0f);
							if (GetScene()->IsRuntime())
								GetComponent<Teddy::Rigidbody2DComponent>().SetPosition(transform);
						}
						else
						{
							transform.Translation -= glm::vec3(std::cos(transform.Rotation.z + glm::radians(90.0f)) * ts * m_Velocity,
								std::sin(transform.Rotation.z + glm::radians(90.0f)) * ts * m_Velocity, 0.0f);
							if(GetScene()->IsRuntime())
								GetComponent<Teddy::Rigidbody2DComponent>().SetPosition(transform);
						}

						if (m_Timer >= 0.15f)
						{
							m_Timer = 0.0f;
							m_Direction = !m_Direction;
						}
					}

					bool m_Direction = true;
					float m_Timer = 0.0f;
					float m_Velocity = 0.5f;
				};

				ent.AddComponent<Teddy::NativeScriptComponent>().Bind<PsychicEye>();

				auto& sensor = ent.AddComponent<Teddy::Sensor2DComponent>();
				
				if (count >= maxCount)
				{
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

				m_Scene->RefreshBody(ent);
				rb.SetVelocity(std::cos(transform.Rotation.z) * 5.0f, std::sin(transform.Rotation.z) * 5.0f);

				m_ProjectileEntities.push_back(ent);

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
		static int maxShots = 2; 
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
				maxShots = 2;
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
				if (Randomizer::Get().RandomBool(0.3f))
					maxShots = 3;
				PsychicEye(ts);
			}
			else
			{
				auto& transform = m_Entity.GetComponent<Teddy::TransformComponent>();
				transform.Translation += glm::vec3(0.0f, ts * 5.0f, 0.0f); // TODO: fix this
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
		for (auto& ent : m_ProjectileEntities)
		{
			auto& transform = ent.GetComponent<Teddy::TransformComponent>();
			if (transform.Translation.x >= 10.0f || transform.Translation.x <= -10.0f ||
				transform.Translation.y <= -3.5f || transform.Translation.y >= 3.5f)
			{
				m_Scene->DestroyEntity(ent);
			}
			else
			{
				newPeashots.push_back(ent);
			}
		}
		m_ProjectileEntities = newPeashots;
		std::vector<FirebubbleProjectile> newFirebubbles;
		for (auto& ent : m_Firebubbles)
		{
			auto& transform = ent.Entity.GetComponent<Teddy::TransformComponent>();
			if (transform.Translation.x >= 10.0f || transform.Translation.x <= -10.0f ||
				transform.Translation.y <= -3.5f || transform.Translation.y >= 3.5f)
			{
				m_Scene->DestroyEntity(ent.Entity);
			}
			else
			{
				newFirebubbles.push_back(ent);
			}
		}
		m_Firebubbles = newFirebubbles;
	}

	void Dragon::Pause()
	{
		m_Entity.GetComponent<Teddy::SpriteAnimationComponent>().Pause = true;
		if (m_PsychicEyeEntity)
			m_PsychicEyeEntity.GetComponent<Teddy::SpriteAnimationComponent>().Pause = true;
		for (auto& ent : m_ProjectileEntities)
		{
			ent.GetComponent<Teddy::SpriteAnimationComponent>().Pause = true;
		}
	}

	void Dragon::Unpause()
	{
		m_Entity.GetComponent<Teddy::SpriteAnimationComponent>().Pause = false;
		if (m_PsychicEyeEntity)
			m_PsychicEyeEntity.GetComponent<Teddy::SpriteAnimationComponent>().Pause = false;
		for (auto& ent : m_ProjectileEntities)
		{
			ent.GetComponent<Teddy::SpriteAnimationComponent>().Pause = false;
		}
	}

	void Dragon::StartMeteor()
	{
		auto& sprite = m_Entity.GetComponent<Teddy::SpriteAnimationComponent>();
		sprite.Pause = false;
		sprite.Loop = false;
		sprite.PingPong = false;
		sprite.PlayableIndicies.clear();
		sprite.Textures = m_MeteorTextures;

		auto& atlas = m_Entity.GetComponent<Teddy::SpriteAtlasComponent>();
		atlas.SpriteWidth = 800;
		atlas.SpriteHeight = 800;

		auto& atlasAnim = m_Entity.GetComponent<Teddy::SpriteAnimationAtlasComponent>();
		atlasAnim.GenerateFrames(sprite, atlas);
		atlasAnim.Index = 0;

		for (int i = 0; i < 16; i++)
			sprite.PlayableIndicies.push_back(i);

		auto& transform = m_Entity.GetComponent<Teddy::TransformComponent>();
		transform.Translation = glm::vec3(4.25f, -0.5f, 2.011f);
		m_Entity.GetComponent<Teddy::Rigidbody2DComponent>().SetPosition(transform);

		auto& sensor = m_Entity.GetComponent<Teddy::Sensor2DComponent>();
		m_Scene->DeleteSensor(sensor.Sensors["BellyHitBox"]);
		m_Scene->DeleteSensor(sensor.Sensors["NeckHitBox"]);
		m_Scene->DeleteSensor(sensor.Sensors["HeadHitBox"]);
		sensor.Sensors["BellyHitBox"] = { { 0.0f, 0.75f }, { 1.0f, 1.0f }, 0.0f, false};
		sensor.Sensors["NeckHitBox"] = { { -0.25f, -0.5f }, { 0.5f, 0.75f }, 0.0f, true };
		sensor.Sensors["HeadHitBox"] = { { 0.5f, 2.0f }, { 1.25f, 0.5f }, 0.0f, true	};
		m_Scene->RefreshSensor(m_Entity, sensor.Sensors["BellyHitBox"]);
		m_Scene->RefreshSensor(m_Entity, sensor.Sensors["NeckHitBox"]);
		m_Scene->RefreshSensor(m_Entity, sensor.Sensors["HeadHitBox"]);

		m_State = DragonState::Meteor;
	}

	void Dragon::Meteor(Teddy::Timestep ts)
	{
		int maxMeteors = 2;
		if(m_LaunchThreeMeteors)
			maxMeteors = 3;
		if (m_MeteorsLaunched >= maxMeteors)
		{
			auto& sprite = m_Entity.GetComponent<Teddy::SpriteAnimationComponent>();
			if (sprite.PlayableIndicies.size() > 6)
			{
				auto& aA = m_Entity.GetComponent<Teddy::SpriteAnimationAtlasComponent>();
				sprite.Loop = false;
				sprite.Reverse = false;
				sprite.PlayableIndicies = { 26, 27, 28, 29, 30, 31 };

				aA.Index = 26;
				m_ChargingMeteor = false;
				m_DidMeteorLaunch = false;
			}
			else
			{
				auto& aA = m_Entity.GetComponent<Teddy::SpriteAnimationAtlasComponent>();
				if (aA.Index == 31)
				{
					StartIdle();
					m_MeteorsLaunched = 0;
				}
			}
			return;
		}

		if (m_DidMeteorLaunch)
		{
			auto& aA = m_Entity.GetComponent<Teddy::SpriteAnimationAtlasComponent>();
			if (aA.Index == 25)
			{
				if (m_MeteorsLaunched == 2 && m_LaunchThreeMeteors)
					LaunchMeteor();
				LaunchMeteor();
				auto& sprite = m_Entity.GetComponent<Teddy::SpriteAnimationComponent>();
				sprite.Loop = false;
				sprite.Reverse = true;
				sprite.PlayableIndicies = { 16, 17, 18, 19, 20, 23, 24, 25 };

				aA.Index = 25;
				m_ChargingMeteor = false;
				m_DidMeteorLaunch = false;
				m_MeteorsLaunched++;
			}
		}
		else
		{
			if (m_ChargingMeteor)
			{
				static float timer = 0.0f;
				timer += ts;

				if (timer >= 2.5f)
				{
					auto& aA = m_Entity.GetComponent<Teddy::SpriteAnimationAtlasComponent>();
					if (aA.Index == 15)
					{
						auto& sprite = m_Entity.GetComponent<Teddy::SpriteAnimationComponent>();
						sprite.Loop = false;
						sprite.Reverse = false;
						sprite.PlayableIndicies = {16, 17, 18, 19, 20, 21, 22, 25};

						aA.Index = 16;
						m_ChargingMeteor = false;
						m_DidMeteorLaunch = true;
						timer = 0.0f;
					}
				}
			}
			else
			{
				auto& aA = m_Entity.GetComponent<Teddy::SpriteAnimationAtlasComponent>();
				if (aA.Index == 15)
				{
					auto& sprite = m_Entity.GetComponent<Teddy::SpriteAnimationComponent>();
					sprite.Loop = true;
					sprite.Reverse = false;
					sprite.PlayableIndicies.clear();
					for (int i = 7; i < 16; i++)
						sprite.PlayableIndicies.push_back(i);

					aA.Index = 7;
					m_ChargingMeteor = true;
				}
				else if (aA.Index == 16)
				{
					auto& sprite = m_Entity.GetComponent<Teddy::SpriteAnimationComponent>();
					sprite.Loop = true;
					sprite.Reverse = false;
					sprite.PlayableIndicies.clear();
					for (int i = 7; i < 16; i++)
						sprite.PlayableIndicies.push_back(i);

					aA.Index = 11;
					m_ChargingMeteor = true;
				}
			}
		}
		
	}

	void Dragon::LaunchMeteor()
	{
		static bool up = true;
		up = !up;

		auto ent = m_Scene->CreateEntity("Meteor Projectile");
		auto& sprite = ent.AddComponent<Teddy::SpriteAnimationComponent>(0.05f, 0.05f, 0.05f);
		sprite.Textures = m_MeteorProjectileTextures;
		sprite.Loop = true;
		auto& atlas = ent.AddComponent<Teddy::SpriteAtlasComponent>(0, 0, 240, 193);
		auto& aA = ent.GetComponent<Teddy::SpriteAnimationAtlasComponent>();
		sprite.PlayableIndicies = { 0, 1, 2, 3, 4, 5, 6, 7 };

		auto& transform = ent.GetComponent<Teddy::TransformComponent>();
		transform.Translation = m_Entity.GetComponent<Teddy::TransformComponent>().Translation + glm::vec3(-1.0f, 1.5f, up ? 0.11f : 0.1f);
		transform.Scale = glm::vec3(1.5f, 1.5f, 1.0f);
		transform.Rotation.z = glm::radians(up ? -70.0f : 70.0f);

		auto& rb = ent.AddComponent<Teddy::Rigidbody2DComponent>();
		rb.Type = Teddy::Rigidbody2DComponent::BodyType::Kinematic;
		auto& filter = ent.AddComponent<Teddy::CollisionFilter2DComponent>();
		filter.CategoryBits = LevelCategories::ENEMY;
		filter.MaskBits = LevelCategories::PLAYER;

		class Meteor : public Teddy::ScriptableEntity
		{
		public:
			void OnCreate() override
			{
				auto& transform = GetComponent<Teddy::TransformComponent>();
				float angle = transform.Rotation.z;

				m_MovingUp = angle < 0.0f;
			}

			void OnDestroy() override
			{
				for (auto& smokeEnt : m_SmokeEntities)
				{
					auto scene = GetScene();
					if (scene)
					{
						scene->DestroyEntity(smokeEnt);
					}
				}
			}

			void OnUpdate(Teddy::Timestep ts) override
			{
				auto& transform = GetComponent<Teddy::TransformComponent>();

				const float edge = 2.0f;
				const float minSpeed = 0.25f;
				const float maxSpeed = 7.5f;

				float t = std::abs(transform.Translation.y) / edge;
				t = glm::clamp(t, 0.0f, 1.0f);

				float velocityMultiplier = 1.0f - t * t;
				float moveY = glm::mix(minSpeed, maxSpeed, velocityMultiplier) * ts;

				float signedMoveY = m_MovingUp ? moveY : -moveY;
				transform.Translation.y += signedMoveY;

				if (transform.Translation.y >= edge - 0.01f)
					m_MovingUp = false;
				else if (transform.Translation.y <= -edge + 0.01f)
					m_MovingUp = true;

				transform.Translation.x -= 7.5f * ts / 2.5f;

				float maxMoveY = maxSpeed * ts;
				float normalizedSpeed = 0.0f;
				if (maxMoveY > 0.0f)
					normalizedSpeed = signedMoveY / maxMoveY;

				float angleDegrees = - normalizedSpeed * 70.0f;
				transform.Rotation.z = glm::radians(angleDegrees);

				GetComponent<Teddy::Rigidbody2DComponent>().SetPosition(transform);

				auto& sensor = GetComponent<Teddy::Sensor2DComponent>();
				sensor.Sensors["HitBox"] = { { -0.25f * cos(transform.Rotation.z), -0.25f * sin(transform.Rotation.z)}, {0.5f, 0.5f}, 0.0f, false, sensor.Sensors["HitBox"].RuntimeFixture };
				auto entity = GetEntity();
				Teddy::Scene::RefreshSensor(entity, sensor.Sensors["HitBox"]);

				m_Timer += ts;
				if (m_Timer >= 0.15f)
				{
					auto scene = GetScene();
					if (scene)
					{
						auto smoke = scene->CreateEntity("Meteor Smoke");
						auto& smokeSprite = smoke.AddComponent<Teddy::SpriteAnimationComponent>(0.05f);
						smokeSprite.Textures = Teddy::AssetManager::Get().LoadMultiple<Teddy::Texture2D>({
							"assets/Textures/Dragon/Projectiles/Meteor_240x193_1024x1024_0.png",
							"assets/Textures/Dragon/Projectiles/Meteor_240x193_1024x1024_1.png",
							"assets/Textures/Dragon/Projectiles/Meteor_240x193_1024x1024_2.png"
							});
						smokeSprite.Loop = false;
						auto& smokeAtlas = smoke.AddComponent<Teddy::SpriteAtlasComponent>(0, 0, 240, 193);
						auto& smokeAA = smoke.GetComponent<Teddy::SpriteAnimationAtlasComponent>();
						smokeSprite.PlayableIndicies.clear();
						for (int i = 8; i <= 43; i++)
							smokeSprite.PlayableIndicies.push_back(i);
						smokeAA.Index = 8;

						auto& smokeTransform = smoke.GetComponent<Teddy::TransformComponent>();
						smokeTransform = transform;
						smokeTransform.Translation.x += 0.5f;
						smokeTransform.Translation.z += - 0.05f + (0.001f * (m_Count + 1));

						m_Count++;

						m_SmokeEntities.push_back(smoke);
					}
					m_Timer = 0.0f;
				}

				std::vector<Teddy::Entity> newSmokeEntities;
				std::vector<Teddy::Entity> expiredSmokeEntities;
				for (auto& smokeEnt : m_SmokeEntities)
				{
					auto scene = GetScene();
					if (scene)
					{
						auto& smokeAA = smokeEnt.GetComponent<Teddy::SpriteAnimationAtlasComponent>();
						if (smokeAA.Index >= 43)
						{
							expiredSmokeEntities.push_back(smokeEnt);
						}
						else
						{
							newSmokeEntities.push_back(smokeEnt);
						}
					}
				}
				for (auto& expiredEnt : expiredSmokeEntities)
				{
					auto scene = GetScene();
					if (scene)
					{
						scene->DestroyEntity(expiredEnt);
					}
				}
				m_SmokeEntities = newSmokeEntities;
			}

			bool m_MovingUp = true;
			float m_Timer = 0.0f;
			int m_Count = 0;
			std::vector<Teddy::Entity> m_SmokeEntities;
		};

		auto& sensor = ent.AddComponent<Teddy::Sensor2DComponent>();
		sensor.Sensors["HitBox"] = { { -0.25f * cos(transform.Rotation.z), -0.25f * sin(transform.Rotation.z)}, {0.5f, 0.5f}, 0.0f, false};

		ent.AddComponent<Teddy::NativeScriptComponent>().Bind<Meteor>();

		m_Scene->RefreshBody(ent);

		m_ProjectileEntities.push_back(ent);
	}

	void Dragon::StartTail()
	{
		m_TailActive = true;
	}

	void Dragon::Tail(Teddy::Timestep ts)
	{
		if (m_TailActive)
		{
			static float timer = 0.0f;
			timer += ts;

			if (!m_TailEntity)
			{
				if ((timer >= 1.5f && m_State == DragonState::Peashot) || (timer >= 5.0f && m_State == DragonState::Meteor))
				{
					m_TailEntity = m_Scene->CreateEntity("Dragon Tail");
					auto& sprite = m_TailEntity.AddComponent<Teddy::SpriteAnimationComponent>(0.075f);
					sprite.Textures = m_TailTextures;
					sprite.Loop = true;

					auto& atlas = m_TailEntity.AddComponent<Teddy::SpriteAtlasComponent>(0, 0, 303, 856);

					auto& transform = m_TailEntity.GetComponent<Teddy::TransformComponent>();
					transform.Translation = glm::vec3(m_PlayerPosition.x, -5.5f, 2.1f);
					transform.Scale = glm::vec3(6.5f, 6.5f, 1.0f);

					auto& body = m_TailEntity.AddComponent<Teddy::Rigidbody2DComponent>();
					body.Type = Teddy::Rigidbody2DComponent::BodyType::Kinematic;
					auto& filter = m_TailEntity.AddComponent<Teddy::CollisionFilter2DComponent>();
					filter.CategoryBits = LevelCategories::ENEMY;
					filter.MaskBits = LevelCategories::PLAYER;

					auto& sensor = m_TailEntity.AddComponent<Teddy::Sensor2DComponent>();
					sensor.Sensors["HitBox"] = { { -0.5f, 0.0f }, { 0.25f, 3.0f }, 0.0f, true };

					m_Scene->RefreshBody(m_TailEntity);

					m_TailPick = false;
					m_TailUp = false;
					timer = 0.0f;
				}
			}
			else
			{
				if (!m_TailPick)
				{
					auto& transform = m_TailEntity.GetComponent<Teddy::TransformComponent>();
					transform.Translation.y += 2.0f * ts;

					if (transform.Translation.y > -5.0f)
					{
						transform.Translation.y = -5.0f;
						m_TailPick = true;
					}

					auto& body = m_TailEntity.GetComponent<Teddy::Rigidbody2DComponent>();
					body.SetPosition(transform);

					timer = 0.0f;
				}
				else if (!m_TailUp)
				{
					if (timer > 1.0f)
					{
						auto& transform = m_TailEntity.GetComponent<Teddy::TransformComponent>();
						transform.Translation.y += 10.0f * ts;

						if (transform.Translation.y > -0.75f)
						{
							transform.Translation.y = -0.75f;
							m_TailUp = true;
							timer = 0.0f;
						}

						auto& body = m_TailEntity.GetComponent<Teddy::Rigidbody2DComponent>();
						body.SetPosition(transform);
					}
				}
				else if (timer > 0.5f)
				{
					auto& transform = m_TailEntity.GetComponent<Teddy::TransformComponent>();
					transform.Translation.y -= 10.0f * ts;

					auto& body = m_TailEntity.GetComponent<Teddy::Rigidbody2DComponent>();
					body.SetPosition(transform);

					if (transform.Translation.y < -5.5f)
					{
						m_Scene->DestroyEntity(m_TailEntity);
						m_TailEntity = {};
						m_TailPick = false;
						m_TailUp = false;
						m_TailActive = false;
						timer = 0.0f;
					}
				}
			}
		}
	}

	void Dragon::SetColor(glm::vec4 color)
	{ 
		m_Entity.GetComponent<Teddy::SpriteAnimationComponent>().Color = color; 
		if (m_Phase3Heads.LeftHead)
		{
			m_Phase3Heads.LeftHead.GetComponent<Teddy::SpriteAnimationComponent>().Color = color;
		}
		if (m_Phase3Heads.RightHead)
		{
			m_Phase3Heads.RightHead.GetComponent<Teddy::SpriteAnimationComponent>().Color = color;
		}
		if (m_Phase3Heads.MidHead)
		{
			m_Phase3Heads.MidHead.GetComponent<Teddy::SpriteAnimationComponent>().Color = color;
		}
		if (m_TailEntity)
		{
			m_TailEntity.GetComponent<Teddy::SpriteAnimationComponent>().Color = color;
		}
	}

}