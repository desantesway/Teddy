#include "MainMenuScene.h"

#include <Teddy.h>

namespace Cuphead
{
	glm::vec4 MainMenuScene::m_WhiteColor = glm::vec4(1.0f);
	glm::vec4 MainMenuScene::m_GrayColor = glm::vec4(110.0f / 255.0f, 110.0f / 255.0f, 110.0f / 255.0f, 1.0f);
	glm::vec4 MainMenuScene::m_BlackColor = glm::vec4(70.0f / 255.0f, 70.0f / 255.0f, 70.0f / 255.0f, 1.0f);
	glm::vec4 MainMenuScene::m_RedColor = glm::vec4(172.0f / 255.0f, 32.0f / 255.0f, 54.0f / 255.0f, 1.0f);
	glm::vec4 MainMenuScene::m_InvisibleColor = glm::vec4(0.0f);

	MainMenu MainMenuScene::m_MainMenuOptions;
	OptionsMenu MainMenuScene::m_OptionsMenu;
	VisualMenu MainMenuScene::m_VisualMenu;
	unsigned int MainMenuScene::m_CurrentMenu = 0;

	Teddy::Ref<Teddy::Scene> MainMenuScene::Init()
	{
		TED_PROFILE_FUNCTION();

		m_MainMenu = Teddy::CreateRef<Teddy::Scene>();
		m_MainMenu->OnRuntimeStart();

		auto camEntt = m_MainMenu->CreateEntity("Main Menu Camera");
		camEntt.GetComponent<Teddy::TransformComponent>().Translation = { 0.0f, 0.0f, 9.0f };
		auto& cam = camEntt.AddComponent<Teddy::CameraComponent>();
		cam.Camera.SetProjectionType(Teddy::SceneCamera::ProjectionType::Perspective);
		cam.Camera.SetViewportSize(1920, 1080); // TODO: Viewport sharing + other scenes

		// Background
		auto& assets = Teddy::AssetManager::Get();

		auto background = m_MainMenu->CreateEntity("Main Menu Background");
		auto& sprite = background.AddComponent<Teddy::SpriteRendererComponent>();
		sprite.IsBackground = true;
		sprite.Texture = assets.Load<Teddy::Texture2D>("Main Menu Background", "assets/Textures/SpriteAtlasTexture-Slot_Select_BG-2048x1024-fmt10.png");

		background.AddComponent<Teddy::SpriteAtlasComponent>(0, 0, 1410, 840);
		background.GetComponent<Teddy::TransformComponent>().Scale *= 1.1f;

		InitMainMenu();
		InitOptionsMenu();

		return m_MainMenu;
	}

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

		// Options Text
		auto& app = Teddy::Application::Get();
		{
			m_VisualMenu.Resolution = m_MainMenu->CreateEntity("Visual Resolution Text");
			auto& resText = m_VisualMenu.Resolution.AddComponent<Teddy::TextComponent>();
			resText.FontAsset = assets.Load<Teddy::Font>("assets/Fonts/CupheadVogue-ExtraBold.otf", Teddy::Boolean::True);
			resText.SetString("RESOLUTION:");
			resText.TextAlignment = Teddy::TextComponent::AlignmentType::LeftCenter;
			auto& resTransform = m_VisualMenu.Resolution.GetComponent<Teddy::TransformComponent>();
			resTransform.Scale *= 0.4f;
			resTransform.Translation += glm::vec3(0.0f, 1.75f, 0.1f);

			m_VisualMenu.ResolutionButton = m_MainMenu->CreateEntity("Visual Resolution Button Text");
			auto& resButText = m_VisualMenu.ResolutionButton.AddComponent<Teddy::TextComponent>();
			resButText.FontAsset = assets.Load<Teddy::Font>("assets/Fonts/CupheadVogue-ExtraBold.otf", Teddy::Boolean::True);
			std::string resolution = std::to_string(app.GetWindow().GetWidth()) + "x" + std::to_string(app.GetWindow().GetHeight());
			resButText.SetString(resolution);
			resButText.TextAlignment = Teddy::TextComponent::AlignmentType::RightCenter;
			auto& resButTransform = m_VisualMenu.ResolutionButton.GetComponent<Teddy::TransformComponent>();
			resButTransform.Scale *= 0.4f;
			resButTransform.Translation += glm::vec3(0.1f, 1.75f, 0.1f);
		}

		{
			m_VisualMenu.Display = m_MainMenu->CreateEntity("Visual Display Text");
			auto& disText = m_VisualMenu.Display.AddComponent<Teddy::TextComponent>();
			disText.FontAsset = assets.Load<Teddy::Font>("assets/Fonts/CupheadVogue-ExtraBold.otf", Teddy::Boolean::True);
			disText.SetString("DISPLAY:");
			disText.TextAlignment = Teddy::TextComponent::AlignmentType::LeftCenter;
			auto& disTransform = m_VisualMenu.Display.GetComponent<Teddy::TransformComponent>();
			disTransform.Scale *= 0.4f;
			disTransform.Translation += glm::vec3(0.0f, 1.4f, 0.1f);

			m_VisualMenu.DisplayButton = m_MainMenu->CreateEntity("Visual Display Button Text");
			auto& disButText = m_VisualMenu.DisplayButton.AddComponent<Teddy::TextComponent>();
			disButText.FontAsset = assets.Load<Teddy::Font>("assets/Fonts/CupheadVogue-ExtraBold.otf", Teddy::Boolean::True);
			disButText.SetString("FULLSCREEN");
			disButText.TextAlignment = Teddy::TextComponent::AlignmentType::RightCenter;
			auto& disButTransform = m_VisualMenu.DisplayButton.GetComponent<Teddy::TransformComponent>();
			disButTransform.Scale *= 0.4f;
			disButTransform.Translation += glm::vec3(0.1f, 1.4f, 0.1f);
		}

		{
			m_VisualMenu.VSync = m_MainMenu->CreateEntity("Visual VSync Text");
			auto& vsyncText = m_VisualMenu.VSync.AddComponent<Teddy::TextComponent>();
			vsyncText.FontAsset = assets.Load<Teddy::Font>("assets/Fonts/CupheadVogue-ExtraBold.otf", Teddy::Boolean::True);
			vsyncText.SetString("VSYNC:");
			vsyncText.TextAlignment = Teddy::TextComponent::AlignmentType::LeftCenter;
			auto& vsyncTransform = m_VisualMenu.VSync.GetComponent<Teddy::TransformComponent>();
			vsyncTransform.Scale *= 0.4f;
			vsyncTransform.Translation += glm::vec3(0.0f, 1.05f, 0.1f);

			m_VisualMenu.VSyncButton = m_MainMenu->CreateEntity("Visual VSync Button Text");
			auto& vsyncButText = m_VisualMenu.VSyncButton.AddComponent<Teddy::TextComponent>();
			vsyncButText.FontAsset = assets.Load<Teddy::Font>("assets/Fonts/CupheadVogue-ExtraBold.otf", Teddy::Boolean::True);
			vsyncButText.SetString("ON");
			vsyncButText.TextAlignment = Teddy::TextComponent::AlignmentType::RightCenter;
			auto& vsyncButTransform = m_VisualMenu.VSyncButton.GetComponent<Teddy::TransformComponent>();
			vsyncButTransform.Scale *= 0.4f;
			vsyncButTransform.Translation += glm::vec3(0.1f, 1.05f, 0.1f);
		}

		{
			m_VisualMenu.Overscan = m_MainMenu->CreateEntity("Visual OverScan Text");
			auto& overscanText = m_VisualMenu.Overscan.AddComponent<Teddy::TextComponent>();
			overscanText.FontAsset = assets.Load<Teddy::Font>("assets/Fonts/CupheadVogue-ExtraBold.otf", Teddy::Boolean::True);
			overscanText.SetString("OVERSCAN:");
			overscanText.TextAlignment = Teddy::TextComponent::AlignmentType::LeftCenter;
			auto& overscanTransform = m_VisualMenu.Overscan.GetComponent<Teddy::TransformComponent>();
			overscanTransform.Scale *= 0.4f;
			overscanTransform.Translation += glm::vec3(0.0f, 0.7f, 0.1f);

			
			Teddy::TransformComponent barTransform;
			barTransform.Scale *= glm::vec3(0.075f, 0.075f, 1.0f);
			barTransform.Translation += glm::vec3(0.2f, 0.65f, 0.1f);

			m_VisualMenu.OverscanBar = ScrollBar(m_MainMenu, barTransform, 0, 0.125f);
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
			//m_VisualMenu.Brightness.GetComponent<Teddy::TextComponent>().Color = m_BlackColor;
			//m_VisualMenu.BrightnessBar.GetComponent<Teddy::TextComponent>().Color = (m_VisualMenu.CurrentSelection == 4) ? m_RedColor : m_BlackColor;
			//m_VisualMenu.ColorBleed.GetComponent<Teddy::TextComponent>().Color = m_BlackColor;
			//m_VisualMenu.ColorBleedBar.GetComponent<Teddy::TextComponent>().Color = (m_VisualMenu.CurrentSelection == 5) ? m_RedColor : m_BlackColor;
			//m_VisualMenu.TitleScreen.GetComponent<Teddy::TextComponent>().Color = m_BlackColor;
			//m_VisualMenu.TitleScreenButton.GetComponent<Teddy::TextComponent>().Color = (m_VisualMenu.CurrentSelection == 6) ? m_RedColor : m_BlackColor;
			//m_VisualMenu.Back.GetComponent<Teddy::TextComponent>().Color = (m_VisualMenu.CurrentSelection == 7) ? m_RedColor : m_BlackColor;

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
		//m_VisualMenu.Brightness.GetComponent<Teddy::TextComponent>().Color = m_InvisibleColor;
		//m_VisualMenu.BrightnessBar.GetComponent<Teddy::TextComponent>().Color = m_InvisibleColor;
		//m_VisualMenu.ColorBleed.GetComponent<Teddy::TextComponent>().Color = m_InvisibleColor;
		//m_VisualMenu.ColorBleedBar.GetComponent<Teddy::TextComponent>().Color = m_InvisibleColor;
		//m_VisualMenu.TitleScreen.GetComponent<Teddy::TextComponent>().Color = m_InvisibleColor;
		//m_VisualMenu.TitleScreenButton.GetComponent<Teddy::TextComponent>().Color = m_InvisibleColor;
		//m_VisualMenu.Back.GetComponent<Teddy::TextComponent>().Color = m_InvisibleColor;
	}

	bool MainMenuScene::OptionsMenuEnter()
	{
		switch (m_OptionsMenu.CurrentSelection)
		{
		case 0:
			break;
		case 1:
			m_CurrentMenu = 5;
			HideOptionsMenu();
			UpdateVisualColors();
			break;
		case 2:
			break;
		case 3:
			break;
		case 4:
			m_CurrentMenu = 0;
			HideOptionsMenu();
			UpdateMainMenuButtonColors();
			return true;
		default:
			break;
		}

		return false;
	}

	void MainMenuScene::InitOptionsMenu()
	{
		auto& assets = Teddy::AssetManager::Get();

		// Options background
		m_OptionsMenu.Background = m_MainMenu->CreateEntity("Options Background");
		auto& pauseSprite = m_OptionsMenu.Background.AddComponent<Teddy::SpriteRendererComponent>();
		pauseSprite.Texture = assets.Load<Teddy::Texture2D>("Options Background", "assets/Textures/SpriteAtlasTexture-Pause (Group 0)-1024x1024-fmt12.png");
		m_OptionsMenu.Background.AddComponent<Teddy::SpriteAtlasComponent>(0, 0, 512, 304);
		auto& pauseTransform = m_OptionsMenu.Background.GetComponent<Teddy::TransformComponent>();
		pauseTransform.Translation = glm::vec3(0.0f, 0.0f, 0.01f);
		pauseTransform.Scale *= 2.6f;

		// Options Text
		m_OptionsMenu.AudioButton = m_MainMenu->CreateEntity("Options Audio Text");
		auto& audioText = m_OptionsMenu.AudioButton.AddComponent<Teddy::TextComponent>();
		audioText.FontAsset = assets.Load<Teddy::Font>("assets/Fonts/CupheadVogue-ExtraBold.otf", Teddy::Boolean::True);
		audioText.SetString("AUDIO");
		audioText.TextAlignment = Teddy::TextComponent::AlignmentType::Center;
		auto& audioTransform = m_OptionsMenu.AudioButton.GetComponent<Teddy::TransformComponent>();
		audioTransform.Scale *= 0.4f;
		audioTransform.Translation += glm::vec3(0.0f, 0.85f, 0.1f);

		m_OptionsMenu.VisualButton = m_MainMenu->CreateEntity("Options Visual Text");
		auto& visualText = m_OptionsMenu.VisualButton.AddComponent<Teddy::TextComponent>();
		visualText.FontAsset = assets.Load<Teddy::Font>("assets/Fonts/CupheadVogue-ExtraBold.otf", Teddy::Boolean::True);
		visualText.SetString("VISUAL");
		visualText.TextAlignment = Teddy::TextComponent::AlignmentType::Center;
		auto& visualTransform = m_OptionsMenu.VisualButton.GetComponent<Teddy::TransformComponent>();
		visualTransform.Scale *= 0.4f;
		visualTransform.Translation += glm::vec3(0.0f, 0.5f, 0.1f);

		m_OptionsMenu.ControlsButton = m_MainMenu->CreateEntity("Options Controls Text");
		auto& controlsText = m_OptionsMenu.ControlsButton.AddComponent<Teddy::TextComponent>();
		controlsText.FontAsset = assets.Load<Teddy::Font>("assets/Fonts/CupheadVogue-ExtraBold.otf", Teddy::Boolean::True);
		controlsText.SetString("CONTROLS");
		controlsText.TextAlignment = Teddy::TextComponent::AlignmentType::Center;
		auto& controlsTransform = m_OptionsMenu.ControlsButton.GetComponent<Teddy::TransformComponent>();
		controlsTransform.Scale *= 0.4f;
		controlsTransform.Translation += glm::vec3(0.0f, 0.15f, 0.1f);

		m_OptionsMenu.LanguageButton = m_MainMenu->CreateEntity("Options Language Text");
		auto& langText = m_OptionsMenu.LanguageButton.AddComponent<Teddy::TextComponent>();
		langText.FontAsset = assets.Load<Teddy::Font>("assets/Fonts/CupheadVogue-ExtraBold.otf", Teddy::Boolean::True);
		langText.SetString("LANGUAGE");
		langText.TextAlignment = Teddy::TextComponent::AlignmentType::Center;
		auto& langTransform = m_OptionsMenu.LanguageButton.GetComponent<Teddy::TransformComponent>();
		langTransform.Scale *= 0.4f;
		langTransform.Translation += glm::vec3(0.0f, -0.2f, 0.1f);

		m_OptionsMenu.BackButton = m_MainMenu->CreateEntity("Options Back Text");
		auto& backText = m_OptionsMenu.BackButton.AddComponent<Teddy::TextComponent>();
		backText.FontAsset = assets.Load<Teddy::Font>("assets/Fonts/CupheadVogue-ExtraBold.otf", Teddy::Boolean::True);
		backText.SetString("BACK");
		backText.TextAlignment = Teddy::TextComponent::AlignmentType::Center;
		auto& backTransform = m_OptionsMenu.BackButton.GetComponent<Teddy::TransformComponent>();
		backTransform.Scale *= 0.4f;
		backTransform.Translation += glm::vec3(0.0f, -0.55f, 0.1f);

		UpdateOptionsButtonColors();

		InitVisualMenu();
	}

	void MainMenuScene::HideOptionsMenu()
	{
		m_OptionsMenu.Background.GetComponent<Teddy::SpriteRendererComponent>().Color = m_InvisibleColor;
		m_OptionsMenu.AudioButton.GetComponent<Teddy::TextComponent>().Color = m_InvisibleColor;
		m_OptionsMenu.VisualButton.GetComponent<Teddy::TextComponent>().Color = m_InvisibleColor;
		m_OptionsMenu.ControlsButton.GetComponent<Teddy::TextComponent>().Color = m_InvisibleColor;
		m_OptionsMenu.LanguageButton.GetComponent<Teddy::TextComponent>().Color = m_InvisibleColor;
		m_OptionsMenu.BackButton.GetComponent<Teddy::TextComponent>().Color = m_InvisibleColor;
	}

	void MainMenuScene::UpdateOptionsButtonColors()
	{
		if (m_CurrentMenu == 2)
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
			HideOptionsMenu();
		}
	}

	void MainMenuScene::InitMainMenu()
	{
		TED_PROFILE_FUNCTION();

		auto& assets = Teddy::AssetManager::Get();

		// Main Menu Text
		m_MainMenuOptions.PlayButton = m_MainMenu->CreateEntity("Main Menu Start Text");
		auto& titleText = m_MainMenuOptions.PlayButton.AddComponent<Teddy::TextComponent>();
		titleText.FontAsset = assets.Load<Teddy::Font>("assets/Fonts/CupheadVogue-ExtraBold.otf", Teddy::Boolean::True);
		titleText.SetString("START");
		titleText.TextAlignment = Teddy::TextComponent::AlignmentType::Center;
		auto& titleTransform = m_MainMenuOptions.PlayButton.GetComponent<Teddy::TransformComponent>();
		titleTransform.Scale *= 0.4f;
		titleTransform.Translation += glm::vec3(0.0f, 0.8f, 0.0f);

		m_MainMenuOptions.OptionsButton = m_MainMenu->CreateEntity("Main Menu Options Text");
		auto& titleOptionsText = m_MainMenuOptions.OptionsButton.AddComponent<Teddy::TextComponent>();
		titleOptionsText.FontAsset = assets.Load<Teddy::Font>("assets/Fonts/CupheadVogue-ExtraBold.otf", Teddy::Boolean::True);
		titleOptionsText.SetString("OPTIONS");
		titleOptionsText.TextAlignment = Teddy::TextComponent::AlignmentType::Center;
		auto& titleOptionsTransform = m_MainMenuOptions.OptionsButton.GetComponent<Teddy::TransformComponent>();
		titleOptionsTransform.Scale *= 0.4f;
		titleOptionsTransform.Translation += glm::vec3(0.0f, 0.4f, 0.0f);

		m_MainMenuOptions.DlcButton = m_MainMenu->CreateEntity("Main Menu Dlc Text");
		auto& titleDlcText = m_MainMenuOptions.DlcButton.AddComponent<Teddy::TextComponent>();
		titleDlcText.FontAsset = assets.Load<Teddy::Font>("assets/Fonts/CupheadVogue-ExtraBold.otf", Teddy::Boolean::True);
		titleDlcText.SetString("DLC");
		titleDlcText.TextAlignment = Teddy::TextComponent::AlignmentType::Center;
		auto& titleDlcTransform = m_MainMenuOptions.DlcButton.GetComponent<Teddy::TransformComponent>();
		titleDlcTransform.Scale *= 0.4f;
		titleOptionsTransform.Translation += glm::vec3(0.0f, 0.0f, 0.0f);

		m_MainMenuOptions.ExitButton = m_MainMenu->CreateEntity("Main Menu Exit Text");
		auto& titleExitText = m_MainMenuOptions.ExitButton.AddComponent<Teddy::TextComponent>();
		titleExitText.FontAsset = assets.Load<Teddy::Font>("assets/Fonts/CupheadVogue-ExtraBold.otf", Teddy::Boolean::True);
		titleExitText.SetString("EXIT");
		titleExitText.TextAlignment = Teddy::TextComponent::AlignmentType::Center;
		auto& titleExitTransform = m_MainMenuOptions.ExitButton.GetComponent<Teddy::TransformComponent>();
		titleExitTransform.Scale *= 0.4f;
		titleExitTransform.Translation += glm::vec3(0.0f, -0.4f, 0.0f);

		UpdateMainMenuButtonColors();
	}

	void MainMenuScene::OnEvent(Teddy::Event& event)
	{
		TED_PROFILE_FUNCTION();

		Teddy::EventDispatcher dispatcher(event);
		dispatcher.Dispatch<Teddy::KeyPressedEvent>(TED_BIND_EVENT_FN(MainMenuScene::OnKeyPressed));
	}

	void MainMenuScene::UpdateMainMenuButtonColors()
	{
		if (m_CurrentMenu == 0)
		{
			m_MainMenuOptions.PlayButton.GetComponent<Teddy::TextComponent>().Color = (m_MainMenuOptions.CurrentSelection == 0) ? m_WhiteColor : m_GrayColor;
			m_MainMenuOptions.OptionsButton.GetComponent<Teddy::TextComponent>().Color = (m_MainMenuOptions.CurrentSelection == 1) ? m_WhiteColor : m_GrayColor;
			m_MainMenuOptions.DlcButton.GetComponent<Teddy::TextComponent>().Color = (m_MainMenuOptions.CurrentSelection == 2) ? m_WhiteColor : m_GrayColor;
			m_MainMenuOptions.ExitButton.GetComponent<Teddy::TextComponent>().Color = (m_MainMenuOptions.CurrentSelection == 3) ? m_WhiteColor : m_GrayColor;
		}
		else
		{
			HideMainMenu();
		}
	}

	void MainMenuScene::HideMainMenu()
	{
		m_MainMenuOptions.PlayButton.GetComponent<Teddy::TextComponent>().Color = MainMenuScene::m_InvisibleColor;
		m_MainMenuOptions.OptionsButton.GetComponent<Teddy::TextComponent>().Color = MainMenuScene::m_InvisibleColor;
		m_MainMenuOptions.DlcButton.GetComponent<Teddy::TextComponent>().Color = MainMenuScene::m_InvisibleColor;
		m_MainMenuOptions.ExitButton.GetComponent<Teddy::TextComponent>().Color = MainMenuScene::m_InvisibleColor;
	}

	void MainMenuScene::EnterMainMenuOption()
	{
		switch (m_MainMenuOptions.CurrentSelection)
		{
		case 0:
			m_CurrentMenu = 1;
			HideMainMenu();
			break;
		case 1:
			m_CurrentMenu = 2;
			HideMainMenu();
			UpdateOptionsButtonColors();
			break;
		case 2:
			m_CurrentMenu = 3;
			HideMainMenu();
			break;
		case 3:
			Teddy::Application::Get().Close();
			break;
		default:
			break;
		}
	}

	bool MainMenuScene::OnMainMenuKeyPressed(Teddy::KeyPressedEvent& e)
	{
		switch (e.GetKeyCode())
		{
		case Teddy::Key::Down:
		case Teddy::Key::S:
			m_MainMenuOptions.CurrentSelection = (m_MainMenuOptions.CurrentSelection + 1) % 4;
			UpdateMainMenuButtonColors();
			return true;
		case Teddy::Key::Up:
		case Teddy::Key::W:
			m_MainMenuOptions.CurrentSelection = (m_MainMenuOptions.CurrentSelection - 1 + 4) % 4;
			UpdateMainMenuButtonColors();
			return true;
		case Teddy::Key::Return:
			EnterMainMenuOption();
			return true;
		default:
			break;
		}

		return false;
	}

	bool MainMenuScene::OnPlayMenuKeyPressed(Teddy::KeyPressedEvent& e)
	{
		switch (e.GetKeyCode())
		{
		case Teddy::Key::Escape:
			m_CurrentMenu = 0;
			UpdateMainMenuButtonColors();
			return true;
		default:
			break;
		}

		return false;
	}

	bool MainMenuScene::OnOptionsMenuKeyPressed(Teddy::KeyPressedEvent& e)
	{
		switch (e.GetKeyCode())
		{
		case Teddy::Key::Escape:
			m_CurrentMenu = 0;
			HideOptionsMenu();
			UpdateMainMenuButtonColors();
			return true;
		case Teddy::Key::Down:
		case Teddy::Key::S:
			m_OptionsMenu.CurrentSelection = (m_OptionsMenu.CurrentSelection + 1) % 5;
			UpdateOptionsButtonColors();
			return true;
		case Teddy::Key::Up:
		case Teddy::Key::W:
			m_OptionsMenu.CurrentSelection = (m_OptionsMenu.CurrentSelection - 1 + 5) % 5;
			UpdateOptionsButtonColors();
			return true;
		case Teddy::Key::Return:
			return OptionsMenuEnter();
		default:
			break;
		}

		return false;
	}

	bool MainMenuScene::OnDlcMenuKeyPressed(Teddy::KeyPressedEvent& e)
	{
		switch (e.GetKeyCode())
		{
		case Teddy::Key::Escape:
			m_CurrentMenu = 0;
			UpdateMainMenuButtonColors();
			return true;
		default:
			break;
		}

		return false;
	}

	bool MainMenuScene::OnVisualMenuKeyPressed(Teddy::KeyPressedEvent& e)
	{
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
				case 3:
					m_VisualMenu.OverscanBar.Increment();
					UpdateVisualColors();
					break;
			return true;
		case Teddy::Key::Left:
		case Teddy::Key::A:
			switch (m_VisualMenu.CurrentSelection)
				case 3:
					m_VisualMenu.OverscanBar.Decrement();
					UpdateVisualColors();
					break;
			return true;
		default:
			break;
		}

		return false;
	}

	bool MainMenuScene::OnKeyPressed(Teddy::KeyPressedEvent& e)
	{
		TED_PROFILE_FUNCTION();

		switch (m_CurrentMenu)
		{
		case 0: 
			return OnMainMenuKeyPressed(e);
		case 1:
			return OnPlayMenuKeyPressed(e);
		case 2:
			return OnOptionsMenuKeyPressed(e);
		case 3:
			return OnDlcMenuKeyPressed(e);
		case 5:
			return OnVisualMenuKeyPressed(e);
		default:
			break;
		}
		
		return false;
	}
}