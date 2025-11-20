#pragma once

#include "Teddy/Scene/Scene.h"
#include "Teddy/Core/Timestep.h"
#include "Teddy/Scene/Entity.h"
#include "Teddy/Physics/ContactEvent.h"
#include "Teddy/Physics/SensorEvent.h"
#include "Player.h"
#include "CloudPlatform.h"
#include "PauseMenu.h"
#include "DeathMenu.h"
#include "Dragon.h"

namespace Cuphead
{

	class LevelScene
	{
	public:
		LevelScene() = default;
		~LevelScene();
		
		Teddy::Ref<Teddy::Scene> Init(bool character);
		Teddy::Ref<Teddy::Scene> GetScene() { return m_Scene; }

		void OnUpdate(Teddy::Timestep ts);
		void OnEvent(Teddy::Event& event);

		void StartIntro();
		void LoadIntro();
		bool IsIntroDone() { return m_IntroDone; }
		bool HasIntroStarted() { return m_StartIntro; }

		bool WantsToRetry() { return m_State == 1; }
		bool WantsToExit() { return m_State == 2; }
		bool WantsToResult() { return m_State == 3; }

		float GetHealth() { return m_Player.GetHealth(); }
		int GetParry() { return m_Player.GetParry(); }
		int GetSuper() { return m_Player.GetSuper(); }
		float GetTimer() { return m_Player.GetTimer(); }
	private:
		bool Freeze(Teddy::Timestep ts);
		void FreezeWithoutPlayer(Teddy::Timestep ts);

		void InitPhase1();
		void InitPhase1Background();
		void InitPhase1Foreground();
		void OnUpdatePhase1();
		
		void InitPhase3();
		void InitPhase3Background();
		void InitPhase3Foreground();
		void OnUpdatePhase3(Teddy::Timestep ts);
		void OnPhase3Start();
		void OnUpdatePhase3Start(Teddy::Timestep ts);

		void CameraShake(Teddy::Timestep ts);

		void StartPauseMenu();
		bool Pause(Teddy::Timestep ts);

		bool OnKeyPressed(Teddy::KeyPressedEvent& e);
		bool OnSensorBegin(Teddy::SensorBeginEvent& e);
		bool OnSensorEnd(Teddy::SensorEndEvent& e);
		bool OnContactBegin(Teddy::ContactBeginEvent& e);
		bool OnContactEnd(Teddy::ContactEndEvent& e);
	private:
		Dragon m_Dragon;
		Player m_Player;
		CloudPlatform m_Clouds;
		Teddy::Entity m_Floor;
		Teddy::Entity m_Camera;

		float m_MovementVelocity = 0.0f;
		float m_MovementSpeed = 0.0f;

		struct Background
		{
			Teddy::Entity LeftBackground;
			Teddy::Entity RightBackground;

			Teddy::Entity Spire;

			Teddy::Entity Cloud1Left;
			Teddy::Entity Cloud1Right;
			Teddy::Entity Cloud2Left;
			Teddy::Entity Cloud2Right;
			Teddy::Entity Cloud3Left;
			Teddy::Entity Cloud3Right;
			Teddy::Entity Cloud4Left;
			Teddy::Entity Cloud4Right;
			Teddy::Entity Cloud5Left;
			Teddy::Entity Cloud5Right;
		};

		struct Foreground
		{
			Teddy::Entity Cloud1Left;
			Teddy::Entity Cloud1Right;
			Teddy::Entity Cloud2Left;
			Teddy::Entity Cloud2Right;
			Teddy::Entity Cloud3Left;
			Teddy::Entity Cloud3Right;
		};

		Background m_Background;
		Foreground m_Foreground;

		struct Ph3Background
		{
			Teddy::Entity LeftBackground;
			Teddy::Entity RightBackground;

			Teddy::Entity Spire;

			Teddy::Entity Cloud1Left;
			Teddy::Entity Cloud1Right;
			Teddy::Entity Cloud2Left;
			Teddy::Entity Cloud2Right;
			Teddy::Entity Cloud3Left;
			Teddy::Entity Cloud3Right;
		};

		struct Rain
		{
			Teddy::Entity Rain1;
			Teddy::Entity Rain2;
			Teddy::Entity Rain3;
		};

		Ph3Background m_BackgroundPhase3;
		Foreground m_ForegroundPhase3;
		Rain m_Rain;
		Teddy::Entity m_LightningFlash;
		bool m_Lightning = false;

		std::vector<Teddy::Ref<Teddy::Texture2D>> m_Phase3SpireTextures;
		Teddy::Ref<Teddy::Texture2D> m_Phase3BackgroundTexture;
		Teddy::Ref<Teddy::Texture2D> m_Phase3BackgroundNightCloudTexture;
		Teddy::Ref<Teddy::Texture2D> m_Phase3ForegroundNightCloudTexture;
		Teddy::Ref<Teddy::Texture2D> m_Phase3BackgroundFlash1CloudTexture;
		Teddy::Ref<Teddy::Texture2D> m_Phase3ForegroundFlash1CloudTexture;
		Teddy::Ref<Teddy::Texture2D> m_Phase3BackgroundFlash2CloudTexture;
		Teddy::Ref<Teddy::Texture2D> m_Phase3ForegroundFlash2CloudTexture;
		std::vector<Teddy::Ref<Teddy::Texture2D>> m_RainTextures;
		std::vector<Teddy::Ref<Teddy::Texture2D>> m_LightningTextures;
		std::vector<Teddy::Ref<Teddy::Texture2D>> m_KnockoutTextures;

		int m_State = 0;
		int m_Phase = 1;

		bool m_Paused = false;
		PauseMenu m_PauseMenu;

		bool m_IntroDone = false;
		bool m_StartIntro = false;
		Teddy::Entity m_FightText;

		bool m_FloorHitContact = false;
		bool m_HitContact = false;
		bool m_CameraShake = false;

		bool m_IsCuphead = true;

		DeathMenu m_DeathMenu;
		bool m_FirstDeath = true;

		bool m_IncreasingSpeed = false;

		bool m_Freeze = false;
		float m_FreezeTimer = 0.15f;

		bool m_TransitioningPhase = false;

		bool m_Unfreeze = false;

		Teddy::Ref<Teddy::Scene> m_Scene = nullptr;
	};
}