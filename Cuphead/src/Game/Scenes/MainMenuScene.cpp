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
	
	unsigned int MainMenuScene::m_CurrentMenu = 0;

	Teddy::Ref<Teddy::Scene> MainMenuScene::Init()
	{
		TED_PROFILE_FUNCTION();

		m_MainMenu = Teddy::CreateRef<Teddy::Scene>(true);

		auto camEntt = m_MainMenu->CreateEntity("Main Menu Camera");
		camEntt.GetComponent<Teddy::TransformComponent>().Translation = { 0.0f, 0.0f, 9.0f };
		auto& cam = camEntt.AddComponent<Teddy::CameraComponent>();
		cam.Camera.SetProjectionType(Teddy::SceneCamera::ProjectionType::Perspective);
		auto& window = Teddy::Application::Get().GetWindow();
		cam.Camera.SetViewportSize(window.GetWidth(), window.GetHeight());

		// Background
		auto& assets = Teddy::AssetManager::Get();

		auto background = m_MainMenu->CreateEntity("Main Menu Background");
		auto& sprite = background.AddComponent<Teddy::SpriteRendererComponent>();
		sprite.IsBackground = true;
		sprite.Texture = assets.Load<Teddy::Texture2D>("assets/Textures/SpriteAtlasTexture-Slot_Select_BG-2048x1024-fmt10.png", Teddy::Boolean::True);

		background.AddComponent<Teddy::SpriteAtlasComponent>(0, 0, 1410, 840);
		background.GetComponent<Teddy::TransformComponent>().Scale *= 1.1f;

		InitMainMenu();
		InitPlayMenu();
		InitOptionsMenu();
		InitDlcMenu();

		return m_MainMenu;
	}

	void MainMenuScene::OnUpdate(Teddy::Timestep ts)
	{
		OnPlayUpdate();
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
		titleTransform.Translation += glm::vec3(0.0f, 0.8f, 0.001f);

		m_MainMenuOptions.OptionsButton = m_MainMenu->CreateEntity("Main Menu Options Text");
		auto& titleOptionsText = m_MainMenuOptions.OptionsButton.AddComponent<Teddy::TextComponent>();
		titleOptionsText.FontAsset = assets.Load<Teddy::Font>("assets/Fonts/CupheadVogue-ExtraBold.otf", Teddy::Boolean::True);
		titleOptionsText.SetString("OPTIONS");
		titleOptionsText.TextAlignment = Teddy::TextComponent::AlignmentType::Center;
		auto& titleOptionsTransform = m_MainMenuOptions.OptionsButton.GetComponent<Teddy::TransformComponent>();
		titleOptionsTransform.Scale *= 0.4f;
		titleOptionsTransform.Translation += glm::vec3(0.0f, 0.4f, 0.001f);

		m_MainMenuOptions.DlcButton = m_MainMenu->CreateEntity("Main Menu Dlc Text");
		auto& titleDlcText = m_MainMenuOptions.DlcButton.AddComponent<Teddy::TextComponent>();
		titleDlcText.FontAsset = assets.Load<Teddy::Font>("assets/Fonts/CupheadVogue-ExtraBold.otf", Teddy::Boolean::True);
		titleDlcText.SetString("DLC");
		titleDlcText.TextAlignment = Teddy::TextComponent::AlignmentType::Center;
		auto& titleDlcTransform = m_MainMenuOptions.DlcButton.GetComponent<Teddy::TransformComponent>();
		titleDlcTransform.Scale *= 0.4f;
		titleDlcTransform.Translation += glm::vec3(0.0f, 0.0f, 0.001f);

		m_MainMenuOptions.ExitButton = m_MainMenu->CreateEntity("Main Menu Exit Text");
		auto& titleExitText = m_MainMenuOptions.ExitButton.AddComponent<Teddy::TextComponent>();
		titleExitText.FontAsset = assets.Load<Teddy::Font>("assets/Fonts/CupheadVogue-ExtraBold.otf", Teddy::Boolean::True);
		titleExitText.SetString("EXIT");
		titleExitText.TextAlignment = Teddy::TextComponent::AlignmentType::Center;
		auto& titleExitTransform = m_MainMenuOptions.ExitButton.GetComponent<Teddy::TransformComponent>();
		titleExitTransform.Scale *= 0.4f;
		titleExitTransform.Translation += glm::vec3(0.0f, -0.4f, 0.001f);

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
			UpdatePlayButtonColors();
			break;
		case 1:
			m_CurrentMenu = 2;
			HideMainMenu();
			UpdateOptionsButtonColors();
			break;
		case 2:
			m_CurrentMenu = 3;
			HideMainMenu();
			UpdateDlcButtonColors();
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