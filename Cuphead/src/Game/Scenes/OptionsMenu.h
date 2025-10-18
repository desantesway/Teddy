#pragma once

#include "Teddy/Scene/Scene.h"
#include "Teddy/Core/Timestep.h"
#include "Teddy/Events/Event.h"
#include "Teddy/Events/KeyEvent.h"
#include "Teddy/Scene/Entity.h"
#include "Slider.h"

namespace Cuphead
{
	class OptionsMenu
	{
	public:
		OptionsMenu() = default;
		~OptionsMenu() = default;

		void Init(Teddy::Ref<Teddy::Scene> scene);

		void Hide();
		void Show();

		void UpdateButtonColors();
		bool OptionsMenuEnter();
		bool OnKeyPressed(Teddy::KeyPressedEvent& e);

		void InitVisualMenu(Teddy::Ref<Teddy::Scene> scene);
		bool OnVisualMenuKeyPressed(Teddy::KeyPressedEvent& e);
		void UpdateVisualColors();
		void HideVisualMenu();

		bool WantsToClose() const { return m_CurrentMenu == -1; }
	private:

		struct Options
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

		static glm::vec4 m_InvisibleColor;
		static glm::vec4 m_WhiteColor;
		static glm::vec4 m_BlackColor;
		static glm::vec4 m_RedColor;

		Options m_OptionsMenu;
		VisualMenu m_VisualMenu;

		unsigned int m_CurrentMenu = -1;
	};
}