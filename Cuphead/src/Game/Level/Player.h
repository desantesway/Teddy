#pragma once

#include "Teddy/Scene/Scene.h"
#include "Teddy/Core/Timestep.h"
#include "Teddy/Scene/Entity.h"
#include "Teddy/Events/KeyEvent.h"
#include "Teddy/Physics/ContactEvent.h"

namespace Cuphead
{
	class Player
	{
	public:
		Player() = default;
		~Player() = default;

		void OnUpdate(Teddy::Timestep ts);
		void OnEvent(Teddy::Event& event);

		Teddy::Entity GetEntity() { return m_Entity; }

		void InitCuphead(Teddy::Ref<Teddy::Scene> scene);
		void InitMugman(Teddy::Ref<Teddy::Scene> scene);

		void StartIntro();

		void SetGrounded(bool grounded) { m_Grounded = grounded; }
	private:
		bool OnKeyPressed(Teddy::KeyPressedEvent& e);

		void InitPlayer();
		
		void StartIdle();

		void StartRunning();
		void Running();

		void StartCrouching();
		void Crouching();

		void StartJumping();
		void Jumping(Teddy::Timestep ts);

		void LoadCupheadTextures();
		void BreakCookie();
		void DeleteCookie(Teddy::Timestep ts);
	private:
		Teddy::Ref<Teddy::Scene> m_Scene = nullptr;

		Teddy::Entity m_Entity;
		Teddy::Entity m_Cookie;

		std::vector<Teddy::Ref<Teddy::Texture2D>> m_MovementTextures;
		std::vector<Teddy::Ref<Teddy::Texture2D>> m_IntroTextures;
		std::vector<Teddy::Ref<Teddy::Texture2D>> m_JumpTextures;

		enum class PlayerState
		{
			Idle = 0,
			Running,
			Jumping,
			DoneJumping,
			Dashing,
			Crouching,
			Intro0,
			Intro1,
			Intro2,
			AnimationDone
		};

		PlayerState m_State = PlayerState::Idle;
		bool m_DirectionRight = true;
		bool m_Jump = false;
		bool m_Grounded = false;

		bool m_ZHeld = false;
	};
}