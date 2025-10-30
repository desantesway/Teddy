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

		void Pause();
		void Unpause();

		bool IsSensor(b2ShapeId shape);
		bool IsParry(b2ShapeId shape);
		void DestroyParry(b2ShapeId shape);

		void ClearProjectiles();

		void Hit(int damage);
		bool IsShooting() const { return m_Shooting; }

		void SetPlayerPosition(const glm::vec2& pos) { m_PlayerPosition = pos; }
	private:
		bool OnKeyPressed(Teddy::KeyPressedEvent& e);
		bool OnKeyReleased(Teddy::KeyReleasedEvent& e);

		void StartIdle();
		void Idle(Teddy::Timestep ts);

		void Intro();

		void StartPeashot();
		void Peashot(Teddy::Timestep ts);
		void PsychicEye(Teddy::Timestep ts);

		void StartMeteor();
		void Meteor(Teddy::Timestep ts);
		void LaunchMeteor();

		void StartTail();
		void Tail(Teddy::Timestep ts);

		void Hitting(Teddy::Timestep ts);

		void LoadTextures();
	private:
		
		Teddy::Entity m_Entity;

		std::vector<Teddy::Ref<Teddy::Texture2D>> m_IntroTextures;
		std::vector<Teddy::Ref<Teddy::Texture2D>> m_IdleTextures;
		std::vector<Teddy::Ref<Teddy::Texture2D>> m_PeashotTextures;
		std::vector<Teddy::Ref<Teddy::Texture2D>> m_PsychicEyeTexture;
		std::vector<Teddy::Ref<Teddy::Texture2D>> m_MeteorTextures;
		std::vector<Teddy::Ref<Teddy::Texture2D>> m_MeteorProjectileTextures;
		std::vector<Teddy::Ref<Teddy::Texture2D>> m_TailTextures;
		std::vector<Teddy::Ref<Teddy::Texture2D>> m_DashTextures;
		std::vector<Teddy::Ref<Teddy::Texture2D>> m_DragonTongueTextures;
		std::vector<Teddy::Ref<Teddy::Texture2D>> m_TongueTextures;

		enum class DragonState
		{
			Idle = 0,
			Intro,
			Peashot,
			Meteor
		};

		DragonState m_State = DragonState::Intro;

		int m_Phase = 1;

		glm::vec2 m_PlayerPosition = { 0.0f, 0.0f };

		bool m_IntroDone = false;
		bool m_Shooting = false;
		bool m_MeteorStart = true;

		int m_Health = 1072; // 629, 510, 561 /// 1700, 1535, 1071, 561
		// grim firebubble has 5 health
		bool m_Hit = false;

		float m_PeashotTimer = 0.0f;

		Teddy::Entity m_PsychicEyeEntity;
		std::vector<Teddy::Entity> m_ProjectileEntities;

		bool m_ChargingMeteor = false;
		bool m_DidMeteorLaunch = false;
		int m_MeteorsLaunched = 0;
		bool m_LaunchThreeMeteors = false;

		Teddy::Entity m_TailEntity;
		Teddy::Entity m_DragonTongueEntity;
		bool m_TailActive = false;
		bool m_TailPick = false;
		bool m_TailUp = false;

		bool m_PhaseStart = false;
		bool m_Transitioning = false;
		bool m_Phase2Start = false;
		bool m_TongueToLoop = false;

		Teddy::Ref<Teddy::Scene> m_Scene = nullptr;
	};

}