#include "Dragon.h"

#include <Teddy.h>

namespace Cuphead
{
	void Dragon::OnUpdate(Teddy::Timestep ts)
	{
		switch (m_State)
		{
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

		auto& transform = m_Entity.GetComponent<Teddy::TransformComponent>();
		transform.Translation = glm::vec3(4.25f, -0.65f, 2.011f);

		m_State = DragonState::Idle;
	}

	void Dragon::Idle()
	{
				
	}
}