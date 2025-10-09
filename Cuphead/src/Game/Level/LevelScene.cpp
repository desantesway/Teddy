#include "LevelScene.h"

#include <Teddy.h>

namespace Cuphead
{
	Teddy::Ref<Teddy::Scene> LevelScene::Init(unsigned int character)
	{
		m_Scene = Teddy::CreateRef<Teddy::Scene>(true);

		auto camEntt = m_Scene->CreateEntity("Main Menu Camera");
		camEntt.GetComponent<Teddy::TransformComponent>().Translation = { 0.0f, 0.0f, 9.0f };
		auto& cam = camEntt.AddComponent<Teddy::CameraComponent>();
		cam.Camera.SetProjectionType(Teddy::SceneCamera::ProjectionType::Perspective);


		auto& window = Teddy::Application::Get().GetWindow();
		m_Scene->OnViewportResize(window.GetWidth(), window.GetHeight());
		cam.Camera.GetWidthAndHeight(m_WorldWidth, m_WorldHeight);

		auto& assets = Teddy::AssetManager::Get();

		m_Background.LeftBackground = m_Scene->CreateEntity("Main Menu Background");
		auto& sprite = m_Background.LeftBackground.AddComponent<Teddy::SpriteRendererComponent>();
		sprite.IsBackground = true;
		sprite.Texture = assets.Load<Teddy::Texture2D>("assets/Textures/SpriteAtlasTexture-Dragon_Background-2048x2048-fmt12 #0710017.png", Teddy::Boolean::True);
		m_Background.LeftBackground.AddComponent<Teddy::SpriteAtlasComponent>(0, 0, 1520, 780);
		m_Background.LeftBackground.GetComponent<Teddy::TransformComponent>().Translation = glm::vec3(-14.5f, 0.0f, 0.01f);

		m_Background.RightBackground = m_Scene->CreateEntity("Main Menu Background");
		auto& rsprite = m_Background.RightBackground.AddComponent<Teddy::SpriteRendererComponent>();
		rsprite.IsBackground = true;
		rsprite.Texture = assets.Load<Teddy::Texture2D>("assets/Textures/SpriteAtlasTexture-Dragon_Background-2048x2048-fmt12 #0710017.png", Teddy::Boolean::True);
		m_Background.RightBackground.AddComponent<Teddy::SpriteAtlasComponent>(0, 0, 1520, 780);

		m_Background.RightBackground.GetComponent<Teddy::TransformComponent>().Translation = glm::vec3(0.0f, 0.0f, 0.001f);

		// Scroll background (having two entities shifted)

		return m_Scene;
	}

	void LevelScene::OnUpdate(Teddy::Timestep ts)
	{
		m_MovementSpeed = m_MovementVelocity * ts;

		auto& leftTransform = m_Background.LeftBackground.GetComponent<Teddy::TransformComponent>();
		leftTransform.Translation.x += m_MovementSpeed;

		auto& rightTransform = m_Background.RightBackground.GetComponent<Teddy::TransformComponent>();
		rightTransform.Translation.x += m_MovementSpeed;

		if (leftTransform.Translation.x >= 0.0f || rightTransform.Translation.x  >= 14.5f)
		{
			leftTransform.Translation.x = -14.5f;
			rightTransform.Translation.x =  0.0f;
		}
	}
}