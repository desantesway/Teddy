#pragma once

#include "Teddy/Core/Timestep.h"
#include "Teddy/Core/UUID.h"
#include "Teddy/Renderer/EditorCamera.h"
#include "Teddy/Events/Event.h"

#include <entt/entt.hpp>

#include <box2d/box2d.h>

namespace Teddy 
{
	class Entity;

	class Scene
	{
	public:
		Scene();
		~Scene(); 

		static Ref<Scene> Copy(Ref<Scene> other);

		Entity CreateEntity(const std::string& name = std::string());
		Entity CreateEntityWithUUID(UUID uuid, const std::string& name = std::string());
		void DestroyEntity(Entity entity);

		void OnRuntimeStart();
		void OnRuntimeStop();

		void OnUpdateRuntime(Timestep ts);
		void OnUpdateEditor(Timestep ts, EditorCamera& camera);

		void OnEvent(Event& event);
		void OnViewportResize(uint32_t width, uint32_t height);

		void DuplicateEntity(Entity entity);
	private:
		template<typename T>
		void OnComponentAdded(Entity entity, T& component);
	private:
		entt::registry m_Registry;
		uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;

		b2WorldId m_PhysicsWorld;

		friend class Entity;
		friend class SceneSerializer;
		friend class SceneHierarchyPanel;
		};
}