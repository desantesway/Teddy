#pragma once

#include "Teddy/Scene/Scene.h"
#include "Teddy/Core/Timestep.h"

class GameScenes
{
public:
	GameScenes();
	~GameScenes() = default;

	static GameScenes* Get() { return instance; }

	static bool OnUpdate(Teddy::Timestep ts);

	static void ProceedToMainMenu() { m_ProceedToMainMenu = true; }

	static Teddy::Ref<Teddy::Scene> InitNextScene();
	static Teddy::Ref<Teddy::Scene> InitTitleStart();
	static Teddy::Ref<Teddy::Scene> InitTitleMenu();
private:
	static void LoadMainMenu(Teddy::Timestep ts);
	static void StartTransition(Teddy::Timestep ts);
private:
	static bool m_ProceedToMainMenu;
	static bool m_Transitioning;
	static bool m_LoadingMainMenu;
	static bool m_LoadMainMenu;

	static Teddy::Ref<Teddy::Entity> m_TransitionCircle; // put this on a class
	static Teddy::Ref<Teddy::Entity> m_TransitionQuad;

private:
	static GameScenes* instance;
};