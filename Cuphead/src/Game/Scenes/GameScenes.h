#pragma once

#include <Teddy.h>
#include "MainTitleScene.h"
#include "TransitionScenes.h"

namespace Cuphead
{
	class GameScenes
	{
	public:
		GameScenes();
		~GameScenes() = default;

		static GameScenes* Get() { return instance; }

		static void Init();
		static bool OnUpdate(Teddy::Timestep ts);

		void OnEvent(Teddy::Event& event);

		static Teddy::Ref<Teddy::Scene> InitNextScene();
	private:
		bool OnKeyPressed(Teddy::KeyPressedEvent& e);

		static Teddy::Ref<Teddy::Scene> InitMainTitle();
		static Teddy::Ref<Teddy::Scene> InitTitleMenu();
	private:
		static int m_CurrentScene;

		static TransitionScenes m_TransitionScenes;
		static MainTitleScene m_MainTitleScene;
	private:
		static GameScenes* instance;
	};
}