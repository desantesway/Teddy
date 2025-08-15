#include "TeddyPch.h"
#include "Teddy/Scene/Scene.h"
#include "Teddy/Scene/Components.h"

#include "Teddy/Renderer/Renderer2D.h"
#include "Teddy/Scene/Entity.h"
#include "Teddy/Scene/ScriptableEntity.h"

#include <glm/glm.hpp>


namespace Teddy
{

	static b2BodyType Rigidbody2DTypeToBox2Body(Rigidbody2DComponent::BodyType type)
	{
		switch (type)
		{
			case Rigidbody2DComponent::BodyType::Static:		return b2_staticBody;
			case Rigidbody2DComponent::BodyType::Kinematic:		return b2_kinematicBody;
			case Rigidbody2DComponent::BodyType::Dynamic:		return b2_dynamicBody;
			default: TED_CORE_ASSERT(false, "Unknown Box2D body type!"); return b2_staticBody;
		}
	}

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
		return CreateEntityWithUUID(UUID(), name);
	}

	Entity Scene::CreateEntityWithUUID(UUID uuid, const std::string& name)
	{ 
		Entity entity{ m_Registry.create(), this };
		entity.AddComponent<IDComponent>(uuid);
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

	// TODO: Reset scene if loaded while playing
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

		// Physics
		{
			float timeStep = 1.0f / 60.0f;
			int subStepCount = 4;

			// TODO/DEBUG: Timeset probably should be fixed, not variable
			b2World_Step(m_PhysicsWorld, ts, subStepCount);

			auto view = m_Registry.view<Rigidbody2DComponent>();
			for (auto e : view)
			{
				Entity entity{ e, this };
				auto& transform = entity.GetComponent<TransformComponent>();
				auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();

				b2BodyId body = *static_cast<b2BodyId*>(rb2d.RuntimeBody);
				b2Vec2 position = b2Body_GetPosition(body);
				b2Rot rotation = b2Body_GetRotation(body);

				transform.Translation.x = position.x;
				transform.Translation.y = position.y;
				transform.Rotation.z = b2Rot_GetAngle(rotation);
			}

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

			Renderer2D::DrawQuad(transform.GetTransform(), sprite, static_cast<int>(static_cast<uint32_t>(entity)));
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

			Renderer2D::DrawQuad(transform.GetTransform(), sprite, static_cast<int>(static_cast<uint32_t>(entity)));
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

	void Scene::OnRuntimeStart()
	{
		b2WorldDef worldDef = b2DefaultWorldDef();
		worldDef.gravity = b2Vec2(0.0f, -9.81f);
		worldDef.restitutionThreshold = 0.5f;
		m_PhysicsWorld = b2CreateWorld(&worldDef);

		auto view = m_Registry.view<Rigidbody2DComponent>();
		for (auto e : view)
		{
			Entity entity{ e, this };
			auto& transform = entity.GetComponent<TransformComponent>();
			auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();

			b2BodyDef bodyDef = b2DefaultBodyDef();
			bodyDef.type = Rigidbody2DTypeToBox2Body(rb2d.Type);
			bodyDef.position = b2Vec2( transform.Translation.x, transform.Translation.y );
			bodyDef.rotation = b2MakeRot(transform.Rotation.z);
			bodyDef.motionLocks.angularZ = rb2d.FixedRotation;
			b2BodyId bodyId = b2CreateBody(m_PhysicsWorld, &bodyDef);
			rb2d.RuntimeBody = new b2BodyId(bodyId);
			
			if (entity.HasComponent<BoxCollider2DComponent>())
			{
				auto& bc2d = entity.GetComponent<BoxCollider2DComponent>();

				b2ShapeDef shapeDef = b2DefaultShapeDef();
				shapeDef.density = bc2d.Density;
				shapeDef.material.restitution = bc2d.Restitution;
				shapeDef.material.friction = bc2d.Friction;

				b2Polygon box = b2MakeBox(bc2d.Size.x * transform.Scale.x, bc2d.Size.y * transform.Scale.y);
				b2ShapeId myShapeId = b2CreatePolygonShape(*static_cast<b2BodyId*>(rb2d.RuntimeBody), &shapeDef, &box);
			}
		}
	}

	void Scene::OnRuntimeStop()
	{
	}

	template<typename T>
	void Scene::OnComponentAdded(Entity entity, T& component)
	{
		//static_assert(false);
	}

	template<>
	void Scene::OnComponentAdded<IDComponent>(Entity entity, IDComponent& component)
	{
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

	template<>
	void Scene::OnComponentAdded<Rigidbody2DComponent>(Entity entity, Rigidbody2DComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<BoxCollider2DComponent>(Entity entity, BoxCollider2DComponent& component)
	{
	}
}