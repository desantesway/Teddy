#include "Player.h"

#include <Teddy.h>

namespace Cuphead
{
	void Player::OnUpdate(Teddy::Timestep ts)
	{
		DeleteCookie(ts);
		switch (m_State)
		{
		case PlayerState::Intro0:
			if (m_Entity.GetComponent<Teddy::SpriteAnimationAtlasComponent>().Index == 27)
				StartIdle();
			break;
		case PlayerState::Intro1:
			if (m_Entity.GetComponent<Teddy::SpriteAnimationAtlasComponent>().Index == 73)
				StartIdle();
			break;
		case PlayerState::Intro2:
			if (m_Entity.GetComponent<Teddy::SpriteAnimationAtlasComponent>().Index == 106)
				StartIdle();
			if (m_Entity.GetComponent<Teddy::SpriteAnimationAtlasComponent>().Index == 95)
				BreakCookie();
			break;
		case PlayerState::Running:
			if (m_DirectionRight)
			{
				if (Teddy::Input::IsKeyPressed(Teddy::Key::D) || Teddy::Input::IsKeyPressed(Teddy::Key::Right))
					m_Entity.GetComponent<Teddy::Rigidbody2DComponent>().ApplyForce(1.0f, 0.0f, false);
				else
					StartIdle();
			}
			else
			{
				if (Teddy::Input::IsKeyPressed(Teddy::Key::A) || Teddy::Input::IsKeyPressed(Teddy::Key::Left))
					m_Entity.GetComponent<Teddy::Rigidbody2DComponent>().ApplyForce(-1.0f, 0.0f, false);
				else
					StartIdle();
			}
			break;
		default:
			break;
		}
	}

	void Player::LoadCupheadTextures()
	{
		auto& assets = Teddy::AssetManager::Get();

		m_MovementTextures = assets.LoadMultiple<Teddy::Texture2D>({
			"assets/Textures/Cuphead/Movement/Cuphead_Movement_347x192_2048x2048_0.png",
			"assets/Textures/Cuphead/Movement/Cuphead_Movement_347x192_2048x2048_1.png",
			"assets/Textures/Cuphead/Movement/Cuphead_Movement_347x192_2048x2048_2.png"
			});

		m_IntroTextures = assets.LoadMultiple<Teddy::Texture2D>({
			"assets/Textures/Cuphead/Intros/Cuphead_Intros_212x270_2048x2048_0.png",
			"assets/Textures/Cuphead/Intros/Cuphead_Intros_212x270_2048x2048_1.png"
			});
	}

	void Player::InitCuphead(Teddy::Ref<Teddy::Scene> scene)
	{
		m_Scene = scene;
		m_Entity = m_Scene->CreateEntity("Cuphead Player");

		LoadCupheadTextures();

		InitPlayer();
	}

	void Player::InitMugman(Teddy::Ref<Teddy::Scene> scene)
	{
		m_Scene = scene;
		m_Entity = m_Scene->CreateEntity("Mugman Player");

		LoadCupheadTextures();

		InitPlayer();
	}

	void Player::InitPlayer()
	{
		auto& sprite = m_Entity.AddComponent<Teddy::SpriteAnimationComponent>(0.05f, 0.05f, 0.05f);
		sprite.Textures = m_MovementTextures;
		m_Entity.AddComponent<Teddy::SpriteAtlasComponent>(0, 0, 347, 192);
		auto& transform = m_Entity.GetComponent<Teddy::TransformComponent>();
		transform.Translation = glm::vec3(0.0f, 0.0f, 2.0f);

		sprite.PlayableIndicies = { 84, 85, 86, 87, 88 };
		sprite.PingPong = true;
		sprite.Loop = true;
		m_Entity.GetComponent<Teddy::SpriteAnimationAtlasComponent>().Index = 84;

		// Physics
		auto& body = m_Entity.AddComponent<Teddy::Rigidbody2DComponent>();
		body.Type = Teddy::Rigidbody2DComponent::BodyType::Dynamic;
		body.FixedRotation = true;
		auto& box = m_Entity.AddComponent<Teddy::BoxCollider2DComponent>();
		box.Offset = {0.0f, -0.05f};
		box.Size = {0.15f, 0.4f};
	}

	void Player::StartIdle()
	{
		if (!m_Entity.HasComponent<Teddy::SpriteAnimationAtlasComponent>())
			return;

		auto& sprite = m_Entity.GetComponent<Teddy::SpriteAnimationComponent>();
		sprite.Textures = m_MovementTextures;
		sprite.PingPong = true;
		sprite.Loop = true;
		sprite.PlayableIndicies = { 84, 85, 86, 87, 88 };

		auto& atlas = m_Entity.GetComponent<Teddy::SpriteAtlasComponent>();
		atlas.SpriteWidth = 347;
		atlas.SpriteHeight = 192;

		auto& indicies = m_Entity.GetComponent<Teddy::SpriteAnimationAtlasComponent>();
		indicies.GenerateFrames(sprite, atlas);
		indicies.Index = 84;

		auto& transform = m_Entity.GetComponent<Teddy::TransformComponent>();
		
		auto& boxCollider = m_Entity.GetComponent<Teddy::BoxCollider2DComponent>();
		boxCollider.Offset = { 0.0f, -0.05f };
		boxCollider.Size = { 0.15f, 0.4f };

		auto& boxBody = m_Entity.GetComponent<Teddy::Rigidbody2DComponent>();
		if (m_State == PlayerState::Intro1 || m_State == PlayerState::Intro2)
		{
			transform.Translation -= glm::vec3(0.0f, 0.15f, 0.0f);
			indicies.Index = 88;
			transform.Scale = glm::vec3(1.0f);
			m_Scene->RefreshBody(boxBody, boxCollider, transform);
		}
		else if (m_State == PlayerState::Intro0)
		{
			transform.Translation -= glm::vec3(0.0f, 0.2f, 0.0f);
			indicies.Index = 87;
			transform.Scale = glm::vec3(1.0f);
			m_Scene->RefreshBody(boxBody, boxCollider, transform);
		}

		m_State = PlayerState::Idle;
	}

	void Player::DeleteCookie(Teddy::Timestep ts)
	{
		static bool fadeOut = false;
		static bool done = false;
		if (!done && m_Cookie)
		{
			auto& color = m_Cookie.GetComponent<Teddy::SpriteAnimationComponent>().Color;
			if (m_Cookie.GetComponent<Teddy::SpriteAnimationAtlasComponent>().Index == 113)
			{
				fadeOut = true;
			}
			if (color.a > 0 && fadeOut)
			{
				color -= ts.GetSeconds();
			}
			if (color.a <= 0)
			{
				fadeOut = false;
				done = true;
				m_Scene->DestroyEntity(m_Cookie);
				m_Cookie = {};
			}
		}
	}

	void Player::BreakCookie()
	{
		static bool created = false;
		if (created)
			return;
		m_Cookie = m_Scene->CreateEntity("Cookie");
		auto& sprite = m_Cookie.AddComponent<Teddy::SpriteAnimationComponent>(0.05f, 0.05f, 0.05f);
		sprite.Textures = m_IntroTextures;
		sprite.PingPong = false;
		sprite.Loop = false;

		auto& atlas = m_Cookie.AddComponent<Teddy::SpriteAtlasComponent>();
		atlas.SpriteWidth = 212;
		atlas.SpriteHeight = 270;

		auto& indicies = m_Cookie.GetComponent<Teddy::SpriteAnimationAtlasComponent>();
		indicies.GenerateFrames(sprite, atlas);
		indicies.Index = 107;
		
		sprite.PlayableIndicies = {107, 108, 109, 110, 111, 112, 113};

		auto& transform = m_Cookie.GetComponent<Teddy::TransformComponent>();
		transform.Translation = glm::vec3(0.2f, -0.15f, 0.01f);

		transform.Translation += m_Entity.GetComponent<Teddy::TransformComponent>().Translation;

		created = true;
	}

	void Player::StartIntro()
	{
		if (!m_Entity.HasComponent<Teddy::SpriteAnimationAtlasComponent>())
			return;

		static std::random_device rd;
		static std::mt19937 gen(rd());
		static std::uniform_int_distribution<> distr(0, 2);

		int choice = distr(gen); // TODO: Fix intro 1, x axis is shaken in the atlas

		auto& sprite = m_Entity.GetComponent<Teddy::SpriteAnimationComponent>();
		sprite.Textures = m_IntroTextures;
		sprite.PingPong = false;
		sprite.Loop = false;
		sprite.Reverse = false;
		
		auto& atlas = m_Entity.GetComponent<Teddy::SpriteAtlasComponent>();
		atlas.SpriteWidth = 212;
		atlas.SpriteHeight = 270;

		auto& indicies = m_Entity.GetComponent<Teddy::SpriteAnimationAtlasComponent>();
		indicies.GenerateFrames(sprite, atlas);

		sprite.PlayableIndicies.clear();

		auto& transform = m_Entity.GetComponent<Teddy::TransformComponent>();
		auto& boxCollider = m_Entity.GetComponent<Teddy::BoxCollider2DComponent>();
		auto& boxBody = m_Entity.GetComponent<Teddy::Rigidbody2DComponent>();

		switch (choice)
		{
		case 0:
		{
			for (int i = 0; i < 28; i++)
				sprite.PlayableIndicies.push_back(i);
			indicies.Index = 0;
			transform.Scale *= 1.4f;
			transform.Translation += glm::vec3(0.0f,0.2f,0.0f);
			m_State = PlayerState::Intro0;
			boxCollider.Offset = { 0.0f, -0.1f };
			break;
		}
		case 1:
		{
			for (int i = 28; i < 74; i++)
				sprite.PlayableIndicies.push_back(i);
			indicies.Index = 28;
			transform.Scale *= 1.3f;
			m_State = PlayerState::Intro1;
			boxCollider.Offset = { 0.0f, -0.1f };
			break;
		}
		case 2:
		{
			for (int i = 74; i < 107; i++)
				sprite.PlayableIndicies.push_back(i);
			indicies.Index = 74;
			transform.Scale *= 1.3f;
			m_State = PlayerState::Intro2;
			boxCollider.Offset = { 0.0f, -0.1f };
			break;
		}
		default:
			for (int i = 28; i < 74; i++)
				sprite.PlayableIndicies.push_back(i);
			indicies.Index = 28;
			transform.Scale *= 1.3f;
			m_State = PlayerState::Intro1;
			boxCollider.Offset = { 0.0f, -0.1f };
			break;
		}

		 m_Scene->RefreshBody(boxBody, boxCollider, transform);
	}

	// TODO: Bug stuck if left idle for a while
	// TODO: Verify hit box
	void Player::StartRunning() // TODO: is there a transition?
	{
		m_Entity.GetComponent<Teddy::TransformComponent>().Scale.x = m_DirectionRight ? 1.0f : -1.0f;

		if (m_State == PlayerState::Running) return;

		auto& sprite = m_Entity.GetComponent<Teddy::SpriteAnimationComponent>();
		sprite.Textures = m_MovementTextures;
		sprite.PingPong = false;
		sprite.Loop = true;
		sprite.Reverse = true;

		auto& atlas = m_Entity.GetComponent<Teddy::SpriteAtlasComponent>();
		atlas.SpriteWidth = 347;
		atlas.SpriteHeight = 192;

		auto& indicies = m_Entity.GetComponent<Teddy::SpriteAnimationAtlasComponent>();
		indicies.GenerateFrames(sprite, atlas);

		sprite.PlayableIndicies.clear();
		for (int i = 68; i < 84; i++)
			sprite.PlayableIndicies.push_back(i);
		indicies.Index = 68;

		m_State = PlayerState::Running;
	}

	bool Player::OnKeyPressed(Teddy::KeyPressedEvent& e)
	{
		switch (e.GetKeyCode())
		{
		case Teddy::Key::A:
		case Teddy::Key::Left:
			m_DirectionRight = false;
			StartRunning();
			return true;
		case Teddy::Key::D:
		case Teddy::Key::Right:
			m_DirectionRight = true;
			StartRunning();
			return true;
		default:
			break;
		}

		return false;
	}

	void Player::OnEvent(Teddy::Event& event)
	{
		if (m_State != PlayerState::Intro1 && m_State != PlayerState::Intro2 && m_State != PlayerState::Intro0)
		{
			Teddy::EventDispatcher dispatcher(event);
			dispatcher.Dispatch<Teddy::KeyPressedEvent>(TED_BIND_EVENT_FN(Player::OnKeyPressed));
		}
	}
}