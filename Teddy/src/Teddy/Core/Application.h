#pragma once

#include "Teddy/Core/Base.h"

#include "Teddy/Core/Window.h"

#include "Teddy/Core/LayerStack.h"
#include "Teddy/Events/Event.h"
#include "Teddy/Events/ApplicationEvent.h"

#include "Teddy/Core/Timestep.h"

#include "Teddy/ImGui/ImGuiLayer.h"

#include "Teddy/Core/Input.h"

#ifdef TED_DIST
	int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
		LPSTR lpCmdLine, int nCmdShow);
#else
	int main(int argc, char** argv);
#endif

namespace Teddy
{

	class Application
	{
	public:
		Application(const std::string& name = "Teddy App");
		virtual ~Application();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		Window& GetWindow() { return *m_Window; }

		void Close() { m_Running = false; }

		ImGuiLayer* GetImGuiLayer() { return m_ImGuiLayer; }

		static Application& Get() { return *s_Instance; }

	private:
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);
	private:
		void OnUpdate();
		void Run();

		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true; 
		bool m_Minimized = false;
		LayerStack m_LayerStack;

		float m_LastFrameTime = 0.0f;

		Input m_Input;
	private:
		static Application* s_Instance;
		
		#ifdef TED_DIST
			friend int WINAPI ::WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
				LPSTR lpCmdLine, int nCmdShow);
		#else
			friend int ::main(int argc, char** argv);
		#endif
	};

	// to be defined in CLIENT
	Application* CreateApplication();
}