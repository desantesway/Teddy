#include "Winscreen.h"

#include <Teddy.h>

namespace Cuphead
{
	Teddy::Ref<Teddy::Scene> Winscreen::Init()
	{
		auto& assets = Teddy::AssetManager::Get();

		m_BackgroundTexture = assets.Load<Teddy::Texture2D>("assets/Textures/UI/Results/winscreen_bg.png", Teddy::Boolean::True);

		m_BoardTexture = assets.Load<Teddy::Texture2D>("assets/Textures/UI/Results/Winscreen_Board_566x458_1024x1024_0.png", Teddy::Boolean::True);

		m_TheResultsTextures = assets.LoadMultiple<Teddy::Texture2D>({
			"assets/Textures/UI/Results/Winscreen_Results_792x150_1024x1024_0.png"
			});

		m_GradeTextures = assets.LoadMultiple<Teddy::Texture2D>({
			"assets/Textures/UI/Results/Winscreen_Grade_82x73_512x512_0.png"
			});

		m_BoardFont = assets.Load<Teddy::Font>("assets/Fonts/CupheadMemphis-Medium.otf", Teddy::Boolean::True);

		m_Scene = Teddy::CreateRef<Teddy::Scene>();

		auto camEnt = m_Scene->CreateEntity("Main Menu Camera");
		camEnt.GetComponent<Teddy::TransformComponent>().Translation = { 0.0f, 0.0f, 9.0f };
		auto& cam = camEnt.AddComponent<Teddy::CameraComponent>();
		cam.Camera.SetProjectionType(Teddy::SceneCamera::ProjectionType::Perspective);

		auto& window = Teddy::Application::Get().GetWindow();
		m_Scene->OnViewportResize(window.GetWidth(), window.GetHeight());

		auto bgEnt = m_Scene->CreateEntity("Winscreen Background");
		auto& bgSprite = bgEnt.AddComponent<Teddy::SpriteRendererComponent>();
		bgSprite.Texture = m_BackgroundTexture;
		bgSprite.IsBackground = true;

		auto& bgTransform = bgEnt.GetComponent<Teddy::TransformComponent>();
		bgTransform.Scale = glm::vec3(2.25f, 2.25f, 1.0f);

		class BGRotation : public Teddy::ScriptableEntity
		{
		public:
			void OnUpdate(Teddy::Timestep ts) override
			{
				auto& rot = GetComponent<Teddy::TransformComponent>().Rotation;
				rot.z += ts * 0.2f;
			}
		};

		bgEnt.AddComponent<Teddy::NativeScriptComponent>().Bind<BGRotation>();

		auto boardEnt = m_Scene->CreateEntity("Winscreen Board");
		auto& boardSprite = boardEnt.AddComponent<Teddy::SpriteRendererComponent>();
		boardSprite.Texture = m_BoardTexture;

		auto& boardAtlas = boardEnt.AddComponent<Teddy::SpriteAtlasComponent>(0, 0, 566, 458);

		auto& boardTransform = boardEnt.GetComponent<Teddy::TransformComponent>();
		boardTransform.Translation = glm::vec3(0.0f, -0.75f, 0.1f);
		boardTransform.Scale = glm::vec3(5.0f, 5.0f, 1.0f);

		auto resultsEnt = m_Scene->CreateEntity("The Results Text");
		auto& resultsSprite = resultsEnt.AddComponent<Teddy::SpriteAnimationComponent>(0.1f);
		resultsSprite.Textures = m_TheResultsTextures;
		resultsSprite.PlayableIndicies = {0,1,2};
		resultsEnt.AddComponent<Teddy::SpriteAtlasComponent>(0, 0, 792, 150);

		auto& resultsTransform = resultsEnt.GetComponent<Teddy::TransformComponent>();
		resultsTransform.Translation = glm::vec3(0.0f, 2.75f, 0.1f);
		resultsTransform.Scale = glm::vec3(1.65f, 1.65f, 1.0f);

		class Slash : public Teddy::ScriptableEntity
		{
		public:

			void OnCreate() override
			{
				m_SlashEntity = GetScene()->CreateEntity("Slash");
				auto& text = m_SlashEntity.AddComponent<Teddy::TextComponent>();
				text.SetString("/");
				text.TextAlignment = Teddy::TextComponent::AlignmentType::Center;
				text.Color = glm::vec4(1.0f);
				auto& textTransform = m_SlashEntity.GetComponent<Teddy::TransformComponent>();
				textTransform.Translation = GetComponent<Teddy::TransformComponent>().Translation + glm::vec3(-0.26f, 0.025f, 0.01f);
				textTransform.Scale = glm::vec3(0.45f, 0.45f, 1.0f);
			}

			Teddy::Entity m_SlashEntity;
		};

		{
			{
				auto textEnt = m_Scene->CreateEntity("Time Scene");
				auto& text = textEnt.AddComponent<Teddy::TextComponent>();
				text.FontAsset = m_BoardFont;
				text.SetString("TIME");
				text.TextAlignment = Teddy::TextComponent::AlignmentType::RightCenter;
				text.Color = glm::vec4(1.0f);
				auto& textTransform = textEnt.GetComponent<Teddy::TransformComponent>();
				textTransform.Translation = glm::vec3(-2.1f, 1.0f, 0.11f);
				textTransform.Scale = glm::vec3(0.5f, 0.5f, 1.0f);
			}

			{
				auto textEnt = m_Scene->CreateEntity("Time Scene");
				auto& text = textEnt.AddComponent<Teddy::TextComponent>();
				text.FontAsset = m_BoardFont;
				text.SetString(". . . . . . . . . .");
				text.TextAlignment = Teddy::TextComponent::AlignmentType::Center;
				text.Color = glm::vec4(1.0f);
				auto& textTransform = textEnt.GetComponent<Teddy::TransformComponent>();
				textTransform.Translation = glm::vec3(0.05f, 0.875f, 0.11f);
				textTransform.Scale = glm::vec3(0.4f, 0.4f, 1.0f);
			}

			{
				m_TimeEntity = m_Scene->CreateEntity("Time Scene");
				auto& text = m_TimeEntity.AddComponent<Teddy::TextComponent>();
				text.FontAsset = m_BoardFont;
				text.SetString("00:00");
				text.TextAlignment = Teddy::TextComponent::AlignmentType::LeftCenter;
				text.Color = glm::vec4(1.0f);
				auto& textTransform = m_TimeEntity.GetComponent<Teddy::TransformComponent>();
				textTransform.Translation = glm::vec3(2.1f, 1.0f, 0.11f);
				textTransform.Scale = glm::vec3(0.5f, 0.5f, 1.0f);
			}
		}
		
		{
			{
				auto textEnt = m_Scene->CreateEntity("Hp Bonus");
				auto& text = textEnt.AddComponent<Teddy::TextComponent>();
				text.FontAsset = m_BoardFont;
				text.SetString("HP BONUS");
				text.TextAlignment = Teddy::TextComponent::AlignmentType::RightCenter;
				text.Color = glm::vec4(1.0f);
				auto& textTransform = textEnt.GetComponent<Teddy::TransformComponent>();
				textTransform.Translation = glm::vec3(-2.1f, 0.5f, 0.11f);
				textTransform.Scale = glm::vec3(0.5f, 0.5f, 1.0f);
			}

			{
				auto textEnt = m_Scene->CreateEntity("Hp Bonus");
				auto& text = textEnt.AddComponent<Teddy::TextComponent>();
				text.FontAsset = m_BoardFont;
				text.SetString(". . . . . .");
				text.TextAlignment = Teddy::TextComponent::AlignmentType::Center;
				text.Color = glm::vec4(1.0f);
				auto& textTransform = textEnt.GetComponent<Teddy::TransformComponent>();
				textTransform.Translation = glm::vec3(0.85f, 0.375f, 0.11f);
				textTransform.Scale = glm::vec3(0.4f, 0.4f, 1.0f);
			}

			{
				m_HpEntity = m_Scene->CreateEntity("Hp Bonus");
				auto& text = m_HpEntity.AddComponent<Teddy::TextComponent>();
				text.FontAsset = m_BoardFont;
				text.SetString("0 3");
				text.TextAlignment = Teddy::TextComponent::AlignmentType::LeftCenter;
				text.Color = glm::vec4(1.0f);
				auto& textTransform = m_HpEntity.GetComponent<Teddy::TransformComponent>();
				textTransform.Translation = glm::vec3(2.1f, 0.5f, 0.11f);
				textTransform.Scale = glm::vec3(0.5f, 0.5f, 1.0f);

				m_HpEntity.AddComponent<Teddy::NativeScriptComponent>().Bind<Slash>();
			}
		}

		{
			{
				auto textEnt = m_Scene->CreateEntity("Parry");
				auto& text = textEnt.AddComponent<Teddy::TextComponent>();
				text.FontAsset = m_BoardFont;
				text.SetString("PARRY");
				text.TextAlignment = Teddy::TextComponent::AlignmentType::RightCenter;
				text.Color = glm::vec4(1.0f);
				auto& textTransform = textEnt.GetComponent<Teddy::TransformComponent>();
				textTransform.Translation = glm::vec3(-2.1f, 0.0f, 0.11f);
				textTransform.Scale = glm::vec3(0.5f, 0.5f, 1.0f);
			}

			{
				auto textEnt = m_Scene->CreateEntity("Parry");
				auto& text = textEnt.AddComponent<Teddy::TextComponent>();
				text.FontAsset = m_BoardFont;
				text.SetString(". . . . . . . . . . .");
				text.TextAlignment = Teddy::TextComponent::AlignmentType::Center;
				text.Color = glm::vec4(1.0f);
				auto& textTransform = textEnt.GetComponent<Teddy::TransformComponent>();
				textTransform.Translation = glm::vec3(0.45f, -0.125f, 0.11f);
				textTransform.Scale = glm::vec3(0.4f, 0.4f, 1.0f);
			}

			{
				m_ParryEntity = m_Scene->CreateEntity("Parry");
				auto& text = m_ParryEntity.AddComponent<Teddy::TextComponent>();
				text.FontAsset = m_BoardFont;
				text.SetString("0 3");
				text.TextAlignment = Teddy::TextComponent::AlignmentType::LeftCenter;
				text.Color = glm::vec4(1.0f);
				auto& textTransform = m_ParryEntity.GetComponent<Teddy::TransformComponent>();
				textTransform.Translation = glm::vec3(2.1f, 0.0f, 0.11f);
				textTransform.Scale = glm::vec3(0.5f, 0.5f, 1.0f);

				m_ParryEntity.AddComponent<Teddy::NativeScriptComponent>().Bind<Slash>();
			}
		}

		{
			{
				auto textEnt = m_Scene->CreateEntity("Super Meter");
				auto& text = textEnt.AddComponent<Teddy::TextComponent>();
				text.FontAsset = m_BoardFont;
				text.SetString("SUPER METER");
				text.TextAlignment = Teddy::TextComponent::AlignmentType::RightCenter;
				text.Color = glm::vec4(1.0f);
				auto& textTransform = textEnt.GetComponent<Teddy::TransformComponent>();
				textTransform.Translation = glm::vec3(-2.1f, -0.5f, 0.11f);
				textTransform.Scale = glm::vec3(0.5f, 0.5f, 1.0f);
			}

			{
				auto textEnt = m_Scene->CreateEntity("Super Meter");
				auto& text = textEnt.AddComponent<Teddy::TextComponent>();
				text.FontAsset = m_BoardFont;
				text.SetString(". . .");
				text.TextAlignment = Teddy::TextComponent::AlignmentType::Center;
				text.Color = glm::vec4(1.0f);
				auto& textTransform = textEnt.GetComponent<Teddy::TransformComponent>();
				textTransform.Translation = glm::vec3(1.3f, -0.625f, 0.11f);
				textTransform.Scale = glm::vec3(0.4f, 0.4f, 1.0f);
			}

			{
				m_SuperEntity = m_Scene->CreateEntity("Super Meter");
				auto& text = m_SuperEntity.AddComponent<Teddy::TextComponent>();
				text.FontAsset = m_BoardFont;
				text.SetString("0 6");
				text.TextAlignment = Teddy::TextComponent::AlignmentType::LeftCenter;
				text.Color = glm::vec4(1.0f);
				auto& textTransform = m_SuperEntity.GetComponent<Teddy::TransformComponent>();
				textTransform.Translation = glm::vec3(2.1f, -0.5f, 0.11f);
				textTransform.Scale = glm::vec3(0.5f, 0.5f, 1.0f);

				m_SuperEntity.AddComponent<Teddy::NativeScriptComponent>().Bind<Slash>();
			}
		}

		{
			{
				auto textEnt = m_Scene->CreateEntity("Skill Level");
				auto& text = textEnt.AddComponent<Teddy::TextComponent>();
				text.FontAsset = m_BoardFont;
				text.SetString("SKILL LEVEL");
				text.TextAlignment = Teddy::TextComponent::AlignmentType::RightCenter;
				text.Color = glm::vec4(1.0f);
				auto& textTransform = textEnt.GetComponent<Teddy::TransformComponent>();
				textTransform.Translation = glm::vec3(-2.1f, -1.0f, 0.11f);
				textTransform.Scale = glm::vec3(0.5f, 0.5f, 1.0f);
			}

			{
				auto textEnt = m_Scene->CreateEntity("Skill Level");
				auto& text = textEnt.AddComponent<Teddy::TextComponent>();
				text.FontAsset = m_BoardFont;
				text.SetString(". . . .");
				text.TextAlignment = Teddy::TextComponent::AlignmentType::Center;
				text.Color = glm::vec4(1.0f);
				auto& textTransform = textEnt.GetComponent<Teddy::TransformComponent>();
				textTransform.Translation = glm::vec3(1.1f, -1.125f, 0.11f);
				textTransform.Scale = glm::vec3(0.4f, 0.4f, 1.0f);
			}

			{
				auto textEnt = m_Scene->CreateEntity("Skill Level");
				auto& text = textEnt.AddComponent<Teddy::SpriteRendererComponent>();
				text.Texture = m_GradeTextures[0];
				auto& atlas = textEnt.AddComponent<Teddy::SpriteAtlasComponent>(0, 2, 82, 73);
				auto& textTransform = textEnt.GetComponent<Teddy::TransformComponent>();
				textTransform.Translation = glm::vec3(2.0f, -1.05f, 0.12f);
				textTransform.Scale = glm::vec3(0.8f, 0.8f, 1.0f);
			}

			{
				auto textEnt = m_Scene->CreateEntity("Skill Level");
				auto& text = textEnt.AddComponent<Teddy::SpriteRendererComponent>();
				text.Texture = m_GradeTextures[0];
				auto& atlas = textEnt.AddComponent<Teddy::SpriteAtlasComponent>(2, 2, 82, 73);
				auto& textTransform = textEnt.GetComponent<Teddy::TransformComponent>();
				textTransform.Translation = glm::vec3(1.65f, -1.05f, 0.12f);
				textTransform.Scale = glm::vec3(0.8f, 0.8f, 1.0f);
			}
		}

		auto separator = m_Scene->CreateEntity("Separator Line");

		auto& sepSprite = separator.AddComponent<Teddy::SpriteRendererComponent>();
		sepSprite.Texture = m_BoardTexture;

		separator.AddComponent<Teddy::SpriteAtlasComponent>(0, 1, 566, 458);

		auto& separatorTransform = separator.GetComponent<Teddy::TransformComponent>();
		separatorTransform.Translation = glm::vec3(0.0f, -1.45f, 0.11f);
		separatorTransform.Scale = glm::vec3(5.0f, 5.0f, 1.0f);

		{
			{
				auto textEnt = m_Scene->CreateEntity("Grade");
				auto& text = textEnt.AddComponent<Teddy::TextComponent>();
				text.FontAsset = m_BoardFont;
				text.SetString("GRADE");
				text.TextAlignment = Teddy::TextComponent::AlignmentType::RightCenter;
				text.Color = m_YellowColor;
				auto& textTransform = textEnt.GetComponent<Teddy::TransformComponent>();
				textTransform.Translation = glm::vec3(-1.5f, -1.9f, 0.11f);
				textTransform.Scale = glm::vec3(0.6f, 0.6f, 1.0f);
			}

			{
				auto textEnt = m_Scene->CreateEntity("Grade");
				auto& text = textEnt.AddComponent<Teddy::TextComponent>();
				text.FontAsset = m_BoardFont;
				text.SetString(". . .");
				text.TextAlignment = Teddy::TextComponent::AlignmentType::Center;
				text.Color = m_YellowColor;
				auto& textTransform = textEnt.GetComponent<Teddy::TransformComponent>();
				textTransform.Translation = glm::vec3(0.7f, -2.06f, 0.11f);
				textTransform.Scale = glm::vec3(0.4f, 0.4f, 1.0f);
			}

			{
				auto textEnt = m_Scene->CreateEntity("Grade");
				auto& text = textEnt.AddComponent<Teddy::TextComponent>();
				text.FontAsset = m_BoardFont;
				text.SetString("B+");
				text.TextAlignment = Teddy::TextComponent::AlignmentType::Center;
				text.Color = glm::vec4(1.0f);
				auto& textTransform = textEnt.GetComponent<Teddy::TransformComponent>();
				textTransform.Translation = glm::vec3(1.25f, -1.9f, 0.11f);
				textTransform.Scale = glm::vec3(0.6f, 0.6f, 1.0f);
			}
		}

		auto gradeEnt = m_Scene->CreateEntity("Grade Image");
		auto& gradeSprite = gradeEnt.AddComponent<Teddy::SpriteAnimationComponent>(0.1f);
		gradeSprite.Textures = m_GradeTextures;
		gradeSprite.Loop = false;
		gradeSprite.PlayableIndicies = { 0,1,2,3,4,5,6,7,8,9,10,11 };

		auto& atlas = gradeEnt.AddComponent<Teddy::SpriteAtlasComponent>(0, 0, 82, 73);

		auto& gradeTransform = gradeEnt.GetComponent<Teddy::TransformComponent>();
		gradeTransform.Translation = glm::vec3(1.25f, -1.9f, 0.12f);
		gradeTransform.Scale = glm::vec3(0.8f, 0.8f, 1.0f);

		class GradeLoop : public Teddy::ScriptableEntity
		{
			public:
			void OnUpdate(Teddy::Timestep ts) override
			{
				auto& anim = GetComponent<Teddy::SpriteAnimationComponent>();
				auto& aA = GetComponent<Teddy::SpriteAnimationAtlasComponent>();
				if (aA.Index >= 9 && !anim.Loop)
				{
					anim.PlayableIndicies = { 9, 10, 11 };
					anim.Loop = true;
				}
			}
		};

		gradeEnt.AddComponent<Teddy::NativeScriptComponent>().Bind<GradeLoop>();

		return m_Scene;
	}

	void Winscreen::OnUpdate(Teddy::Timestep ts)
	{
		if (!m_Start) return;

		m_Timer += ts;
		if (!m_SkipTime)
		{
			TimeInscrease();
			m_Timer = 0.0f;
			return;
		}
		else if (!m_SkipHp)
		{
			if (m_Timer > 0.1f)
			{
				m_Timer = 0.0f;
				SlashIncrease(m_HpEntity.GetComponent<Teddy::TextComponent>(), m_Hp, m_SkipHp);
			}
			return;
		}
		else if (!m_SkipParry)
		{
			if (m_Timer > 0.1f)
			{
				m_Timer = 0.0f;
				SlashIncrease(m_ParryEntity.GetComponent<Teddy::TextComponent>(), m_Parry, m_SkipParry);
			}
			return;
		}
		else if (!m_SkipSuper)
		{
			if (m_Timer > 0.1f)
			{
				m_Timer = 0.0f;
				SlashIncrease(m_SuperEntity.GetComponent<Teddy::TextComponent>(), m_Super, m_SkipSuper);
			}
			return;
		}
		else if (!m_SkipSkill)
		{
			Skill();

			return;
		}
	}

	void Winscreen::Skill()
	{
		if (m_Skill > 1)
		{
			auto textEnt = m_Scene->CreateEntity("Skill Level");
			auto& text = textEnt.AddComponent<Teddy::SpriteRendererComponent>();
			text.Texture = m_GradeTextures[0];
			auto& atlas = textEnt.AddComponent<Teddy::SpriteAtlasComponent>(4, 2, 82, 73);
			auto& textTransform = textEnt.GetComponent<Teddy::TransformComponent>();
			textTransform.Translation = glm::vec3(2.0f, -1.05f, 0.12f);
			textTransform.Scale = glm::vec3(0.8f, 0.8f, 1.0f);
		}

		if (m_Skill > 0)
		{
			auto textEnt = m_Scene->CreateEntity("Skill Level");
			auto& text = textEnt.AddComponent<Teddy::SpriteRendererComponent>();
			text.Texture = m_GradeTextures[0];
			auto& atlas = textEnt.AddComponent<Teddy::SpriteAtlasComponent>(5, 2, 82, 73);
			auto& textTransform = textEnt.GetComponent<Teddy::TransformComponent>();
			textTransform.Translation = glm::vec3(1.65f, -1.05f, 0.12f);
			textTransform.Scale = glm::vec3(0.8f, 0.8f, 1.0f);
		}

		m_SkipSkill = true;
	}

	void Winscreen::TimeInscrease()
	{
		auto& textComp = m_TimeEntity.GetComponent<Teddy::TextComponent>();
		auto& timeText = textComp.TextString;

		int minutes = 0;
		int seconds = 0;
		bool parsed = false;
		auto colonPos = timeText.find(':');
		if (colonPos != std::string::npos)
		{
			try
			{
				minutes = std::stoi(timeText.substr(0, colonPos));
				seconds = std::stoi(timeText.substr(colonPos + 1));
				parsed = true;
			}
			catch (...)
			{
				parsed = false;
			}
		}
		else
		{
			try
			{
				int total = std::stoi(timeText);
				minutes = total / 60;
				seconds = total % 60;
				parsed = true;
			}
			catch (...)
			{
				parsed = false;
			}
		}

		float currentSeconds = parsed ? static_cast<float>(minutes * 60 + seconds) : 0.0f;
		if (currentSeconds < m_Time)
		{
			float newTime = (std::min)(currentSeconds + 1.0f, static_cast<float>(m_Time));
			int displaySeconds = static_cast<int>(std::floor(newTime + 0.0001f));
			int dispMin = displaySeconds / 60;
			int dispSec = displaySeconds % 60;

			std::ostringstream oss;
			oss << std::setfill('0') << std::setw(2) << dispMin << ":" << std::setfill('0') << std::setw(2) << dispSec;

			textComp.SetString(oss.str());
		}
		else
		{
			if(m_Time < 130.0f)
				textComp.Color = m_YellowColor;
			m_SkipTime = true;
		}
	}

	void Winscreen::TimeSkip()
	{
		auto& textComp = m_TimeEntity.GetComponent<Teddy::TextComponent>();

		int displaySeconds = static_cast<int>(std::floor(m_Time + 0.0001f));
		int dispMin = displaySeconds / 60;
		int dispSec = displaySeconds % 60;

		std::ostringstream oss;
		oss << std::setfill('0') << std::setw(2) << dispMin << ":" << std::setfill('0') << std::setw(2) << dispSec;

		textComp.SetString(oss.str());

		if (m_Time < 130.0f)
			textComp.Color = m_YellowColor;
		m_SkipTime = true;
	}

	void Winscreen::SlashIncrease(Teddy::TextComponent& textComp, int& toComp, bool &isDone)
	{
		auto& text = textComp.TextString;
		int current = 0;
		try
		{
			current = text.empty() ? 0 : std::stoi(std::string(1, text[0]));
		}
		catch (...)
		{
			current = 0;
		}
		if (current < toComp)
		{
			current++;
			textComp.SetString(std::to_string(current) + " " + text[2]);
		}
		else
		{
			if(current == 3)
				textComp.Color = m_YellowColor;
			isDone = true;
		}
	}

	void Winscreen::SlashSkip(Teddy::TextComponent& textComp, int& toComp, bool& isDone)
	{
		auto& hpText = textComp.TextString;

		textComp.SetString(std::to_string(toComp) + " " + hpText[2]);
		if (toComp == 3)
			textComp.Color = m_YellowColor;
		isDone = true;
	}

	void Winscreen::OnEvent(Teddy::Event& event)
	{
		TED_PROFILE_FUNCTION();

		Teddy::EventDispatcher dispatcher(event);
		dispatcher.Dispatch<Teddy::KeyPressedEvent>(TED_BIND_EVENT_FN(Winscreen::OnKeyPressed));
	}

	bool Winscreen::OnKeyPressed(Teddy::KeyPressedEvent& e)
	{
		switch (e.GetKeyCode())
		{
		case Teddy::Key::Return:
			if (!m_SkipTime)
			{
				TimeSkip();
				return true;
			}
			else if (!m_SkipHp)
			{
				SlashSkip(m_HpEntity.GetComponent<Teddy::TextComponent>(), m_Hp, m_SkipHp);
				return true;
			}
			else if (!m_SkipParry)
			{
				SlashSkip(m_ParryEntity.GetComponent<Teddy::TextComponent>(), m_Parry, m_SkipParry);
				return true;
			}
			else if (!m_SkipSuper)
			{
				SlashSkip(m_SuperEntity.GetComponent<Teddy::TextComponent>(), m_Super, m_SkipSuper);
				return true;
			}
			else if (!m_SkipSkill)
			{
				Skill();
				return true;
			}
			else
			{
				m_ProceedToMenu = true;
				return true;
			}
		default:
			break;
		}
	}
} 