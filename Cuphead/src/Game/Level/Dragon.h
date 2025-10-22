#pragma once

#include "Teddy/Scene/Entity.h"
#include "Teddy/Events/KeyEvent.h"

namespace Cuphead
{
	class Dragon
	{
	public:
		Dragon() = default;
		~Dragon() = default;

		void Init(Teddy::Ref<Teddy::Scene> scene);

		void OnUpdate(Teddy::Timestep ts);
		void OnEvent(Teddy::Event& event);

		Teddy::Entity GetEntity() { return m_Entity; }

		bool IsIntroDone() { return m_State != DragonState::Intro; }

		void StartIntro();
		void LoadIntro();

		bool IsDead() { return m_Health <= 0; }

		void Pause() { m_Entity.GetComponent<Teddy::SpriteAnimationComponent>().Pause = true; }
		void Unpause() { m_Entity.GetComponent<Teddy::SpriteAnimationComponent>().Pause = false; }

	private:
		bool OnKeyPressed(Teddy::KeyPressedEvent& e);
		bool OnKeyReleased(Teddy::KeyReleasedEvent& e);

		void StartIdle();
		void Idle();

		void Intro();

		void LoadTextures();
	private:
		Teddy::Ref<Teddy::Scene> m_Scene = nullptr;
		Teddy::Entity m_Entity;

		std::vector<Teddy::Ref<Teddy::Texture2D>> m_IntroTextures;
		std::vector<Teddy::Ref<Teddy::Texture2D>> m_IdleTextures;

		enum class DragonState
		{
			Idle = 0,
			Intro
		};

		DragonState m_State = DragonState::Intro;

		bool m_IntroDone = false;

		int m_Health = 100;
	};

}