#include "CloudPlatform.h"
#include "LevelCategories.h"

namespace Cuphead
{
	void CloudPlatform::OnUpdate(Teddy::Timestep ts)
	{
		UpdateCollisionFilters();
		UpdateAnimations();
	}

	void CloudPlatform::Init(Teddy::Ref<Teddy::Scene> scene)
	{
		m_Scene = scene;

		auto& assets = Teddy::AssetManager::Get();

		m_CloudTextures = assets.LoadMultiple<Teddy::Texture2D>({
			"assets/Textures/Dragon/Platforms/Cloud_Platform_242x90_1024x1024_0.png",
			"assets/Textures/Dragon/Platforms/Cloud_Platform_242x90_1024x1024_1.png"
			});
	}

	void CloudPlatform::InitPhase1()
	{
		StartCloudA(-4.0f, -1.25f);
		StartCloudA(-3.75f, 1.6f);
		StartCloudA(0.3f, 1.25f);
		StartCloudA(1.35f, -1.5f);

		StartCloudB(-1.4f, -2.0f);
		StartCloudB(3.0f, 0.0f);

		StartCloudC(4.5f, 2.0f);
		StartCloudC(-1.75f, 0.25f);
	}

	void CloudPlatform::StartCloudA(float x, float y)
	{
		auto cloud = m_Scene->CreateEntity("Cloud Platform A#" + std::to_string(m_Clouds.size()));
		cloud.AddComponent<Teddy::SpriteAtlasComponent>(0, 0, 242, 90);
		auto& sprite = cloud.AddComponent<Teddy::SpriteAnimationComponent>(0.1f, 0.1f, 0.1f);
		sprite.Textures = m_CloudTextures;
		sprite.PlayableIndicies = { 0, 1, 2 };
		sprite.Loop = true;
		sprite.PingPong = false;
		auto& transform = cloud.GetComponent<Teddy::TransformComponent>();
		transform.Translation = glm::vec3(x, y, 1.999f);
		transform.Scale *= 0.75f;

		auto& collider = cloud.AddComponent<Teddy::BoxCollider2DComponent>();
		collider.Friction = 0.0f;
		collider.Size = { 1.0f, 0.2f };
		collider.Offset = { 0.0f, -0.075f };
		collider.EnableSensorEvents = true;
		collider.EnableContactEvents = true;
		auto& body = cloud.AddComponent<Teddy::Rigidbody2DComponent>();

		auto& filter = cloud.AddComponent<Teddy::CollisionFilter2DComponent>();
		filter.CategoryBits = LevelCategories::CLOUDPLATFORMON;
		filter.MaskBits = LevelCategories::PLAYER;

		m_Clouds.push_back({ cloud, 0 });
	}

	void CloudPlatform::StartCloudB(float x, float y)
	{
		auto cloud = m_Scene->CreateEntity("Cloud Platform B#" + std::to_string(m_Clouds.size()));
		cloud.AddComponent<Teddy::SpriteAtlasComponent>(0, 0, 242, 90);
		auto& sprite = cloud.AddComponent<Teddy::SpriteAnimationComponent>(0.1f, 0.1f, 0.1f);
		sprite.Textures = m_CloudTextures;
		sprite.PlayableIndicies = { 23, 24, 25 };
		sprite.Loop = true;
		sprite.PingPong = false;
		auto& aA = cloud.GetComponent<Teddy::SpriteAnimationAtlasComponent>();
		aA.Index = 23;
		auto& transform = cloud.GetComponent<Teddy::TransformComponent>();
		transform.Translation = glm::vec3(x, y, 1.999f);
		transform.Scale *= 0.75f;

		auto& collider = cloud.AddComponent<Teddy::BoxCollider2DComponent>();
		collider.Friction = 0.0f;
		collider.Size = { 0.75f, 0.2f };
		collider.Offset = { 0.0f, -0.1f };
		collider.EnableSensorEvents = true;
		collider.EnableContactEvents = true;
		auto& body = cloud.AddComponent<Teddy::Rigidbody2DComponent>();

		auto& filter = cloud.AddComponent<Teddy::CollisionFilter2DComponent>();
		filter.CategoryBits = LevelCategories::CLOUDPLATFORMON;
		filter.MaskBits = LevelCategories::PLAYER;

		m_Clouds.push_back({ cloud, 1 });
	}

	void CloudPlatform::StartCloudC(float x, float y)
	{
		auto cloud = m_Scene->CreateEntity("Cloud Platform C#" + std::to_string(m_Clouds.size()));
		cloud.AddComponent<Teddy::SpriteAtlasComponent>(0, 0, 242, 90);
		auto& sprite = cloud.AddComponent<Teddy::SpriteAnimationComponent>(0.1f, 0.1f, 0.1f);
		sprite.Textures = m_CloudTextures;
		sprite.PlayableIndicies = { 46, 47, 48 };
		sprite.Loop = true;
		sprite.PingPong = false;
		auto& aA = cloud.GetComponent<Teddy::SpriteAnimationAtlasComponent>();
		aA.Index = 46;
		auto& transform = cloud.GetComponent<Teddy::TransformComponent>();
		transform.Translation = glm::vec3(x, y, 1.999f);
		transform.Scale *= 0.75f;

		auto& collider = cloud.AddComponent<Teddy::BoxCollider2DComponent>();
		collider.Friction = 0.0f;
		collider.Size = { 1.0f, 0.2f };
		collider.Offset = { 0.0f, -0.075f };
		collider.EnableSensorEvents = true;
		collider.EnableContactEvents = true;
		auto& body = cloud.AddComponent<Teddy::Rigidbody2DComponent>();

		auto& filter = cloud.AddComponent<Teddy::CollisionFilter2DComponent>();
		filter.CategoryBits = LevelCategories::CLOUDPLATFORMON;
		filter.MaskBits = LevelCategories::PLAYER;

		m_Clouds.push_back({ cloud, 2 });
	}

	void CloudPlatform::UpdateCollisionFilters()
	{
		for (auto& plats : m_Clouds)
		{
			auto& cloud = plats.Entity;
			auto& transform = cloud.GetComponent<Teddy::TransformComponent>();
			auto& filter = cloud.GetComponent<Teddy::CollisionFilter2DComponent>();
			auto& collider = cloud.GetComponent<Teddy::BoxCollider2DComponent>();

			if (transform.Translation.y < m_PlayerY - 0.75f)
			{
				if (filter.CategoryBits != LevelCategories::CLOUDPLATFORMON)
				{
					filter.CategoryBits = LevelCategories::CLOUDPLATFORMON;
					filter.SetFilterCategory(collider, filter.CategoryBits);

				}
				if (filter.MaskBits != LevelCategories::PLAYER)
				{
					filter.MaskBits = LevelCategories::PLAYER;
					filter.SetFilterMask(collider, filter.MaskBits);
				}
			}
			else
			{
				if (filter.CategoryBits != LevelCategories::CLOUDPLATFORMOFF)
				{
					filter.CategoryBits = LevelCategories::CLOUDPLATFORMOFF;
					filter.SetFilterCategory(collider, filter.CategoryBits);

				}
				if (filter.MaskBits != 0)
				{
					filter.MaskBits = 0;
					filter.SetFilterMask(collider, filter.MaskBits);
				}
			}
		}
	}

	void CloudPlatform::SetPlayerY(float y)
	{
		m_PlayerY = y;
	}

	void CloudPlatform::CloudContactBegin(const b2ShapeId& cloudShape)
	{
		for (auto& cloud : m_Clouds)
		{
			auto& collider = cloud.Entity.GetComponent<Teddy::BoxCollider2DComponent>();
			if (collider.RuntimeFixture != nullptr)
			{
				auto shapeId = *static_cast<b2ShapeId*>(collider.RuntimeFixture);
				if (B2_ID_EQUALS(shapeId, cloudShape))
				{
					if (!cloud.Active)
					{
						cloud.Active = true;

						auto& sprite = cloud.Entity.GetComponent<Teddy::SpriteAnimationComponent>();
						sprite.Textures = m_CloudTextures;
						sprite.Loop = false;
						sprite.PingPong = false;
						sprite.Reverse = false;
						sprite.FrameTime = 0.025f;
						auto& aA = cloud.Entity.GetComponent<Teddy::SpriteAnimationAtlasComponent>();

						switch (cloud.Type)
						{
							case 0:
							{
								sprite.PlayableIndicies = { 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 };
								aA.Index = 3;
								break;
							}
							case 1:
							{
								sprite.PlayableIndicies = { 26, 27, 28, 29, 30, 31, 32, 33, 34, 35 };
								aA.Index = 26;
								break;
							}
							case 2:
							{
								sprite.PlayableIndicies = { 49, 50, 51, 52, 53, 54, 55, 56, 57, 58 };
								aA.Index = 49;
								break;
							}
						}

						sprite.Timer = 0.0f;
						if (!cloud.Overlay)
						{
							cloud.Overlay = m_Scene->CreateEntity("Cloud Overlay" + std::to_string(cloud.Type) + "#" + std::to_string(m_Clouds.size()));
							cloud.Overlay.AddComponent<Teddy::SpriteAtlasComponent>(0, 0, 242, 90);
							auto& spriteOverlay = cloud.Overlay.AddComponent<Teddy::SpriteAnimationComponent>(0.1f, 0.025f, 0.1f);
							spriteOverlay.Textures = m_CloudTextures;
							std::vector<int> overlayIndicies;
							for (int i: sprite.PlayableIndicies)
								overlayIndicies.push_back(i + 10);
							spriteOverlay.PlayableIndicies = overlayIndicies;
							spriteOverlay.Loop = false;
							spriteOverlay.PingPong = false;
							spriteOverlay.Reverse = false;
							auto& aAOverlay = cloud.Overlay.GetComponent<Teddy::SpriteAnimationAtlasComponent>();
							aAOverlay.Index = aA.Index + 10;
							auto& transform = cloud.Overlay.GetComponent<Teddy::TransformComponent>();
							auto& originalTransform = cloud.Entity.GetComponent<Teddy::TransformComponent>();
							transform.Translation = glm::vec3(originalTransform.Translation.x, originalTransform.Translation.y, 2.01f);
							transform.Scale *= 0.75f;
						}
						else
						{
							sprite.FrameTime = 0.025f;
							auto& spriteOverlay = cloud.Overlay.GetComponent<Teddy::SpriteAnimationComponent>();
							spriteOverlay.Reverse = false;
						}
					}
				}
			}
		}
	}

	void CloudPlatform::CloudContactEnd(const b2ShapeId& cloudShape)
	{
		for (auto& cloud : m_Clouds)
		{
			auto& collider = cloud.Entity.GetComponent<Teddy::BoxCollider2DComponent>();
			if (collider.RuntimeFixture != nullptr)
			{
				auto shapeId = *static_cast<b2ShapeId*>(collider.RuntimeFixture);
				if (B2_ID_EQUALS(shapeId, cloudShape))
				{
					if (cloud.Active)
					{
						cloud.Active = false;

						auto& sprite = cloud.Entity.GetComponent<Teddy::SpriteAnimationComponent>();
						sprite.Textures = m_CloudTextures;
						sprite.Loop = false;
						sprite.PingPong = false;
						sprite.Reverse = true;
						auto& aA = cloud.Entity.GetComponent<Teddy::SpriteAnimationAtlasComponent>();

						if (cloud.Overlay)
						{
							sprite.Timer = 0.0f;
							auto& spriteOverlay = cloud.Overlay.GetComponent<Teddy::SpriteAnimationComponent>();
							spriteOverlay.Textures = m_CloudTextures;
							spriteOverlay.Loop = false;
							spriteOverlay.PingPong = false;
							spriteOverlay.Reverse = true;
						}

						if (sprite.PlayableIndicies.size() < 4)
						{
							sprite.FrameTime = 0.05f;
							switch (cloud.Type)
							{
							case 0:
							{
								sprite.PlayableIndicies = { 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 };
								break;
							}
							case 1:
							{
								sprite.PlayableIndicies = { 26, 27, 28, 29, 30, 31, 32, 33, 34, 35 };
								break;
							}
							case 2:
							{
								sprite.PlayableIndicies = { 49, 50, 51, 52, 53, 54, 55, 56, 57, 58 };
								break;
							}
							}

							if (cloud.Overlay)
							{
								auto& spriteOverlay = cloud.Overlay.GetComponent<Teddy::SpriteAnimationComponent>();
								spriteOverlay.Textures = m_CloudTextures;
								spriteOverlay.FrameTime = 0.05f;

								auto& aAOverlay = cloud.Overlay.GetComponent<Teddy::SpriteAnimationAtlasComponent>();
								std::vector<int> overlayIndicies;
								for (int i : sprite.PlayableIndicies)
									overlayIndicies.push_back(i + 10);
								spriteOverlay.PlayableIndicies = overlayIndicies;
								aAOverlay.Index = aA.Index + 10;
							}
						}

						
					}
				}
			}
		}
	}

	void CloudPlatform::UpdateAnimations()
	{
		for (auto& cloud : m_Clouds)
		{
			auto& sprite = cloud.Entity.GetComponent<Teddy::SpriteAnimationComponent>();
			auto& aA = cloud.Entity.GetComponent<Teddy::SpriteAnimationAtlasComponent>();
			
			if (sprite.PlayableIndicies.size() > 3)
			{
				if (cloud.Active)
				{
					int maxIndex = (cloud.Type == 0) ? 10 : (cloud.Type == 1) ? 33 : 56;
					if (aA.Index >= maxIndex)
					{
						sprite.Loop = true;
						sprite.PingPong = false;
						sprite.Reverse = false;
						sprite.FrameTime = 0.1f;

						switch (cloud.Type)
						{
						case 0:
						{
							sprite.PlayableIndicies = { 10, 11, 12 };
							break;
						}
						case 1:
						{
							sprite.PlayableIndicies = { 33, 34, 35 };
							break;
						}
						case 2:
						{
							sprite.PlayableIndicies = { 56, 57, 58 };
							break;
						}
						}

						if (cloud.Overlay)
						{
							sprite.Timer = 0.0f;
							auto& spriteOverlay = cloud.Overlay.GetComponent<Teddy::SpriteAnimationComponent>();
							spriteOverlay.Textures = m_CloudTextures;
							spriteOverlay.FrameTime = 0.1f;
							spriteOverlay.InitialFrameTime = 0.1f;
							spriteOverlay.FinalFrameTime = 0.1f;
							spriteOverlay.Loop = true;
							spriteOverlay.PingPong = false;
							spriteOverlay.Reverse = false;

							std::vector<int> overlayIndicies;
							for (int i : sprite.PlayableIndicies)
								overlayIndicies.push_back(i + 10);
							spriteOverlay.PlayableIndicies = overlayIndicies;
							auto& aAOverlay = cloud.Overlay.GetComponent<Teddy::SpriteAnimationAtlasComponent>();
							aAOverlay.Index = aA.Index + 10;
						}
					}
				}
				else
				{
					int minIndex = (cloud.Type == 0) ? 3 : (cloud.Type == 1) ? 26 : 49;
					if (aA.Index <= minIndex && sprite.PlayableIndicies.size() > 3)
					{
						sprite.Loop = true;
						sprite.PingPong = false;
						sprite.Reverse = false;
						sprite.FrameTime = 0.1f;
						switch (cloud.Type)
						{
						case 0:
						{
							sprite.PlayableIndicies = { 0, 1, 2 };
							aA.Index = 0;
							break;
						}
						case 1:
						{
							sprite.PlayableIndicies = { 23, 24, 25 };
							aA.Index = 23;
							break;
						}
						case 2:
						{
							sprite.PlayableIndicies = { 46, 47, 48 };
							aA.Index = 46;
							break;
						}
						}

						if (cloud.Overlay)
						{
							m_Scene->DestroyEntity(cloud.Overlay);
							cloud.Overlay = {};
						}
					}
				}
			}
		}
	}
}
