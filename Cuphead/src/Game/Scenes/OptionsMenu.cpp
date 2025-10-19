#include "OptionsMenu.h"

#include <Teddy.h>

// TODO: create entities only when needing to show, destroy when hiding to save memory
namespace Cuphead
{
	glm::vec4 OptionsMenu::m_WhiteColor = glm::vec4(1.0f);;
	glm::vec4 OptionsMenu::m_BlackColor = glm::vec4(70.0f / 255.0f, 70.0f / 255.0f, 70.0f / 255.0f, 1.0f);
	glm::vec4 OptionsMenu::m_RedColor = glm::vec4(172.0f / 255.0f, 32.0f / 255.0f, 54.0f / 255.0f, 1.0f);
	glm::vec4 OptionsMenu::m_InvisibleColor = glm::vec4(0.0f);

	void OptionsMenu::Init(Teddy::Ref<Teddy::Scene> scene)
	{
		auto& assets = Teddy::AssetManager::Get();

		// Options background
		m_OptionsMenu.Background = scene->CreateEntity("Options Background");
		auto& pauseSprite = m_OptionsMenu.Background.AddComponent<Teddy::SpriteRendererComponent>();
		pauseSprite.Texture = assets.Load<Teddy::Texture2D>("assets/Textures/UI/UI_Cards_595x668_2048x2048_0.png", Teddy::Boolean::True);
		m_OptionsMenu.Background.AddComponent<Teddy::SpriteAtlasComponent>(0, 0, 595, 668);
		auto& pauseTransform = m_OptionsMenu.Background.GetComponent<Teddy::TransformComponent>();
		pauseTransform.Translation = glm::vec3(0.0f, 0.8f, 5.01f);
		pauseTransform.Scale *= 3.0f;

		// Options Text
		m_OptionsMenu.AudioButton = scene->CreateEntity("Options Audio Text");
		auto& audioText = m_OptionsMenu.AudioButton.AddComponent<Teddy::TextComponent>();
		audioText.FontAsset = assets.Load<Teddy::Font>("assets/Fonts/CupheadVogue-ExtraBold.otf", Teddy::Boolean::True);
		audioText.SetString("AUDIO");
		audioText.TextAlignment = Teddy::TextComponent::AlignmentType::Center;
		auto& audioTransform = m_OptionsMenu.AudioButton.GetComponent<Teddy::TransformComponent>();
		audioTransform.Scale *= 0.175f;
		audioTransform.Translation += glm::vec3(0.0f, 0.425f, 5.1f);

		m_OptionsMenu.VisualButton = scene->CreateEntity("Options Visual Text");
		auto& visualText = m_OptionsMenu.VisualButton.AddComponent<Teddy::TextComponent>();
		visualText.FontAsset = assets.Load<Teddy::Font>("assets/Fonts/CupheadVogue-ExtraBold.otf", Teddy::Boolean::True);
		visualText.SetString("VISUAL");
		visualText.TextAlignment = Teddy::TextComponent::AlignmentType::Center;
		auto& visualTransform = m_OptionsMenu.VisualButton.GetComponent<Teddy::TransformComponent>();
		visualTransform.Scale *= 0.175f;
		visualTransform.Translation += glm::vec3(0.0f, 0.25f, 5.1f);

		m_OptionsMenu.ControlsButton = scene->CreateEntity("Options Controls Text");
		auto& controlsText = m_OptionsMenu.ControlsButton.AddComponent<Teddy::TextComponent>();
		controlsText.FontAsset = assets.Load<Teddy::Font>("assets/Fonts/CupheadVogue-ExtraBold.otf", Teddy::Boolean::True);
		controlsText.SetString("CONTROLS");
		controlsText.TextAlignment = Teddy::TextComponent::AlignmentType::Center;
		auto& controlsTransform = m_OptionsMenu.ControlsButton.GetComponent<Teddy::TransformComponent>();
		controlsTransform.Scale *= 0.175f;
		controlsTransform.Translation += glm::vec3(0.0f, 0.075f, 5.1f);

		m_OptionsMenu.LanguageButton = scene->CreateEntity("Options Language Text");
		auto& langText = m_OptionsMenu.LanguageButton.AddComponent<Teddy::TextComponent>();
		langText.FontAsset = assets.Load<Teddy::Font>("assets/Fonts/CupheadVogue-ExtraBold.otf", Teddy::Boolean::True);
		langText.SetString("LANGUAGE");
		langText.TextAlignment = Teddy::TextComponent::AlignmentType::Center;
		auto& langTransform = m_OptionsMenu.LanguageButton.GetComponent<Teddy::TransformComponent>();
		langTransform.Scale *= 0.175f;
		langTransform.Translation += glm::vec3(0.0f, -0.1f, 5.1f);

		m_OptionsMenu.BackButton = scene->CreateEntity("Options Back Text");
		auto& backText = m_OptionsMenu.BackButton.AddComponent<Teddy::TextComponent>();
		backText.FontAsset = assets.Load<Teddy::Font>("assets/Fonts/CupheadVogue-ExtraBold.otf", Teddy::Boolean::True);
		backText.SetString("BACK");
		backText.TextAlignment = Teddy::TextComponent::AlignmentType::Center;
		auto& backTransform = m_OptionsMenu.BackButton.GetComponent<Teddy::TransformComponent>();
		backTransform.Scale *= 0.175f;
		backTransform.Translation += glm::vec3(0.0f, -0.275f, 5.1f);

		UpdateButtonColors();

		InitVisualMenu(scene);
	}

	void OptionsMenu::Show()
	{
		m_CurrentMenu = 0;
		UpdateButtonColors();
	}

	bool OptionsMenu::OptionsMenuEnter()
	{
		switch (m_OptionsMenu.CurrentSelection)
		{
		case 0:
			break;
		case 1:
			m_CurrentMenu = 1;
			Hide();
			UpdateVisualColors();
			break;
		case 2:
			break;
		case 3:
			break;
		case 4:
			m_CurrentMenu = -1;
			Hide();
			return true;
		default:
			break;
		}

		return false;
	}

	void OptionsMenu::Hide()
	{
		m_OptionsMenu.Background.GetComponent<Teddy::SpriteRendererComponent>().Color = m_InvisibleColor;
		m_OptionsMenu.AudioButton.GetComponent<Teddy::TextComponent>().Color = m_InvisibleColor;
		m_OptionsMenu.VisualButton.GetComponent<Teddy::TextComponent>().Color = m_InvisibleColor;
		m_OptionsMenu.ControlsButton.GetComponent<Teddy::TextComponent>().Color = m_InvisibleColor;
		m_OptionsMenu.LanguageButton.GetComponent<Teddy::TextComponent>().Color = m_InvisibleColor;
		m_OptionsMenu.BackButton.GetComponent<Teddy::TextComponent>().Color = m_InvisibleColor;
	}

	void OptionsMenu::UpdateButtonColors()
	{
		if (m_CurrentMenu == 0)
		{
			m_OptionsMenu.Background.GetComponent<Teddy::SpriteRendererComponent>().Color = m_WhiteColor;
			m_OptionsMenu.AudioButton.GetComponent<Teddy::TextComponent>().Color = (m_OptionsMenu.CurrentSelection == 0) ? m_RedColor : m_BlackColor;
			m_OptionsMenu.VisualButton.GetComponent<Teddy::TextComponent>().Color = (m_OptionsMenu.CurrentSelection == 1) ? m_RedColor : m_BlackColor;
			m_OptionsMenu.ControlsButton.GetComponent<Teddy::TextComponent>().Color = (m_OptionsMenu.CurrentSelection == 2) ? m_RedColor : m_BlackColor;
			m_OptionsMenu.LanguageButton.GetComponent<Teddy::TextComponent>().Color = (m_OptionsMenu.CurrentSelection == 3) ? m_RedColor : m_BlackColor;
			m_OptionsMenu.BackButton.GetComponent<Teddy::TextComponent>().Color = (m_OptionsMenu.CurrentSelection == 4) ? m_RedColor : m_BlackColor;
		}
		else
		{
			Hide();
		}
	}

	bool OptionsMenu::OnKeyPressed(Teddy::KeyPressedEvent& e)
	{
		switch (m_CurrentMenu)
		{
		case 0:
		{
			switch (e.GetKeyCode())
			{
			case Teddy::Key::Escape:
				Hide();
				m_CurrentMenu = -1;
				return true;
			case Teddy::Key::Down:
			case Teddy::Key::S:
				m_OptionsMenu.CurrentSelection = (m_OptionsMenu.CurrentSelection + 1) % 5;
				UpdateButtonColors();
				return true;
			case Teddy::Key::Up:
			case Teddy::Key::W:
				m_OptionsMenu.CurrentSelection = (m_OptionsMenu.CurrentSelection - 1 + 5) % 5;
				UpdateButtonColors();
				return true;
			case Teddy::Key::Return:
				return OptionsMenuEnter();
			default:
				break;
			}
		}
		case 1:
		{
			return OnVisualMenuKeyPressed(e);
		}
		default:
			break;
		}
		
		return false;
	}
}