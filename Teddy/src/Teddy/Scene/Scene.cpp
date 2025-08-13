#include "TeddyPch.h"
#include "Teddy/Scene/Scene.h"
#include "Teddy/Scene/Components.h"

#include "Teddy/Renderer/Renderer2D.h"

#include <glm/glm.hpp>

#include "Teddy/Scene/Entity.h"

namespace Teddy
{

	Scene::Scene()
	{
		TED_PROFILE_FUNCTION();
	}

	Scene::~Scene()
	{
		TED_PROFILE_FUNCTION();
	}

	Entity Scene::CreateEntity(const std::string& name)
	{ 
		Entity entity{ m_Registry.create(), this };
		entity.AddComponent<TransformComponent>();
		
		auto& tag = entity.AddComponent<TagComponent>();
		tag.Tag = name.empty() ? "Entity" : name;

		return entity;
	}

	void Scene::DestroyEntity(Entity entity)
	{
		m_Registry.destroy(entity);
	}

	void Scene::OnEvent(Event& event)
	{
		m_Registry.view<NativeScriptComponent>().each([&](auto entity, auto& nsc)
			{
				// TODO: Move to Scene::OnScenePlay
				if (!nsc.Instance)
				{
					nsc.Instance = nsc.InstantiateScript();
					nsc.Instance->m_Entity = Entity{ entity, this };
					nsc.Instance->OnCreate();
				}
				nsc.Instance->OnEvent(event);
			});
	}

	void Scene::OnUpdateRuntime(Timestep ts)
	{
		TED_PROFILE_FUNCTION();
		
		// Scripts
		{
			m_Registry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc)
				{
					// TODO: Move to Scene::OnScenePlay
					if (!nsc.Instance)
					{
						nsc.Instance = nsc.InstantiateScript();
						nsc.Instance->m_Entity = Entity{ entity, this };
						nsc.Instance->OnCreate();
					}
					nsc.Instance->OnUpdate(ts);
				});
		}

		// Render
		Camera* activeCamera = nullptr;
		glm::mat4 cameraTransform;
		{
			auto view = m_Registry.view<CameraComponent>();
			for (auto entity : view) 
			{
				auto& camera = view.get<CameraComponent>(entity);
				auto& transform = m_Registry.get<TransformComponent>(entity);
				
				if (camera.Primary) 
				{
					activeCamera = &camera.Camera;
					cameraTransform = transform.GetTransform();
					break;
				}
			}
		}

		static int errorDelay = 499;
		errorDelay++;
		if (!activeCamera) 
		{
			if (errorDelay == 500)
			{
				TED_CORE_ERROR("No active camera found in the scene!");
				errorDelay = 0;
			}
			
			return;
		}
		
		// Entities
		Renderer2D::BeginScene(*activeCamera, cameraTransform);

		auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
		for (auto entity : group) 
		{
			auto tuple = group.get<TransformComponent, SpriteRendererComponent>(entity);
			auto& transform = std::get<0>(tuple);
			auto& sprite = std::get<1>(tuple);

			Renderer2D::DrawQuad(transform.GetTransform(), {.Color = sprite.Color});
		}

		Renderer2D::EndScene();
	}

	void Scene::OnUpdateEditor(Timestep ts, EditorCamera& camera)
	{
		Renderer2D::BeginScene(camera);

		auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);

		for (auto entity : group)
		{
			auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

			Renderer2D::DrawQuad(transform.GetTransform(), { .Color = sprite.Color });
		}

		Renderer2D::EndScene();
	}

	void Scene::OnViewportResize(uint32_t width, uint32_t height)
	{
		TED_PROFILE_FUNCTION();

		m_ViewportWidth = width;
		m_ViewportHeight = height;

		auto view = m_Registry.view<CameraComponent>();
		for (auto entity : view) 
		{
			auto& camera = view.get<CameraComponent>(entity);
			if (!camera.FixedAspectRatio)
			{
				camera.Camera.SetViewportSize(width, height);
			}
		}
	
	}

	template<typename T>
	void Scene::OnComponentAdded(Entity entity, T& component)
	{
		static_assert(false);
	}

	template<>
	void Scene::OnComponentAdded<TransformComponent>(Entity entity, TransformComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<CameraComponent>(Entity entity, CameraComponent& component)
	{
		if (m_ViewportWidth > 0 && m_ViewportHeight > 0)
			component.Camera.SetViewportSize(m_ViewportWidth, m_ViewportHeight);
	}

	template<>
	void Scene::OnComponentAdded<SpriteRendererComponent>(Entity entity, SpriteRendererComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<TagComponent>(Entity entity, TagComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<NativeScriptComponent>(Entity entity, NativeScriptComponent& component)
	{
	}
}