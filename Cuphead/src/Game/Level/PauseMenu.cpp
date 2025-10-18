#include "PauseMenu.h"

#include <Teddy.h>

// TODO: make menu atlas
// TODO: bug after leaving pause menu where player has weird animations

namespace Cuphead
{
	Teddy::Entity PauseMenu::m_Background;
	Teddy::Entity PauseMenu::m_Resume;
	Teddy::Entity PauseMenu::m_Retry;
	Teddy::Entity PauseMenu::m_Exit;
	int PauseMenu::m_State = -1;
	Teddy::Ref<Teddy::Texture2D> PauseMenu::m_BackgroundTexture = nullptr;
	Teddy::Ref<Teddy::Scene> PauseMenu::m_Scene = nullptr;
	bool PauseMenu::m_Visible = false;

	void PauseMenu::Init(Teddy::Ref<Teddy::Scene> scene)
	{
		auto& assets = Teddy::AssetManager::Get();
		m_BackgroundTexture = assets.Load<Teddy::Texture2D>("assets/Textures/SpriteAtlasTexture-Pause (Group 0)-1024x1024-fmt12.png", Teddy::Boolean::True);

		m_Scene = scene;
	}

	void PauseMenu::Show()
	{
		m_Background = m_Scene->CreateEntity("Pause Menu Background");
		auto& sprite = m_Background.AddComponent<Teddy::SpriteRendererComponent>();
		sprite.Texture = m_BackgroundTexture;
		m_Background.AddComponent<Teddy::SpriteAtlasComponent>(0, 0, 512, 300);
		auto& transform = m_Background.GetComponent<Teddy::TransformComponent>();
		transform.Translation = glm::vec3(0.0f, 0.0f, 5.01f);
		transform.Scale *= 1.5f;
	}

	void PauseMenu::Hide()
	{
		m_Scene->DestroyEntity(m_Background);
		m_Background = {};
	}

	bool PauseMenu::OnKeyPressed(Teddy::KeyPressedEvent& e)
	{
		switch (e.GetKeyCode())
		{
		case Teddy::Key::Escape:
			if(m_State == -1)
				m_State = 0;
			else
				m_State = -1;
			return true;
		default:
			break;
		}
	}

	void PauseMenu::OnEvent(Teddy::Event& event)
	{
		Teddy::EventDispatcher dispatcher(event);
		dispatcher.Dispatch<Teddy::KeyPressedEvent>(TED_BIND_EVENT_FN(PauseMenu::OnKeyPressed));
	}
}