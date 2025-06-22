#include "teddy_pch.h"
#include "Application.h"

#include "Teddy/Log.h"

#include <SDL3/SDL.h>
#include <glad/glad.h>

namespace Teddy {

	#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application::Application(){
		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));
	}
	Application::~Application(){
	
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose)); // if the event is Window close do OnWindowClose()

		TED_CORE_TRACE("{0}", e);
	}

	void Application::Run() {
		while (m_Running)
		{
			m_Window->Events();
			glClearColor(1, 0, 1, 1);
			glClear(GL_COLOR_BUFFER_BIT);
			m_Window->OnUpdate();
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}
}