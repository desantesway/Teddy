#include "DeathMenu.h"

namespace Cuphead
{
	void DeathMenu::OnUpdate(Teddy::Timestep ts, bool isCuphead, int bossProgress)
	{
		if (!m_Started) return;

		static float timer = 0.0f;
		timer += ts.GetSeconds();

		if (!m_LoadedMenu && timer >= 2.0f)
		{
			if (m_YouDied)
				m_Scene->DestroyEntity(m_YouDied);

			if (m_Background)
				m_Scene->DestroyEntity(m_Background);
			m_Background = m_Scene->CreateEntity("Death Menu Background");
			auto& sprite = m_Background.AddComponent<Teddy::SpriteRendererComponent>();
			sprite.Texture = m_BackgroundTexture;
			m_Background.AddComponent<Teddy::SpriteAtlasComponent>(1, 2, 595, 668);
			auto& transform = m_Background.GetComponent<Teddy::TransformComponent>();
			transform.Translation = glm::vec3(0.0f, 0.0f, 5.01f);
			transform.Scale *= 3.0f;
			transform.Rotation.z = glm::radians(0.0f);
		}
	}

	void DeathMenu::Init(Teddy::Ref<Teddy::Scene> scene)
	{
		auto& assets = Teddy::AssetManager::Get();
		m_BackgroundTexture = assets.Load<Teddy::Texture2D>("assets/Textures/UI/UI_Cards_595x668_2048x2048_0.png", Teddy::Boolean::True);

		m_YouDiedTexture = assets.LoadMultiple<Teddy::Texture2D>({ 
			"assets/Textures/UI/Death/You_Died_1076x232_2048x2048_0.png",
			"assets/Textures/UI/Death/You_Died_1076x232_2048x2048_1.png",
			"assets/Textures/UI/Death/You_Died_1076x232_2048x2048_2.png" });

		m_OptionsFont = assets.Load<Teddy::Font>("assets/Fonts/CupheadVogue-ExtraBold.otf", Teddy::Boolean::True);

		m_Scene = scene;
	}

	void DeathMenu::Show()
	{
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
		m_YouDied = m_Scene->CreateEntity("You Died Text");
		auto& youDiedSprite = m_YouDied.AddComponent<Teddy::SpriteAnimationComponent>(0.05f, 0.05f, 0.05f);
		youDiedSprite.Textures = m_YouDiedTexture;
		m_YouDied.AddComponent<Teddy::SpriteAtlasComponent>(0, 0, 1076, 232);
		while (youDiedSprite.PlayableIndicies.back() != 19)
			youDiedSprite.PlayableIndicies.pop_back();
		auto& youDiedTransform = m_YouDied.GetComponent<Teddy::TransformComponent>();
		youDiedTransform.Translation = glm::vec3(0.0f, 0.0f, 5.0f);

		m_Started = true;
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
		m_YouDiedTexture.clear();

		m_Started = false;
	}
}