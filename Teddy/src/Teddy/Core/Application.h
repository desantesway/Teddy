#pragma once

#include "Teddy/Core/Core.h"

#include "Teddy/Core/Window.h"

#include "Teddy/Core/LayerStack.h"
#include "Teddy/Events/Event.h"
#include "Teddy/Events/ApplicationEvent.h"

#include "Teddy/Core/Timestep.h"

#include "Teddy/ImGui/ImGuiLayer.h"

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
			Application();
			virtual ~Application();

			void OnEvent(Event& e);

			void PushLayer(Layer* layer);
			void PushOverlay(Layer* layer);

			static Application& Get() { return *s_Instance; }
			Window& GetWindow() { return *m_Window; }

	private:
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);
	private:
		void Run();

		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true; 
		bool m_Minimized = false;
		LayerStack m_LayerStack;

		float m_LastFrameTime = 0.0f;
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