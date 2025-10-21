#include "Dragon.h"

#include <Teddy.h>

namespace Cuphead
{
	void Dragon::OnUpdate(Teddy::Timestep ts)
	{
		switch (m_State)
		{
		case DragonState::Idle:
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
		if (aA.Index >= 35)
		{
			if (aA.Index < 39)
			{
				auto& sprite = m_Entity.GetComponent<Teddy::SpriteAnimationComponent>();
				sprite.FowardAfterRender = true;
				auto& transform = m_Entity.GetComponent<Teddy::TransformComponent>();
				switch (aA.Index)
				{
				case 36:
					transform.Translation = glm::vec3(4.55f, -0.85f, 2.011f);
					break;
				case 37:
					transform.Translation = glm::vec3(5.05f, -0.85f, 2.011f);
					break;
				case 38:
					transform.Translation = glm::vec3(4.0f, -0.75f, 2.011f);
					break;
				default:
					break;
				}
				
			}
			else
			{
				auto& transform = m_Entity.GetComponent<Teddy::TransformComponent>();
				transform.Translation = glm::vec3(5.05f, -0.85f, 2.011f);
				//StartIdle();
			}
		}

	}

	void Dragon::Init(Teddy::Ref<Teddy::Scene> scene)
	{
		m_Scene = scene;

		LoadTextures();

		m_Entity = m_Scene->CreateEntity("Dragon");
		auto& spriteAnim = m_Entity.AddComponent<Teddy::SpriteAnimationComponent>(0.05f, 0.05f, 0.05f);
		spriteAnim.Pause = true;
		spriteAnim.Loop = false;
		spriteAnim.Textures = m_IntroTextures;
		auto& atlas = m_Entity.AddComponent<Teddy::SpriteAtlasComponent>(0, 0, 947, 805);
	}

	void Dragon::LoadTextures()
	{
		auto& assets = Teddy::AssetManager::Get();

		m_IntroTextures = assets.LoadMultiple<Teddy::Texture2D>({
			"assets/Textures/Dragon/Entity/Intro/Dragon_Intro_947x805_2048x2048_0.png",
			"assets/Textures/Dragon/Entity/Intro/Dragon_Intro_947x805_2048x2048_1.png",
			"assets/Textures/Dragon/Entity/Intro/Dragon_Intro_947x805_2048x2048_2.png",
			"assets/Textures/Dragon/Entity/Intro/Dragon_Intro_947x805_2048x2048_3.png",
			"assets/Textures/Dragon/Entity/Intro/Dragon_Intro_947x805_2048x2048_4.png",
			"assets/Textures/Dragon/Entity/Intro/Dragon_Intro_947x805_2048x2048_5.png",
			"assets/Textures/Dragon/Entity/Intro/Dragon_Intro_947x805_2048x2048_6.png",
			"assets/Textures/Dragon/Entity/Intro/Dragon_Intro_947x805_2048x2048_7.png",
			"assets/Textures/Dragon/Entity/Intro/Dragon_Intro_947x805_2048x2048_8.png",
			"assets/Textures/Dragon/Entity/Intro/Dragon_Intro_947x805_2048x2048_9.png"
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
		transform.Translation = glm::vec3(4.25f,-0.75f, 2.011f);
		transform.Scale = glm::vec3(6.25f, 6.25f, 1.0f);
	}
}