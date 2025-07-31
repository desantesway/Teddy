#pragma once

#include "Teddy/Core/Input.h"
#include "Teddy/Events/MidiEvent.h"

namespace Teddy
{
	class TeddyInput : public Input
	{
		protected:
			virtual void OnEventImpl(Event& e) override;
			virtual bool OnMidiKeyPressed(MidiKeyPressedEvent& e);
			virtual bool OnMidiKeyReleased(MidiKeyReleasedEvent& e);
			virtual bool OnMidiPedalPressed(MidiPedalPressedEvent& e);
			virtual bool OnMidiPedalReleased(MidiPedalReleasedEvent& e);

			virtual bool IsKeyPressedImpl(KeyCode keycode) override;

			virtual bool IsMidiKeyPressedImpl(MidiCode keycode) override;
			virtual bool IsMidiPedalPressedImpl() override;

			virtual bool IsMouseButtonPressedImpl(MouseCode button) override;
			virtual std::pair<float, float> GetMousePositionImpl() override;
			virtual float GetMouseXImpl() override;
			virtual float GetMouseYImpl() override;
	private:
		bool m_MidiPedal = false;
		std::unordered_map<int, bool> m_MidiKeysPressed;
	};
}