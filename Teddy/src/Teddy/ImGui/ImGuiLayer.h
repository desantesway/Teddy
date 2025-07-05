#pragma once

#include "Teddy/Core/Layer.h"
#include "Teddy/Events/MouseEvent.h"
#include "Teddy/Events/KeyEvent.h"
#include "Teddy/Events/ApplicationEvent.h"

namespace Teddy
{
	class TED_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer() = default;
		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImGuiRender() override;

		// SDL consumes events when listened, so we cant use the default ImGui event handling.
		virtual void OnEvent(Event& event);

		void Begin();
		void End();
	private:
		bool OnMouseButtonPressedEvent(MouseButtonPressedEvent& e);
		bool OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e);
		bool OnMouseMovedEvent(MouseMovedEvent& e);
		bool OnMouseScrolledEvent(MouseScrolledEvent& e);
		
		bool OnKeyPressedEvent(KeyPressedEvent& e);
		bool OnKeyReleasedEvent(KeyReleasedEvent& e);
		bool OnKeyTypedEvent(KeyTypedEvent& e);
		bool OnWindowResizeEvent(WindowResizeEvent& e);
	};
} // namespace Teddy