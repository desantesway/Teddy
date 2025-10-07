#include "MainMenuScene.h"

#include <Teddy.h>

namespace Cuphead
{
	glm::vec4 MainMenuScene::m_HighlightColor = glm::vec4(1.0f);
	glm::vec4 MainMenuScene::m_NormalColor = glm::vec4(110.0f / 255.0f, 110.0f / 255.0f, 110.0f / 255.0f, 1.0f);
	glm::vec4 MainMenuScene::m_InvisibleColor = glm::vec4(0.0f);

	Teddy::Entity MainMenuScene::m_PlayButton;
	Teddy::Entity MainMenuScene::m_OptionsButton;
	Teddy::Entity MainMenuScene::m_DlcButton;
	Teddy::Entity MainMenuScene::m_ExitButton;
	unsigned int MainMenuScene::m_CurrentMainMenuSelection = 0;
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
		
		// Main Menu Text
		m_PlayButton = m_MainMenu->CreateEntity("Main Menu Start Text");
		auto& titleText = m_PlayButton.AddComponent<Teddy::TextComponent>();
		titleText.FontAsset = assets.Load<Teddy::Font>("assets/Fonts/CupheadVogue-ExtraBold.otf", Teddy::Boolean::True);
		titleText.SetString("START");
		titleText.TextAlignment = Teddy::TextComponent::AlignmentType::Center;
		auto& titleTransform = m_PlayButton.GetComponent<Teddy::TransformComponent>();
		titleTransform.Scale *= 0.4f;
		titleTransform.Translation += glm::vec3(0.0f, 0.8f, 0.0f);

		m_OptionsButton = m_MainMenu->CreateEntity("Main Menu Options Text");
		auto& titleOptionsText = m_OptionsButton.AddComponent<Teddy::TextComponent>();
		titleOptionsText.FontAsset = assets.Load<Teddy::Font>("assets/Fonts/CupheadVogue-ExtraBold.otf", Teddy::Boolean::True);
		titleOptionsText.SetString("OPTIONS");
		titleOptionsText.TextAlignment = Teddy::TextComponent::AlignmentType::Center;
		auto& titleOptionsTransform = m_OptionsButton.GetComponent<Teddy::TransformComponent>();
		titleOptionsTransform.Scale *= 0.4f;
		titleOptionsTransform.Translation += glm::vec3(0.0f, 0.4f, 0.0f);

		m_DlcButton = m_MainMenu->CreateEntity("Main Menu Dlc Text");
		auto& titleDlcText = m_DlcButton.AddComponent<Teddy::TextComponent>();
		titleDlcText.FontAsset = assets.Load<Teddy::Font>("assets/Fonts/CupheadVogue-ExtraBold.otf", Teddy::Boolean::True);
		titleDlcText.SetString("DLC");
		titleDlcText.TextAlignment = Teddy::TextComponent::AlignmentType::Center;
		auto& titleDlcTransform = m_DlcButton.GetComponent<Teddy::TransformComponent>();
		titleDlcTransform.Scale *= 0.4f;
		titleOptionsTransform.Translation += glm::vec3(0.0f, 0.0f, 0.0f);

		m_ExitButton = m_MainMenu->CreateEntity("Main Menu Exit Text");
		auto& titleExitText = m_ExitButton.AddComponent<Teddy::TextComponent>();
		titleExitText.FontAsset = assets.Load<Teddy::Font>("assets/Fonts/CupheadVogue-ExtraBold.otf", Teddy::Boolean::True);
		titleExitText.SetString("EXIT");
		titleExitText.TextAlignment = Teddy::TextComponent::AlignmentType::Center;
		auto& titleExitTransform = m_ExitButton.GetComponent<Teddy::TransformComponent>();
		titleExitTransform.Scale *= 0.4f;
		titleExitTransform.Translation += glm::vec3(0.0f, -0.4f, 0.0f);

		UpdateButtonColor();

		// Options Menu
		auto pauseBackground = m_MainMenu->CreateEntity("Options Background");
		auto& pauseSprite = pauseBackground.AddComponent<Teddy::SpriteRendererComponent>();
		pauseSprite.Texture = assets.Load<Teddy::Texture2D>("Options Background", "assets/Textures/SpriteAtlasTexture-Pause (Group 0)-1024x1024-fmt12.png");
		pauseSprite.Color = glm::vec4(1.0f, 1.0f, 1.0f, 0.0f);
		pauseBackground.AddComponent<Teddy::SpriteAtlasComponent>(0, 0, 512, 304);
		auto& pauseTransform = pauseBackground.GetComponent<Teddy::TransformComponent>();
		pauseTransform.Translation = glm::vec3(0.0f, 0.0f, 1.0f);
		pauseTransform.Scale *= 2.6f;

		return m_MainMenu;
	}

	void MainMenuScene::OnEvent(Teddy::Event& event)
	{
		TED_PROFILE_FUNCTION();

		Teddy::EventDispatcher dispatcher(event);
		dispatcher.Dispatch<Teddy::KeyPressedEvent>(TED_BIND_EVENT_FN(MainMenuScene::OnKeyPressed));
	}

	void MainMenuScene::UpdateButtonColor()
	{
		m_PlayButton.GetComponent<Teddy::TextComponent>().Color = (m_CurrentMainMenuSelection == 0) ? m_HighlightColor : m_NormalColor;
		m_OptionsButton.GetComponent<Teddy::TextComponent>().Color = (m_CurrentMainMenuSelection == 1) ? m_HighlightColor : m_NormalColor;
		m_DlcButton.GetComponent<Teddy::TextComponent>().Color = (m_CurrentMainMenuSelection == 2) ? m_HighlightColor : m_NormalColor;
		m_ExitButton.GetComponent<Teddy::TextComponent>().Color = (m_CurrentMainMenuSelection == 3) ? m_HighlightColor : m_NormalColor;
	}

	void MainMenuScene::HideMainMenu()
	{
		MainMenuScene::m_PlayButton.GetComponent<Teddy::TextComponent>().Color = MainMenuScene::m_InvisibleColor;
		MainMenuScene::m_OptionsButton.GetComponent<Teddy::TextComponent>().Color = MainMenuScene::m_InvisibleColor;
		MainMenuScene::m_DlcButton.GetComponent<Teddy::TextComponent>().Color = MainMenuScene::m_InvisibleColor;
		MainMenuScene::m_ExitButton.GetComponent<Teddy::TextComponent>().Color = MainMenuScene::m_InvisibleColor;
	}

	void MainMenuScene::EnterOption()
	{
		switch (m_CurrentMainMenuSelection)
		{
		case 0:
			m_CurrentMenu = 1;
			HideMainMenu();
			break;
		case 1:
			m_CurrentMenu = 2;
			HideMainMenu();
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

	bool MainMenuScene::OnKeyPressed(Teddy::KeyPressedEvent& e)
	{
		TED_PROFILE_FUNCTION();

		switch (m_CurrentMenu)
		{
		case 0: 
			switch (e.GetKeyCode())
			{
			case Teddy::Key::Down:
			case Teddy::Key::S:
				m_CurrentMainMenuSelection = (m_CurrentMainMenuSelection + 1) % 4;
				UpdateButtonColor();
				break;
			case Teddy::Key::Up:
			case Teddy::Key::W:
				m_CurrentMainMenuSelection = (m_CurrentMainMenuSelection - 1 + 4) % 4;
				UpdateButtonColor();
				break;
			case Teddy::Key::Return:
				EnterOption();
				break;
			default:
				break;
			}
			break;
		case 1:
			switch (e.GetKeyCode())
			{
			case Teddy::Key::Escape:
				m_CurrentMenu = 0;
				UpdateButtonColor();
				break;
			default:
				break;
			}
			break;
		case 2:
			switch (e.GetKeyCode())
			{
			case Teddy::Key::Escape:
				m_CurrentMenu = 0;
				UpdateButtonColor();
				break;
			default:
				break;
			}
			break;
		case 3:
			switch (e.GetKeyCode())
			{
			case Teddy::Key::Escape:
				m_CurrentMenu = 0;
				UpdateButtonColor();
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