#pragma once

#include "Teddy/Scene/Entity.h"
#include "Teddy/Core/Timestep.h"
#include "Teddy/Scene/Scene.h"
#include "Teddy/Events/Event.h"
#include "Teddy/Events/KeyEvent.h"

namespace Cuphead
{
	class DeathMenu
	{
	public:
		DeathMenu() = default;
		~DeathMenu() = default;

		void Shutdown();

		static void Init(Teddy::Ref<Teddy::Scene> scene);
		void OnEvent(Teddy::Event& event);
		static void OnUpdate(Teddy::Timestep ts);

		static void Show(bool isCuphead, int bossProgress);
		static void Hide();

		static int GetState() { return m_State; }
		static bool WantsToRetry() { return m_State == 1; }
		static bool WantsToExit() { return m_State == 2; }
		static bool WantsToQuit() { return m_State == 3; }
	private:
		static void UpdateColors();

		bool OnKeyPressed(Teddy::KeyPressedEvent& e);
	private:
		static Teddy::Entity m_Overlay;
		static Teddy::Entity m_YouDied;
		static Teddy::Entity m_Background;
		static Teddy::Entity m_BossPhoto;
		static Teddy::Entity m_BossQuote;
		static Teddy::Entity m_ProgressBar;
		static Teddy::Entity m_Retry;
		static Teddy::Entity m_ExitToMap;
		static Teddy::Entity m_Quit;

		static Teddy::Ref<Teddy::Texture2D> m_BackgroundTexture;
		static Teddy::Ref<Teddy::Font> m_OptionsFont;
		static Teddy::Ref<Teddy::Font> m_QuoteFont;

		static Teddy::Ref<Teddy::Scene> m_Scene;

		static glm::vec4 m_BlackColor;
		static glm::vec4 m_RedColor;

		static int m_State;
		static int m_CurrentSelection;
	};
}