#pragma once

#include "Teddy/Core/Base.h"
#include "Teddy/Events/Event.h"

#include "Teddy/Core/MouseCodes.h"
#include "Teddy/Core/KeyCodes.h"
#include "Teddy/Core/MidiCodes.h"

namespace Teddy 
{
	class Input 
	{

	protected:
		Input() = default;
	public:
		Input(const Input&) = delete;
		Input& operator=(const Input&) = delete;

		static void OnEvent(Event& e) { return s_Instance->OnEventImpl(e); }

		static bool IsKeyPressed(KeyCode keycode) { return s_Instance->IsKeyPressedImpl(keycode); }
		static bool IsMidiKeyPressed(MidiCode keycode) { return s_Instance->IsMidiKeyPressedImpl(keycode); }
		static bool IsMidiPedalPressed() { return s_Instance->IsMidiPedalPressedImpl(); }
		static bool IsMouseButtonPressed(MouseCode button) { return s_Instance->IsMouseButtonPressedImpl(button); }
		static std::pair<float, float> GetMousePosition() { return s_Instance->GetMousePositionImpl(); }
		static float GetMouseX() { return s_Instance->GetMouseXImpl(); }
		static float GetMouseY() { return s_Instance->GetMouseYImpl(); }

		static Scope<Input> Create();
	protected:
		virtual void OnEventImpl(Event& e) = 0;
		virtual bool IsKeyPressedImpl(KeyCode keycode) = 0;

		virtual bool IsMidiKeyPressedImpl(MidiCode keycode) = 0;
		virtual bool IsMidiPedalPressedImpl() = 0;

		virtual bool IsMouseButtonPressedImpl(MouseCode button) = 0;
		virtual std::pair<float, float> GetMousePositionImpl() = 0;
		virtual float GetMouseXImpl() = 0;
		virtual float GetMouseYImpl() = 0;
	private:
		static Scope<Input> s_Instance;
	};
}

