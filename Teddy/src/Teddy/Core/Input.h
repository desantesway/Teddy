#pragma once

#include "Teddy/Core/Core.h"
#include "Teddy/Events/Event.h"

namespace Teddy {
	class Input {

	protected:
		Input() = default;
	public:
		Input(const Input&) = delete;
		Input& operator=(const Input&) = delete;

		inline static void OnEvent(Event& e) { return s_Instance->OnEventImpl(e); }

		inline static bool IsKeyPressed(int keycode) { return s_Instance->IsKeyPressedImpl(keycode); }
		inline static bool IsMidiKeyPressed(int keycode) { return s_Instance->IsMidiKeyPressedImpl(keycode); }
		inline static bool IsMidiPedalPressed() { return s_Instance->IsMidiPedalPressedImpl(); }
		inline static bool IsMouseButtonPressed(int button) { return s_Instance->IsMouseButtonPressedImpl(button); }
		inline static std::pair<float, float> GetMousePosition() { return s_Instance->GetMousePositionImpl(); }
		inline static float GetMouseX() { return s_Instance->GetMouseXImpl(); }
		inline static float GetMouseY() { return s_Instance->GetMouseYImpl(); }

	protected:
		virtual void OnEventImpl(Event& e) = 0;
		virtual bool IsKeyPressedImpl(int keycode) = 0;

		virtual bool IsMidiKeyPressedImpl(int keycode) = 0;
		virtual bool IsMidiPedalPressedImpl() = 0;

		virtual bool IsMouseButtonPressedImpl(int button) = 0;
		virtual std::pair<float, float> GetMousePositionImpl() = 0;
		virtual float GetMouseXImpl() = 0;
		virtual float GetMouseYImpl() = 0;
	private:
		static Scope<Input> s_Instance;
	};
}

