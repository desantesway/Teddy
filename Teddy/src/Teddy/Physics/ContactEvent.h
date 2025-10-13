#pragma once

#include "Teddy/Events/Event.h"

#include <box2d/box2d.h>

namespace Teddy
{
	class ContactEvent : public Event
	{
	public:
		b2ContactId GetContactId() const { return m_Contact; }
		b2ShapeId GetShapeIdA() const { return m_A; }
		b2ShapeId GetShapeIdB() const { return m_B; }

		EVENT_CLASS_CATEGORY(EventCategoryContact | EventCategoryPhysics)
	protected:
		ContactEvent(const b2WorldId& m_World, const b2ContactId contact, const b2ShapeId a, const b2ShapeId b)
			: m_Contact(contact), m_A(a), m_B(b), m_World(m_World)
		{
		}

		b2WorldId m_World;
		b2ContactId m_Contact;
		b2ShapeId m_A;
		b2ShapeId m_B;
	};

	class ContactBeginEvent : public ContactEvent
	{
	public:
		ContactBeginEvent(const b2WorldId& world, b2ContactBeginTouchEvent* beginContact)
			: ContactEvent(world, beginContact->contactId, beginContact->shapeIdA, beginContact->shapeIdB)
		{
		}
	
		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "ContactBeginEvent -> ContactId index: " << m_Contact.index1 << " ShapeA index: " << m_A.index1 << " ShapeB index: " << m_B.index1 << " From world index: " << m_World.index1;
			return ss.str();
		}
	
		EVENT_CLASS_TYPE(ContactBegin)
	};

	class ContactEndEvent : public ContactEvent
	{
	public:
		ContactEndEvent(const b2WorldId& world, b2ContactEndTouchEvent* endContact)
			: ContactEvent(world, endContact->contactId, endContact->shapeIdA, endContact->shapeIdB)
		{
		}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "ContactEndEvent -> ContactId index: " << m_Contact.index1 << " ShapeA index: " << m_A.index1 << " ShapeB index: " << m_B.index1 << " From world index: " << m_World.index1;
			return ss.str();
		}

		EVENT_CLASS_TYPE(ContactEnd)
	};
}