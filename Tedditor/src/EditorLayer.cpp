#include "TeddyPch.h"

#include "EditorLayer.h"

#include "Teddy/PostProcessing/PostProcessing.h"

namespace Teddy
{
    EditorLayer::EditorLayer()
        : Layer("Editor Layer")
    {
    }

    void EditorLayer::OnAttach()
    {
        OPTICK_FRAME("MainThread");

        AssetManager::Get().SetHotReloading(true);
        m_Editor.Init();
    }

    void EditorLayer::OnDetach()
    {
        TED_PROFILE_FUNCTION();
    }

    void EditorLayer::OnUpdate(Timestep ts)
    {
        TED_PROFILE_FUNCTION();

        m_Editor.ResizeFramebuffer();

        Renderer2D::ResetStats();

        {
            TED_PROFILE_SCOPE("Renderer Prep");

            PostProcessing::Bind();

            RenderCommand::SetClearColor({ 1.0f, 0.1f, 0.1f, 1 });
            RenderCommand::Clear();
        }

        PostProcessing::Clear();

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
        m_Editor.OnEvent(event);
    }

    void EditorLayer::OnImGuiRender()
    {
		m_Editor.OnImGuiRender();
    }
}