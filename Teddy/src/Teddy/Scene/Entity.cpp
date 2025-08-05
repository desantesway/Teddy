#include "TeddyPch.h"
#include "Teddy/Scene/Entity.h"

namespace Teddy
{
	Entity::Entity(entt::entity handle, Scene* scene)
		: m_EntityHandle(handle), m_Scene(scene)
	{
	}

	
}