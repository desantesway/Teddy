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

		void Init(Teddy::Ref<Teddy::Scene> scene);
		void OnEvent(Teddy::Event& event);
		void OnUpdate(Teddy::Timestep ts, bool isCuphead, int bossProgress);

		void Show();
		void Hide();

		int GetState() { return m_State; }
		bool WantsToRetry() { return m_State == 1; }
		bool WantsToExit() { return m_State == 2; }
		bool WantsToQuit() { return m_State == 3; }
	private:
		void UpdateColors();

		bool OnKeyPressed(Teddy::KeyPressedEvent& e);
	private:
		Teddy::Entity m_Overlay;
		Teddy::Entity m_YouDied;
		Teddy::Entity m_Background;
		Teddy::Entity m_BossPhoto;
		Teddy::Entity m_BossQuote;
		Teddy::Entity m_ProgressBar;
		Teddy::Entity m_Retry;
		Teddy::Entity m_ExitToMap;
		Teddy::Entity m_Quit;

		Teddy::Ref<Teddy::Texture2D> m_BackgroundTexture;
		std::vector<Teddy::Ref<Teddy::Texture2D>> m_YouDiedTexture;
		Teddy::Ref<Teddy::Font> m_OptionsFont;
		Teddy::Ref<Teddy::Font> m_QuoteFont;

		Teddy::Ref<Teddy::Scene> m_Scene;

		glm::vec4 m_BlackColor = glm::vec4(70.0f / 255.0f, 70.0f / 255.0f, 70.0f / 255.0f, 1.0f);
		glm::vec4 m_RedColor = glm::vec4(172.0f / 255.0f, 32.0f / 255.0f, 54.0f / 255.0f, 1.0f);

		int m_State = 0;
		int m_CurrentSelection = 0;

		bool m_Started = false;
		bool m_LoadedMenu = false;
	};
}