#pragma once

#include <entt/entt.hpp>
#include "Teddy/Core/Timestep.h"

namespace Teddy 
{
	class Entity;

	class Scene
	{
	public:
		Scene();
		~Scene(); 

		Entity CreateEntity(const std::string& name = std::string());

		void OnUpdate(Timestep ts);
	private:
		entt::registry m_Registry;

		friend class Entity;
		};
}