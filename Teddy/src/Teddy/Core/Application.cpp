#include "teddyPch.h"
#include "Application.h"

#include "Teddy/Core/Log.h"

#include <SDL3/SDL.h>
#include <glad/glad.h>

#include "Teddy/Core/Input.h"
#include "Teddy/Core/MidiCodes.h"
#include "Teddy/Core/KeyCodes.h"
#include "Teddy/Core/MouseCodes.h"

#include "Teddy/Renderer/Renderer.h"

namespace Teddy {

	Application* Application::s_Instance = nullptr;

	Application::Application()
	{

		TED_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(TED_BIND_EVENT_FN(Application::OnEvent));

		Renderer::Init();

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);

	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
	}

	void Application::PushOverlay(Layer* layer)
	{
		m_LayerStack.PushOverlay(layer);
	}

	void Application::OnEvent(Event& e)
	{
		
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(TED_BIND_EVENT_FN(Application::OnWindowClose)); // if the event is Window close do OnWindowClose()
		dispatcher.Dispatch<WindowResizeEvent>(TED_BIND_EVENT_FN(Application::OnWindowResize));

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); )
		{
			(*--it)->OnEvent(e);
			if (e.Handled)
				break;
		}

		Input::OnEvent(e);
	}

	void Application::Run() {
		while (m_Running)
		{
			float time = (float)SDL_GetTicks(); //Platform::GetTime
			Timestep timestep = time - m_LastFrameTime;
			m_LastFrameTime = time;

			if (!m_Minimized) {
				for (Layer* layer : m_LayerStack)
					layer->OnUpdate(timestep);

				// ImGui minimizes too (?)
				m_ImGuiLayer->Begin();
				for (Layer* layer : m_LayerStack)
					layer->OnImGuiRender();
				m_ImGuiLayer->End();
			}

			m_Window->OnUpdate();
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}

	bool Application::OnMidiKeyPressed(MidiKeyPressedEvent& e)
	{
		TED_CORE_TRACE("{0}", e);
		return true;
	}

	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		if (e.GetWidth() == 0 || e.GetHeight() == 0) {
			m_Minimized = true;
			return false;
		}

		m_Window->SetWidth(e.GetWidth());
		m_Window->SetHeight(e.GetHeight());
		Renderer::OnWindowResize(m_Window->GetWidth(), m_Window->GetHeight());

		m_Minimized = false;

		return false;
	}
}