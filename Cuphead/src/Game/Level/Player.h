#pragma once

#include "Teddy/Scene/Scene.h"
#include "Teddy/Core/Timestep.h"
#include "Teddy/Scene/Entity.h"

namespace Cuphead
{
	class Player
	{
	public:
		Player() = default;
		~Player() = default;

		void OnUpdate(Teddy::Timestep ts);

		Teddy::Entity GetEntity() { return m_Entity; }

		void InitCuphead(Teddy::Ref<Teddy::Scene> scene);
		void InitMugman(Teddy::Ref<Teddy::Scene> scene);

		void StartIntro();

	private:
		void InitPlayer();
		void StartIdle();

		void LoadCupheadTextures();
		void BreakCookie();
		void DeleteCookie(Teddy::Timestep ts);
	private:
		Teddy::Ref<Teddy::Scene> m_Scene = nullptr;

		Teddy::Entity m_Entity;
		Teddy::Entity m_Cookie;

		std::vector<Teddy::Ref<Teddy::Texture2D>> m_MovementTextures;
		std::vector<Teddy::Ref<Teddy::Texture2D>> m_IntroTextures;

		enum class PlayerState
		{
			Idle = 0,
			Running,
			Jumping,
			Falling,
			Dashing,
			Crouching,
			Intro0,
			Intro1,
			Intro2
		};

		PlayerState m_State = PlayerState::Idle;
	};
}