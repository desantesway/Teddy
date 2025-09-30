#include "TeddyPch.h"

#include "EditorLayer.h"

#include "Teddy/PostProcessing/PostProcessing.h"

namespace Teddy
{
    extern const std::filesystem::path g_AssetPath;

    EditorLayer::EditorLayer()
        :CupheadLayer()
    {
    }

    void EditorLayer::OnAttach()
    {
        OPTICK_FRAME("MainThread");
        CupheadLayer::OnAttach();

        m_Editor.Init(m_ActiveScene);
    }

    void EditorLayer::OnDetach()
    {
        CupheadLayer::OnDetach();

        TED_PROFILE_FUNCTION();
    }

    void EditorLayer::OnUpdate(Timestep ts)
    {
        TED_PROFILE_FUNCTION();

        m_Editor.ResizeFramebuffer();

        Teddy::Renderer2D::ResetStats();

        {
            TED_PROFILE_SCOPE("Renderer Prep");

            Teddy::PostProcessing::Bind();

            Teddy::RenderCommand::SetClearColor({ 1.0f, 0.1f, 0.1f, 1 });
            Teddy::RenderCommand::Clear();

            Teddy::PostProcessing::Clear();
        }

        m_Editor.OnUpdate(ts);

        {
            TED_PROFILE_SCOPE("Post Processing");

            PostProcessing::Unbind();

            m_Editor.BindFramebuffer();

            PostProcessing::Apply();

            m_Editor.UnbindFramebuffer();
        }

    }

    void EditorLayer::OnEvent(Event& event)
    {

        if (m_Editor.GetState() == Editor::SceneState::Play)
        {
            CupheadLayer::OnEvent(event);
        }

        m_Editor.OnEvent(event);
    }

    void EditorLayer::OnImGuiRender()
    {
        m_Editor.OnImGuiRender();
    }

}