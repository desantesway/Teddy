#include "MainMenuScene.h"

#include <Teddy.h>

namespace Cuphead
{
	PlayMenu MainMenuScene::m_PlayMenu;
	unsigned int MainMenuScene::m_LoadLevel = 0;

	void MainMenuScene::InitPlayMenu()
	{
		auto& assets = Teddy::AssetManager::Get();

		m_PlayMenu.Background = m_MainMenu->CreateEntity("Play Menu Background");
		auto& sprite = m_PlayMenu.Background.AddComponent<Teddy::SpriteRendererComponent>();
		sprite.Texture = assets.Load<Teddy::Texture2D>("assets/Textures/SpriteAtlasTexture-Slot_Select-2048x2048-fmt12.png", Teddy::Boolean::True);
		m_PlayMenu.Background.AddComponent<Teddy::SpriteAtlasComponent>(0, 0, 581, 581);
		auto& transform = m_PlayMenu.Background.GetComponent<Teddy::TransformComponent>();
		transform.Translation = glm::vec3(0.0f, 0.0f, 0.01f);
		transform.Scale *= 5.5f;

		m_PlayMenu.Selected = m_MainMenu->CreateEntity("Play Menu Selected");
		auto& spriteSelected = m_PlayMenu.Selected.AddComponent<Teddy::SpriteRendererComponent>();
		spriteSelected.Texture = assets.Load<Teddy::Texture2D>("assets/Textures/SpriteAtlasTexture-Slot_Select-2048x2048-fmt12.png", Teddy::Boolean::True);
		m_PlayMenu.Selected.AddComponent<Teddy::SpriteAtlasComponent>(1, 2, 585, 155);
		auto& transformSelected = m_PlayMenu.Selected.GetComponent<Teddy::TransformComponent>();
		transformSelected.Translation = glm::vec3(0.1f, 0.3f, 0.1f);
		transformSelected.Scale *= 1.4f;

		m_PlayMenu.Text = m_MainMenu->CreateEntity("Play Menu Text");
		auto& text = m_PlayMenu.Text.AddComponent<Teddy::TextComponent>();
		text.FontAsset = assets.Load<Teddy::Font>("assets/Fonts/CupheadVogue-Bold.otf", Teddy::Boolean::True);
		text.SetString("PLEASE SELECT");
		text.TextAlignment = Teddy::TextComponent::AlignmentType::Center;
		auto& textTransform = m_PlayMenu.Text.GetComponent<Teddy::TransformComponent>();
		textTransform.Scale *= 0.25f;
		textTransform.Translation += glm::vec3(-1.1f, 0.35f, 1.0f);

		m_PlayMenu.PlayerText = m_MainMenu->CreateEntity("Play Menu Player Text");
		auto& pText = m_PlayMenu.PlayerText.AddComponent<Teddy::TextComponent>();
		pText.FontAsset = assets.Load<Teddy::Font>("assets/Fonts/CupheadVogue-Bold.otf", Teddy::Boolean::True);
		pText.SetString("PLAYER");
		pText.TextAlignment = Teddy::TextComponent::AlignmentType::Center;
		auto& pTextTransform = m_PlayMenu.PlayerText.GetComponent<Teddy::TransformComponent>();
		pTextTransform.Scale *= 0.25f;
		pTextTransform.Translation += glm::vec3(-1.1f, 0.1f, 1.0f);

		m_PlayMenu.Cuphead = m_MainMenu->CreateEntity("Cuphead Select");
		auto& cupheadSprite = m_PlayMenu.Cuphead.AddComponent<Teddy::SpriteAnimationComponent>();
		cupheadSprite.PlayableIndicies = { 11, 12, 13 };
		cupheadSprite.Loop = true;
		cupheadSprite.Textures.push_back(assets.Load<Teddy::Texture2D>("assets/Textures/Cuphead/CupheadSelect_141x169_1024x1024_0.png", Teddy::Boolean::True));
		m_PlayMenu.Cuphead.AddComponent<Teddy::SpriteAtlasComponent>(4, 1, 141, 169);
		auto& cupheadTransform = m_PlayMenu.Cuphead.GetComponent<Teddy::TransformComponent>();
		cupheadTransform.Translation = glm::vec3(0.4f, 0.45f, 1.0f);
		cupheadTransform.Scale *= 1.3f;

		m_PlayMenu.Mugman = m_MainMenu->CreateEntity("Mugman Select");
		auto& mugmanSprite = m_PlayMenu.Mugman.AddComponent<Teddy::SpriteAnimationComponent>();
		mugmanSprite.PlayableIndicies = { 11, 12, 13 };
		mugmanSprite.Loop = true;
		mugmanSprite.Textures.push_back(assets.Load<Teddy::Texture2D>("assets/Textures/Mugman/MugmanSelect_134x157_1024x1024_0.png", Teddy::Boolean::True));
		m_PlayMenu.Mugman.AddComponent<Teddy::SpriteAtlasComponent>(4, 1, 136, 159);
		auto& mugmanTransform = m_PlayMenu.Mugman.GetComponent<Teddy::TransformComponent>();
		mugmanTransform.Translation = glm::vec3(1.5f, 0.45f, 1.0f);
		mugmanTransform.Scale *= 1.2f;

		UpdatePlayButtonColors();
	}

	void MainMenuScene::HidePlayMenu()
	{
		m_PlayMenu.Background.GetComponent<Teddy::SpriteRendererComponent>().Color = m_InvisibleColor;
		m_PlayMenu.Selected.GetComponent<Teddy::SpriteRendererComponent>().Color = m_InvisibleColor;
		m_PlayMenu.Text.GetComponent<Teddy::TextComponent>().Color = m_InvisibleColor;
		m_PlayMenu.PlayerText.GetComponent<Teddy::TextComponent>().Color = m_InvisibleColor;
		m_PlayMenu.Cuphead.GetComponent<Teddy::SpriteAnimationComponent>().Color = m_InvisibleColor;
		m_PlayMenu.Mugman.GetComponent<Teddy::SpriteAnimationComponent>().Color = m_InvisibleColor;
	}

	void MainMenuScene::OnPlayUpdate()
	{
		if (m_PlayMenu.Cuphead.HasComponent<Teddy::SpriteAnimationAtlasComponent>() && m_PlayMenu.Mugman.HasComponent<Teddy::SpriteAnimationAtlasComponent>())
		{
			if (m_PlayMenu.Mugman.GetComponent<Teddy::SpriteAnimationAtlasComponent>().Index == 7)
			{
				m_LoadLevel = 2;
				m_PlayMenu.Mugman.GetComponent<Teddy::SpriteAnimationComponent>().PlayableIndicies = std::vector<int>{ 5, 6, 7 };
				m_PlayMenu.Mugman.GetComponent<Teddy::SpriteAnimationComponent>().PingPong = true;
			}
			else if (m_PlayMenu.Cuphead.GetComponent<Teddy::SpriteAnimationAtlasComponent>().Index == 7)
			{
				m_LoadLevel = 1;
				m_PlayMenu.Cuphead.GetComponent<Teddy::SpriteAnimationComponent>().PlayableIndicies = std::vector<int>{ 5, 6, 7 };
				m_PlayMenu.Cuphead.GetComponent<Teddy::SpriteAnimationComponent>().PingPong = true;
			}
		}
	}

	void MainMenuScene::UpdatePlayButtonColors()
	{
		if (m_CurrentMenu == 1)
		{
			m_PlayMenu.Background.GetComponent<Teddy::SpriteRendererComponent>().Color = m_WhiteColor;
			m_PlayMenu.Selected.GetComponent<Teddy::SpriteRendererComponent>().Color = 
				m_PlayMenu.CurrentSelection == 0 ? 
				glm::vec4(197.0f / 255.0f, 93.0f / 255.0f, 66.0f / 255.0f, 1.0f): 
				glm::vec4(80.0f / 155.0f, 152.0f / 255.0f, 96.0f / 255.0f, 1.0f);
			m_PlayMenu.Text.GetComponent<Teddy::TextComponent>().Color = m_WhiteColor * glm::vec4(0.9f, 0.9f, 0.9f, 1.0f);
			m_PlayMenu.PlayerText.GetComponent<Teddy::TextComponent>().Color = m_WhiteColor * glm::vec4(0.9f, 0.9f, 0.9f, 1.0f);

			auto& cupheadAnimation = m_PlayMenu.Cuphead.GetComponent<Teddy::SpriteAnimationComponent>();
			cupheadAnimation.Color = m_WhiteColor;

			auto& mugmanAnimation = m_PlayMenu.Mugman.GetComponent<Teddy::SpriteAnimationComponent>();
			mugmanAnimation.Color = m_WhiteColor;

			if (!m_LoadLevel)
			{
				if (m_PlayMenu.CurrentSelection == 0)
				{
					cupheadAnimation.PlayableIndicies = std::vector<int>{ 8, 9, 10 };
					m_PlayMenu.Cuphead.GetComponent<Teddy::SpriteAnimationAtlasComponent>().Index = 8;

					mugmanAnimation.PlayableIndicies = std::vector<int>{ 11, 12, 13 };
					m_PlayMenu.Mugman.GetComponent<Teddy::SpriteAnimationAtlasComponent>().Index = 11;
				}
				else
				{
					cupheadAnimation.PlayableIndicies = std::vector<int>{ 11, 12, 13 };
					m_PlayMenu.Cuphead.GetComponent<Teddy::SpriteAnimationAtlasComponent>().Index = 11;

					mugmanAnimation.PlayableIndicies = std::vector<int>{ 8, 9, 10 };
					m_PlayMenu.Mugman.GetComponent<Teddy::SpriteAnimationAtlasComponent>().Index = 8;
				}
			}
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
		case Teddy::Key::Left:
		case Teddy::Key::A:
			m_PlayMenu.CurrentSelection = 0;
			UpdatePlayButtonColors();
			return true;
		case Teddy::Key::Right:
		case Teddy::Key::D:
			m_PlayMenu.CurrentSelection = 1;
			UpdatePlayButtonColors();
			return true;
		case Teddy::Key::Return:
			switch (m_PlayMenu.CurrentSelection)
			{
				case 0:
					m_PlayMenu.Cuphead.GetComponent<Teddy::SpriteAnimationComponent>().PlayableIndicies = std::vector<int>{ 0, 1, 2, 3, 4, 5, 6, 7 };
					m_PlayMenu.Cuphead.GetComponent<Teddy::SpriteAnimationAtlasComponent>().Index = 0;
					break;
				case 1:
					m_PlayMenu.Mugman.GetComponent<Teddy::SpriteAnimationComponent>().PlayableIndicies = std::vector<int>{ 0, 1, 2, 3, 4, 5, 6, 7 };
					m_PlayMenu.Mugman.GetComponent<Teddy::SpriteAnimationAtlasComponent>().Index = 0;
					break;
				default:
					break;
			}
			break;
		default:
			break;
		}

		return false;
	}
}