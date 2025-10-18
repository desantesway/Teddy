#pragma once

#include "Teddy/Scene/Scene.h"
#include "Teddy/Core/Timestep.h"
#include "Teddy/Events/Event.h"
#include "Teddy/Events/KeyEvent.h"
#include "Teddy/Scene/Entity.h"
#include "OptionsMenu.h"

namespace Cuphead
{
	struct MainMenu
	{
		Teddy::Entity PlayButton;
		Teddy::Entity OptionsButton;
		Teddy::Entity DlcButton;
		Teddy::Entity ExitButton;

		unsigned int CurrentSelection = 0;
	};

	struct DlcMenu
	{
		Teddy::Entity Entity;
	};

	struct PlayMenu
	{
		Teddy::Entity Background;
		Teddy::Entity Selected;
		Teddy::Entity Text;
		Teddy::Entity PlayerText;

		Teddy::Entity Cuphead;
		Teddy::Entity Mugman;

		bool CurrentSelection = 0;
	};

	class MainMenuScene
	{
	public:
		MainMenuScene() = default;
		~MainMenuScene() { m_MainMenu = nullptr; }

		Teddy::Ref<Teddy::Scene> Init();

		void OnUpdate(Teddy::Timestep ts);
		void OnEvent(Teddy::Event& event);

		static unsigned int LoadLevel() { return m_LoadLevel; }
		void Shutdown();
	private:
		void InitMainMenu();
		static void UpdateMainMenuButtonColors();
		static void HideMainMenu();
		static void EnterMainMenuOption();
		static bool OnMainMenuKeyPressed(Teddy::KeyPressedEvent& e);

		void InitDlcMenu();
		static void HideDlcMenu();
		static void UpdateDlcButtonColors();
		static bool OnDlcMenuKeyPressed(Teddy::KeyPressedEvent& e);

		void InitPlayMenu();
		static void HidePlayMenu();
		static void UpdatePlayButtonColors();
		static bool OnPlayMenuKeyPressed(Teddy::KeyPressedEvent& e);
		void OnPlayUpdate();

		static bool OnKeyPressed(Teddy::KeyPressedEvent& e);
	private:
		Teddy::Ref<Teddy::Scene> m_MainMenu;
		static glm::vec4 m_GrayColor;
		static glm::vec4 m_WhiteColor;
		static glm::vec4 m_BlackColor;
		static glm::vec4 m_RedColor;
		static glm::vec4 m_InvisibleColor;
		
		static MainMenu m_MainMenuOptions;
		static DlcMenu m_DlcMenu;
		static PlayMenu m_PlayMenu;
		static OptionsMenu m_OptionsMenu;

		static unsigned int m_LoadLevel; // 0 = none, 1 = cuphead, 2 = mugman
		static unsigned int m_CurrentMenu;
	};
}