#pragma once

#include "Teddy/Core/Timestep.h"
#include "Teddy/Core/UUID.h"
#include "Teddy/Renderer/EditorCamera.h"
#include "Teddy/Events/Event.h"
#include "Teddy/Scene/Components.h"
#include "Teddy/Physics/PhysicsEvent.h"

#include <entt/entt.hpp>

#include <box2d/box2d.h>

namespace Teddy 
{
	class Entity;

	class Scene
	{
	public:
		Scene(bool isRuntime) { OnRuntimeStart(); }
		Scene();
		~Scene(); 

		Entity GetPrimaryCameraEntity();

		static Ref<Scene> Copy(Ref<Scene> other);

		Entity CreateEntity(const std::string& name = std::string());
		Entity CreateEntityWithUUID(UUID uuid, const std::string& name = std::string());
		void DestroyEntity(Entity entity);

		void OnRuntimeStart();
		void OnRuntimeStop();

		void AlwaysOnUpdate();
		void OnUpdateRuntime(Timestep ts);
		void OnUpdateEditor(Timestep ts, EditorCamera& camera);

		void OnEvent(Event& event);
		void OnViewportResize(uint32_t width, uint32_t height);

		void SimulatePhysics(Timestep ts);
		void DuplicateEntity(Entity entity);

		void RefreshBody(Rigidbody2DComponent& rigidBody, BoxCollider2DComponent& boxCollider, TransformComponent& transform);
		b2WorldId GetPhysicsWorld() const { return m_PhysicsWorld; }
		void SetPhysicsWorld(b2WorldId world) { m_PhysicsWorld = world; }

		template<typename... Components>
		auto GetAllEntitiesWith()
		{
			return m_Registry.view<Components...>();
		}
		entt::registry m_Registry;
	private:
		template<typename T>
		void OnComponentAdded(Entity entity, T& component);
	private:
		uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;

		b2WorldId m_PhysicsWorld = {0,0};

		friend class Entity;
		friend class SceneSerializer;
		friend class SceneHierarchyPanel;
		};
}