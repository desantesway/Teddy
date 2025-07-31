#pragma once

#include "Event.h"
#include "Teddy/Core/CodeConverter.h"

namespace Teddy {

	class MidiKeyPressedEvent : public Event
	{
	public:
		MidiKeyPressedEvent(MidiCode key, unsigned int velocity)
			: m_Key(key), m_Velocity(velocity) {
		}

		inline MidiCode GetKey() const { return m_Key; }
		inline int GetVelocity() const { return m_Velocity; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MidiKeyPressedEvent: key: " << KeyToNote(m_Key) << ", velocity: " << m_Velocity;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MidiKeyPressed)
		EVENT_CLASS_CATEGORY(EventCategoryMidi | EventCategoryInput)
	private:
		MidiCode m_Key;
		unsigned int m_Velocity;
	};

	class MidiKeyReleasedEvent : public Event
	{
	public:
		MidiKeyReleasedEvent(MidiCode key, unsigned int velocity)
			: m_Key(key), m_Velocity(velocity) {
		}

		inline MidiCode GetKey() const { return m_Key; }
		inline int GetVelocity() const { return m_Velocity; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MidiKeyReleasedEvent: key: " << KeyToNote(m_Key) << ", velocity: " << m_Velocity;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MidiKeyReleased)
		EVENT_CLASS_CATEGORY(EventCategoryMidi | EventCategoryInput)
	private:
		MidiCode m_Key;
		unsigned int m_Velocity;
	};

	class MidiPedalPressedEvent : public Event
	{
	public:
		MidiPedalPressedEvent() {}

		EVENT_CLASS_TYPE(MidiPedalPressed)
		EVENT_CLASS_CATEGORY(EventCategoryMidi | EventCategoryInput)
	};

	class MidiPedalReleasedEvent : public Event
	{
	public:
		MidiPedalReleasedEvent() {}

		EVENT_CLASS_TYPE(MidiPedalReleased)
		EVENT_CLASS_CATEGORY(EventCategoryMidi | EventCategoryInput)
	};
}