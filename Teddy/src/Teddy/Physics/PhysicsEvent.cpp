#include "Teddy/Physics/PhysicsEvent.h"
#include "Teddy/Physics/ContactEvent.h"
#include "Teddy/Physics/SensorEvent.h"

namespace Teddy
{
	PhysicsEvent* PhysicsEvent::s_Instance = nullptr;
	PhysicsEvent::EventCallbackFn PhysicsEvent::m_EventCallback;

	PhysicsEvent::PhysicsEvent()
	{
		TED_PROFILE_FUNCTION();
		TED_CORE_ASSERT(!s_Instance, "PhysicsEvent already exists!");
		s_Instance = this;
	}

	void PhysicsEvent::SetEventCallback(const EventCallbackFn& callback)
	{
		m_EventCallback = callback;
	}

	void PhysicsEvent::OnUpdate(const b2WorldId& world)
	{
		b2ContactEvents contactEvents = b2World_GetContactEvents(world);

		// contacts
		for (int i = 0; i < contactEvents.endCount; ++i)
		{
			b2ContactEndTouchEvent* endEvent = contactEvents.endEvents + i;

			if (b2Shape_IsValid(endEvent->shapeIdA) && b2Shape_IsValid(endEvent->shapeIdB))
			{
				ContactEndEvent e(world, endEvent);
				m_EventCallback(e);
			}
		}

		for (int i = 0; i < contactEvents.beginCount; ++i)
		{
			b2ContactBeginTouchEvent* beginEvent = contactEvents.beginEvents + i;

			ContactBeginEvent e(world, beginEvent);
			m_EventCallback(e);
		}

		// sensors
		b2SensorEvents sensorEvents = b2World_GetSensorEvents(world);
		for (int i = 0; i < sensorEvents.beginCount; ++i)
		{
			b2SensorBeginTouchEvent* beginTouch = sensorEvents.beginEvents + i;
			SensorBeginEvent e(world, beginTouch);
			m_EventCallback(e);
		}

		for (int i = 0; i < sensorEvents.endCount; ++i)
		{
			b2SensorEndTouchEvent* endTouch = sensorEvents.endEvents + i;
			SensorEndEvent e(world, endTouch);
			m_EventCallback(e);
		}
	}
}