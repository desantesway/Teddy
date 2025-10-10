#include "Teddy.h"
#include "Editor.h"

#include <imgui.h>
#include <ImGuizmo.h>

#include <glm/gtc/type_ptr.hpp>

namespace Teddy
{
    extern const std::filesystem::path g_AssetPath;

    void Editor::Init(Ref<Scene> activeScene)
    {
        ButtonInteractionSystem::Get().HoveredHandled(true);

        auto& assets = AssetManager::Get();

        m_IconPlay = assets.Load<Texture2D>("PlayButton", "Resources/Icons/PlayButton.png");
        m_IconSimulate = assets.Load<Texture2D>("Simulate", "Resources/Icons/Simulate.png");
        m_IconStop = assets.Load<Texture2D>("StopButton", "Resources/Icons/StopButton.png");

        // Post processing framebuffer (for editor layer)
        m_PostProcessedFramebuffer = Framebuffer::Create(PostProcessing::Get().GetFramebufferSpec());

        m_ActiveScene = activeScene;
        auto& window = Teddy::Application::Get().GetWindow();
        m_ActiveScene->OnViewportResize((uint32_t)window.GetWidth(), (uint32_t)window.GetHeight());
        m_SceneHierarchyPanel.SetContext(m_ActiveScene);

        m_EditorScenePath = std::filesystem::path();
        m_EditorScene = m_ActiveScene;

        auto commandLineArgs = Application::Get().GetSpecification().CommandLineArgs;

        if (commandLineArgs.Count > 1)
        {
            auto sceneFilePath = commandLineArgs[1];
            SceneSerializer serializer(m_ActiveScene);
            serializer.Deserialize(sceneFilePath);
        }

        m_EditorCamera = EditorCamera(30.0f, 1.778f, 0.1f, 1000.0f);

        class HoveringTest : public ScriptableEntity
        {
        public:
            void OnCreate() override
            {
				m_Color = GetComponent<Teddy::SpriteRendererComponent>().Color;
            }

            void OnUpdate(Timestep ts) override
            {
                auto& sprite = GetComponent<Teddy::SpriteRendererComponent>();
                if (GetComponent<Teddy::ButtonComponent>().Hovered)
                {
					sprite.Color = m_HoveredColor;
                }
                else
                {
                    sprite.Color = m_Color;
                }
            }

        private:
			glm::vec4 m_HoveredColor = { 0.2f, 0.2f, 0.9f, 1.0f };
            glm::vec4 m_Color = glm::vec4(1.0f);
        };

		auto test = m_ActiveScene->CreateEntity("Mugman Animation");
        auto& cupheadSprite = test.AddComponent<Teddy::SpriteAnimationComponent>();
        cupheadSprite.PlayableIndicies = { 0, 1, 2, 3, 4, 5, 6, 7 };
        cupheadSprite.Loop = true;
        cupheadSprite.Textures.push_back(assets.Load<Teddy::Texture2D>("assets/textures/Cuphead_143x171_1024x1024_0.png", Teddy::Boolean::True));
        test.AddComponent<Teddy::SpriteAtlasComponent>(4, 1, 143, 171);
        auto& cupheadTransform = test.GetComponent<Teddy::TransformComponent>();
        cupheadTransform.Translation = glm::vec3(0.4f, 0.24f, 1.0f);
        cupheadTransform.Scale *= 1.3f;

        auto cupheadAnimation = m_ActiveScene->CreateEntity("Main Title Animation Title");
        cupheadAnimation.AddComponent<Teddy::SpriteAtlasComponent>(0, 0, 1013, 552);
        auto& spriteAnimation = cupheadAnimation.AddComponent<Teddy::SpriteAnimationComponent>(0.25f, 0.05f, 0.25f);
        spriteAnimation.PingPong = true;
        spriteAnimation.Textures = assets.LoadMultiple<Teddy::Texture2D>(
            {
                "assets/textures/Cuphead_And_Mugman_TitleScreen_1013x552_2048x2048_0.png",
                "assets/textures/Cuphead_And_Mugman_TitleScreen_1013x552_2048x2048_1.png",
                "assets/textures/Cuphead_And_Mugman_TitleScreen_1013x552_2048x2048_2.png",
                "assets/textures/Cuphead_And_Mugman_TitleScreen_1013x552_2048x2048_3.png"
            });
        auto& animationTransform = cupheadAnimation.GetComponent<Teddy::TransformComponent>();
        animationTransform.Translation = glm::vec3(0.2f, -0.55f, 1.0f);
        animationTransform.Scale = glm::vec3(0.0f, 0.75f, 1.0f);
        spriteAnimation.IsBackground = true;

        Renderer2D::SetLineWidth(4.0f);
    }

    void Editor::ResizeFramebuffer()
    {
        if (FramebufferSpecification spec = m_PostProcessedFramebuffer->GetSpecification();
            m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f && // zero sized framebuffer is invalid
            (spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y))
        {
            PostProcessing::Get().Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
            m_PostProcessedFramebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
            m_EditorCamera.SetViewportSize(m_ViewportSize.x, m_ViewportSize.y);
            m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
        }
    }

    void Editor::OnUpdate(Timestep ts)
    {
        {
            TED_PROFILE_SCOPE("Renderer Draw (CPU)");

            switch (m_SceneState)
            {
            case SceneState::Edit:
            {
                m_ActiveScene->AlwaysOnUpdate();
                m_EditorCamera.OnUpdate(ts);
                m_ActiveScene->OnUpdateEditor(ts, m_EditorCamera);
                break;
            }
            case SceneState::Play:
            {
                m_ActiveScene->AlwaysOnUpdate();
                m_ActiveScene->OnUpdateRuntime(ts);
                break;
            }
            case SceneState::Simulate:
            {
                m_ActiveScene->AlwaysOnUpdate();
                m_EditorCamera.OnUpdate(ts);
                m_ActiveScene->SimulatePhysics(ts);
                m_ActiveScene->OnUpdateEditor(ts, m_EditorCamera);
                break;
            }
            }

            {
                TED_PROFILE_SCOPE("Overlay Renderer");
                OnOverlayRender();
            }

            if (!ImGuizmo::IsOver())
            {
                auto [mx, my] = ImGui::GetMousePos();
                mx -= m_ViewportBounds[0].x;
                my -= m_ViewportBounds[0].y;
                glm::vec2 viewportSize = m_ViewportBounds[1] - m_ViewportBounds[0];
                my = viewportSize.y - my;
                int mouseX = (int)mx;
                int mouseY = (int)my;

                if (mouseX >= 0 && mouseY >= 0 && mouseX < (int)viewportSize.x && mouseY < (int)viewportSize.y)
                {
                    int pixelData = PostProcessing::Get().GetFramebuffer()->ReadPixel(1, mouseX, mouseY);
                    entt::entity handle = (entt::entity)pixelData;
                    
                    if (pixelData == -1) 
                    {
                        m_HoveredEntity = {};
                        ButtonInteractionSystem::Get().SetHoveredEntity(entt::null);
                    }
                    else 
                    {
                        ButtonInteractionSystem::Get().SetHoveredEntity(handle);
                        m_HoveredEntity = Entity(handle, m_ActiveScene.get());
                    }
                }
            }

        }
    }

    void Editor::OnOverlayRender()
    {
        if (m_SceneState == SceneState::Play)
        {
            Entity camera = m_ActiveScene->GetPrimaryCameraEntity();
            if (camera)
            {
                Renderer2D::BeginScene(camera.GetComponent<CameraComponent>().Camera, camera.GetComponent<TransformComponent>().GetTransform());
            }
            else
            {
                Renderer2D::BeginScene(m_EditorCamera);
            }
        }
        else
        {
            Renderer2D::BeginScene(m_EditorCamera);
        }

        if (m_ShowPhysicsColliders)
        {
            auto viewCircle = m_ActiveScene->GetAllEntitiesWith<TransformComponent, CircleCollider2DComponent>();
            for (auto [entity, tc, cc2d] : viewCircle.each())
            {
                float sx = std::abs(tc.Scale.x);
                float sy = std::abs(tc.Scale.y);
                float maxScale = glm::max(sx, sy) * cc2d.Radius;
                glm::vec3 scale = glm::vec3(maxScale * 2, maxScale * 2, 1.0f);

                glm::mat4 transform = glm::translate(glm::mat4(1.0f), tc.Translation)
                    * glm::rotate(glm::mat4(1.0f), tc.Rotation.z, glm::vec3(0.0f, 0.0f, 1.0f))
                    * glm::translate(glm::mat4(1.0f), glm::vec3(cc2d.Offset, 0.001f))
                    * glm::scale(glm::mat4(1.0f), scale);

                Renderer2D::DrawCircleLine(transform, glm::vec4(0, 1, 0, 1), Renderer2D::GetLineWidth() / 100);
            }

            auto viewBox = m_ActiveScene->GetAllEntitiesWith<TransformComponent, BoxCollider2DComponent>();
            for (auto [entity, tc, bc2d] : viewBox.each())
            {
                Entity ent = Entity{ entity, m_ActiveScene.get() };
                if (ent.HasComponent<TextComponent>())
                {
                    auto& text = ent.GetComponent<TextComponent>();
                    TransformComponent textQuad{ text.TextQuad };
                    switch (text.TextAlignment) // TODO: FIX THIS COMPLETELY (Scale/rotation) and then fix physics actual location
                    {
                    case TextComponent::AlignmentType::Center:
                    {
                        textQuad.Translation -= glm::vec3(text.TextQuad.Scale.x / 2,
                            text.TextQuad.Scale.y / 2,
                            0.0f);
                        break;
                    }
                    case TextComponent::AlignmentType::LeftCenter:
                    {
                        textQuad.Translation -= glm::vec3(text.TextQuad.Scale.x,
                            text.TextQuad.Scale.y / 2,
                            0.0f);
                        break;
					}
                    case TextComponent::AlignmentType::RightCenter:
                    {
                        textQuad.Translation -= glm::vec3(0.0f,
                            text.TextQuad.Scale.y / 2,
                            0.0f);
                        break;
                    }
                    default:
                        break;
                    }

                    glm::mat4 transform = glm::translate(glm::mat4(1.0f), tc.Translation)
                        * glm::rotate(glm::mat4(1.0f), tc.Rotation.z, glm::vec3(0.0f, 0.0f, 1.0f))
                        * glm::translate(glm::mat4(1.0f), glm::vec3(bc2d.Offset, 0.001f))
                        * glm::scale(glm::mat4(1.0f), tc.Scale * glm::vec3(bc2d.Size * 2.0f, 1.0f))
                        //* ent.GetComponent<TransformComponent>().GetTransform() // Collisions are only 2D
                        * textQuad.GetTransform();

                    Renderer2D::DrawRect(transform, glm::vec4(0, 1, 0, 1));
                }
                else
                {
                    glm::vec3 scale = tc.Scale * glm::vec3(bc2d.Size * 2.0f, 1.0f);

                    glm::mat4 transform = glm::translate(glm::mat4(1.0f), tc.Translation)
                        * glm::rotate(glm::mat4(1.0f), tc.Rotation.z, glm::vec3(0.0f, 0.0f, 1.0f))
                        * glm::translate(glm::mat4(1.0f), glm::vec3(bc2d.Offset, 0.001f))
                        * glm::scale(glm::mat4(1.0f), scale);

                    Renderer2D::DrawRect(transform, glm::vec4(0, 1, 0, 1));
                }


            }
        }

        // Draw selected entity outline 
        if (Entity selectedEntity = m_SceneHierarchyPanel.GetSelectedEntity()) {

            if (selectedEntity.HasComponent<TextComponent>())
            {
                TransformComponent tc = selectedEntity.GetComponent<TransformComponent>();

                auto& text = selectedEntity.GetComponent<TextComponent>();
                TransformComponent textQuad{ text.TextQuad };
                switch (text.TextAlignment) // TODO: More alignment options
                { // TODO: FIX THIS COMPLETELY (Scale/rotation) and then fix physics actual location
                case TextComponent::AlignmentType::Center:
                {
                    textQuad.Translation -= glm::vec3(text.TextQuad.Scale.x / 2,
                        text.TextQuad.Scale.y / 2,
                        0.0f);
                    break;
                }
                case TextComponent::AlignmentType::LeftCenter:
                {
                    textQuad.Translation -= glm::vec3(text.TextQuad.Scale.x,
                        text.TextQuad.Scale.y / 2,
                        0.0f);
                    break;
                }
                case TextComponent::AlignmentType::RightCenter:
                {
                    textQuad.Translation -= glm::vec3(0.0f,
                        text.TextQuad.Scale.y / 2,
                        0.0f);
                    break;
                }
                default:
                    break;
                }

                glm::mat4 transform = tc.GetTransform() * textQuad.GetTransform();

                Renderer2D::DrawRect(transform, glm::vec4(1, 0.5f, 0, 1));
            }
            else if (selectedEntity.HasComponent<SpriteRendererComponent>() ||
                selectedEntity.HasComponent<SpriteAnimationComponent>() ||
                selectedEntity.HasComponent<CircleRendererComponent>())
            {
                TransformComponent transform = selectedEntity.GetComponent<TransformComponent>();
                Renderer2D::DrawRect(transform.GetTransform(), glm::vec4(1, 0.5f, 0, 1));
            }
        }

        Renderer2D::EndScene();
    }

    void Editor::OnImGuiRender()
    {
        TED_PROFILE_FUNCTION();

        static bool dockspaceOpen = true;
        static bool opt_fullscreen = true;
        static bool opt_padding = false;
        static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

        // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
        // because it would be confusing to have two docking targets within each others.
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
        if (opt_fullscreen)
        {
            const ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->WorkPos);
            ImGui::SetNextWindowSize(viewport->WorkSize);
            ImGui::SetNextWindowViewport(viewport->ID);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
            window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
            window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        }
        else
        {
            dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
        }

        if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
            window_flags |= ImGuiWindowFlags_NoBackground;

        if (!opt_padding)
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
        if (!opt_padding)
            ImGui::PopStyleVar();

        if (opt_fullscreen)
            ImGui::PopStyleVar(2);

        ImGuiIO& io = ImGui::GetIO();
        ImGuiStyle& style = ImGui::GetStyle();
        float minWinSizeX = style.WindowMinSize.x;
        style.WindowMinSize.x = 400.0f;

        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
        {
            ImGuiID dockspace_id = ImGui::GetID("Teddy DockSpace");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
        }

        style.WindowMinSize.x = minWinSizeX;

        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen);
                ImGui::MenuItem("Padding", NULL, &opt_padding);
                ImGui::Separator();

                if (ImGui::MenuItem("New", "Ctrl+N"))
                    NewScene();

                if (ImGui::MenuItem("Open...", "Ctrl+O"))
                    OpenScene();

                if (ImGui::MenuItem("Save", "Ctrl+S"))
                    SaveScene();

                if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S"))
                    SaveSceneAs();

                if (ImGui::MenuItem("Generate Atlas", "Ctrl+Shift+A"))
                {
                    m_AtlasPopup = true;
                }

                if (ImGui::MenuItem("Exit")) Application::Get().Close();
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("ImGui Options"))
            {
                ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen);
                ImGui::MenuItem("Padding", NULL, &opt_padding);
                ImGui::Separator();

                if (ImGui::MenuItem("Flag: NoDockingOverCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_NoDockingOverCentralNode) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoDockingOverCentralNode; }
                if (ImGui::MenuItem("Flag: NoDockingSplit", "", (dockspace_flags & ImGuiDockNodeFlags_NoDockingSplit) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoDockingSplit; }
                if (ImGui::MenuItem("Flag: NoUndocking", "", (dockspace_flags & ImGuiDockNodeFlags_NoUndocking) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoUndocking; }
                if (ImGui::MenuItem("Flag: NoResize", "", (dockspace_flags & ImGuiDockNodeFlags_NoResize) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoResize; }
                if (ImGui::MenuItem("Flag: AutoHideTabBar", "", (dockspace_flags & ImGuiDockNodeFlags_AutoHideTabBar) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_AutoHideTabBar; }
                if (ImGui::MenuItem("Flag: PassthruCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) != 0, opt_fullscreen)) { dockspace_flags ^= ImGuiDockNodeFlags_PassthruCentralNode; }

                ImGui::EndMenu();
            }

            ImGui::EndMenuBar();
        }

        {
            static int atlasWidth = 2048;
            static int atlasHeight = 2048;
            static int atlasTolerance = 10;
            static char atlasName[128] = "";

            if (m_AtlasPopup) {
                ImGui::OpenPopup("Generate Atlas");
            }

            if (ImGui::BeginPopupModal("Generate Atlas", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
                ImGui::InputInt("Width", &atlasWidth);
                ImGui::InputInt("Height", &atlasHeight);
                ImGui::InputInt("Tolerance", &atlasTolerance);
                ImGui::InputText("Name", atlasName, IM_ARRAYSIZE(atlasName));

                if (ImGui::Button("Generate", ImVec2(120, 0))) {
                    GenerateAtlas(atlasWidth, atlasHeight, atlasTolerance, atlasName);
                    ImGui::CloseCurrentPopup();
                    m_AtlasPopup = false;
                }
                ImGui::SameLine();
                if (ImGui::Button("Cancel", ImVec2(120, 0))) {
                    ImGui::CloseCurrentPopup();
                    m_AtlasPopup = false;
                }
                ImGui::EndPopup();
            }
        }

        ImGui::Begin("Stats");

        auto stats = Renderer2D::GetStats();
        auto windowStats = Application::Get().GetWindow().GetStats();

        ImGui::Text("Window Stats:");
        ImGui::Text("Fps: %.1f", windowStats.FPS);

        ImGui::Text("Renderer2D Stats:");
        ImGui::Text("Draw Calls: %d", stats.DrawCalls);
        ImGui::Text("Quads: %d", stats.QuadCount);
        ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
        ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

        ImGui::End();

        m_SceneHierarchyPanel.OnImGuiRender();
        m_ContentBrowserPanel.OnImGuiRender();

        ImGui::Begin("Settings");
        ImGui::Checkbox("Show physics colliders", &m_ShowPhysicsColliders);
        auto& assets = AssetManager::Get();
        ImGui::Checkbox("Shader hot reloading", &assets.IsHotReloading<Shader>());
        ImGui::Checkbox("Texture2D hot reloading", &assets.IsHotReloading<Texture2D>());
        ImGui::End();

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0,0 });

        ImGui::Begin("ViewPort");

        auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
        auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
        auto viewportOffset = ImGui::GetWindowPos();
        m_ViewportBounds[0] = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
        m_ViewportBounds[1] = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };

        m_ViewportFocused = ImGui::IsWindowFocused();
        m_ViewportHovered = ImGui::IsWindowHovered();

        Application::Get().GetImGuiLayer()->SetBlockEvents(!m_ViewportHovered);

        ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
        m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

        ImGui::Image(m_PostProcessedFramebuffer->GetColorAttachmentRendererID(),
            ImVec2(m_ViewportSize.x, m_ViewportSize.y), ImVec2{ 0,1 }, ImVec2{ 1,0 });

        if (ImGui::BeginDragDropTarget())
        {
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("SCENE_CONTENT_BROWSER_ITEM"))
            {
                const wchar_t* path = (const wchar_t*)payload->Data;
                OpenScene(std::filesystem::path(g_AssetPath) / path);
            }
            ImGui::EndDragDropTarget();
        }

        // TODO: Implement anchor point
        // Gizmos
        if (m_SceneState == SceneState::Edit || m_SceneState == SceneState::Simulate)
        {
            Entity selectedEntity = m_SceneHierarchyPanel.GetSelectedEntity();
            if (selectedEntity && m_GizmoType != -1)
            {
                ImGuizmo::SetOrthographic(false);
                ImGuizmo::SetDrawlist();

                ImGuizmo::SetRect(m_ViewportBounds[0].x, m_ViewportBounds[0].y,
                    m_ViewportBounds[1].x - m_ViewportBounds[0].x,
                    m_ViewportBounds[1].y - m_ViewportBounds[0].y);

                // Editor camera
                const glm::mat4& cameraProjection = m_EditorCamera.GetProjection();
                glm::mat4 cameraView = m_EditorCamera.GetViewMatrix();

                // Entity transform
                auto& tc = selectedEntity.GetComponent<TransformComponent>();
                glm::mat4 transform = tc.GetTransform();

                // Snapping
                bool snap = Input::IsKeyPressed(Key::LCtrl);
                float snapValue = 0.5f;
                if (m_GizmoType == ImGuizmo::OPERATION::ROTATE)
                    snapValue = 45.0f;

                float snapValues[3] = { snapValue, snapValue, snapValue };

                ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
                    (ImGuizmo::OPERATION)m_GizmoType, ImGuizmo::MODE::LOCAL, glm::value_ptr(transform));

                if (ImGuizmo::IsUsing())
                {
                    glm::vec3 translation, rotation, scale;
                    ImGuizmo::DecomposeMatrixToComponents(
                        glm::value_ptr(transform),
                        glm::value_ptr(translation),
                        glm::value_ptr(rotation),
                        glm::value_ptr(scale));

                    glm::vec3 deltaRotation = glm::radians(rotation) - tc.Rotation;

                    if (m_GizmoType == ImGuizmo::OPERATION::TRANSLATE) { tc.Translation = translation; }
                    else if (m_GizmoType == ImGuizmo::OPERATION::ROTATE) { tc.Rotation += deltaRotation; }
                    else if (m_GizmoType == ImGuizmo::OPERATION::SCALE) { tc.Scale = scale; }

                }
            }
        }

        ImGui::Begin("Post Processing Effects");

        bool enabled = PostProcessing::IsEffectEnabled(PostProcessing::Effect::ChromaticAberration);
        ImGui::Checkbox("Chromatic Aberration", &enabled);
        if (enabled)
        {
            PostProcessing::EnableEffect(PostProcessing::Effect::ChromaticAberration);
            glm::vec3 offset = PostProcessing::GetChromaticAberrationOffset();
            ImGui::DragFloat3("Offset", glm::value_ptr(offset), 0.01f, -10.0f, 10.0f);
            PostProcessing::SetChromaticAberrationOffset(offset);
        }
        else
            PostProcessing::DisableEffect(PostProcessing::Effect::ChromaticAberration);

        ImGui::End();

        ImGui::End();

        ImGui::PopStyleVar();

        UI_Toolbar();

        ImGui::End();

    }

    void Editor::UI_Toolbar()
    {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 2));
        ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(0, 0));
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
        auto& colors = ImGui::GetStyle().Colors;
        const auto& buttonHovered = colors[ImGuiCol_ButtonHovered];
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(buttonHovered.x, buttonHovered.y, buttonHovered.z, 0.5f));
        const auto& buttonActive = colors[ImGuiCol_ButtonActive];
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(buttonActive.x, buttonActive.y, buttonActive.z, 0.5f));
        ImGui::Begin("toolbar##toolbar", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

        float size = ImGui::GetWindowHeight() - 4.0f;
        Ref<Texture2D> playIcon;
        Ref<Texture2D> simulateIcon;

        switch (m_SceneState)
        {
        case SceneState::Edit:
            playIcon = m_IconPlay;
            simulateIcon = m_IconSimulate;
            break;
        case SceneState::Play:
            playIcon = m_IconStop;
            simulateIcon = m_IconSimulate;
            break;
        case SceneState::Simulate:
            playIcon = m_IconPlay;
            simulateIcon = m_IconStop;
            break;
        }

        ImGui::SetCursorPosX((ImGui::GetWindowContentRegionMax().x * 0.5f) - (size));

        if (ImGui::ImageButton("Play", (ImTextureID)playIcon->GetRendererID(),
            ImVec2(size, size), ImVec2(0, 1), ImVec2(1, 0), ImVec4(0.0f, 0.0f, 0.0f, 0.0f),
            ImVec4(1.0f, 1.0f, 1.0f, 1.0f))) {
            if (m_SceneState == SceneState::Edit)
                OnScenePlay();

            else if (m_SceneState == SceneState::Play)
                OnSceneStop();
        }

        ImGui::SameLine();
        if (ImGui::ImageButton("Simulate", (ImTextureID)simulateIcon->GetRendererID(),
            ImVec2(size, size), ImVec2(0, 1), ImVec2(1, 0), ImVec4(0.0f, 0.0f, 0.0f, 0.0f),
            ImVec4(1.0f, 1.0f, 1.0f, 1.0f))) {
            if (m_SceneState == SceneState::Edit)
                OnSceneSimulate();

            else if (m_SceneState == SceneState::Simulate)
                OnSceneStop();
        }


        ImGui::PopStyleVar(2);
        ImGui::PopStyleColor(3);

        ImGui::End();
    }

    void Editor::RemoveSceneAssetBypass()
    {
        TED_PROFILE_FUNCTION();

        AssetManager::Get().RemoveBypassAll();
        AssetManager::Get().RemoveExpiredAll();
    }

    void Editor::SceneAssetBypass()
    {
        TED_PROFILE_FUNCTION();
        AssetManager::Get().BypassAll();
    }

    void Editor::GenerateAtlas(const int width, const int height, const int toleration, const std::string name)
    {
        std::vector<std::string> filepaths = FileDialogs::OpenFiles(
            "Image Files (*.png;*.jpg;*.jpeg;*.bmp;*.gif;*.tiff;*.tif;*.tga;*.dds;*.webp)\0*.png;*.jpg;*.jpeg;*.bmp;*.gif;*.tiff;*.tif;*.tga;*.dds;*.webp\0All Files (*.*)\0*.*\0"
        );
        GenerateAtlas(filepaths, width, height, toleration, "");
    }

    void Editor::GenerateAtlas()
    {
        static int width = 2048;
        static int height = 2048;
        static int tolerance = 10;
        static char name[128] = "";

        std::vector<std::string> filepaths = FileDialogs::OpenFiles(
            "Image Files (*.png;*.jpg;*.jpeg;*.bmp;*.gif;*.tiff;*.tif;*.tga;*.dds;*.webp)\0*.png;*.jpg;*.jpeg;*.bmp;*.gif;*.tiff;*.tif;*.tga;*.dds;*.webp\0All Files (*.*)\0*.*\0"
        );
        GenerateAtlas(filepaths, width, height, tolerance, name);
    }

    void Editor::GenerateAtlas(const std::vector<std::string>& filepaths, const int width, const int height, const int toleration, const std::string name)
    {
        for (std::string file : filepaths)
        {
            if (!std::filesystem::exists(file))
            {
                TED_CORE_WARN("File does not exist: {0}", file);
                return;
            }
        }

        if (filepaths.size() > 0)
            Atlas::Generate(filepaths, width, height, toleration, name);
    }

    void Editor::NewScene()
    {
        m_ActiveScene = CreateRef<Scene>();
        m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
        m_SceneHierarchyPanel.SetContext(m_ActiveScene);

        m_EditorScenePath = std::filesystem::path();
    }

    void Editor::OpenScene()
    {
        if (m_SceneState != SceneState::Edit)
            OnSceneStop();

        std::string filepath = FileDialogs::OpenFile("Teddy Scene (*.teddy)\0*.teddy\0");
        if (!filepath.empty())
        {
            OpenScene(filepath);
        }
    }

    void Editor::OpenScene(const std::filesystem::path& path)
    {
        if (m_SceneState != SceneState::Edit)
            OnSceneStop();

        if (path.extension().string() != ".teddy")
        {
            TED_CORE_WARN("Could not load {0} - not a scene file", path.filename().string());
            return;
        }

        SceneAssetBypass();
        Ref<Scene> newScene = CreateRef<Scene>();
        SceneSerializer serializer(newScene);

        if (serializer.Deserialize(path.string()))
        {

            m_EditorScene = newScene;
            m_EditorScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
            m_SceneHierarchyPanel.SetContext(m_EditorScene);

            m_ActiveScene = m_EditorScene;
            m_EditorScenePath = path;

        }

        RemoveSceneAssetBypass();
    }

    void Editor::SaveScene()
    {
        if (!m_EditorScenePath.empty())
            SerializeScene(m_ActiveScene, m_EditorScenePath);
        else
            SaveSceneAs();
    }

    void Editor::SaveSceneAs()
    {
        std::string filepath = FileDialogs::SaveFile("Teddy Scene (*.teddy)\0*.teddy\0");
        if (!filepath.empty())
        {
            SerializeScene(m_ActiveScene, filepath);
            m_EditorScenePath = filepath;
        }
    }

    void Editor::SerializeScene(Ref<Scene> scene, const std::filesystem::path& path)
    {
        SceneSerializer serializer(scene);
        serializer.Serialize(path.string());
    }

    void Editor::OnScenePlay()
    {
        m_ActiveScene = Scene::Copy(m_EditorScene);
        m_ActiveScene->OnRuntimeStart();
        m_SceneState = SceneState::Play;

        m_SceneHierarchyPanel.SetContext(m_ActiveScene);
    }

    void Editor::OnSceneStop()
    {
        m_ActiveScene->OnRuntimeStop();
        m_SceneState = SceneState::Edit;

        m_ActiveScene = m_EditorScene;
        m_SceneHierarchyPanel.SetContext(m_ActiveScene);
    }

    void Editor::OnSceneSimulate()
    {
        m_ActiveScene = Scene::Copy(m_EditorScene);
        m_ActiveScene->OnRuntimeStart();
        m_SceneState = SceneState::Simulate;

        m_SceneHierarchyPanel.SetContext(m_ActiveScene);
    }

    void Editor::OnDuplicateEntity()
    {
        if (m_SceneState != SceneState::Edit)
            return;
        Entity selectedEntity = m_SceneHierarchyPanel.GetSelectedEntity();

        if (selectedEntity)
            m_EditorScene->DuplicateEntity(selectedEntity);
    }

    void Editor::OnEvent(Event& event)
    {
        if (m_SceneState == SceneState::Play)
        {
            m_ActiveScene->OnEvent(event);
        }
        else if (m_SceneState == SceneState::Edit)
        {
            m_EditorCamera.OnEvent(event);
        }

        EventDispatcher dispatcher(event);
        dispatcher.Dispatch<KeyPressedEvent>(TED_BIND_EVENT_FN(Editor::OnKeyPressed));
        dispatcher.Dispatch<MouseButtonPressedEvent>(TED_BIND_EVENT_FN(Editor::OnMouseButtonPressed));
    }

    bool Editor::OnKeyPressed(KeyPressedEvent& e)
    {
        // Shortcuts
        if (e.IsRepeat())
            return false;

        bool control = Input::IsKeyPressed(Key::LCtrl) || Input::IsKeyPressed(Key::RCtrl);
        bool shift = Input::IsKeyPressed(Key::LShift) || Input::IsKeyPressed(Key::RShift);

        switch (e.GetKeyCode())
        {
        case Key::A:
        {
            if (control && shift)
                m_AtlasPopup = true;
            break;
        }
        case Key::N:
        {
            if (control)
                NewScene();
            break;
        }
        case Key::O:
        {
            if (control)
                OpenScene();
            break;
        }
        case Key::S:
        {
            if (control)
            {
                if (shift)
                    SaveSceneAs();
                else
                    SaveScene();
            }
            break;
        }
        case Key::D:
        {
            if (control)
                OnDuplicateEntity();
            break;
        }

        // Gizmos
        case Key::Q:
        {
            if (!ImGuizmo::IsUsing())
                m_GizmoType = -1;
            break;
        }
        case Key::W:
        {
            if (!ImGuizmo::IsUsing())
                m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;
            break;
        }
        case Key::E:
        {
            if (!ImGuizmo::IsUsing())
                m_GizmoType = ImGuizmo::OPERATION::ROTATE;
            break;
        }
        case Key::R:
        {
            if (!ImGuizmo::IsUsing())
                m_GizmoType = ImGuizmo::OPERATION::SCALE;
            break;
        }
        case Key::Delete:
        {
            if (Application::Get().GetImGuiLayer()->GetActiveWidgetID() == 0)
            {
                Entity selectedEntity = m_SceneHierarchyPanel.GetSelectedEntity();
                if (selectedEntity)
                {
                    m_SceneHierarchyPanel.SetSelectedEntity({});
                    m_ActiveScene->DestroyEntity(selectedEntity);
                }
            }
            break;
        }
        }
        return false;
    }

    bool Editor::OnMouseButtonPressed(MouseButtonPressedEvent& e)
    {
        if (e.GetMouseButton() == Mouse::Left)
        {
            if (m_ViewportHovered && !Input::IsKeyPressed(Key::LAlt) &&
                !ImGuizmo::IsOver())
            {
                m_SceneHierarchyPanel.SetSelectedEntity(m_HoveredEntity);
            }
        }
        return false;
    }
}