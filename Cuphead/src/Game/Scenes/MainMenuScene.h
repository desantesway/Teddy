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

		unsigned int LoadLevel() { return m_LoadLevel; }
		void Shutdown();
	private:
		void InitMainMenu();
		void UpdateMainMenuButtonColors();
		void HideMainMenu();
		void EnterMainMenuOption();
		bool OnMainMenuKeyPressed(Teddy::KeyPressedEvent& e);

		void InitDlcMenu();
		void HideDlcMenu();
		void UpdateDlcButtonColors();
		bool OnDlcMenuKeyPressed(Teddy::KeyPressedEvent& e);

		void InitPlayMenu();
		void HidePlayMenu();
		void UpdatePlayButtonColors();
		bool OnPlayMenuKeyPressed(Teddy::KeyPressedEvent& e);
		void OnPlayUpdate();

		bool OnKeyPressed(Teddy::KeyPressedEvent& e);
	private:
		Teddy::Ref<Teddy::Scene> m_MainMenu;
		glm::vec4 m_WhiteColor = glm::vec4(1.0f);
		glm::vec4 m_GrayColor = glm::vec4(110.0f / 255.0f, 110.0f / 255.0f, 110.0f / 255.0f, 1.0f);
		glm::vec4 m_BlackColor = glm::vec4(70.0f / 255.0f, 70.0f / 255.0f, 70.0f / 255.0f, 1.0f);
		glm::vec4 m_RedColor = glm::vec4(172.0f / 255.0f, 32.0f / 255.0f, 54.0f / 255.0f, 1.0f);
		glm::vec4 m_InvisibleColor = glm::vec4(0.0f);
		
		MainMenu m_MainMenuOptions;
		DlcMenu m_DlcMenu;
		PlayMenu m_PlayMenu;
		OptionsMenu m_OptionsMenu;

		unsigned int m_LoadLevel = 0; // 0 = none, 1 = cuphead, 2 = mugman
		unsigned int m_CurrentMenu = 0;
	};
}