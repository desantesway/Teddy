#include "MainMenuScene.h"

#include <Teddy.h>

namespace Cuphead
{
	PlayMenu MainMenuScene::m_PlayMenu;

	void MainMenuScene::InitPlayMenu()
	{
		auto& assets = Teddy::AssetManager::Get();

		m_PlayMenu.Background = m_MainMenu->CreateEntity("Play Menu Background");
		auto& sprite = m_PlayMenu.Background.AddComponent<Teddy::SpriteRendererComponent>();
		sprite.Texture = assets.Load<Teddy::Texture2D>("assets/Textures/SpriteAtlasTexture-Slot_Select-2048x2048-fmt12.png", Teddy::Boolean::True);
		m_PlayMenu.Background.AddComponent<Teddy::SpriteAtlasComponent>(0, 0, 581, 581);
		auto& transform = m_PlayMenu.Background.GetComponent<Teddy::TransformComponent>();
		transform.Translation = glm::vec3(0.0f, 0.0f, 0.01f);
		transform.Scale *= 4.0f;

		UpdatePlayButtonColors();
	}

	void MainMenuScene::HidePlayMenu()
	{
		m_PlayMenu.Background.GetComponent<Teddy::SpriteRendererComponent>().Color = m_InvisibleColor;
	}

	void MainMenuScene::UpdatePlayButtonColors()
	{
		if (m_CurrentMenu == 1)
		{
			m_PlayMenu.Background.GetComponent<Teddy::SpriteRendererComponent>().Color = m_WhiteColor;
		}
		else
		{
			HidePlayMenu();
		}
	}

	bool MainMenuScene::OnPlayMenuKeyPressed(Teddy::KeyPressedEvent& e)
	{
		switch (e.GetKeyCode())
		{
		case Teddy::Key::Escape:
			m_CurrentMenu = 0;
			HidePlayMenu();
			UpdateMainMenuButtonColors();
			return true;
		default:
			break;
		}

		return false;
	}
}