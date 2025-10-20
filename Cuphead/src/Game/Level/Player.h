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

		void Shutdown();

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

		bool IsDead() { return m_Health <= 0; }

		bool CanHit() { return !m_HitTolerance && !m_Hitting; }

		void Pause() { m_Entity.GetComponent<Teddy::SpriteAnimationComponent>().Pause = true; }
		void Unpause() { m_Entity.GetComponent<Teddy::SpriteAnimationComponent>().Pause = false; }

	private:
		bool OnKeyPressed(Teddy::KeyPressedEvent& e);
		bool OnKeyReleased(Teddy::KeyReleasedEvent& e);

		void InitPlayer();
		void InitPlayerHUD();
		void InitProjectiles();

		void Intro0();
		void Intro1();
		void Intro2();

		void Move(Teddy::Timestep ts);
		void BlockMove();

		void StartIdle();
		void Idle();

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
		void StartDeath();
		void Dying(Teddy::Timestep ts);
		void UpdateHUD();
		void HUDAnimation(Teddy::Timestep ts);

		void Shoot(Teddy::Timestep ts);
		void LaunchProjectile();

		void LoadCupheadTextures();
		void BreakCookie();
		void DeleteCookie(Teddy::Timestep ts);
	private:
		Teddy::Ref<Teddy::Scene> m_Scene = nullptr;

		bool m_IntroLoaded = false;

		Teddy::Entity m_Entity;
		Teddy::Entity m_Cookie;
		Teddy::Entity m_HealthHUD;

		std::vector<Teddy::Ref<Teddy::Texture2D>> m_MovementTextures;
		std::vector<Teddy::Ref<Teddy::Texture2D>> m_IntroTextures;
		std::vector<Teddy::Ref<Teddy::Texture2D>> m_JumpTextures;
		std::vector<Teddy::Ref<Teddy::Texture2D>> m_HealthTextures;
		std::vector<Teddy::Ref<Teddy::Texture2D>> m_HealthHudTextures;

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
			AnimationDone,
			Dead
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

		int m_Health = 1;
		bool m_Hitting = false;
		bool m_HitTolerance = false;
		bool m_HitAnimation = false;

		bool m_FirstDeath = true;

		bool m_CookieDeleted = false;
		bool m_CookieCreated = false;

		bool m_DownPressed = false;
		bool m_RightPressed = false;
		bool m_LeftPressed = false;

		bool m_Shooting = false;

		const float m_ShootingRestTime = 0.25f;
		const float m_ShootingActiveTime = 0.04f;

		enum class ProjectileType
		{
			Roundabout,
			Lobber
		};

		std::vector<Teddy::Entity> m_ActiveProjectiles;
		ProjectileType m_Projectile = ProjectileType::Lobber;
		std::vector<Teddy::Ref<Teddy::Texture2D>> m_LobberTextures;
		float m_ShootTimer = 0.0f;
		bool m_Shot = false;
	};
}