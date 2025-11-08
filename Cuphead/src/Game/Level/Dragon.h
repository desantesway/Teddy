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
		bool IsIdle() { return m_State == DragonState::Idle; }

		void StartIntro();
		void LoadIntro();

		bool IsDead() { return m_Health <= 0; }

		void Pause();
		void Unpause();

		bool IsSensor(b2ShapeId shape);
		bool IsParry(b2ShapeId shape);
		bool IsFirebubble(b2ShapeId shape);
		void HitFirebubble(b2ShapeId shape, float damage);
		void DestroyParry(b2ShapeId shape);

		void ClearProjectiles();

		void Hit(int damage);
		bool IsShooting() const { return m_Shooting; }

		void SetPlayerPosition(const glm::vec2& pos) { m_PlayerPosition = pos; }
		bool IsLastPhase() const { return m_Phase3Start; }

		void SetColor(glm::vec4 color);
	private:
		bool OnKeyPressed(Teddy::KeyPressedEvent& e);
		bool OnKeyReleased(Teddy::KeyReleasedEvent& e);

		void StartIdle();
		void Idle(Teddy::Timestep ts);
		void Phase1Part1(Teddy::Timestep ts);
		void Phase1Part2(Teddy::Timestep ts);
		void Phase1To2(Teddy::Timestep ts);
		void Phase2Start(Teddy::Timestep ts);

		void Intro();

		void StartPeashot();
		void Peashot(Teddy::Timestep ts);
		void PsychicEye(Teddy::Timestep ts);

		void StartMeteor();
		void Meteor(Teddy::Timestep ts);
		void LaunchMeteor();

		void StartTail();
		void Tail(Teddy::Timestep ts);

		void Smoke(Teddy::Timestep ts);
		void FireMarchers(Teddy::Timestep ts);
		void SpawnFireMarcherA();
		void SpawnFireMarcherB();
		void SpawnFireMarcherC();
		void Phase2Ending();

		void Phase2To3(Teddy::Timestep ts);
		void Phase3Start(Teddy::Timestep ts);
		void Phase3StartLoop(Teddy::Timestep ts);
		void StartPhase3Attack();

		void Firebubble();
		void CreateFirebubbleSpit(float x, float y);
		void CreateFirebubble(float x, float y);
		void CreateMiniFirebubbles(glm::vec3& pos);
		void StartFireTorch();
		void FireTorch(Teddy::Timestep ts);
		void CreateFiretorchProjectile();

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
		std::vector<Teddy::Ref<Teddy::Texture2D>> m_SmokeTextures;
		std::vector<Teddy::Ref<Teddy::Texture2D>> m_FireMarcherTextures;
		std::vector<Teddy::Ref<Teddy::Texture2D>> m_Phase2DeathTextures;

		std::vector<Teddy::Ref<Teddy::Texture2D>> m_Phase3IntroTextures;
		std::vector<Teddy::Ref<Teddy::Texture2D>> m_Phase3IdleBodyTextures;
		std::vector<Teddy::Ref<Teddy::Texture2D>> m_Phase3IdleHeadTextures;
		std::vector<Teddy::Ref<Teddy::Texture2D>> m_Phase3AttackHeadTextures;
		std::vector<Teddy::Ref<Teddy::Texture2D>> m_Phase3FirebubbleTextures;
		std::vector<Teddy::Ref<Teddy::Texture2D>> m_Phase3MiniFirebubbleTextures;
		std::vector<Teddy::Ref<Teddy::Texture2D>> m_Phase3FiretorchTextures;
		std::vector<Teddy::Ref<Teddy::Texture2D>> m_Phase3FiretorchProjectileTextures;

		enum class DragonState
		{
			Idle = 0,
			Intro,
			Peashot,
			Meteor,
			Firebubble,
			Firetorch
		};

		DragonState m_State = DragonState::Intro;

		int m_Phase = 1;

		glm::vec2 m_PlayerPosition = { 0.0f, 0.0f };

		bool m_IntroDone = false;
		bool m_Shooting = false;
		bool m_MeteorStart = true;

		int m_Health = 565; // 629, 510, 561 /// 1700, 1535, 1071, 561
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
		
		bool m_TailActive = false;
		bool m_TailPick = false;
		bool m_TailUp = false;

		Teddy::Entity m_DragonTongueEntity;
		Teddy::Entity m_SmokeEntity;
		bool m_PhaseStart = false;
		bool m_Transitioning = false;

		bool m_Phase2Start = false;
		bool m_TongueToLoop = false;
		bool m_ResetFireLoop = false;

		struct AttackerFireMarcher
		{
			Teddy::Entity Entity;
			float XToAttack;
			bool ToAttack = true;
			bool Attacked = false;

			AttackerFireMarcher(Teddy::Entity entity, float x)
				: Entity(entity), XToAttack(x) {}
		};
		std::vector<AttackerFireMarcher> m_AttackableEntities;

		bool m_Phase3Start = false;
		bool m_Phase3StartLoop = false;

		struct Phase3Heads
		{
			Teddy::Entity RightHead; // 0
			Teddy::Entity MidHead; // 1
			Teddy::Entity LeftHead; // 2

			int SelectedHead = 0;
			bool Shot = false;
		};

		struct FirebubbleProjectile
		{
			Teddy::Entity Entity;
			float Health;

			FirebubbleProjectile(Teddy::Entity entity, float health)
				: Entity(entity), Health(health) {}
			FirebubbleProjectile(Teddy::Entity entity)
				: Entity(entity), Health(5.0f) {
			}
		};

		Phase3Heads m_Phase3Heads;
		Teddy::Entity m_FirebubbleSpitEntity;
		std::vector<FirebubbleProjectile> m_Firebubbles;

		bool m_FireTorchStart = true;
		Teddy::Entity m_FiretorchProjectileEntity;

		Teddy::Ref<Teddy::Scene> m_Scene = nullptr;
	};

}