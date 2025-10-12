#pragma once

#include "Teddy/Scene/Scene.h"
#include "Teddy/Core/Timestep.h"
#include "Teddy/Events/Event.h"
#include "Teddy/Events/KeyEvent.h"
#include "Teddy/Scene/Entity.h"
#include "Slider.h"

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

	struct OptionsMenu
	{
		Teddy::Entity AudioButton;
		Teddy::Entity VisualButton;
		Teddy::Entity ControlsButton;
		Teddy::Entity LanguageButton;
		Teddy::Entity BackButton;

		Teddy::Entity Background;

		unsigned int CurrentSelection = 0;
	};

	struct VisualMenu
	{

		Teddy::Entity Resolution;
		Teddy::Entity ResolutionButton;
		Teddy::Entity Display;
		Teddy::Entity DisplayButton;
		Teddy::Entity VSync;
		Teddy::Entity VSyncButton;
		Teddy::Entity Overscan;
		Slider OverscanBar;
		Teddy::Entity Brightness;
		Slider BrightnessBar;
		Teddy::Entity ColorBleed;
		Slider ColorBleedBar;
		Teddy::Entity TitleScreen;
		Teddy::Entity TitleScreenButton;
		Teddy::Entity Back;

		Teddy::Entity Background;

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
		~MainMenuScene() = default;

		Teddy::Ref<Teddy::Scene> Init();

		void OnUpdate(Teddy::Timestep ts);
		void OnEvent(Teddy::Event& event);

		static unsigned int LoadLevel() { return m_LoadLevel; }
	private:
		void InitMainMenu();
		static void UpdateMainMenuButtonColors();
		static void HideMainMenu();
		static void EnterMainMenuOption();
		static bool OnMainMenuKeyPressed(Teddy::KeyPressedEvent& e);

		void InitOptionsMenu();
		static void HideOptionsMenu();
		static void UpdateOptionsButtonColors();
		static bool OptionsMenuEnter();
		static bool OnOptionsMenuKeyPressed(Teddy::KeyPressedEvent& e);

		void InitVisualMenu();
		static bool OnVisualMenuKeyPressed(Teddy::KeyPressedEvent& e);
		static void UpdateVisualColors();
		static void HideVisualMenu();

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
		static OptionsMenu m_OptionsMenu;
		static VisualMenu m_VisualMenu;
		static DlcMenu m_DlcMenu;
		static PlayMenu m_PlayMenu;

		static unsigned int m_LoadLevel; // 0 = none, 1 = cuphead, 2 = mugman
		static unsigned int m_CurrentMenu;
	};
}