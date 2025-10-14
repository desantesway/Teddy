#include "CloudPlatform.h"
#include "LevelCategories.h"

namespace Cuphead
{
	void CloudPlatform::OnUpdate(Teddy::Timestep ts)
	{
		
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

	void CloudPlatform::StartCloudA(float X, float Y)
	{
		auto cloud = m_Scene->CreateEntity("Cloud Platform A#" + std::to_string(m_Clouds.size()));
		cloud.AddComponent<Teddy::SpriteAtlasComponent>(0, 0, 242, 90);
		auto& sprite = cloud.AddComponent<Teddy::SpriteAnimationComponent>(0.1f, 0.1f, 0.1f);
		sprite.Textures = m_CloudTextures;
		sprite.PlayableIndicies = { 0, 1, 2 }; // TODO: random starting point?
		sprite.Loop = true;
		sprite.PingPong = false;
		auto& transform = cloud.GetComponent<Teddy::TransformComponent>();
		transform.Translation = glm::vec3(X, Y, 1.999f);
		transform.Scale *= 0.75f;

		auto& collider = cloud.AddComponent<Teddy::BoxCollider2DComponent>();
		collider.Friction = 0.0f;
		collider.Size = { 1.0f, 0.2f };
		collider.Offset = { 0.0f, -0.2f };
		collider.EnableSensorEvents = true;
		auto& body = cloud.AddComponent<Teddy::Rigidbody2DComponent>();

		auto& filter = cloud.AddComponent<Teddy::CollisionFilter2DComponent>();
		filter.CategoryBits = LevelCategories::CLOUDPLATFORMON; // TODO: Compare if it is bellow player or not
		filter.MaskBits = LevelCategories::PLAYER;
	}

	void CloudPlatform::StartCloudB(float X, float Y)
	{
		auto cloud = m_Scene->CreateEntity("Cloud Platform B#" + std::to_string(m_Clouds.size()));
		cloud.AddComponent<Teddy::SpriteAtlasComponent>(0, 0, 242, 90);
		auto& sprite = cloud.AddComponent<Teddy::SpriteAnimationComponent>(0.1f, 0.1f, 0.1f);
		sprite.Textures = m_CloudTextures;
		sprite.PlayableIndicies = { 23, 24, 25 }; // TODO: random starting point?
		sprite.Loop = true;
		sprite.PingPong = false;
		auto& aA = cloud.GetComponent<Teddy::SpriteAnimationAtlasComponent>();
		aA.Index = 23;
		auto& transform = cloud.GetComponent<Teddy::TransformComponent>();
		transform.Translation = glm::vec3(X, Y, 1.999f);
		transform.Scale *= 0.75f;

		auto& collider = cloud.AddComponent<Teddy::BoxCollider2DComponent>();
		collider.Friction = 0.0f;
		collider.Size = { 1.0f, 0.2f };
		collider.Offset = { 0.0f, -0.2f };
		collider.EnableSensorEvents = true;
		auto& body = cloud.AddComponent<Teddy::Rigidbody2DComponent>();

		auto& filter = cloud.AddComponent<Teddy::CollisionFilter2DComponent>();
		filter.CategoryBits = LevelCategories::CLOUDPLATFORMON; // TODO: Compare if it is bellow player or not
		filter.MaskBits = LevelCategories::PLAYER;
	}

	void CloudPlatform::StartCloudC(float X, float Y)
	{
		auto cloud = m_Scene->CreateEntity("Cloud Platform C#" + std::to_string(m_Clouds.size()));
		cloud.AddComponent<Teddy::SpriteAtlasComponent>(0, 0, 242, 90);
		auto& sprite = cloud.AddComponent<Teddy::SpriteAnimationComponent>(0.1f, 0.1f, 0.1f);
		sprite.Textures = m_CloudTextures;
		sprite.PlayableIndicies = { 46, 47, 48 }; // TODO: random starting point?
		sprite.Loop = true;
		sprite.PingPong = false;
		auto& aA = cloud.GetComponent<Teddy::SpriteAnimationAtlasComponent>();
		aA.Index = 46;
		auto& transform = cloud.GetComponent<Teddy::TransformComponent>();
		transform.Translation = glm::vec3(X, Y, 1.999f);
		transform.Scale *= 0.75f;

		auto& collider = cloud.AddComponent<Teddy::BoxCollider2DComponent>();
		collider.Friction = 0.0f;
		collider.Size = { 1.0f, 0.2f };
		collider.Offset = { 0.0f, -0.2f };
		collider.EnableSensorEvents = true;
		auto& body = cloud.AddComponent<Teddy::Rigidbody2DComponent>();

		auto& filter = cloud.AddComponent<Teddy::CollisionFilter2DComponent>();
		filter.CategoryBits = LevelCategories::CLOUDPLATFORMON; // TODO: Compare if it is bellow player or not
		filter.MaskBits = LevelCategories::PLAYER;
	}
}
