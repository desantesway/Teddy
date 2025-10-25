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
		GameScenes() = default;
		~GameScenes();

		static GameScenes& Get()
		{
			static GameScenes instance;
			return instance;
		}

		void Init();
		bool OnUpdate(Teddy::Timestep& ts);

		void OnEvent(Teddy::Event& event);

		Teddy::Ref<Teddy::Scene> InitNextScene();
		void FreeScenes();
	private:
		bool OnKeyPressed(Teddy::KeyPressedEvent& e);

		Teddy::Ref<Teddy::Scene> InitTitle();
		Teddy::Ref<Teddy::Scene> InitMainMenu();
		Teddy::Ref<Teddy::Scene> InitLevel();
	private:
		int m_CurrentScene = 2;

		TransitionScenes m_TransitionScenes;  // TODO: make this on another layer
		MainTitleScene m_MainTitleScene;
		Teddy::Ref<MainMenuScene> m_MainMenuScene = nullptr;
		Teddy::Ref<LevelScene> m_LevelScene = nullptr;

		bool m_IsCuphead = false;

		Teddy::Ref<Teddy::Scene> m_ActiveScene = nullptr;
	};
}