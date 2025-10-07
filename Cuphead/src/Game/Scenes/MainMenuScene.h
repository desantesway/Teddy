#pragma once

#include "Teddy/Scene/Scene.h"
#include "Teddy/Core/Timestep.h"
#include "Teddy/Events/Event.h"
#include "Teddy/Events/KeyEvent.h"

namespace Cuphead
{
	class MainMenuScene
	{
	public:
		MainMenuScene() = default;
		~MainMenuScene() = default;

		Teddy::Ref<Teddy::Scene> Init();

		void OnUpdate(Teddy::Timestep ts);
		void OnEvent(Teddy::Event& event);

	private:
		static void UpdateButtonColor();
		static void EnterOption();
		static void HideMainMenu();

		static bool OnKeyPressed(Teddy::KeyPressedEvent& e);

	private:
		Teddy::Ref<Teddy::Scene> m_MainMenu;
		static glm::vec4 m_NormalColor;
		static glm::vec4 m_HighlightColor;
		static glm::vec4 m_InvisibleColor;
		 
		static Teddy::Entity m_PlayButton;
		static Teddy::Entity m_OptionsButton;
		static Teddy::Entity m_DlcButton;
		static Teddy::Entity m_ExitButton;

		static unsigned int m_CurrentMainMenuSelection;
		static unsigned int m_CurrentMenu;
	};
}