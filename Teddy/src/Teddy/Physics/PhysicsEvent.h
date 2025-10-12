#pragma once

#include "Teddy/Events/Event.h"

#include <box2d/box2d.h>

namespace Teddy
{
	class PhysicsEvent
	{
	public:
		PhysicsEvent();
		~PhysicsEvent() = default;

		using EventCallbackFn = std::function<void(Event&)>;

		void SetEventCallback(const EventCallbackFn& callback);

		static void OnUpdate(const b2WorldId& world);

	private:
		static EventCallbackFn m_EventCallback;

		static PhysicsEvent* s_Instance;
	};
}