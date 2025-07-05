#pragma once

#include "Event.h"
#include "Teddy/Core/CodeConverter.h"

namespace Teddy {

	class TED_API MidiKeyPressedEvent : public Event
	{
	public:
		MidiKeyPressedEvent(unsigned int key, unsigned int velocity)
			: m_Key(key), m_Velocity(velocity) {
		}

		inline float GetKey() const { return m_Key; }
		inline float GetVelocity() const { return m_Velocity; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MidiKeyPressedEvent: key: " << KeyToNote(m_Key) << ", velocity: " << m_Velocity;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MidiKeyPressed)
		EVENT_CLASS_CATEGORY(EventCategoryMidi | EventCategoryInput)
	private:
		unsigned int m_Key, m_Velocity;
	};

	class TED_API MidiKeyReleasedEvent : public Event
	{
	public:
		MidiKeyReleasedEvent(unsigned int key, unsigned int velocity)
			: m_Key(key), m_Velocity(velocity) {
		}

		inline float GetKey() const { return m_Key; }
		inline float GetVelocity() const { return m_Velocity; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MidiKeyReleasedEvent: key: " << KeyToNote(m_Key) << ", velocity: " << m_Velocity;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MidiKeyReleased)
		EVENT_CLASS_CATEGORY(EventCategoryMidi | EventCategoryInput)
	private:
		unsigned int m_Key, m_Velocity;
	};

	class TED_API MidiPedalPressedEvent : public Event
	{
	public:
		MidiPedalPressedEvent() {}

		EVENT_CLASS_TYPE(MidiPedalPressed)
		EVENT_CLASS_CATEGORY(EventCategoryMidi | EventCategoryInput)
	};

	class TED_API MidiPedalReleasedEvent : public Event
	{
	public:
		MidiPedalReleasedEvent() {}

		EVENT_CLASS_TYPE(MidiPedalReleased)
		EVENT_CLASS_CATEGORY(EventCategoryMidi | EventCategoryInput)
	};
}