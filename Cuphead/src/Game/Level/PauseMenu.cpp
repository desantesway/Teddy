#include "PauseMenu.h"

#include <Teddy.h>

// TODO: make menu atlas

namespace Cuphead
{

	void PauseMenu::OnUpdate(Teddy::Timestep ts)
	{
		if (m_State == 3 && m_OptionsMenu.WantsToClose())
		{
			m_State = 0;
			m_OptionsMenu.Hide();
			PartialShow();
			UpdateColors();
		}
	}

	void PauseMenu::Init(Teddy::Ref<Teddy::Scene> scene)
	{
		auto& assets = Teddy::AssetManager::Get();
		m_BackgroundTexture = assets.Load<Teddy::Texture2D>("assets/Textures/UI/UI_Cards_595x668_2048x2048_0.png", Teddy::Boolean::True);

		m_OptionsFont = assets.Load<Teddy::Font>("assets/Fonts/CupheadVogue-ExtraBold.otf", Teddy::Boolean::True);

		m_OptionsMenu.Init(scene);

		m_Scene = scene;
	}

	void PauseMenu::PartialShow()
	{
		if(m_Background)
			m_Scene->DestroyEntity(m_Background);
		m_Background = m_Scene->CreateEntity("Pause Menu Background");
		auto& sprite = m_Background.AddComponent<Teddy::SpriteRendererComponent>();
		sprite.Texture = m_BackgroundTexture;
		m_Background.AddComponent<Teddy::SpriteAtlasComponent>(0, 0, 595, 668);
		auto& transform = m_Background.GetComponent<Teddy::TransformComponent>();
		transform.Translation = glm::vec3(0.0f, 0.8f, 5.01f);
		transform.Scale *= 3.0f;

		if (m_Resume)
			m_Scene->DestroyEntity(m_Resume);
		m_Resume = m_Scene->CreateEntity("Pause Menu Resume Option");
		auto& text = m_Resume.AddComponent<Teddy::TextComponent>();
		text.FontAsset = m_OptionsFont;
		text.SetString("RESUME");
		text.TextAlignment = Teddy::TextComponent::AlignmentType::Center;
		auto& textTransform = m_Resume.GetComponent<Teddy::TransformComponent>();
		textTransform.Scale *= 0.175f;
		textTransform.Translation = glm::vec3(0.0f, 0.35f, 5.1f);

		if (m_Retry)
			m_Scene->DestroyEntity(m_Retry);
		m_Retry = m_Scene->CreateEntity("Pause Menu Retry Option");
		auto& rtext = m_Retry.AddComponent<Teddy::TextComponent>();
		rtext.FontAsset = m_OptionsFont;
		rtext.SetString("RETRY");
		rtext.TextAlignment = Teddy::TextComponent::AlignmentType::Center;
		auto& rtextTransform = m_Retry.GetComponent<Teddy::TransformComponent>();
		rtextTransform.Scale *= 0.175f;
		rtextTransform.Translation = glm::vec3(0.0f, 0.175f, 5.1f);

		if (m_Options)
			m_Scene->DestroyEntity(m_Options);
		m_Options = m_Scene->CreateEntity("Pause Menu Options Option");
		auto& otext = m_Options.AddComponent<Teddy::TextComponent>();
		otext.FontAsset = m_OptionsFont;
		otext.SetString("OPTIONS");
		otext.TextAlignment = Teddy::TextComponent::AlignmentType::Center;
		auto& otextTransform = m_Options.GetComponent<Teddy::TransformComponent>();
		otextTransform.Scale *= 0.175f;
		otextTransform.Translation = glm::vec3(0.0f, 0.00f, 5.1f);

		if (m_Exit)
			m_Scene->DestroyEntity(m_Exit);
		m_Exit = m_Scene->CreateEntity("Pause Menu Exit Option");
		auto& etext = m_Exit.AddComponent<Teddy::TextComponent>();
		etext.FontAsset = m_OptionsFont;
		etext.SetString("EXIT TO MAP");
		etext.TextAlignment = Teddy::TextComponent::AlignmentType::Center;
		auto& etextTransform = m_Exit.GetComponent<Teddy::TransformComponent>();
		etextTransform.Scale *= 0.175f;
		etextTransform.Translation = glm::vec3(0.0f, -0.175f, 5.1f);

		m_State = 0;
		m_CurrentSelection = 0;

		UpdateColors();
	}

	void PauseMenu::Show()
	{
		if (m_Overlay)
			m_Scene->DestroyEntity(m_Overlay);
		m_Overlay = m_Scene->CreateEntity("Pause Menu Overlay");
		auto& overlaySprite = m_Overlay.AddComponent<Teddy::SpriteRendererComponent>();
		overlaySprite.Color = glm::vec4(0.0f, 0.0f, 0.0f, 0.5f);
		overlaySprite.IsBackground = true;
		auto& overlayTransform = m_Overlay.GetComponent<Teddy::TransformComponent>();
		overlayTransform.Translation = glm::vec3(0.0f, 0.0f, 3.5f);

		PartialShow();
	}

	void PauseMenu::UpdateColors()
	{
		if (m_State == 0)
		{
			m_Resume.GetComponent<Teddy::TextComponent>().Color = m_CurrentSelection == 0 ? m_RedColor : m_BlackColor;
			m_Retry.GetComponent<Teddy::TextComponent>().Color = m_CurrentSelection == 1 ? m_RedColor : m_BlackColor;
			m_Options.GetComponent<Teddy::TextComponent>().Color = m_CurrentSelection == 2 ? m_RedColor : m_BlackColor;
			m_Exit.GetComponent<Teddy::TextComponent>().Color = m_CurrentSelection == 3 ? m_RedColor : m_BlackColor;
		}
	}

	void PauseMenu::Hide()
	{
		PartialHide();

		m_Scene->DestroyEntity(m_Overlay);
		m_Overlay = {};
	}

	void PauseMenu::PartialHide()
	{
		m_Scene->DestroyEntity(m_Background);
		m_Background = {};

		m_Scene->DestroyEntity(m_Resume);
		m_Resume = {};

		m_Scene->DestroyEntity(m_Options);
		m_Options = {};

		m_Scene->DestroyEntity(m_Retry);
		m_Retry = {};

		m_Scene->DestroyEntity(m_Exit);
		m_Exit = {};
	}

	bool PauseMenu::OnKeyPressed(Teddy::KeyPressedEvent& e)
	{
		switch (m_State)
		{
		case 0:
		{
			switch (e.GetKeyCode())
			{
			case Teddy::Key::Escape:
				m_State = 1;
				Hide();
				return true;
			case Teddy::Key::Down:
			case Teddy::Key::S:
				m_CurrentSelection = (m_CurrentSelection + 1) % 4;
				UpdateColors();
				return true;
			case Teddy::Key::Up:
			case Teddy::Key::W:
				m_CurrentSelection = (m_CurrentSelection - 1 + 4) % 4;
				UpdateColors();
				return true;
			case Teddy::Key::Return:
				switch (m_CurrentSelection)
				{
				case 0:
					m_State = 1;
					break;
				case 1:
					m_State = 2;
					break;
				case 2:
					m_State = 3;
					PartialHide();
					m_OptionsMenu.Show();
					break;
				case 3:
					m_State = 4;
					break;
				default:
					break;
				}
				return true;
			default:
				break;
			}
			break;
		}
		case 3:
			m_OptionsMenu.OnKeyPressed(e);
			break;
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