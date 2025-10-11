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

		m_Registry.clear();
		m_PhysicsWorld = { 0, 0 };
	}

	template<typename... Component>
	static void CopyComponent(entt::registry& dst, entt::registry& src, const std::unordered_map<UUID, entt::entity>& enttMap)
	{
		([&]()
		{
			auto view = src.view<Component>();
			for (auto srcEntity : view)
			{
				entt::entity dstEntity = enttMap.at(src.get<IDComponent>(srcEntity).ID);
				auto& srcComponent = src.get<Component>(srcEntity);
				dst.emplace_or_replace<Component>(dstEntity, srcComponent);
			}
		}(), ...);
	}

	template<typename... Component>
	static void CopyComponent(ComponentGroup<Component...>, entt::registry& dst, entt::registry& src, const std::unordered_map<UUID, entt::entity>& enttMap)
	{
		CopyComponent<Component...>(dst, src, enttMap);
	}

	template<typename... Component>
	static void CopyComponentIfExists(Entity dst, Entity src)
	{
		([&]()
			{
				if (src.HasComponent<Component>())
					dst.AddOrReplaceComponent<Component>(src.GetComponent<Component>());
			}(), ...);
	}

	template<typename... Component>
	static void CopyComponentIfExists(ComponentGroup<Component...>, Entity dst, Entity src)
	{
		CopyComponentIfExists<Component...>(dst, src);
	}

	Entity Scene::CreateEntity(const std::string& name)
	{
		return CreateEntityWithUUID(UUID(), name);
	}

	Ref<Scene> Scene::Copy(Ref<Scene> other)
	{
		Ref<Scene> newScene = CreateRef<Scene>();
		newScene->m_ViewportWidth = other->m_ViewportWidth;
		newScene->m_ViewportHeight = other->m_ViewportHeight;

		auto& srcSceneRegistry = other->m_Registry;
		auto& dstSceneRegistry = newScene->m_Registry;
		std::unordered_map<UUID, entt::entity> enttMap;

		// Create entities in new scene
		auto idView = srcSceneRegistry.view<IDComponent>();

		for (auto e : idView)
		{
			UUID uuid = srcSceneRegistry.get<IDComponent>(e).ID;
			const auto& name = srcSceneRegistry.get<TagComponent>(e).Tag;
			Entity newEntity = newScene->CreateEntityWithUUID(uuid, name);
			enttMap[uuid] = (entt::entity)newEntity;
		}

		// Copy components (except IDComponent and TagComponent)
		CopyComponent(AllComponents{}, dstSceneRegistry, srcSceneRegistry, enttMap);

		return newScene;
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
		TED_PROFILE_CAT(InstrumentorCategory::Input);
		m_Registry.view<NativeScriptComponent>().each([&](auto entity, auto& nsc)
			{
				if (!nsc.Instance)
				{
					nsc.Instance = nsc.InstantiateScript();
					nsc.Instance->m_Entity = Entity{ entity, this };
					nsc.Instance->OnCreate();
				}
				nsc.Instance->OnEvent(event);
			});
	}

	void Scene::RefreshBody(Rigidbody2DComponent& rigidBody, BoxCollider2DComponent& boxCollider, TransformComponent& transform) // TODO: circle
	{
		if (rigidBody.RuntimeBody)
		{
			b2DestroyBody(*static_cast<b2BodyId*>(rigidBody.RuntimeBody));
			delete static_cast<b2BodyId*>(rigidBody.RuntimeBody);
			rigidBody.RuntimeBody = nullptr;
		}

		b2BodyDef bodyDef = b2DefaultBodyDef();
		bodyDef.type = Rigidbody2DTypeToBox2Body(rigidBody.Type);
		bodyDef.position = b2Vec2(transform.Translation.x, transform.Translation.y);
		bodyDef.rotation = b2MakeRot(transform.Rotation.z);
		bodyDef.motionLocks.angularZ = rigidBody.FixedRotation;

		b2BodyId bodyId = b2CreateBody(m_PhysicsWorld, &bodyDef);
		rigidBody.RuntimeBody = new b2BodyId(bodyId);

		b2ShapeDef shapeDef = b2DefaultShapeDef();
		shapeDef.density = boxCollider.Density;
		shapeDef.material.restitution = boxCollider.Restitution;
		shapeDef.material.friction = boxCollider.Friction;

		b2Polygon box;

		box = b2MakeOffsetBox(boxCollider.Size.x * transform.Scale.x,
			boxCollider.Size.y * transform.Scale.y, { boxCollider.Offset.x, boxCollider.Offset.y },
			b2MakeRot(0));

		b2ShapeId myShapeId = b2CreatePolygonShape(*static_cast<b2BodyId*>(rigidBody.RuntimeBody), &shapeDef, &box);
	}

	void Scene::SimulatePhysics(Timestep ts)
	{
		TED_PROFILE_CAT(InstrumentorCategory::Physics);

		if (m_PhysicsWorld.index1 == b2_nullWorldId.index1 && m_PhysicsWorld.generation == b2_nullWorldId.generation)
			return;

		constexpr float fixedTimeStep = 1.0f / 60.0f;
		constexpr int subStepCount = 4;
		static float accumulator = 0.0f;

		float frameDelta = ts.GetSeconds();

		accumulator += frameDelta;

		if (accumulator < fixedTimeStep)
		{
			b2World_Step(m_PhysicsWorld, accumulator, subStepCount);
		}
		while (accumulator >= fixedTimeStep)
		{
			accumulator -= fixedTimeStep;
		}

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

	void FowardAtlasAnimation(Timestep ts, SpriteAnimationComponent& animation, SpriteAtlasComponent& atlas, SpriteAnimationAtlasComponent& indicies)
	{
		if (indicies.AnimationSprites.size() == 0 || animation.PlayableIndicies.size() == 0) indicies.GenerateFrames(animation, atlas);
		if (!animation.Pause)
		{
			animation.Timer += ts;
			bool changed = true;
			while (animation.Timer > ts && changed)
			{
				changed = false;
				bool present = false;
				for (int i = 0; i < animation.PlayableIndicies.size(); i++)
					if (animation.PlayableIndicies[i] == indicies.Index)
						present = true;
				if (indicies.Index == animation.PlayableIndicies[0] || !present) {
					if (animation.Timer >= animation.InitialFrameTime) {
						if (animation.PingPong && animation.Reverse)
							animation.Reverse = false;
						if (animation.Loop)
						{
							indicies.Index = animation.Reverse ?
								animation.PlayableIndicies.back() :
								animation.PlayableIndicies.size() > 1 ? animation.PlayableIndicies[1] :
								animation.PlayableIndicies[0];
						}
						else
						{
							indicies.Index = animation.Reverse ? animation.PlayableIndicies[0]
								: animation.PlayableIndicies.size() > 1 ? animation.PlayableIndicies[1] : animation.PlayableIndicies[0];
						}
						
						animation.Timer -= animation.InitialFrameTime;
						changed = true;
					}
				}
				else if (indicies.Index == animation.PlayableIndicies.back()) {
					if (animation.Timer >= animation.FinalFrameTime) {
						if (animation.Loop)
						{
							if (animation.PingPong || animation.Reverse)
							{
								animation.Reverse = true;
								indicies.Index = animation.PlayableIndicies[animation.PlayableIndicies.size() - 2];
							}
							else
								indicies.Index = animation.PlayableIndicies[0];
						}
						animation.Timer -= animation.FinalFrameTime;
						changed = true;
					}
				}
				else if (indicies.Index <= animation.PlayableIndicies.back() && animation.Timer >= animation.FrameTime) {
					for (int i = 0; i < animation.PlayableIndicies.size(); i++)
					{
						if (indicies.Index == animation.PlayableIndicies[i])
						{
							if (animation.Reverse && i - 1 >= 0)
								indicies.Index = animation.PlayableIndicies[i - 1];
							else if (!animation.Reverse && i + 1 < animation.PlayableIndicies.size())
								indicies.Index = animation.PlayableIndicies[i + 1];
							break;
						}
					}
					animation.Timer -= animation.FrameTime;
					changed = true;
				}

				atlas.X = indicies.AnimationSprites[indicies.Index].X;
				atlas.Y = indicies.AnimationSprites[indicies.Index].Y;
				animation.TextureIndex = indicies.AnimationSprites[indicies.Index].TextureIndex;
			}
		}
	}

	void FowardAnimation(Timestep ts, SpriteAnimationComponent& animation)
	{
		if (!animation.Pause)
		{
			animation.Timer += ts;
			bool changed = true;
			while (animation.Timer > ts && changed)
			{
				changed = false;
				bool atFirstFrame;
				bool atLastFrame;
				if (animation.PlayableIndicies.size() > 0)
				{
					atFirstFrame = animation.TextureIndex <= animation.PlayableIndicies[0];
					atLastFrame = animation.TextureIndex >= animation.PlayableIndicies.back();
				}
				else
				{
					atFirstFrame = animation.TextureIndex <= 0;
					atLastFrame = animation.TextureIndex >= animation.Textures.size() - 1;
				}

				if (atFirstFrame)
				{
					if (animation.InitialFrameTime < animation.Timer)
					{
						if (animation.PingPong && animation.Reverse)
							animation.Reverse = false;
						if (animation.Reverse && animation.Loop)
						{
							animation.TextureIndex = animation.PlayableIndicies.size() > 0 ? animation.PlayableIndicies.back() : animation.Textures.size() - 1;
						}
						else
						{
							animation.TextureIndex++;
							if (animation.PlayableIndicies.size() > 0)
							{
								while (std::find(animation.PlayableIndicies.begin(), animation.PlayableIndicies.end(), animation.TextureIndex)
									== animation.PlayableIndicies.end() && animation.TextureIndex < animation.Textures.size())
									animation.TextureIndex++;
							}
						}
						changed = true;
						animation.Timer -= animation.InitialFrameTime;
					}
				}
				else if (atLastFrame)
				{
					if (animation.FinalFrameTime < animation.Timer)
					{
						if (animation.Loop)
						{
							if (animation.PingPong || animation.Reverse)
							{
								animation.Reverse = true;
								animation.TextureIndex--;
								if (animation.PlayableIndicies.size() > 0)
								{
									while (std::find(animation.PlayableIndicies.begin(), animation.PlayableIndicies.end(), animation.TextureIndex)
										== animation.PlayableIndicies.end() && animation.TextureIndex >= animation.PlayableIndicies[0])
										animation.TextureIndex--;
								}
							}
							else
								animation.TextureIndex = 0;
						}
						changed = true;
						animation.Timer -= animation.FinalFrameTime;
					}
				}
				else if (animation.TextureIndex < animation.Textures.size() - 1 && animation.FrameTime < animation.Timer)
				{
					if (animation.Reverse)
					{
						animation.TextureIndex--;
						if (animation.PlayableIndicies.size() > 0)
						{
							while (std::find(animation.PlayableIndicies.begin(), animation.PlayableIndicies.end(), animation.TextureIndex)
								== animation.PlayableIndicies.end() && animation.TextureIndex >= animation.PlayableIndicies[0])
								animation.TextureIndex--;
						}
					}
					else
					{
						animation.TextureIndex++;
						if (animation.PlayableIndicies.size() > 0)
						{
							while (std::find(animation.PlayableIndicies.begin(), animation.PlayableIndicies.end(), animation.TextureIndex)
								== animation.PlayableIndicies.end() && animation.TextureIndex < animation.Textures.size())
								animation.TextureIndex++;
						}
					}
					changed = true;
					animation.Timer -= animation.FrameTime;
				}
			}

		}
		if (animation.TextureIndex < 0 || animation.TextureIndex >= animation.Textures.size())
		{
			if(animation.PlayableIndicies.size() > 0)
				animation.TextureIndex = animation.PlayableIndicies[0];
			else
				animation.TextureIndex = 0;
		}
	}

	void Scene::AlwaysOnUpdate()
	{
		// Buttons
		auto viewButton = m_Registry.view<ButtonComponent>();
		for (auto buttonEntity : viewButton)
		{
			auto& button = viewButton.get<ButtonComponent>(buttonEntity);

			if ((int)buttonEntity == (int)ButtonInteractionSystem::GetHoveredEntity())
			{
				button.Hovered = true;
			}
			else
			{
				button.Hovered = false;
			}
		}

		auto& assets = AssetManager::Get();

		// Texture hot reloading
		auto viewSprite = m_Registry.view<SpriteRendererComponent>();
		for (auto entity : viewSprite)
		{
			auto& sprite = viewSprite.get<SpriteRendererComponent>(entity);

			if (sprite.Texture && assets.IsHotReloading<Texture2D>())
			{
				std::string texturePath = assets.AssetNeedsToReload<Texture2D>(sprite.Texture->GetPath(), true);

				if (texturePath != "")
				{
					sprite.Texture = nullptr;
					assets.RemoveExpired<Texture2D>(texturePath, Boolean::True);
					sprite.Texture = assets.Load<Texture2D>(texturePath, Boolean::True, true);
				}

			}
		}

		auto viewAnimation = m_Registry.view<SpriteAnimationComponent>();
		for (auto animationEntity : viewAnimation)
		{
			auto& spriteAnimation = viewAnimation.get<SpriteAnimationComponent>(animationEntity);

			if (spriteAnimation.Textures.size() > 0 && assets.IsHotReloading<Texture2D>())
			{
				for (auto& animation : spriteAnimation.Textures)
				{
					std::string texturePath = assets.AssetNeedsToReload<Texture2D>(animation->GetPath(), true);

					if (texturePath != "")
					{
						animation = nullptr;
						assets.RemoveExpired<Texture2D>(texturePath, Boolean::True);
						animation = assets.Load<Texture2D>(texturePath, Boolean::True, true);
					}
				}
			}
		}
	}

	void Scene::OnUpdateRuntime(Timestep ts)
	{
		TED_PROFILE_CAT(InstrumentorCategory::Scene);
		
		// Scripts
		{
			m_Registry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc)
				{
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
		TransformComponent cameraTransform;
		{
			auto view = m_Registry.view<CameraComponent>();
			for (auto entity : view) 
			{
				auto& camera = view.get<CameraComponent>(entity);
				auto& transform = m_Registry.get<TransformComponent>(entity);
				
				if (camera.Primary) 
				{
					activeCamera = &camera.Camera;
					cameraTransform = transform;
					break;
				}
			}
		}
		
		// Entities
		if (activeCamera)
		{
			Renderer2D::BeginScene(*activeCamera, cameraTransform.GetTransform());

			m_Registry.sort<TransformComponent>([](const TransformComponent& a, const TransformComponent& b) {
				return a.Translation.z < b.Translation.z;
				});

			auto view = m_Registry.view<TransformComponent>();
			for (auto& entity : view)
			{
				Entity ent{ entity, this };
				if (ent.HasComponent<SpriteRendererComponent>())
				{
					auto& sprite = ent.GetComponent<SpriteRendererComponent>();
					auto& transform = ent.GetComponent<TransformComponent>();
					if (ent.HasComponent<SpriteAtlasComponent>()) {
						auto& atlas = ent.GetComponent<SpriteAtlasComponent>();
						Renderer2D::DrawQuad(transform, sprite, *activeCamera, cameraTransform, atlas, (int)entity);
					}
					else {
						Renderer2D::DrawQuad(transform, sprite, *activeCamera, cameraTransform, (int)entity);
					}
				}
				else if (ent.HasComponent<SpriteAnimationComponent>())
				{
					auto& animation = ent.GetComponent<SpriteAnimationComponent>();
					auto& transform = ent.GetComponent<TransformComponent>();
					if (animation.Textures.size()) {
						if (ent.HasComponent<SpriteAtlasComponent>()) {
							auto& atlas = ent.GetComponent<SpriteAtlasComponent>();
							auto& indicies = ent.GetComponent<SpriteAnimationAtlasComponent>();
							FowardAtlasAnimation(ts, animation, atlas, indicies);
							Renderer2D::DrawQuad(transform, animation, *activeCamera, cameraTransform, atlas, (int)entity);
						}
						else {
							FowardAnimation(ts, animation);
							Renderer2D::DrawQuad(transform, animation, *activeCamera, cameraTransform, (int)entity);
						}
					}
				}
				else if (ent.HasComponent<CircleRendererComponent>())
				{
					auto& circle = ent.GetComponent<CircleRendererComponent>();
					auto& transform = ent.GetComponent<TransformComponent>();
					Renderer2D::DrawCircle(transform.GetTransform(),
						circle.Color, circle.Thickness, circle.Fade, (int)entity);
				}
				else if(ent.HasComponent<TextComponent>())
				{
					auto& text = ent.GetComponent<TextComponent>();
					auto& transform = ent.GetComponent<TransformComponent>();
					Renderer2D::DrawString(text, transform, (int)entity);
				}
			}

			Renderer2D::EndScene();
		}

		activeCamera = nullptr;
	}

	void Scene::OnUpdateEditor(Timestep ts, EditorCamera& camera)
	{
		TED_PROFILE_CAT(InstrumentorCategory::Scene);

		Renderer2D::BeginScene(camera);

		m_Registry.sort<TransformComponent>([](const TransformComponent& a, const TransformComponent& b) {
			return a.Translation.z < b.Translation.z;
			});

		auto view = m_Registry.view<TransformComponent>();
		for (auto& entity : view)
		{
			Entity ent{ entity, this };
			if (ent.HasComponent<SpriteRendererComponent>())
			{
				auto& sprite = ent.GetComponent<SpriteRendererComponent>();
				auto& transform = ent.GetComponent<TransformComponent>();
				if (ent.HasComponent<SpriteAtlasComponent>()) {
					auto& atlas = ent.GetComponent<SpriteAtlasComponent>();
					Renderer2D::DrawQuad(transform, sprite, atlas, (int)entity);
				}
				else {
					Renderer2D::DrawQuad(transform, sprite, (int)entity);
				}
			}
			else if (ent.HasComponent<SpriteAnimationComponent>())
			{
				auto& animation = ent.GetComponent<SpriteAnimationComponent>();
				auto& transform = ent.GetComponent<TransformComponent>();
				if (animation.Textures.size()) {
					if (ent.HasComponent<SpriteAtlasComponent>()) {
						auto& atlas = ent.GetComponent<SpriteAtlasComponent>();
						auto& indicies = ent.GetComponent<SpriteAnimationAtlasComponent>();
						FowardAtlasAnimation(ts, animation, atlas, indicies);
						Renderer2D::DrawQuad(transform, animation, atlas, (int)entity);
					}
					else {
						FowardAnimation(ts, animation);
						Renderer2D::DrawQuad(transform, animation, (int)entity);
					}
				}
			}
			else if (ent.HasComponent<CircleRendererComponent>())
			{
				auto& circle = ent.GetComponent<CircleRendererComponent>();
				auto& transform = ent.GetComponent<TransformComponent>();
				Renderer2D::DrawCircle(transform.GetTransform(),
					circle.Color, circle.Thickness, circle.Fade, (int)entity);
			}
			else if (ent.HasComponent<TextComponent>())
			{
				auto& text = ent.GetComponent<TextComponent>();
				auto& transform = ent.GetComponent<TransformComponent>();
				Renderer2D::DrawString(text, transform, (int)entity);
			}
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
		TED_PROFILE_CAT(InstrumentorCategory::Scene);

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
			bodyDef.rotation = b2MakeRot(transform.Rotation.z);
			bodyDef.motionLocks.angularZ = rb2d.FixedRotation;
			bodyDef.position = b2Vec2(transform.Translation.x, transform.Translation.y);
			b2BodyId bodyId = b2CreateBody(m_PhysicsWorld, &bodyDef);
			rb2d.RuntimeBody = new b2BodyId(bodyId);

			
			if (entity.HasComponent<BoxCollider2DComponent>())
			{
				auto& bc2d = entity.GetComponent<BoxCollider2DComponent>();
				
				b2ShapeDef shapeDef = b2DefaultShapeDef();
				shapeDef.density = bc2d.Density;
				shapeDef.material.restitution = bc2d.Restitution;
				shapeDef.material.friction = bc2d.Friction;

				b2CosSin cs = b2ComputeCosSin(transform.Rotation.z);
				b2Polygon box;

				if (entity.HasComponent<TextComponent>())
				{
					auto& text = entity.GetComponent<TextComponent>();

					box = b2MakeOffsetBox(bc2d.Size.x * transform.Scale.x * text.TextQuad.Scale.x,
						bc2d.Size.y * transform.Scale.y * text.TextQuad.Scale.y,
						{ bc2d.Offset.x + text.TextQuad.Translation.x,
						bc2d.Offset.y + text.TextQuad.Translation.y },
						b2MakeRot(0));
				}
				else
				{
					box = b2MakeOffsetBox(bc2d.Size.x * transform.Scale.x,
						bc2d.Size.y * transform.Scale.y, { bc2d.Offset.x, bc2d.Offset.y },
						b2MakeRot(0));
				}

				b2ShapeId myShapeId = b2CreatePolygonShape(*static_cast<b2BodyId*>(rb2d.RuntimeBody), &shapeDef, &box);
			}

			if (entity.HasComponent<CircleCollider2DComponent>())
			{
				auto& bc2d = entity.GetComponent<CircleCollider2DComponent>();

				b2ShapeDef shapeDef = b2DefaultShapeDef();
				shapeDef.density = bc2d.Density;
				shapeDef.material.restitution = bc2d.Restitution;
				shapeDef.material.friction = bc2d.Friction;

				b2Circle circle;
				circle.center = { bc2d.Offset.x, bc2d.Offset.y};
				float sx = std::abs(transform.Scale.x);
				float sy = std::abs(transform.Scale.y);
				circle.radius = glm::max(sx, sy) * bc2d.Radius;
				b2ShapeId myShapeId = b2CreateCircleShape(*static_cast<b2BodyId*>(rb2d.RuntimeBody), &shapeDef, &circle);
			}
		}
	}

	Entity Scene::GetPrimaryCameraEntity()
	{
		TED_PROFILE_FUNCTION();

		auto view = m_Registry.view<CameraComponent>();
		for (auto entity : view)
		{
			auto& camera = view.get<CameraComponent>(entity);
			if (camera.Primary)
				return Entity{ entity, this };
		}
		return Entity();
	}

	void Scene::OnRuntimeStop()
	{
		b2DestroyWorld(m_PhysicsWorld);
		m_PhysicsWorld = b2_nullWorldId;
	}

	void Scene::DuplicateEntity(Entity entity)
	{
		Entity newEntity = CreateEntity(entity.GetName());

		CopyComponentIfExists(AllComponents{}, newEntity, entity);
	}

	template<typename T>
	void Scene::OnComponentAdded(Entity entity, T& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<CameraComponent>(Entity entity, CameraComponent& component)
	{
		if (m_ViewportWidth > 0 && m_ViewportHeight > 0)
			component.Camera.SetViewportSize(m_ViewportWidth, m_ViewportHeight);
	}

	template<>
	void Scene::OnComponentAdded<SpriteAnimationComponent>(Entity entity, SpriteAnimationComponent& component)
	{
		if (entity.HasComponent<SpriteAtlasComponent>())
		{
			if (!entity.HasComponent<SpriteAnimationAtlasComponent>())
			{
				entity.AddComponent<SpriteAnimationAtlasComponent>().GenerateFrames(component, entity.GetComponent<SpriteAtlasComponent>());
			}
		}
	}

	template<>
	void Scene::OnComponentAdded<SpriteAtlasComponent>(Entity entity, SpriteAtlasComponent& component)
	{
		if (entity.HasComponent<SpriteAnimationComponent>())
		{
			if (!entity.HasComponent<SpriteAnimationAtlasComponent>())
			{
				entity.AddComponent<SpriteAnimationAtlasComponent>().GenerateFrames(entity.GetComponent<SpriteAnimationComponent>(), component);
			}
		}
	}
}