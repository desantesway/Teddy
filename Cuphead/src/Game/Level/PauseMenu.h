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

		void Init(Teddy::Ref<Teddy::Scene> scene);
		void OnEvent(Teddy::Event& event);
		void OnUpdate(Teddy::Timestep ts);

		void Show();
		void Hide();
		void PartialHide();
		void PartialShow();

		int GetState() { return m_State; }
		bool WantsToResume() { return m_State == 1; }
		bool WantsToRetry()  { return m_State == 2; }
		bool WantsToOptions()  { return m_State == 3; }
		bool WantsToExit()   { return m_State == 4; }
	private:
		void UpdateColors();

		bool OnKeyPressed(Teddy::KeyPressedEvent& e);
	private:
		Teddy::Entity m_Overlay;
		Teddy::Entity m_Background;
		Teddy::Entity m_Resume;
		Teddy::Entity m_Retry;
		Teddy::Entity m_Options;
		Teddy::Entity m_Exit;

		Teddy::Ref<Teddy::Texture2D> m_BackgroundTexture = nullptr;
		Teddy::Ref<Teddy::Font> m_OptionsFont = nullptr;

		Teddy::Ref<Teddy::Scene> m_Scene = nullptr;

		glm::vec4 m_BlackColor = glm::vec4(70.0f / 255.0f, 70.0f / 255.0f, 70.0f / 255.0f, 1.0f);
		glm::vec4 m_RedColor = glm::vec4(172.0f / 255.0f, 32.0f / 255.0f, 54.0f / 255.0f, 1.0f);

		int m_State = 0; // 0 = Menu, 1 = Resume,  2 = Retry, 3 = Options, 4 = Exit
		int m_CurrentSelection = 0; //0 = Resume,  1 = Retry, 2 = Options, 3 = Exit

		OptionsMenu m_OptionsMenu;
	};
}