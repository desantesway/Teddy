#include "CloudPlatform.h"
#include "LevelCategories.h"

namespace Cuphead
{
	void CloudPlatform::OnUpdate(Teddy::Timestep ts)
	{
		UpdateCollisionFilters();
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
		collider.Offset = { 0.0f, -0.2f };
		collider.EnableSensorEvents = true;
		collider.EnableContactEvents = true;
		auto& body = cloud.AddComponent<Teddy::Rigidbody2DComponent>();

		auto& filter = cloud.AddComponent<Teddy::CollisionFilter2DComponent>();
		filter.CategoryBits = LevelCategories::CLOUDPLATFORMON;
		filter.MaskBits = LevelCategories::PLAYER;

		m_Clouds.push_back(cloud);
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
		collider.Size = { 1.0f, 0.2f };
		collider.Offset = { 0.0f, -0.2f };
		collider.EnableSensorEvents = true;
		auto& body = cloud.AddComponent<Teddy::Rigidbody2DComponent>();

		auto& filter = cloud.AddComponent<Teddy::CollisionFilter2DComponent>();
		filter.CategoryBits = LevelCategories::CLOUDPLATFORMON;
		filter.MaskBits = LevelCategories::PLAYER;

		m_Clouds.push_back(cloud);
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
		collider.Offset = { 0.0f, -0.2f };
		collider.EnableSensorEvents = true;
		auto& body = cloud.AddComponent<Teddy::Rigidbody2DComponent>();

		auto& filter = cloud.AddComponent<Teddy::CollisionFilter2DComponent>();
		filter.CategoryBits = LevelCategories::CLOUDPLATFORMON;
		filter.MaskBits = LevelCategories::PLAYER;

		m_Clouds.push_back(cloud);
	}

	void CloudPlatform::UpdateCollisionFilters()
	{
		for (auto& cloud : m_Clouds)
		{
			auto& transform = cloud.GetComponent<Teddy::TransformComponent>();
			auto& filter = cloud.GetComponent<Teddy::CollisionFilter2DComponent>();
			auto& collider = cloud.GetComponent<Teddy::BoxCollider2DComponent>();

			if (transform.Translation.y < m_PlayerY - 0.65f)
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
		TED_CORE_INFO("begin");
	}

	void CloudPlatform::CloudContactEnd(const b2ShapeId& cloudShape)
	{
		TED_CORE_INFO("end");
	}
}
