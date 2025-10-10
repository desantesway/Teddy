#include "Player.h"

#include <Teddy.h>

namespace Cuphead
{
	void Player::InitCuphead(Teddy::Ref<Teddy::Scene> scene)
	{
		auto& assets = Teddy::AssetManager::Get();

		m_Entity = scene->CreateEntity("Cuphead Player");
		auto& sprite = m_Entity.AddComponent<Teddy::SpriteAnimationComponent>();
		sprite.Textures = assets.LoadMultiple<Teddy::Texture2D>({
			"assets/Textures/Cuphead/Movement/Cuphead_Movement_347x192_2048x2048_0.png",
			"assets/Textures/Cuphead/Movement/Cuphead_Movement_347x192_2048x2048_1.png",
			"assets/Textures/Cuphead/Movement/Cuphead_Movement_347x192_2048x2048_2.png",
			});
		sprite.PingPong = true;
		sprite.PlayableIndicies = { 84, 85, 86, 87, 88 };
		m_Entity.AddComponent<Teddy::SpriteAtlasComponent>(0, 0, 347, 192);

		auto& transform = m_Entity.GetComponent<Teddy::TransformComponent>();

		transform.Translation = glm::vec3(0.0f, 0.0f, 2.0f);
	}

	void Player::InitMugman(Teddy::Ref<Teddy::Scene> scene)
	{
		InitCuphead(scene);
	}

	void Player::StartIntro()
	{

	}
}