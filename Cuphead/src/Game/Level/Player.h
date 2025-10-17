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

		glm::vec3 GetPosition() { return m_Entity.GetComponent<Teddy::TransformComponent>().Translation; }

		bool IsIntroDone() { return m_State != PlayerState::Intro0 && m_State != PlayerState::Intro1 && m_State != PlayerState::Intro2; }

		void InitCuphead(Teddy::Ref<Teddy::Scene> scene);
		void InitMugman(Teddy::Ref<Teddy::Scene> scene);

		void StartIntro();
		void LoadIntro();

		void SetGrounded(bool grounded);

		void FloorHit();
		void NormalHit();
	private:
		bool OnKeyPressed(Teddy::KeyPressedEvent& e);

		void InitPlayer();
		
		void Intro0();
		void Intro1();
		void Intro2();

		void Move(Teddy::Timestep ts);
		void BlockMove();

		void StartIdle();

		void StartFall();
		void Falling();

		void StartRun(bool isRight);
		void Running();

		void StartCrouch();
		void Crouching();

		void StartJump();
		void Jumping(Teddy::Timestep ts);

		void StartDash();
		void Dashing(Teddy::Timestep ts);

		void StartDrop();
		void Dropping(Teddy::Timestep ts);

		void StartParry();
		void Parrying();

		void StartHit();
		void FlashPlayer(Teddy::Timestep ts);
		void Hitting(Teddy::Timestep ts);
		void Hit(float velocity);

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
		std::vector<Teddy::Ref<Teddy::Texture2D>> m_HealthTextures;

		enum class PlayerState
		{
			Idle = 0,
			Falling,
			Running,
			Jumping,
			DoneJumping,
			Dashing,
			Crouching,
			Dropping,
			Parrying,
			Hit,
			Intro0,
			Intro1,
			Intro2,
			AnimationDone
		};

		PlayerState m_State = PlayerState::Idle;

		bool m_DirectionRight = true;
		bool m_Grounded = true;

		bool m_StartDash = false;
		bool m_DashReset = true;
		bool m_ShiftHeld = false;

		bool m_StartCrouch = false;

		bool m_Dropping = false;

		bool m_StartJump = false;
		bool m_ZHeld = false;
		bool m_Moving = false;

		bool m_ParryReset = true;

		int m_Health = 4;
		bool m_Hitting = false;
		bool m_HitTolerance = false;
	};
}