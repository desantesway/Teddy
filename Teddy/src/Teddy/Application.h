#pragma once

#include "Core.h"

#include "Window.h"

#include "Teddy/LayerStack.h"
#include "Events/Event.h"
#include "Events/ApplicationEvent.h"

#include "Teddy/ImGui/ImGuiLayer.h"

#include "Teddy/Renderer/Shader.h"
#include "Teddy/Renderer/Buffer.h"
#include "Teddy/Renderer/VertexArray.h"

namespace Teddy{

	class TED_API Application
	{
	public:
			Application();
			virtual ~Application() = default;

			void Run();

			void OnEvent(Event& e);

			void PushLayer(Layer* layer);
			void PushOverlay(Layer* layer);

			inline static Application& Get() { return *s_Instance; }
			inline Window& GetWindow() { return *m_Window; }

	private:
		bool OnWindowClose(WindowCloseEvent& e);

		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		LayerStack m_LayerStack;

		std::shared_ptr<Shader> m_Shader;
		std::shared_ptr<VertexArray> m_VertexArray;

		std::shared_ptr<Shader> m_BlueShader;
		std::shared_ptr<VertexArray> m_SquareVA;

		static Application* s_Instance;

	};

	// to be defined in CLIENT
	Application* CreateApplication();
}