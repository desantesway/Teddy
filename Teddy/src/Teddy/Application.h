#pragma once

#include "Core.h"

#include "Window.h"

#include "Teddy/LayerStack.h"
#include "Events/Event.h"
#include "Events/ApplicationEvent.h"

namespace Teddy{

	class TED_API Application
	{
	public:
			Application();
			virtual ~Application();

			void Run();

			void OnEvent(Event& e);
	private:
		bool OnWindowClose(WindowCloseEvent& e);

		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
	};

	// to be defined in CLIENT
	Application* CreateApplication();
}