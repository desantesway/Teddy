#include "DeathMenu.h"

namespace Cuphead
{
	Teddy::Entity DeathMenu::m_Overlay;
	Teddy::Entity DeathMenu::m_YouDied;
	Teddy::Entity DeathMenu::m_Background;
	Teddy::Entity DeathMenu::m_BossPhoto;
	Teddy::Entity DeathMenu::m_BossQuote;
	Teddy::Entity DeathMenu::m_ProgressBar;
	Teddy::Entity DeathMenu::m_Retry;
	Teddy::Entity DeathMenu::m_ExitToMap;
	Teddy::Entity DeathMenu::m_Quit;
	Teddy::Ref<Teddy::Texture2D> DeathMenu::m_BackgroundTexture;
	Teddy::Ref<Teddy::Font> DeathMenu::m_OptionsFont;
	Teddy::Ref<Teddy::Font> DeathMenu::m_QuoteFont;
	Teddy::Ref<Teddy::Scene> DeathMenu::m_Scene;
	glm::vec4 DeathMenu::m_BlackColor = glm::vec4(70.0f / 255.0f, 70.0f / 255.0f, 70.0f / 255.0f, 1.0f);
	glm::vec4 DeathMenu::m_RedColor = glm::vec4(172.0f / 255.0f, 32.0f / 255.0f, 54.0f / 255.0f, 1.0f);
	int DeathMenu::m_State = 0; // 0 = Menu, 1 = Retry, 2 = Exit to Map, 3 = Quit
	int DeathMenu::m_CurrentSelection = 0; //0 = Retry, 1 = Exit to Map, 2 = Quit

	void DeathMenu::Init(Teddy::Ref<Teddy::Scene> scene)
	{
		auto& assets = Teddy::AssetManager::Get();
		m_BackgroundTexture = assets.Load<Teddy::Texture2D>("assets/Textures/UI/UI_Cards_595x668_2048x2048_0.png", Teddy::Boolean::True);

		m_OptionsFont = assets.Load<Teddy::Font>("assets/Fonts/CupheadVogue-ExtraBold.otf", Teddy::Boolean::True);

		m_Scene = scene;
	}

	void DeathMenu::Show(bool isCuphead, int bossProgress)
	{
		//if (m_Background)
		//	m_Scene->DestroyEntity(m_Background);
		//m_Background = m_Scene->CreateEntity("Death Menu Background");
		//auto& sprite = m_Background.AddComponent<Teddy::SpriteRendererComponent>();
		//sprite.Texture = m_BackgroundTexture;
		//m_Background.AddComponent<Teddy::SpriteAtlasComponent>(1, 2, 595, 668);
		//auto& transform = m_Background.GetComponent<Teddy::TransformComponent>();
		//transform.Translation = glm::vec3(0.0f, 0.0f, 5.01f);
		//transform.Scale *= 3.0f;
		//transform.Rotation.z = glm::radians(0.0f);

		if(m_Overlay)
			m_Scene->DestroyEntity(m_Overlay);
		m_Overlay = m_Scene->CreateEntity("Death Menu Overlay");
		auto& overlaySprite = m_Overlay.AddComponent<Teddy::SpriteRendererComponent>();
		overlaySprite.Color = glm::vec4(0.0f, 0.0f, 0.0f, 0.5f);
		overlaySprite.IsBackground = true;
		auto& overlayTransform = m_Overlay.GetComponent<Teddy::TransformComponent>();
		overlayTransform.Translation = glm::vec3(0.0f, 0.0f, 3.5f);

		if(m_YouDied)
			m_Scene->DestroyEntity(m_YouDied);
	}

	void DeathMenu::Shutdown()
	{
		if(m_Overlay)
			m_Scene->DestroyEntity(m_Overlay);
		m_Overlay = {};
		if (m_YouDied)
			m_Scene->DestroyEntity(m_YouDied);
		m_YouDied = {};
		if (m_Background)
			m_Scene->DestroyEntity(m_Background);
		if (m_BossPhoto)
			m_Scene->DestroyEntity(m_BossPhoto);
		m_BossPhoto = {};
		if (m_BossQuote)
			m_Scene->DestroyEntity(m_BossQuote);
		m_BossQuote = {};
		if (m_ProgressBar)
			m_Scene->DestroyEntity(m_ProgressBar);
		m_ProgressBar = {};
		if (m_Retry)
			m_Scene->DestroyEntity(m_Retry);

		m_BlackColor = glm::vec4(70.0f / 255.0f, 70.0f / 255.0f, 70.0f / 255.0f, 1.0f);
		m_RedColor = glm::vec4(172.0f / 255.0f, 32.0f / 255.0f, 54.0f / 255.0f, 1.0f);

		m_State = 0;
		m_CurrentSelection = 0;

		m_Scene = nullptr;

		m_BackgroundTexture = nullptr;
		m_OptionsFont = nullptr;
		m_QuoteFont = nullptr;
	}
}