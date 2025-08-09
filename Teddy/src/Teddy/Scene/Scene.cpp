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

	void Scene::OnUpdate(Timestep ts)
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
		glm::mat4* cameraTransform = nullptr;
		{
			auto view = m_Registry.view<CameraComponent>();
			for (auto entity : view) 
			{
				auto& camera = view.get<CameraComponent>(entity);
				auto& transform = m_Registry.get<TransformComponent>(entity);
				
				if (camera.Primary) 
				{
					activeCamera = &camera.Camera;
					cameraTransform = &transform.Transform;
					break;
				}
			}
		}

		if (!activeCamera) 
		{
			TED_CORE_ERROR("No active camera found in the scene!");
			return;
		}
		
		// Entities
		Renderer2D::BeginScene(*activeCamera, *cameraTransform);

		auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
		for (auto entity : group) 
		{
			auto tuple = group.get<TransformComponent, SpriteRendererComponent>(entity);
			auto& transform = std::get<0>(tuple);
			auto& sprite = std::get<1>(tuple);

			Renderer2D::DrawQuad(transform, { .Color = sprite.Color });
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
}