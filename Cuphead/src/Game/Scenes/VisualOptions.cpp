#include "MainMenuScene.h"

#include <Teddy.h>

namespace Cuphead
{
	VisualMenu MainMenuScene::m_VisualMenu;

	void MainMenuScene::InitVisualMenu()
	{
		auto& assets = Teddy::AssetManager::Get();

		// Visual background
		m_VisualMenu.Background = m_MainMenu->CreateEntity("Visual Background");
		auto& optionsSprite = m_VisualMenu.Background.AddComponent<Teddy::SpriteRendererComponent>();
		optionsSprite.Texture = assets.Load<Teddy::Texture2D>("Visual Background", "assets/Textures/SpriteAtlasTexture-Options-1024x2048-fmt12.png");
		m_VisualMenu.Background.AddComponent<Teddy::SpriteAtlasComponent>(0, 1, 590, 520);
		auto& optionsTransform = m_VisualMenu.Background.GetComponent<Teddy::TransformComponent>();
		optionsTransform.Translation = glm::vec3(0.0f, 0.0f, 0.01f);
		optionsTransform.Scale *= 5.0f;

		// Options
		auto& app = Teddy::Application::Get();
		{
			m_VisualMenu.Resolution = m_MainMenu->CreateEntity("Visual Resolution Text");
			auto& resText = m_VisualMenu.Resolution.AddComponent<Teddy::TextComponent>();
			resText.FontAsset = assets.Load<Teddy::Font>("assets/Fonts/CupheadVogue-ExtraBold.otf", Teddy::Boolean::True);
			resText.SetString("RESOLUTION:");
			resText.TextAlignment = Teddy::TextComponent::AlignmentType::LeftCenter;
			auto& resTransform = m_VisualMenu.Resolution.GetComponent<Teddy::TransformComponent>();
			resTransform.Scale *= 0.35f;
			resTransform.Translation += glm::vec3(0.0f, 1.75f, 0.1f);

			m_VisualMenu.ResolutionButton = m_MainMenu->CreateEntity("Visual Resolution Button Text");
			auto& resButText = m_VisualMenu.ResolutionButton.AddComponent<Teddy::TextComponent>();
			resButText.FontAsset = assets.Load<Teddy::Font>("assets/Fonts/CupheadVogue-ExtraBold.otf", Teddy::Boolean::True);
			std::string resolution = std::to_string(app.GetWindow().GetWidth()) + "x" + std::to_string(app.GetWindow().GetHeight());
			resButText.SetString(resolution);
			resButText.TextAlignment = Teddy::TextComponent::AlignmentType::RightCenter;
			auto& resButTransform = m_VisualMenu.ResolutionButton.GetComponent<Teddy::TransformComponent>();
			resButTransform.Scale *= 0.35f;
			resButTransform.Translation += glm::vec3(0.1f, 1.75f, 0.1f);
		}

		{
			m_VisualMenu.Display = m_MainMenu->CreateEntity("Visual Display Text");
			auto& disText = m_VisualMenu.Display.AddComponent<Teddy::TextComponent>();
			disText.FontAsset = assets.Load<Teddy::Font>("assets/Fonts/CupheadVogue-ExtraBold.otf", Teddy::Boolean::True);
			disText.SetString("DISPLAY:");
			disText.TextAlignment = Teddy::TextComponent::AlignmentType::LeftCenter;
			auto& disTransform = m_VisualMenu.Display.GetComponent<Teddy::TransformComponent>();
			disTransform.Scale *= 0.35f;
			disTransform.Translation += glm::vec3(0.0f, 1.4f, 0.1f);

			m_VisualMenu.DisplayButton = m_MainMenu->CreateEntity("Visual Display Button Text");
			auto& disButText = m_VisualMenu.DisplayButton.AddComponent<Teddy::TextComponent>();
			disButText.FontAsset = assets.Load<Teddy::Font>("assets/Fonts/CupheadVogue-ExtraBold.otf", Teddy::Boolean::True);
			disButText.SetString("FULLSCREEN");
			disButText.TextAlignment = Teddy::TextComponent::AlignmentType::RightCenter;
			auto& disButTransform = m_VisualMenu.DisplayButton.GetComponent<Teddy::TransformComponent>();
			disButTransform.Scale *= 0.35f;
			disButTransform.Translation += glm::vec3(0.1f, 1.4f, 0.1f);
		}

		{
			m_VisualMenu.VSync = m_MainMenu->CreateEntity("Visual VSync Text");
			auto& vsyncText = m_VisualMenu.VSync.AddComponent<Teddy::TextComponent>();
			vsyncText.FontAsset = assets.Load<Teddy::Font>("assets/Fonts/CupheadVogue-ExtraBold.otf", Teddy::Boolean::True);
			vsyncText.SetString("VSYNC:");
			vsyncText.TextAlignment = Teddy::TextComponent::AlignmentType::LeftCenter;
			auto& vsyncTransform = m_VisualMenu.VSync.GetComponent<Teddy::TransformComponent>();
			vsyncTransform.Scale *= 0.35f;
			vsyncTransform.Translation += glm::vec3(0.0f, 1.05f, 0.1f);

			m_VisualMenu.VSyncButton = m_MainMenu->CreateEntity("Visual VSync Button Text");
			auto& vsyncButText = m_VisualMenu.VSyncButton.AddComponent<Teddy::TextComponent>();
			vsyncButText.FontAsset = assets.Load<Teddy::Font>("assets/Fonts/CupheadVogue-ExtraBold.otf", Teddy::Boolean::True);
			vsyncButText.SetString(app.GetWindow().IsVSync() ? "ON" : "OFF");
			vsyncButText.TextAlignment = Teddy::TextComponent::AlignmentType::RightCenter;
			auto& vsyncButTransform = m_VisualMenu.VSyncButton.GetComponent<Teddy::TransformComponent>();
			vsyncButTransform.Scale *= 0.35f;
			vsyncButTransform.Translation += glm::vec3(0.1f, 1.05f, 0.1f);
		}

		{
			m_VisualMenu.Overscan = m_MainMenu->CreateEntity("Visual OverScan Text");
			auto& overscanText = m_VisualMenu.Overscan.AddComponent<Teddy::TextComponent>();
			overscanText.FontAsset = assets.Load<Teddy::Font>("assets/Fonts/CupheadVogue-ExtraBold.otf", Teddy::Boolean::True);
			overscanText.SetString("OVERSCAN:");
			overscanText.TextAlignment = Teddy::TextComponent::AlignmentType::LeftCenter;
			auto& overscanTransform = m_VisualMenu.Overscan.GetComponent<Teddy::TransformComponent>();
			overscanTransform.Scale *= 0.35f;
			overscanTransform.Translation += glm::vec3(0.0f, 0.7f, 0.1f);


			Teddy::TransformComponent barSliderTransform;
			barSliderTransform.Scale *= glm::vec3(0.075f, 0.075f, 1.0f);
			barSliderTransform.Translation += glm::vec3(0.2f, 0.65f, 0.1f);

			m_VisualMenu.OverscanBar = Slider(m_MainMenu, barSliderTransform, 0, 0.125f);
		}

		{
			m_VisualMenu.Brightness = m_MainMenu->CreateEntity("Visual Brightness Text");
			auto& brightText = m_VisualMenu.Brightness.AddComponent<Teddy::TextComponent>();
			brightText.FontAsset = assets.Load<Teddy::Font>("assets/Fonts/CupheadVogue-ExtraBold.otf", Teddy::Boolean::True);
			brightText.SetString("BRIGHTNESS:");
			brightText.TextAlignment = Teddy::TextComponent::AlignmentType::LeftCenter;
			auto& brightTransform = m_VisualMenu.Brightness.GetComponent<Teddy::TransformComponent>();
			brightTransform.Scale *= 0.35f;
			brightTransform.Translation += glm::vec3(0.0f, 0.35f, 0.1f);


			Teddy::TransformComponent brightSliderTransform;
			brightSliderTransform.Scale *= glm::vec3(0.075f, 0.075f, 1.0f);
			brightSliderTransform.Translation += glm::vec3(0.2f, 0.3f, 0.1f);

			m_VisualMenu.BrightnessBar = Slider(m_MainMenu, brightSliderTransform, 5, 0.125f);
		}

		{
			m_VisualMenu.ColorBleed = m_MainMenu->CreateEntity("Visual ColorBleed Text");
			auto& cBleedText = m_VisualMenu.ColorBleed.AddComponent<Teddy::TextComponent>();
			cBleedText.FontAsset = assets.Load<Teddy::Font>("assets/Fonts/CupheadVogue-ExtraBold.otf", Teddy::Boolean::True);
			cBleedText.SetString("COLOR BLEED:");
			cBleedText.TextAlignment = Teddy::TextComponent::AlignmentType::LeftCenter;
			auto& cBleedTransform = m_VisualMenu.ColorBleed.GetComponent<Teddy::TransformComponent>();
			cBleedTransform.Scale *= 0.35f;
			cBleedTransform.Translation += glm::vec3(0.0f, 0.0f, 0.1f);


			Teddy::TransformComponent cBleedSliderTransform;
			cBleedSliderTransform.Scale *= glm::vec3(0.075f, 0.075f, 1.0f);
			cBleedSliderTransform.Translation += glm::vec3(0.2f, -0.05f, 0.1f);

			m_VisualMenu.ColorBleedBar = Slider(m_MainMenu, cBleedSliderTransform, 5, 0.125f);
		}

		{
			m_VisualMenu.TitleScreen = m_MainMenu->CreateEntity("Visual TitleScreen Text");
			auto& ttSText = m_VisualMenu.TitleScreen.AddComponent<Teddy::TextComponent>();
			ttSText.FontAsset = assets.Load<Teddy::Font>("assets/Fonts/CupheadVogue-ExtraBold.otf", Teddy::Boolean::True);
			ttSText.SetString("TITLE SCREEN:");
			ttSText.TextAlignment = Teddy::TextComponent::AlignmentType::LeftCenter;
			auto& ttSTransform = m_VisualMenu.TitleScreen.GetComponent<Teddy::TransformComponent>();
			ttSTransform.Scale *= 0.35f;
			ttSTransform.Translation += glm::vec3(0.0f, -0.35f, 0.1f);

			m_VisualMenu.TitleScreenButton = m_MainMenu->CreateEntity("Visual TitleScreen Button Text");
			auto& ttSButText = m_VisualMenu.TitleScreenButton.AddComponent<Teddy::TextComponent>();
			ttSButText.FontAsset = assets.Load<Teddy::Font>("assets/Fonts/CupheadVogue-ExtraBold.otf", Teddy::Boolean::True);
			ttSButText.SetString("NO");
			ttSButText.TextAlignment = Teddy::TextComponent::AlignmentType::RightCenter;
			auto& ttSButTransform = m_VisualMenu.TitleScreenButton.GetComponent<Teddy::TransformComponent>();
			ttSButTransform.Scale *= 0.35f;
			ttSButTransform.Translation += glm::vec3(0.1f, -0.35f, 0.1f);
		}

		{
			m_VisualMenu.Back = m_MainMenu->CreateEntity("Visual Back Text");
			auto& backText = m_VisualMenu.Back.AddComponent<Teddy::TextComponent>();
			backText.FontAsset = assets.Load<Teddy::Font>("assets/Fonts/CupheadVogue-ExtraBold.otf", Teddy::Boolean::True);
			backText.SetString("BACK");
			backText.TextAlignment = Teddy::TextComponent::AlignmentType::Center;
			auto& backTransform = m_VisualMenu.Back.GetComponent<Teddy::TransformComponent>();
			backTransform.Scale *= 0.35f;
			backTransform.Translation += glm::vec3(0.0f, -1.15f, 0.1f);
		}

		UpdateVisualColors();
	}

	void MainMenuScene::UpdateVisualColors()
	{

		if (m_CurrentMenu == 5)
		{
			m_VisualMenu.Background.GetComponent<Teddy::SpriteRendererComponent>().Color = m_WhiteColor;
			m_VisualMenu.Resolution.GetComponent<Teddy::TextComponent>().Color = m_BlackColor;
			m_VisualMenu.ResolutionButton.GetComponent<Teddy::TextComponent>().Color = (m_VisualMenu.CurrentSelection == 0) ? m_RedColor : m_BlackColor;
			m_VisualMenu.Display.GetComponent<Teddy::TextComponent>().Color = m_BlackColor;
			m_VisualMenu.DisplayButton.GetComponent<Teddy::TextComponent>().Color = (m_VisualMenu.CurrentSelection == 1) ? m_RedColor : m_BlackColor;
			m_VisualMenu.VSync.GetComponent<Teddy::TextComponent>().Color = m_BlackColor;
			m_VisualMenu.VSyncButton.GetComponent<Teddy::TextComponent>().Color = (m_VisualMenu.CurrentSelection == 2) ? m_RedColor : m_BlackColor;
			m_VisualMenu.Overscan.GetComponent<Teddy::TextComponent>().Color = m_BlackColor;
			m_VisualMenu.OverscanBar.UpdateBar((m_VisualMenu.CurrentSelection == 3) ? m_RedColor : m_BlackColor);
			m_VisualMenu.Brightness.GetComponent<Teddy::TextComponent>().Color = m_BlackColor;
			m_VisualMenu.BrightnessBar.UpdateBar((m_VisualMenu.CurrentSelection == 4) ? m_RedColor : m_BlackColor);
			m_VisualMenu.ColorBleed.GetComponent<Teddy::TextComponent>().Color = m_BlackColor;
			m_VisualMenu.ColorBleedBar.UpdateBar((m_VisualMenu.CurrentSelection == 5) ? m_RedColor : m_BlackColor);
			m_VisualMenu.TitleScreen.GetComponent<Teddy::TextComponent>().Color = m_BlackColor;
			m_VisualMenu.TitleScreenButton.GetComponent<Teddy::TextComponent>().Color = (m_VisualMenu.CurrentSelection == 6) ? m_RedColor : m_BlackColor;
			m_VisualMenu.Back.GetComponent<Teddy::TextComponent>().Color = (m_VisualMenu.CurrentSelection == 7) ? m_RedColor : m_BlackColor;

		}
		else
		{
			HideVisualMenu();
		}
	}

	void MainMenuScene::HideVisualMenu()
	{
		m_VisualMenu.Background.GetComponent<Teddy::SpriteRendererComponent>().Color = m_InvisibleColor;
		m_VisualMenu.Resolution.GetComponent<Teddy::TextComponent>().Color = m_InvisibleColor;
		m_VisualMenu.ResolutionButton.GetComponent<Teddy::TextComponent>().Color = m_InvisibleColor;
		m_VisualMenu.Display.GetComponent<Teddy::TextComponent>().Color = m_InvisibleColor;
		m_VisualMenu.DisplayButton.GetComponent<Teddy::TextComponent>().Color = m_InvisibleColor;
		m_VisualMenu.VSync.GetComponent<Teddy::TextComponent>().Color = m_InvisibleColor;
		m_VisualMenu.VSyncButton.GetComponent<Teddy::TextComponent>().Color = m_InvisibleColor;
		m_VisualMenu.Overscan.GetComponent<Teddy::TextComponent>().Color = m_InvisibleColor;
		m_VisualMenu.OverscanBar.UpdateBar(m_InvisibleColor);
		m_VisualMenu.Brightness.GetComponent<Teddy::TextComponent>().Color = m_InvisibleColor;
		m_VisualMenu.BrightnessBar.UpdateBar(m_InvisibleColor);
		m_VisualMenu.ColorBleed.GetComponent<Teddy::TextComponent>().Color = m_InvisibleColor;
		m_VisualMenu.ColorBleedBar.UpdateBar(m_InvisibleColor);
		m_VisualMenu.TitleScreen.GetComponent<Teddy::TextComponent>().Color = m_InvisibleColor;
		m_VisualMenu.TitleScreenButton.GetComponent<Teddy::TextComponent>().Color = m_InvisibleColor;
		m_VisualMenu.Back.GetComponent<Teddy::TextComponent>().Color = m_InvisibleColor;
	}
	
	bool MainMenuScene::OnVisualMenuKeyPressed(Teddy::KeyPressedEvent& e)
	{
		auto& window = Teddy::Application::Get().GetWindow();
		switch (e.GetKeyCode())
		{
		case Teddy::Key::Escape:
			m_CurrentMenu = 2;
			HideVisualMenu();
			UpdateOptionsButtonColors();
			return true;
		case Teddy::Key::Down:
		case Teddy::Key::S:
			m_VisualMenu.CurrentSelection = (m_VisualMenu.CurrentSelection + 1) % 8;
			UpdateVisualColors();
			return true;
		case Teddy::Key::Up:
		case Teddy::Key::W:
			m_VisualMenu.CurrentSelection = (m_VisualMenu.CurrentSelection - 1 + 8) % 8;
			UpdateVisualColors();
			return true;
		case Teddy::Key::Right:
		case Teddy::Key::D:
			switch (m_VisualMenu.CurrentSelection)
			{
			case 2:
				window.IsVSync() ? window.SetVSync(false) : window.SetVSync(true);
				m_VisualMenu.VSyncButton.GetComponent<Teddy::TextComponent>().SetString(window.IsVSync() ? "ON" : "OFF");
				break;
			case 3:
				m_VisualMenu.OverscanBar.Increment();
				UpdateVisualColors();
				break;
			case 4:
				m_VisualMenu.BrightnessBar.Increment();
				UpdateVisualColors();
				break;
			case 5:
				glm::vec3 colorBleed = Teddy::PostProcessing::GetChromaticAberrationOffset();
				colorBleed += glm::vec3(0.5f, 0.5f, -0.5f);
				if (colorBleed.x > 5.0f) colorBleed = glm::vec3(5.0f, 5.0f, -5.0f);
				Teddy::PostProcessing::SetChromaticAberrationOffset(colorBleed);
				m_VisualMenu.ColorBleedBar.Increment();
				UpdateVisualColors();
				break;
			}
			return true;
		case Teddy::Key::Left:
		case Teddy::Key::A:
			switch (m_VisualMenu.CurrentSelection)
			{
			case 2:
				window.IsVSync() ? window.SetVSync(false) : window.SetVSync(true);
				m_VisualMenu.VSyncButton.GetComponent<Teddy::TextComponent>().SetString(window.IsVSync() ? "ON" : "OFF");
				break;
			case 3:
				m_VisualMenu.OverscanBar.Decrement();
				UpdateVisualColors();
				break;
			case 4:
				m_VisualMenu.BrightnessBar.Decrement();
				UpdateVisualColors();
				break;
			case 5:
				glm::vec3 colorBleed = Teddy::PostProcessing::GetChromaticAberrationOffset();
				colorBleed -= glm::vec3(0.5f, 0.5f, -0.5f);
				if (colorBleed.x < 0.0f) colorBleed = glm::vec3(0.0f);
				Teddy::PostProcessing::SetChromaticAberrationOffset(colorBleed);
				m_VisualMenu.ColorBleedBar.Decrement();
				UpdateVisualColors();
				break;
			}
			return true;
		case Teddy::Key::Return:
			switch (m_VisualMenu.CurrentSelection)
			{
			case 2:
				window.IsVSync() ? window.SetVSync(false) : window.SetVSync(true);
				m_VisualMenu.VSyncButton.GetComponent<Teddy::TextComponent>().SetString(window.IsVSync() ? "ON" : "OFF");
				break;
			case 7:
				m_CurrentMenu = 2;
				HideVisualMenu();
				UpdateOptionsButtonColors();
				break;
			}
			return true;
		default:
			break;
		}

		return false;
	}
}