#pragma once

#include "Teddy/Core/Base.h"
#include "Teddy/Events/MidiEvent.h"

#include "Teddy/Core/MouseCodes.h"
#include "Teddy/Core/KeyCodes.h"
#include "Teddy/Core/MidiCodes.h"

namespace Teddy 
{
	class Input
	{
	public:
		void OnEvent(Event& e);
		bool OnMidiKeyPressed(MidiKeyPressedEvent& e);
		bool OnMidiKeyReleased(MidiKeyReleasedEvent& e);
		bool OnMidiPedalPressed(MidiPedalPressedEvent& e);
		bool OnMidiPedalReleased(MidiPedalReleasedEvent& e);

		static bool IsKeyPressed(KeyCode keycode);

		static bool IsMidiKeyPressed(MidiCode keycode);
		static bool IsMidiPedalPressed();

		static bool IsMouseButtonPressed(MouseCode button);
		static std::pair<float, float> GetMousePosition();
		static float GetMouseX();
		static float GetMouseY();
	};
}

