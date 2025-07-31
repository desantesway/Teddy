#include "teddyPch.h"
#include "Teddy/Core/Application.h"

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

		TED_PROFILE_FUNCTION();

		TED_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		m_Window = Window::Create();
		m_Window->SetEventCallback(TED_BIND_EVENT_FN(Application::OnEvent));

		Renderer::Init();

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);

	}

	Application::~Application()
	{
		TED_PROFILE_FUNCTION();

		Renderer::Shutdown();
	}

	void Application::PushLayer(Layer* layer)
	{
		TED_PROFILE_FUNCTION();

		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* layer)
	{
		TED_PROFILE_FUNCTION();

		m_LayerStack.PushOverlay(layer);
		layer->OnAttach();
	}

	void Application::OnEvent(Event& e)
	{
		TED_PROFILE_FUNCTION();

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
		TED_PROFILE_FUNCTION();

		while (m_Running)
		{
			TED_PROFILE_SCOPE("Application Run Loop");

			float time = (float)SDL_GetTicks(); //Platform::GetTime
			Timestep timestep = time - m_LastFrameTime;
			m_LastFrameTime = time;

			if (!m_Minimized) {
				{
					TED_PROFILE_SCOPE("LayerStack OnUpdate");

					for (Layer* layer : m_LayerStack)
						layer->OnUpdate(timestep);
				}

				m_ImGuiLayer->Begin();
				{
					TED_PROFILE_SCOPE("ImGui LayerStack OnUpdate");

					for (Layer* layer : m_LayerStack)
						layer->OnImGuiRender();
				}
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

	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		TED_PROFILE_FUNCTION();

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