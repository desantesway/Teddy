#pragma once

#include <Teddy.h>
#include "MainTitleScene.h"
#include "TransitionScenes.h"
#include "MainMenuScene.h"
#include "../Level/LevelScene.h"

namespace Cuphead
{
	class GameScenes
	{
	public:
		GameScenes();
		~GameScenes() = default;

		static GameScenes* Get() { return instance; }

		static void Init();
		static bool OnUpdate(Teddy::Timestep& ts);

		void OnEvent(Teddy::Event& event);

		static Teddy::Ref<Teddy::Scene> InitNextScene();
		static void FreeScenes();
	private:
		bool OnKeyPressed(Teddy::KeyPressedEvent& e);

		static Teddy::Ref<Teddy::Scene> InitTitle();
		static Teddy::Ref<Teddy::Scene> InitMainMenu();
		static Teddy::Ref<Teddy::Scene> InitLevel();
	private:
		static int m_CurrentScene;

		static Teddy::Ref<Teddy::Scene> m_ActiveScene;

		static TransitionScenes m_TransitionScenes;
		static MainTitleScene m_MainTitleScene;
		static MainMenuScene m_MainMenuScene;
		static LevelScene m_LevelScene;

		static bool m_IsCuphead;
	private:
		static GameScenes* instance;
	};
}