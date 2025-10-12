#include "MainTitleScene.h"

#include <Teddy.h>

namespace Cuphead
{
    Teddy::Ref<Teddy::Scene> MainTitleScene::Init()
    {
        Teddy::Ref<Teddy::Scene> initialScene = Teddy::CreateRef<Teddy::Scene>();

        auto camEntt = initialScene->CreateEntity("Main Title Camera");
        camEntt.GetComponent<Teddy::TransformComponent>().Translation = { 0.0f, 0.0f, 9.0f };
        auto& cam = camEntt.AddComponent<Teddy::CameraComponent>();
        cam.Camera.SetProjectionType(Teddy::SceneCamera::ProjectionType::Perspective);
        auto& window = Teddy::Application::Get().GetWindow();
        cam.Camera.SetViewportSize(window.GetWidth(), window.GetHeight());

        // Cuphead Animation
        auto& assets = Teddy::AssetManager::Get();

        auto cupheadAnimation = initialScene->CreateEntity("Main Title Animation Title");
        cupheadAnimation.AddComponent<Teddy::SpriteAtlasComponent>(0, 0, 1013, 552);
        auto& spriteAnimation = cupheadAnimation.AddComponent<Teddy::SpriteAnimationComponent>(0.25f, 0.05f, 0.25f);
        spriteAnimation.PingPong = true;
        spriteAnimation.Textures = assets.LoadMultiple<Teddy::Texture2D>(
            {
                "assets/textures/Cuphead_And_Mugman_TitleScreen_1013x552_2048x2048_0.png",
                "assets/textures/Cuphead_And_Mugman_TitleScreen_1013x552_2048x2048_1.png",
                "assets/textures/Cuphead_And_Mugman_TitleScreen_1013x552_2048x2048_2.png"
            });
        auto& animationTransform = cupheadAnimation.GetComponent<Teddy::TransformComponent>();
        animationTransform.Translation = glm::vec3(0.2f, -0.55f, 1.0f);
        animationTransform.Scale = glm::vec3(0.0f, 0.75f, 1.0f);
        spriteAnimation.IsBackground = true;

        // Text
        auto textEntity = initialScene->CreateEntity("Main Title Title text");
        auto& text = textEntity.AddComponent<Teddy::TextComponent>();
        text.FontAsset = assets.Load<Teddy::Font>("assets/Fonts/CupheadVogue-ExtraBold.otf", Teddy::Boolean::True);
        text.SetString("Press Any Button");
        text.Color = glm::vec4(233.0f / 255.0f, 193.0f / 255.0f, 80.0f / 255.0f, 1.0f);
        text.TextAlignment = Teddy::TextComponent::AlignmentType::Center;
        auto& textTransform = textEntity.GetComponent<Teddy::TransformComponent>();
        textTransform.Scale *= 0.41;
        textTransform.Translation = glm::vec3(0.0f, -2.60f, 1.5f);

        class TitleText : public Teddy::ScriptableEntity
        {
        public:
            void OnUpdate(Teddy::Timestep ts)
            {
                static float timer = 0.0f;
                timer += ts;

                auto& text = GetComponent<Teddy::TextComponent>();
                if (text.Color.a == 0 && timer > 1)
                {
                    timer = 0.0f;
                    text.Color.a = 1;
                }
                else if (text.Color.a != 0 && timer > 1)
                {
                    timer = 0.0f;
                    text.Color.a = 0;
                }
            }
        };

        textEntity.AddComponent<Teddy::NativeScriptComponent>().Bind<TitleText>();

        // Background
        auto background = initialScene->CreateEntity("Main Title Background");
        auto& sprite = background.AddComponent<Teddy::SpriteRendererComponent>();
        sprite.IsBackground = true;
        sprite.Texture = assets.Load<Teddy::Texture2D>("assets/Textures/SpriteAtlasTexture-Title_Assets (Group 1)-2048x1024-fmt10.png", Teddy::Boolean::True);

        background.AddComponent<Teddy::SpriteAtlasComponent>(0, 0, 1280, 720);

        return initialScene;
    }

}