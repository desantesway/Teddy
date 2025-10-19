#include "DeathMenu.h"

#include <Teddy.h>

#include <imgui.h>
#include <ImGuizmo.h>
#include <glm/gtc/type_ptr.hpp>

namespace Cuphead
{
	// TODO: fix this and put it on Teddy::Math
	inline void SetRotationWithAnchor(Teddy::TransformComponent& transform, const glm::vec3& anchor, float rotationToApply, const glm::vec3& translation, const glm::vec3& scale = glm::vec3(1.0f)) // TODO: Teddy::Math
	{
		glm::mat4 toAnchor = glm::translate(glm::mat4(1.0f), anchor);

		glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), rotationToApply, glm::vec3(0, 0, 1)) * glm::scale(glm::mat4(1.0f), scale);

		glm::mat4 fromAnchor = glm::translate(glm::mat4(1.0f), -anchor + translation);

		glm::mat4 anchorTransform = toAnchor * rotation * fromAnchor;

		glm::vec3 rotationEuler;
		ImGuizmo::DecomposeMatrixToComponents(
			glm::value_ptr(anchorTransform),
			glm::value_ptr(transform.Translation),
			glm::value_ptr(rotationEuler),
			glm::value_ptr(transform.Scale)
		);
		transform.Rotation = glm::radians(rotationEuler);
	}

	void DeathMenu::OnUpdate(Teddy::Timestep ts, int bossProgress)
	{
		if (!m_Started) return;

		static float timer = 0.0f;
		timer += ts.GetSeconds();

		if (!m_LoadedMenu)
		{
			if (timer >= 1.5f)
			{
				if (!m_YouDiedFadeOut)
				{
					auto& youDiedSprite = m_YouDied.GetComponent<Teddy::SpriteAnimationComponent>();
					youDiedSprite.Color -= glm::vec4(1.0f) * ts.GetSeconds() * 2.0f;

					if (youDiedSprite.Color.r <= 0.0f)
					{
						youDiedSprite.Color = glm::vec4(0.0f);
						m_YouDiedFadeOut = true;
					}
				}
				else
				{
					if (m_YouDied)
						m_Scene->DestroyEntity(m_YouDied);

					if (m_Background)
						m_Scene->DestroyEntity(m_Background);
					m_Background = m_Scene->CreateEntity("Death Menu Background");
					auto& sprite = m_Background.AddComponent<Teddy::SpriteRendererComponent>();
					sprite.Texture = m_BackgroundTexture;
					sprite.Color = glm::vec4(0.0f);
					m_Background.AddComponent<Teddy::SpriteAtlasComponent>(1, 2, 595, 668);
					auto& transform = m_Background.GetComponent<Teddy::TransformComponent>();
					transform.Translation = glm::vec3(0.0f, 0.0f, 5.01f);
					transform.Scale *= 3.0f;
					transform.Rotation.z = glm::radians(30.0f);

					if(m_EnemyPhoto)
						m_Scene->DestroyEntity(m_EnemyPhoto);
					m_EnemyPhoto = m_Scene->CreateEntity("Enemy Photo");
					auto& photoSprite = m_EnemyPhoto.AddComponent<Teddy::SpriteRendererComponent>();
					photoSprite.Texture = m_EnemyTexture;
					photoSprite.Color = glm::vec4(0.0f);
					m_EnemyPhoto.AddComponent<Teddy::SpriteAtlasComponent>(0, 0, 298, 232); // TODO: change based on enemy progress
					SetRotationWithAnchor(m_EnemyPhoto.GetComponent<Teddy::TransformComponent>(), glm::vec3(0.0f), glm::radians(30.0f), glm::vec3(0.0f, 0.8f, 5.1f));

					if(m_EnemyQuote)
						m_Scene->DestroyEntity(m_EnemyQuote);
					m_EnemyQuote = m_Scene->CreateEntity("Enemy Quote");
					auto& quoteText = m_EnemyQuote.AddComponent<Teddy::TextComponent>();
					quoteText.FontAsset = m_QuoteFont;
					// TODO: Set quote based on boss progress
					quoteText.SetString("\"One..two..three!");
					quoteText.TextAlignment = Teddy::TextComponent::AlignmentType::Center;
					quoteText.Color = glm::vec4(0.0f);
					auto& quoteTransform = m_EnemyQuote.GetComponent<Teddy::TransformComponent>();
					SetRotationWithAnchor(m_EnemyQuote.GetComponent<Teddy::TransformComponent>(), 
						glm::vec3(0.0f), glm::radians(30.0f), 
						glm::vec3(0.0f, 1.0f, 5.11f), glm::vec3(0.15f, 0.15f, 1.0f));

					if(m_EnemyQuotePart2)
						m_Scene->DestroyEntity(m_EnemyQuotePart2);
					m_EnemyQuotePart2 = m_Scene->CreateEntity("Enemy Quote Part 2");
					auto& quoteText2 = m_EnemyQuotePart2.AddComponent<Teddy::TextComponent>();
					quoteText2.FontAsset = m_QuoteFont;
					quoteText2.SetString("You'd b-b-b-etter flee !\"");
					quoteText2.TextAlignment = Teddy::TextComponent::AlignmentType::Center;
					quoteText2.Color = glm::vec4(0.0f);
					auto& quoteTransform2 = m_EnemyQuotePart2.GetComponent<Teddy::TransformComponent>();
					SetRotationWithAnchor(m_EnemyQuotePart2.GetComponent<Teddy::TransformComponent>(),
						glm::vec3(0.0f), glm::radians(30.0f),
						glm::vec3(0.0f, 0.6f, 5.11f), glm::vec3(0.15f, 0.15f, 1.0f));

					if(m_ProgressBar)
						m_Scene->DestroyEntity(m_ProgressBar);
					m_ProgressBar = m_Scene->CreateEntity("Progress Bar");
					auto& progressSprite = m_ProgressBar.AddComponent<Teddy::SpriteRendererComponent>();
					progressSprite.Texture = m_BackgroundTexture;
					progressSprite.Color = glm::vec4(0.0f);
					m_ProgressBar.AddComponent<Teddy::SpriteAtlasComponent>(2, 0, 595, 668);
					auto& progressTransform = m_ProgressBar.GetComponent<Teddy::TransformComponent>();
					progressTransform.Scale *= 3.0f;
					progressTransform.Translation = glm::vec3(0.0f, 0.0f, 5.09f);
					progressTransform.Rotation.z = glm::radians(30.0f);

					// checkpoints + cuphead

					if(m_Retry)
						m_Scene->DestroyEntity(m_Retry);
					m_Retry = m_Scene->CreateEntity("Death Menu Retry Option");
					auto& rtext = m_Retry.AddComponent<Teddy::TextComponent>();
					rtext.FontAsset = m_OptionsFont;
					rtext.SetString("RETRY");
					rtext.TextAlignment = Teddy::TextComponent::AlignmentType::Center;
					rtext.Color = glm::vec4(0.0f);
					auto& rtextTransform = m_Retry.GetComponent<Teddy::TransformComponent>();
					SetRotationWithAnchor(m_Retry.GetComponent<Teddy::TransformComponent>(), glm::vec3(0.0f), glm::radians(30.0f), glm::vec3(0.0f, -3.0f, 5.11f), glm::vec3(0.2f, 0.2f, 1.0f));

					if(m_ExitToMap)
						m_Scene->DestroyEntity(m_ExitToMap);
					m_ExitToMap = m_Scene->CreateEntity("Death Menu Exit to Map Option");
					auto& etext = m_ExitToMap.AddComponent<Teddy::TextComponent>();
					etext.FontAsset = m_OptionsFont;
					etext.SetString("EXIT TO MAP");
					etext.TextAlignment = Teddy::TextComponent::AlignmentType::Center;
					etext.Color = glm::vec4(0.0f);
					auto& etextTransform = m_ExitToMap.GetComponent<Teddy::TransformComponent>();
					SetRotationWithAnchor(m_ExitToMap.GetComponent<Teddy::TransformComponent>(), glm::vec3(0.0f), glm::radians(30.0f), glm::vec3(0.0f, -4.0f, 5.11f), glm::vec3(0.2f, 0.2f, 1.0f));

					if (m_Quit)
						m_Scene->DestroyEntity(m_Quit);
					m_Quit = m_Scene->CreateEntity("Death Menu Quit Option");
					auto& qtext = m_Quit.AddComponent<Teddy::TextComponent>();
					qtext.FontAsset = m_OptionsFont;
					qtext.SetString("QUIT GAME");
					qtext.TextAlignment = Teddy::TextComponent::AlignmentType::Center;
					qtext.Color = glm::vec4(0.0f);
					auto& qtextTransform = m_Quit.GetComponent<Teddy::TransformComponent>();
					SetRotationWithAnchor(m_Quit.GetComponent<Teddy::TransformComponent>(), glm::vec3(0.0f), glm::radians(30.0f), glm::vec3(0.0f, -4.8f, 5.11f), glm::vec3(0.2f, 0.2f, 1.0f));

					UpdateColors();

					m_LoadedMenu = true;
					timer = 0.0f;
				}
			}
			return;
		}
		else if (!m_AnimationDone)
		{
			const float targetAngle = glm::radians(5.0f);
			const float startAngle = glm::radians(30.0f);/* set this to your initial angle, e.g., glm::radians(90.0f) */
			static float elapsed = 0.0f;
			const float duration = 2.0f;
			elapsed += ts;
			float t = glm::clamp(elapsed / duration, 0.0f, 1.0f);

			float ease = 1.0f - std::pow(1.0f - t, 7);

			m_Background.GetComponent<Teddy::TransformComponent>().Rotation.z = glm::mix(startAngle, targetAngle, ease);
			m_ProgressBar.GetComponent<Teddy::TransformComponent>().Rotation.z = glm::mix(startAngle, targetAngle, ease);

			SetRotationWithAnchor(m_EnemyPhoto.GetComponent<Teddy::TransformComponent>(),
				glm::vec3(0.0f), glm::mix(startAngle, targetAngle, ease), 
				glm::vec3(0.0f, 0.8f, 5.1f));
			SetRotationWithAnchor(m_Retry.GetComponent<Teddy::TransformComponent>(),
				glm::vec3(0.0f), glm::mix(startAngle, targetAngle, ease), 
				glm::vec3(0.0f, -3.0f, 5.11f), glm::vec3(0.2f, 0.2f, 1.0f));
			SetRotationWithAnchor(m_ExitToMap.GetComponent<Teddy::TransformComponent>(),
				glm::vec3(0.0f), glm::mix(startAngle, targetAngle, ease),
				glm::vec3(0.0f, -4.0f, 5.11f), glm::vec3(0.2f, 0.2f, 1.0f));
			SetRotationWithAnchor(m_Quit.GetComponent<Teddy::TransformComponent>(),
				glm::vec3(0.0f), glm::mix(startAngle, targetAngle, ease),
				glm::vec3(0.0f, -4.8f, 5.11f), glm::vec3(0.2f, 0.2f, 1.0f));
			SetRotationWithAnchor(m_EnemyQuote.GetComponent<Teddy::TransformComponent>(),
				glm::vec3(0.0f), glm::mix(startAngle, targetAngle, ease),
				glm::vec3(0.0f, 1.0f, 5.11f), glm::vec3(0.15f, 0.15f, 1.0f));
			SetRotationWithAnchor(m_EnemyQuotePart2.GetComponent<Teddy::TransformComponent>(),
				glm::vec3(0.0f), glm::mix(startAngle, targetAngle, ease),
				glm::vec3(0.0f, 0.0f, 5.11f), glm::vec3(0.15f, 0.15f, 1.0f));

			if (t >= 1.0f)
			{
				m_Background.GetComponent<Teddy::TransformComponent>().Rotation.z = targetAngle;
				m_ProgressBar.GetComponent<Teddy::TransformComponent>().Rotation.z = targetAngle;
				SetRotationWithAnchor(m_EnemyPhoto.GetComponent<Teddy::TransformComponent>(),
					glm::vec3(0.0f), targetAngle, 
					glm::vec3(0.0f, 0.8f, 5.1f));
				SetRotationWithAnchor(m_Retry.GetComponent<Teddy::TransformComponent>(),
					glm::vec3(0.0f), targetAngle, 
					glm::vec3(0.0f, -3.0f, 5.11f), glm::vec3(0.2f, 0.2f, 1.0f));
				SetRotationWithAnchor(m_ExitToMap.GetComponent<Teddy::TransformComponent>(),
					glm::vec3(0.0f), targetAngle,
					glm::vec3(0.0f, -4.0f, 5.11f), glm::vec3(0.2f, 0.2f, 1.0f));
				SetRotationWithAnchor(m_Quit.GetComponent<Teddy::TransformComponent>(),
					glm::vec3(0.0f), targetAngle,
					glm::vec3(0.0f, -4.8f, 5.11f), glm::vec3(0.2f, 0.2f, 1.0f));
				SetRotationWithAnchor(m_EnemyQuote.GetComponent<Teddy::TransformComponent>(),
					glm::vec3(0.0f), targetAngle,
					glm::vec3(0.0f, 1.0f, 5.11f), glm::vec3(0.15f, 0.15f, 1.0f));
				SetRotationWithAnchor(m_EnemyQuotePart2.GetComponent<Teddy::TransformComponent>(),
					glm::vec3(0.0f), targetAngle,
					glm::vec3(0.0f, 0.0f, 5.11f), glm::vec3(0.15f, 0.15f, 1.0f));
				m_AnimationDone = true;
				timer = 0.0f;
				elapsed = 0.0f;
			}

			m_Background.GetComponent<Teddy::SpriteRendererComponent>().Color = glm::vec4(ease, ease, ease, ease);
			m_EnemyPhoto.GetComponent<Teddy::SpriteRendererComponent>().Color = glm::vec4(ease, ease, ease, ease);
			m_ProgressBar.GetComponent<Teddy::SpriteRendererComponent>().Color = glm::vec4(ease, ease, ease, ease);
			m_EnemyQuote.GetComponent<Teddy::TextComponent>().Color = m_BlackColor * glm::vec4(ease, ease, ease, ease);
			m_EnemyQuotePart2.GetComponent<Teddy::TextComponent>().Color = m_BlackColor * glm::vec4(ease, ease, ease, ease);
			
			UpdateColors(glm::vec4(ease, ease, ease, ease));

			return;
		}

		timer = 0.0f;
	}

	void DeathMenu::Init(Teddy::Ref<Teddy::Scene> scene, bool isCuphead)
	{
		auto& assets = Teddy::AssetManager::Get();
		m_BackgroundTexture = assets.Load<Teddy::Texture2D>("assets/Textures/UI/UI_Cards_595x668_2048x2048_0.png", Teddy::Boolean::True);

		m_EnemyTexture = assets.Load<Teddy::Texture2D>("assets/Textures/Dragon/DeathCard/Dragon_Death_Card_298x232_1024x1024_0.png", Teddy::Boolean::True);

		m_YouDiedTexture = assets.LoadMultiple<Teddy::Texture2D>({ 
			"assets/Textures/UI/Death/You_Died_1076x232_2048x2048_0.png",
			"assets/Textures/UI/Death/You_Died_1076x232_2048x2048_1.png",
			"assets/Textures/UI/Death/You_Died_1076x232_2048x2048_2.png" });

		m_OptionsFont = assets.Load<Teddy::Font>("assets/Fonts/CupheadVogue-ExtraBold.otf", Teddy::Boolean::True);

		m_QuoteFont = assets.Load<Teddy::Font>("assets/Fonts/CupheadMemphis-Medium.otf", Teddy::Boolean::True);

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

	void DeathMenu::UpdateColors(glm::vec4 ToMultiply)
	{
		if(m_State == 0)
		{
			m_Retry.GetComponent<Teddy::TextComponent>().Color = m_CurrentSelection == 0 ? m_RedColor * ToMultiply : m_BlackColor * ToMultiply;
			m_ExitToMap.GetComponent<Teddy::TextComponent>().Color = m_CurrentSelection == 1 ? m_RedColor * ToMultiply : m_BlackColor * ToMultiply;
			m_Quit.GetComponent<Teddy::TextComponent>().Color = m_CurrentSelection == 2 ? m_RedColor * ToMultiply : m_BlackColor * ToMultiply;
		}
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
		m_Background = {};
		if (m_EnemyPhoto)
			m_Scene->DestroyEntity(m_EnemyPhoto);
		m_EnemyPhoto = {};
		if (m_EnemyQuote)
			m_Scene->DestroyEntity(m_EnemyQuote);
		m_EnemyQuote = {};
		if (m_EnemyQuotePart2)
			m_Scene->DestroyEntity(m_EnemyQuotePart2);
		m_EnemyQuotePart2 = {};
		if (m_ProgressBar)
			m_Scene->DestroyEntity(m_ProgressBar);
		m_ProgressBar = {};
		if (m_Retry)
			m_Scene->DestroyEntity(m_Retry);
		m_Retry = {};
		if (m_ExitToMap)
			m_Scene->DestroyEntity(m_ExitToMap);
		m_ExitToMap = {};
		if (m_Quit)
			m_Scene->DestroyEntity(m_Quit);
		m_Quit = {};

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
		m_YouDiedFadeOut = false;
		m_LoadedMenu = false;
		m_AnimationDone = false;
	}

	bool DeathMenu::OnKeyPressed(Teddy::KeyPressedEvent& e)
	{
		switch (e.GetKeyCode())
		{
		case Teddy::Key::Down:
		case Teddy::Key::S:
			m_CurrentSelection = (m_CurrentSelection + 1) % 3;
			UpdateColors();
			return true;
		case Teddy::Key::Up:
		case Teddy::Key::W:
			m_CurrentSelection = (m_CurrentSelection - 1 + 3) % 3;
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
				break;

			default:
				break;
			}
			return false;
		default:
			break;
		}
		return false;
	}

	void DeathMenu::OnEvent(Teddy::Event& event)
	{
		Teddy::EventDispatcher dispatcher(event);
		dispatcher.Dispatch<Teddy::KeyPressedEvent>(TED_BIND_EVENT_FN(DeathMenu::OnKeyPressed));
	}
}