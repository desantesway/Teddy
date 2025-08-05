#include "TeddyPch.h"
#include "Teddy/Scene/Scene.h"

#include <glm/glm.hpp>

namespace Teddy
{

	static void DoMath(const glm::mat4& transform) 
	{
	}

	static void OnTransformConstruct(entt::registry& registry, entt::entity entity)
	{
		auto& transform = registry.get<glm::mat4>(entity);
		DoMath(transform);
	}

	Scene::Scene()
	{

		struct TransformComponent
		{
			glm::mat4 Transform;

			TransformComponent() = default;
			TransformComponent(const TransformComponent&) = default;
			TransformComponent(const glm::mat4& transform)
				: Transform(transform) 
			{}

			operator glm::mat4&() { return Transform; }
			operator const glm::mat4& () const { return Transform; }
		};

		TED_CORE_INFO("Yuh");

		entt::entity entity = m_Registry.create();

		m_Registry.emplace<TransformComponent>(entity, glm::mat4(1.0f));

		m_Registry.on_construct<TransformComponent>().connect<&OnTransformConstruct>();

		if (m_Registry.all_of<TransformComponent>(entity))
		{
			TransformComponent& transform = m_Registry.get<TransformComponent>(entity);
			TED_CORE_INFO("Entity {0} Transform: {1}", (uint32_t)entity, transform.Transform[0][0]);
		}

		auto view = m_Registry.view<TransformComponent>();
		for (auto entity : view)
		{
			TransformComponent transform = view.get<TransformComponent>(entity);
			TED_CORE_INFO("Loop: Entity {0} Transform: {1}", (uint32_t)entity, transform.Transform[0][0]);
		}

		m_Registry.remove<TransformComponent>(entity);

		if (m_Registry.all_of<TransformComponent>(entity))
		{
			TransformComponent& transform = m_Registry.get<TransformComponent>(entity);
			TED_CORE_INFO("After removed: Entity {0} Transform: {1}", (uint32_t)entity, transform.Transform[0][0]);
		}
	}

	Scene::~Scene()
	{
		
	}
}