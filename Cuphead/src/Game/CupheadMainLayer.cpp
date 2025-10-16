#include "TeddyPch.h"
#include "CupheadMainLayer.h"

#include <Teddy.h>

#include "Scenes/GameScenes.h"

#include <imgui.h>

namespace Cuphead
{

    CupheadLayer::CupheadLayer()
	    : Layer("Main Layer")
    {

    }

    void CupheadLayer::OnAttach()
    {
	    TED_PROFILE_FUNCTION();

		GameScenes::Init();
	    m_ActiveScene = GameScenes::InitNextScene();
        m_ActiveScene->OnRuntimeStart();
        GameScenes::FreeScenes();

	    Teddy::PostProcessing::EnableEffect(Teddy::PostProcessing::Effect::ChromaticAberration);
	    Teddy::PostProcessing::SetChromaticAberrationOffset({ 2.5f, 2.5f, -2.5f });
		Teddy::Application::Get().GetWindow().SetScreenMode(Teddy::ScreenMode::Windowed); // TODO: Change to fullscreen for release
    }

    void CupheadLayer::OnDetach()
    {
	    TED_PROFILE_FUNCTION();
    }

    void CupheadLayer::OnUpdate(Teddy::Timestep& ts)
    {
	    TED_PROFILE_FUNCTION();

        {
            TED_PROFILE_SCOPE("Renderer Draw (CPU)");
            m_ActiveScene->AlwaysOnUpdate();
            m_ActiveScene->OnUpdateRuntime(ts);
            m_ActiveScene->ShowPhysicsColliders();
        }

        if (GameScenes::OnUpdate(ts))
        {
            m_ActiveScene->OnRuntimeStop();
            m_ActiveScene = GameScenes::InitNextScene();
            m_ActiveScene->OnRuntimeStart();
            GameScenes::FreeScenes();
        }

        m_ActiveScene->SimulatePhysics(ts);
    }

    bool CupheadLayer::OnKeyPressed(Teddy::KeyPressedEvent& e)
    {
        TED_PROFILE_FUNCTION();
        
        return false;
    }

    bool CupheadLayer::OnWindowResize(Teddy::WindowResizeEvent& e)
    {
        TED_PROFILE_FUNCTION();

        m_ActiveScene->OnViewportResize((uint32_t)e.GetWidth(), (uint32_t)e.GetHeight());

        return false;
    }

    void CupheadLayer::OnEvent(Teddy::Event& event)
    {
        TED_PROFILE_FUNCTION();
        
        Teddy::EventDispatcher dispatcher(event);
        dispatcher.Dispatch<Teddy::KeyPressedEvent>(TED_BIND_EVENT_FN(CupheadLayer::OnKeyPressed));
        dispatcher.Dispatch<Teddy::WindowResizeEvent>(TED_BIND_EVENT_FN(CupheadLayer::OnWindowResize)); // TODO: WINDOW SIZE/RESIZE

        GameScenes::Get()->OnEvent(event);
        m_ActiveScene->OnEvent(event);
    }

    void CupheadLayer::OnImGuiRender()
    {
	    TED_PROFILE_FUNCTION();

#ifdef TED_DIST
        return;
#endif

        ImGui::Begin("Stats");

        auto stats = Teddy::Renderer2D::GetStats();
        auto windowStats = Teddy::Application::Get().GetWindow().GetStats();

        ImGui::Text("Window Stats:");
        ImGui::Text("Fps: %.1f", windowStats.FPS);

        ImGui::Text("Renderer2D Stats:");
        ImGui::Text("Draw Calls: %d", stats.DrawCalls);
        ImGui::Text("Quads: %d", stats.QuadCount);
        ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
        ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

        ImGui::End();
    }

}