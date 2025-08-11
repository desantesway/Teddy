#pragma once

#include "Teddy/Core/Layer.h"
#include "Teddy/Events/MouseEvent.h"
#include "Teddy/Events/KeyEvent.h"
#include "Teddy/Events/ApplicationEvent.h"

namespace Teddy
{
	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer() = default;
		virtual void OnAttach() override;
		virtual void OnDetach() override;

		// SDL consumes events when listened, so we cant use the default ImGui event handling.
		virtual void OnEvent(Event& event) override;

		void Begin();
		void End();

		void SetBlockEvents(bool block) { m_BlockEvents = block; }

		void SetDarkThemeColors();
	private:
		bool OnMouseButtonPressedEvent(MouseButtonPressedEvent& e);
		bool OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e);
		bool OnMouseMovedEvent(MouseMovedEvent& e);
		bool OnMouseScrolledEvent(MouseScrolledEvent& e);
		
		bool OnKeyPressedEvent(KeyPressedEvent& e);
		bool OnKeyReleasedEvent(KeyReleasedEvent& e);
		bool OnKeyTypedEvent(KeyTypedEvent& e);
		bool OnWindowResizeEvent(WindowResizeEvent& e);
	private:
		bool m_BlockEvents = false;
	};
} // namespace Teddy