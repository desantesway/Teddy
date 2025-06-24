#include "teddyPch.h"
#include "Application.h"

#include "Teddy/Log.h"

#include <SDL3/SDL.h>
#include <glad/glad.h>

#include "Teddy/Input.h"
#include "Teddy/MidiCodes.h"
#include "Teddy/KeyCodes.h"
#include "Teddy/MouseCodes.h"

namespace Teddy {

	Application* Application::s_Instance = nullptr;

	Application::Application(){

		TED_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));
	}
	Application::~Application(){
	
	}

	void Application::PushLayer(Layer* layer)
	{
			m_LayerStack.PushLayer(layer);
			layer->OnAttach();
	}

	void Application::PushOverlay(Layer* layer)
	{
		m_LayerStack.PushOverlay(layer);
		layer->OnAttach();
	}

	void Application::OnEvent(Event& e)
	{
		
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::OnWindowClose)); // if the event is Window close do OnWindowClose()

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); )
		{
			(*--it)->OnEvent(e);
			if (e.Handled)
				break;
		}
	}

	void Application::Run() {
		while (m_Running)
		{
			glClearColor(1, 0, 1, 1);
			glClear(GL_COLOR_BUFFER_BIT);

			for (Layer* layer : m_LayerStack)
				layer->OnUpdate();

			bool x = Input::IsKeyPressed(TED_KEY_LSHIFT);
			bool y = Input::IsMouseButtonPressed(TED_MOUSE_RIGHT);
			if (x) TED_CORE_TRACE("{0}", x);
			if (y) TED_CORE_TRACE("{0}", y);

			m_Window->OnUpdate();
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}
}