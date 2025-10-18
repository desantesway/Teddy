#pragma once

#include "Teddy/Scene/Entity.h"
#include "Teddy/Core/Timestep.h"
#include "Teddy/Scene/Scene.h"
#include "Teddy/Events/Event.h"
#include "Teddy/Events/KeyEvent.h"

#include "../Scenes/OptionsMenu.h"

namespace Cuphead
{
	class PauseMenu
	{
	public:
		PauseMenu() = default;
		~PauseMenu() = default;

		void Shutdown();

		static void Init(Teddy::Ref<Teddy::Scene> scene);
		void OnEvent(Teddy::Event& event);
		static void OnUpdate(Teddy::Timestep ts);

		static void Show();
		static void Hide();
		static void PartialHide();

		static bool IsVisible() { return m_Visible; }
		static int GetState() { return m_State; }
		static bool WantsToResume() { return m_State == 1; }
		static bool WantsToRetry()  { return m_State == 2; }
		static bool WantsToOptions()  { return m_State == 3; }
		static bool WantsToExit()   { return m_State == 4; }
	private:
		static void UpdateColors();

		bool OnKeyPressed(Teddy::KeyPressedEvent& e);
	private:
		static Teddy::Entity m_Overlay;
		static Teddy::Entity m_Background;
		static Teddy::Entity m_Resume;
		static Teddy::Entity m_Retry;
		static Teddy::Entity m_Options;
		static Teddy::Entity m_Exit;

		static Teddy::Ref<Teddy::Texture2D> m_BackgroundTexture;
		static Teddy::Ref<Teddy::Font> m_OptionsFont;

		static Teddy::Ref<Teddy::Scene> m_Scene;

		static glm::vec4 m_BlackColor;
		static glm::vec4 m_RedColor;

		static bool m_Visible;
		static int m_State; // 0 = Menu, 1 = Resume,  2 = Retry, 3 = Options, 4 = Exit
		static int m_CurrentSelection; //0 = Resume,  1 = Retry, 2 = Options, 3 = Exit

		static OptionsMenu m_OptionsMenu;
	};
}