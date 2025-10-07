#include "TeddyPch.h"
#include "Teddy/Core/Application.h"

#include "Teddy/Renderer/Renderer.h"
#include "Teddy/Utils/PlatformUtils.h"
#include "Teddy/PostProcessing/PostProcessing.h"

#include <SDL3/SDL.h>
#include <glad/glad.h>

namespace Teddy 
{

	Application* Application::s_Instance = nullptr;

	Application::Application(const ApplicationSpecification& specification)
		: m_Specification(specification)
	{
		TED_PROFILE_FUNCTION();

		TED_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		if (!m_Specification.WorkingDirectory.empty())
			std::filesystem::current_path(m_Specification.WorkingDirectory);

		m_Window = Window::Create(WindowProps(m_Specification.Name));
		m_Window->SetEventCallback(TED_BIND_EVENT_FN(Application::OnEvent));

		Renderer::Init();

		m_ButtonInteractionSystem.Init(m_Window->GetWidth(), m_Window->GetHeight());

		m_ImGuiLayer = CreateRef<ImGuiLayer>();
		PushOverlay(m_ImGuiLayer);
	}

	Application::~Application()
	{
		TED_PROFILE_FUNCTION();

		Renderer::Shutdown();
	}

	void Application::PushLayer(Ref<Layer> layer)
	{
		TED_PROFILE_FUNCTION();

		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Ref<Layer> layer)
	{
		TED_PROFILE_FUNCTION();

		m_LayerStack.PushOverlay(layer);
		layer->OnAttach();
	}

	void Application::OnEvent(Event& e)
	{
		TED_PROFILE_CATEGORY("Main App Events", InstrumentorCategory::Input);

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(TED_BIND_EVENT_FN(Application::OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(TED_BIND_EVENT_FN(Application::OnWindowResize));
		dispatcher.Dispatch<WindowMovedEvent>(TED_BIND_EVENT_FN(Application::OnWindowMoved));

		for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it)
		{
			if (e.Handled)
				break;
			(*it)->OnEvent(e);
		}

		m_Input.OnEvent(e);
	}

	void Application::OnUpdate()
	{
		TED_PROFILE_FUNCTION();
		TED_PROFILE_FRAME_SCOPE("MainLoop");

		float time = Time::GetTime();
		Timestep timestep = time - m_LastFrameTime;
		m_LastFrameTime = time;

		if (!m_Minimized)
		{
			m_AssetManager.OnUpdate();

			{
				TED_PROFILE_SCOPE("LayerStack OnUpdate");

				for (Ref<Layer> layer : m_LayerStack)
					layer->OnUpdate(timestep);
			}

			m_ImGuiLayer->Begin();
			{
				TED_PROFILE_SCOPE("ImGui LayerStack OnUpdate");
			
				for (Ref<Layer> layer : m_LayerStack)
					layer->OnImGuiRender();
			}
			m_ImGuiLayer->End();
		
		}

		m_Window->OnUpdate(timestep);
	}

	void Application::Run() 
	{
		TED_PROFILE_FUNCTION();

		while (m_Running)
		{
			OnUpdate();
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

		if (e.GetWidth() == 0 || e.GetHeight() == 0) 
		{
			m_Minimized = true;
			return false;
		}

		m_Window->SetWidth(e.GetWidth());
		m_Window->SetHeight(e.GetHeight());
		Renderer::OnWindowResize(m_Window->GetWidth(), m_Window->GetHeight());
		PostProcessing::Resize(m_Window->GetWidth(), m_Window->GetWidth());
		m_ButtonInteractionSystem.OnWindowResize(m_Window->GetWidth(), m_Window->GetWidth());

		//OnUpdate(); // TODO: same as bellow

		m_Minimized = false;

		return false;
	}

	bool Application::OnWindowMoved(WindowMovedEvent& e)
	{
		//OnUpdate(); ->  TODO: THIS CAUSES CRASH -> Multi viewport stops working -> change to multithreaded renderer instead of doing this
		return true;
	}
}