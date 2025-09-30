#pragma once

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

		static bool IsKeyPressed(const KeyCode keycode);

		static bool IsMidiKeyPressed(const MidiCode keycode);
		static bool IsMidiPedalPressed();

		static bool IsMouseButtonPressed(const MouseCode button);
		static glm::vec2 GetMousePosition();
		static float GetMouseX();
		static float GetMouseY();

	private: 
		static bool m_MidiPedal;
		static std::unordered_map<int, bool> m_MidiKeysPressed;
	};
}

