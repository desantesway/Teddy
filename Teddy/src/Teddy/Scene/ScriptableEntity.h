#pragma once

#include "Teddy/Scene/Entity.h"
#include "Teddy/Events/Event.h"

namespace Teddy {

	class ScriptableEntity
	{
	public:
		template<typename T>
		T& GetComponent()
		{
			return m_Entity.GetComponent<T>();
		}

	protected:
		virtual void OnCreate() {}
		virtual void OnDestroy() {}
		virtual void OnUpdate(Timestep ts) {}
		virtual void OnEvent(Event& event) {}
	private:
		Entity m_Entity;
		friend class Scene;
	};
}