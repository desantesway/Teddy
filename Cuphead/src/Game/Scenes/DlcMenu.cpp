#include "MainMenuScene.h"

#include <Teddy.h>

namespace Cuphead
{
	void MainMenuScene::InitDlcMenu()
	{
		auto& assets = Teddy::AssetManager::Get();

		m_DlcMenu.Entity = m_MainMenu->CreateEntity("DLC Photo");
		auto& sprite = m_DlcMenu.Entity.AddComponent<Teddy::SpriteRendererComponent>();
		sprite.Texture = assets.Load<Teddy::Texture2D>("assets/Textures/PC _ Computer - Cuphead_ Don't Deal With the Devil! - Miscellaneous - DLC Sellscreen.png", Teddy::Boolean::True);
		auto& transform = m_DlcMenu.Entity.GetComponent<Teddy::TransformComponent>();
		transform.Translation = glm::vec3(0.0f, 0.0f, 0.01f);
		transform.Scale *= 4.0f;

		UpdateDlcButtonColors();
	}

	void MainMenuScene::HideDlcMenu()
	{
		m_DlcMenu.Entity.GetComponent<Teddy::SpriteRendererComponent>().Color = m_InvisibleColor;
	}

	void MainMenuScene::UpdateDlcButtonColors()
	{
		if (m_CurrentMenu == 3)
		{
			m_DlcMenu.Entity.GetComponent<Teddy::SpriteRendererComponent>().Color = m_WhiteColor;
		}
		else
		{
			HideDlcMenu();
		}
	}

	bool MainMenuScene::OnDlcMenuKeyPressed(Teddy::KeyPressedEvent& e)
	{
		switch (e.GetKeyCode())
		{
		case Teddy::Key::Escape:
			m_CurrentMenu = 0;
			HideDlcMenu();
			UpdateMainMenuButtonColors();
			return true;
		default:
			break;
		}

		return false;
	}
}