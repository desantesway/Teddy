#pragma once

#include "Teddy/Events/Event.h"

#include <box2d/box2d.h>

namespace Teddy
{
	class SensorEvent : public Event
	{
	public:
		b2WorldId GetWorld() const { return m_World; }
		b2ShapeId GetSensorShape() const { return m_SensorShape; }
		b2ShapeId GetVisitorShape() const { return m_VisitorShape; }

		EVENT_CLASS_CATEGORY(EventCategorySensor | EventCategoryPhysics)
	protected:
		SensorEvent(const b2WorldId& m_World, const b2ShapeId a, const b2ShapeId b)
			: m_SensorShape(a), m_VisitorShape(b), m_World(m_World)
		{
		}

		b2WorldId m_World;
		b2ShapeId m_SensorShape;
		b2ShapeId m_VisitorShape;
	};

	class SensorBeginEvent : public SensorEvent
	{
	public:
		SensorBeginEvent(const b2WorldId& world, b2SensorBeginTouchEvent* beginContact)
			: SensorEvent(world, beginContact->sensorShapeId, beginContact->visitorShapeId)
		{
		}
	
		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "SensorBeginEvent -> Sensor index: " << m_SensorShape.index1 << " Visitor index: " << m_VisitorShape.index1 << " From world index: " << m_World.index1;
			return ss.str();
		}
	
		EVENT_CLASS_TYPE(SensorBegin)
	};

	class SensorEndEvent : public SensorEvent
	{
	public:
		SensorEndEvent(const b2WorldId& world, b2SensorEndTouchEvent* endContact)
			: SensorEvent(world, endContact->sensorShapeId, endContact->visitorShapeId)
		{
		}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "SensorEndEvent -> Sensor index: " << m_SensorShape.index1 << " Visitor index: " << m_VisitorShape.index1 << " From world index: " << m_World.index1;
			return ss.str();
		}

		EVENT_CLASS_TYPE(SensorEnd)
	};
}