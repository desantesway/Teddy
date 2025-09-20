#include "EditorLayer.h"

#include <chrono>

#include "Platform/OpenGL/OpenGLShader.h"
#include "Teddy/Renderer/Shader.h"
#include "Teddy/Renderer/Font.h"
#include "Teddy/Renderer/MSDFData.h"

#include "Teddy/Scene/SceneSerializer.h"
#include "Teddy/Utils/PlatformUtils.h"

#include <imgui.h>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Teddy
{

    extern const std::filesystem::path g_AssetPath;

    EditorLayer::EditorLayer()
        : Layer("Editor Layer")
    {
    }

    void EditorLayer::OnAttach()
    {
        OPTICK_FRAME("MainThread");

		auto& assets = AssetManager::Get();

        m_IconPlay = assets.Load<Texture2D>("PlayButton","Resources/Icons/PlayButton.png");
        m_IconSimulate = assets.Load<Texture2D>("Simulate", "Resources/Icons/Simulate.png");
        m_IconStop = assets.Load<Texture2D>("StopButton", "Resources/Icons/StopButton.png");

        FramebufferSpecification fbSpec;
        fbSpec.Attachments = Teddy::FramebufferAttachmentSpecification({
            Teddy::FramebufferTextureSpecification(Teddy::FramebufferTextureFormat::RGBA8, Teddy::TextureFilterFormat::LINEAR, Teddy::TextureWrapFormat::REPEAT),
            Teddy::FramebufferTextureSpecification(Teddy::FramebufferTextureFormat::RED_INTEGER, Teddy::TextureFilterFormat::LINEAR, Teddy::TextureWrapFormat::REPEAT),
            Teddy::FramebufferTextureSpecification(Teddy::FramebufferTextureFormat::Depth, Teddy::TextureFilterFormat::LINEAR, Teddy::TextureWrapFormat::REPEAT)
        });
        fbSpec.Width = 1280;
        fbSpec.Height = 720;
        m_Framebuffer = Framebuffer::Create(fbSpec);

        NewScene();
        m_EditorScene = m_ActiveScene;

        auto commandLineArgs = Application::Get().GetSpecification().CommandLineArgs;

        if (commandLineArgs.Count > 1)
        {
            auto sceneFilePath = commandLineArgs[1];
            SceneSerializer serializer(m_ActiveScene);
            serializer.Deserialize(sceneFilePath);
        }

        m_EditorCamera = EditorCamera(30.0f, 1.778f, 0.1f, 1000.0f);

        auto redSquare = m_ActiveScene->CreateEntity("Red Square");
        redSquare.AddComponent<TextComponent>();
		redSquare.GetComponent<TextComponent>().SetString("Teddy Engine");

        auto cam = m_ActiveScene->CreateEntity("Camera");
        cam.AddComponent<CameraComponent>();

        //for (int i = 0; i < 50; i++)
        //{
        //    for (int y = 0; y < 50; y++)
        //    {
        //        auto entity = m_ActiveScene->CreateEntity("Entity " + std::to_string(i) + "x" + std::to_string(y));
        //        entity.AddComponent<SpriteRendererComponent>(glm::vec4((float)i / 20.0f, (float)y /20.0f, 0.5f, 1.0f));
        //        entity.GetComponent<TransformComponent>().Translation.x = (float)i + 0.1 * i;
        //        entity.GetComponent<TransformComponent>().Translation.y = (float)y + 0.1 * y;
        //    }
		//}

        Renderer2D::SetLineWidth(4.0f);
    }

    void EditorLayer::OnDetach()
    {
        TED_PROFILE_FUNCTION();
    }

    void EditorLayer::OnUpdate(Timestep ts)
    {
        TED_PROFILE_FUNCTION();

        if (FramebufferSpecification spec = m_Framebuffer->GetSpecification();
            m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f && // zero sized framebuffer is invalid
            (spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y))
        {
            m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);

            m_EditorCamera.SetViewportSize(m_ViewportSize.x, m_ViewportSize.y);
            m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
        }

        Renderer2D::ResetStats();

        {
            TED_PROFILE_SCOPE("Renderer Prep");

            m_Framebuffer->Bind();

            RenderCommand::SetClearColor({ 1.0f, 0.1f, 0.1f, 1 });
            RenderCommand::Clear();
        }

        m_Framebuffer->ClearAttachment(1, -1);

        {
            TED_PROFILE_SCOPE("Renderer Draw (CPU)");

            switch (m_SceneState)
            {
                case SceneState::Edit:
                {
                    m_EditorCamera.OnUpdate(ts);
                    m_ActiveScene->OnUpdateEditor(ts, m_EditorCamera);
                    break;
                }
                case SceneState::Play:
                {
                    m_ActiveScene->OnUpdateRuntime(ts);
                    break;
                }
                case SceneState::Simulate:
                {
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

            auto [mx, my] = ImGui::GetMousePos();
            mx -= m_ViewportBounds[0].x;
            my -= m_ViewportBounds[0].y;
            glm::vec2 viewportSize = m_ViewportBounds[1] - m_ViewportBounds[0];
            my = viewportSize.y - my;
            int mouseX = (int)mx;
            int mouseY = (int)my;

            if (mouseX >= 0 && mouseY >= 0 && mouseX < (int)viewportSize.x && mouseY < (int)viewportSize.y)
            {
                int pixelData = m_Framebuffer->ReadPixel(1, mouseX, mouseY);
                entt::entity handle = (entt::entity)pixelData;
                if (pixelData == -1) {
                    m_HoveredEntity = {};
                }
                else {
                    m_HoveredEntity = Entity(handle, m_ActiveScene.get());
                }
            }

            m_Framebuffer->Unbind();
        }

    }

    void EditorLayer::OnEvent(Event& event)
    {
        if (!(m_ViewportFocused)) return;

		m_ActiveScene->OnEvent(event);  

        if (m_SceneState == SceneState::Edit)
        {
            m_EditorCamera.OnEvent(event);
        }

        EventDispatcher dispatcher(event);
        dispatcher.Dispatch<KeyPressedEvent>(TED_BIND_EVENT_FN(EditorLayer::OnKeyPressed));
        dispatcher.Dispatch<MouseButtonPressedEvent>(TED_BIND_EVENT_FN(EditorLayer::OnMouseButtonPressed));
    }

    void EditorLayer::OnImGuiRender()
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



        ImGui::Begin("Stats");

        auto stats = Renderer2D::GetStats();
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
        ImGui::End();

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{0,0});

        ImGui::Begin("ViewPort");

        auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
        auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
        auto viewportOffset = ImGui::GetWindowPos();
        m_ViewportBounds[0] = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
        m_ViewportBounds[1] = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };

        m_ViewportFocused = ImGui::IsWindowFocused();
        m_ViewportHovered = ImGui::IsWindowHovered();

        ImGuiLayer* layer = Application::Get().GetImGuiLayer();
        if (!ImGui::IsAnyItemActive())
            layer->SetBlockEvents(!m_ViewportFocused && !m_ViewportHovered);
        else
            layer->SetBlockEvents(!m_ViewportFocused || !m_ViewportHovered);

        ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();

        m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

        ImGui::Image(m_Framebuffer->GetColorAttachmentRendererID(),
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

        ImGui::End();

        ImGui::PopStyleVar();

        UI_Toolbar();

        ImGui::End();

    }

    void EditorLayer::OnOverlayRender()
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

                Renderer2D::DrawCircleLine(transform, glm::vec4(0, 1, 0, 1), Renderer2D::GetLineWidth()/100);
            }

            auto viewBox = m_ActiveScene->GetAllEntitiesWith<TransformComponent, BoxCollider2DComponent>();
            for (auto [entity, tc, bc2d] : viewBox.each())
            {
				Entity ent = Entity{ entity, m_ActiveScene.get() };
                if (ent.HasComponent<TextComponent>())
                {
                    glm::vec3 scale = tc.Scale * glm::vec3(bc2d.Size * 2.0f, 1.0f);

                    glm::mat4 transform = glm::translate(glm::mat4(1.0f), tc.Translation)
                        * glm::rotate(glm::mat4(1.0f), tc.Rotation.z, glm::vec3(0.0f, 0.0f, 1.0f))
                        * glm::translate(glm::mat4(1.0f), glm::vec3(bc2d.Offset, 0.001f))
                        * glm::scale(glm::mat4(1.0f), scale)
                        * ent.GetComponent<TextComponent>().TextQuad.GetTransform();

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
                glm::vec3 scale = tc.Scale;

                glm::mat4 transform = glm::translate(glm::mat4(1.0f), tc.Translation)
                    * glm::rotate(glm::mat4(1.0f), tc.Rotation.z, glm::vec3(0.0f, 0.0f, 1.0f))
                    * glm::scale(glm::mat4(1.0f), scale)
                    * selectedEntity.GetComponent<TextComponent>().TextQuad.GetTransform();

                Renderer2D::DrawRect(transform, glm::vec4(1, 0.5f, 0, 1));
            }
            else if(selectedEntity.HasComponent<TextComponent>())
            {
                TransformComponent transform = selectedEntity.GetComponent<TransformComponent>();
                Renderer2D::DrawRect(transform.GetTransform(), glm::vec4(1, 0.5f, 0, 1));
            }
        }

		Renderer2D::EndScene();
    }

    void EditorLayer::UI_Toolbar()
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

    bool EditorLayer::OnKeyPressed(KeyPressedEvent& e)
    {
        // Shortcuts
        if (e.IsRepeat())
            return false;

        bool control = Input::IsKeyPressed(Key::LCtrl) || Input::IsKeyPressed(Key::RCtrl);
        bool shift = Input::IsKeyPressed(Key::LShift) || Input::IsKeyPressed(Key::RShift);

        switch (e.GetKeyCode())
        {
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
            }
        }
        return false;
    }

    bool EditorLayer::OnMouseButtonPressed(MouseButtonPressedEvent& e)
    {
        if (e.GetMouseButton() == Mouse::Left)
        {
            if (m_ViewportHovered && !Input::IsKeyPressed(Key::LAlt))
                m_SceneHierarchyPanel.SetSelectedEntity(m_HoveredEntity);
        }
        return false;
    }

    void RemoveSceneAssetBypass()
    {
        TED_PROFILE_FUNCTION();

        AssetManager::Get().RemoveBypassAll();
        AssetManager::Get().RemoveExpiredAll();
    }

    void SceneAssetBypass()
    {
        TED_PROFILE_FUNCTION();
        AssetManager::Get().BypassAll();
    }

    void EditorLayer::NewScene()
    {
        m_ActiveScene = CreateRef<Scene>();
        m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
        m_SceneHierarchyPanel.SetContext(m_ActiveScene);

        m_EditorScenePath = std::filesystem::path();
    }

    void EditorLayer::OpenScene()
    {
        if (m_SceneState != SceneState::Edit)
            OnSceneStop();

        std::string filepath = FileDialogs::OpenFile("Teddy Scene (*.teddy)\0*.teddy\0");
        if (!filepath.empty())
        {
            OpenScene(filepath);
        }
    }

    void EditorLayer::OpenScene(const std::filesystem::path& path)
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

    void EditorLayer::SaveScene()
    {
        if (!m_EditorScenePath.empty())
            SerializeScene(m_ActiveScene, m_EditorScenePath);
        else
            SaveSceneAs();
    }

    void EditorLayer::SaveSceneAs()
    {
        std::string filepath = FileDialogs::SaveFile("Teddy Scene (*.teddy)\0*.teddy\0");
        if (!filepath.empty())
        {
            SerializeScene(m_ActiveScene, filepath);
            m_EditorScenePath = filepath;
        }
    }

    void EditorLayer::SerializeScene(Ref<Scene> scene, const std::filesystem::path& path)
    {
        SceneSerializer serializer(scene);
        serializer.Serialize(path.string());
    }

    void EditorLayer::OnScenePlay()
    {
        m_ActiveScene = Scene::Copy(m_EditorScene);
		m_ActiveScene->OnRuntimeStart();
        m_SceneState = SceneState::Play;

        m_SceneHierarchyPanel.SetContext(m_ActiveScene);
    }

    void EditorLayer::OnSceneStop()
    {
        m_ActiveScene->OnRuntimeStop();
        m_SceneState = SceneState::Edit;

        m_ActiveScene = m_EditorScene;
        m_SceneHierarchyPanel.SetContext(m_ActiveScene);
    }

    void EditorLayer::OnSceneSimulate()
    {
        m_ActiveScene = Scene::Copy(m_EditorScene);
        m_ActiveScene->OnRuntimeStart();
        m_SceneState = SceneState::Simulate;

        m_SceneHierarchyPanel.SetContext(m_ActiveScene);
    }
    
    void EditorLayer::OnDuplicateEntity()
    {
        if (m_SceneState != SceneState::Edit)
            return;
        Entity selectedEntity = m_SceneHierarchyPanel.GetSelectedEntity();

        if (selectedEntity)
            m_EditorScene->DuplicateEntity(selectedEntity);
    }
}