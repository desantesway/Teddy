#pragma once

#include "Core.h"

#include "Window.h"

#include "Teddy/LayerStack.h"
#include "Events/Event.h"
#include "Events/ApplicationEvent.h"

#include "Teddy/ImGui/ImGuiLayer.h"

#include "Teddy/Renderer/Shader.h"
#include "Teddy/Renderer/Buffer.h"

namespace Teddy{

	class TED_API Application
	{
	public:
			Application();
			virtual ~Application();

			void Run();

			void OnEvent(Event& e);

			void PushLayer(Layer* layer);
			void PushOverlay(Layer* layer);

			inline static Application& Get() { return *s_Instance; }
			inline Window& GetWindow() { return *m_Window; }

			unsigned int m_VertexArray;
			std::unique_ptr<Shader> m_Shader;
			std::unique_ptr <VertexBuffer> m_VertexBuffer; // buffer of the vertices
			std::unique_ptr <IndexBuffer> m_IndexBuffer; // buffer that draws the pixels with the vertices
	private:
		bool OnWindowClose(WindowCloseEvent& e);

		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		LayerStack m_LayerStack;

		static Application* s_Instance;
	};

	// to be defined in CLIENT
	Application* CreateApplication();
}