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
				auto textEnt = m_Scene->CreateEntity("Time Scene");
				auto& text = textEnt.AddComponent<Teddy::TextComponent>();
				text.FontAsset = m_BoardFont;
				text.SetString("02:32");
				text.TextAlignment = Teddy::TextComponent::AlignmentType::LeftCenter;
				text.Color = glm::vec4(1.0f);
				auto& textTransform = textEnt.GetComponent<Teddy::TransformComponent>();
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
				auto textEnt = m_Scene->CreateEntity("Hp Bonus");
				auto& text = textEnt.AddComponent<Teddy::TextComponent>();
				text.FontAsset = m_BoardFont;
				text.SetString("2 3");
				text.TextAlignment = Teddy::TextComponent::AlignmentType::LeftCenter;
				text.Color = glm::vec4(1.0f);
				auto& textTransform = textEnt.GetComponent<Teddy::TransformComponent>();
				textTransform.Translation = glm::vec3(2.1f, 0.5f, 0.11f);
				textTransform.Scale = glm::vec3(0.5f, 0.5f, 1.0f);
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
				auto textEnt = m_Scene->CreateEntity("Parry");
				auto& text = textEnt.AddComponent<Teddy::TextComponent>();
				text.FontAsset = m_BoardFont;
				text.SetString("3 3");
				text.TextAlignment = Teddy::TextComponent::AlignmentType::LeftCenter;
				text.Color = glm::vec4(1.0f);
				auto& textTransform = textEnt.GetComponent<Teddy::TransformComponent>();
				textTransform.Translation = glm::vec3(2.1f, 0.0f, 0.11f);
				textTransform.Scale = glm::vec3(0.5f, 0.5f, 1.0f);
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
				auto textEnt = m_Scene->CreateEntity("Super Meter");
				auto& text = textEnt.AddComponent<Teddy::TextComponent>();
				text.FontAsset = m_BoardFont;
				text.SetString("1 3");
				text.TextAlignment = Teddy::TextComponent::AlignmentType::LeftCenter;
				text.Color = glm::vec4(1.0f);
				auto& textTransform = textEnt.GetComponent<Teddy::TransformComponent>();
				textTransform.Translation = glm::vec3(2.1f, -0.5f, 0.11f);
				textTransform.Scale = glm::vec3(0.5f, 0.5f, 1.0f);
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
				auto& text = textEnt.AddComponent<Teddy::TextComponent>();
				text.FontAsset = m_BoardFont;
				text.SetString("A ");
				text.TextAlignment = Teddy::TextComponent::AlignmentType::LeftCenter;
				text.Color = glm::vec4(1.0f);
				auto& textTransform = textEnt.GetComponent<Teddy::TransformComponent>();
				textTransform.Translation = glm::vec3(2.1f, -1.0f, 0.11f);
				textTransform.Scale = glm::vec3(0.5f, 0.5f, 1.0f);
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
		gradeSprite.PlayableIndicies = { 0,1,2,3,4,5,6,7,8,9,10,11 };

		auto& atlas = gradeEnt.AddComponent<Teddy::SpriteAtlasComponent>(0, 0, 82, 73);

		auto& gradeTransform = gradeEnt.GetComponent<Teddy::TransformComponent>();
		gradeTransform.Translation = glm::vec3(1.25f, -1.9f, 0.12f);
		gradeTransform.Scale = glm::vec3(0.8f, 0.8f, 1.0f);

		return m_Scene;
	}
} 