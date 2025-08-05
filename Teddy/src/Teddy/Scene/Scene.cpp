#include "TeddyPch.h"
#include "Teddy/Scene/Scene.h"
#include "Teddy/Scene/Components.h"

#include "Teddy/Renderer/Renderer2D.h"

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
		TED_PROFILE_FUNCTION();
	}

	Scene::~Scene()
	{
		TED_PROFILE_FUNCTION();
	}

	void Scene::OnUpdate(Timestep ts)
	{
		TED_PROFILE_FUNCTION();

		auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
		for (auto entity : group) {
			auto tuple = group.get<TransformComponent, SpriteRendererComponent>(entity);
			auto& transform = std::get<0>(tuple);
			auto& sprite = std::get<1>(tuple);

			Renderer2D::DrawQuad(transform, sprite.Color);
		}
	}
}