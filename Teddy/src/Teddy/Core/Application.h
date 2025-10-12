#pragma once

#include "Teddy/Core/Window.h"
#include "Teddy/Core/LayerStack.h"
#include "Teddy/ImGui/ImGuiLayer.h"
#include "Teddy/Core/Input.h"
#include "Teddy/Core/AssetManager.h"
#include "Teddy/Core/ButtonInteraction.h"
#include "Teddy/Physics/PhysicsEvent.h"

#ifdef TED_DIST
	int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
		LPSTR lpCmdLine, int nCmdShow);
#else
	int main(int argc, char** argv);
#endif

namespace Teddy
{

	struct ApplicationCommandLineArgs
	{
		int Count = 0;
		char** Args = nullptr;

		const char* operator[](int index) const
		{
			TED_CORE_ASSERT(index < Count);
			return Args[index];

		}
	};

	struct ApplicationSpecification
	{
		std::string Name = "Teddy App";
		std::string WorkingDirectory;
		ApplicationCommandLineArgs CommandLineArgs;
	};

	class Application
	{
	public:
		Application(const ApplicationSpecification& specification);
		virtual ~Application();

		void OnEvent(Event& e);

		void PushLayer(Ref<Layer> layer);
		void PushOverlay(Ref<Layer> layer);

		Window& GetWindow() { return *m_Window; }

		void Close() { m_Running = false; }

		Ref<ImGuiLayer> GetImGuiLayer() { return m_ImGuiLayer; }

		static Application& Get() { return *s_Instance; }

		const ApplicationSpecification& GetSpecification() const { return m_Specification; }
	private:
		void Run();
		void OnUpdate();

		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);
		bool OnWindowMoved(WindowMovedEvent& e);
	private:
		ApplicationSpecification m_Specification;

		Scope<Window> m_Window;
		Ref<ImGuiLayer> m_ImGuiLayer;
		bool m_Running = true; 
		bool m_NeedsRender = false;
		bool m_Minimized = false;
		LayerStack m_LayerStack;

		float m_LastFrameTime = 0.0f;

		Input m_Input;
		AssetManager m_AssetManager;
		ButtonInteractionSystem m_ButtonInteractionSystem;
		PhysicsEvent m_PhysicsEvent;
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
	Application* CreateApplication(ApplicationCommandLineArgs args);
}