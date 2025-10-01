#include "TeddyPch.h"
#include "CupheadMainLayer.h"

#include "Teddy/Scene/SceneSerializer.h"
#include "Teddy/Utils/PlatformUtils.h"

#include "Scenes/GameScenes.h"

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

	    Teddy::PostProcessing::EnableEffect(Teddy::PostProcessing::Effect::ChromaticAberration);
	    Teddy::PostProcessing::SetChromaticAberrationOffset({ 5.0f, 5.0f, -5.0f }); // Max: 10
    }

    void CupheadLayer::OnDetach()
    {
	    TED_PROFILE_FUNCTION();
    }

    void CupheadLayer::OnUpdate(Teddy::Timestep ts)
    {
	    TED_PROFILE_FUNCTION();

        {
            TED_PROFILE_SCOPE("Renderer Draw (CPU)");

            m_ActiveScene->AlwaysOnUpdate();
            m_ActiveScene->OnUpdateRuntime(ts);
        }

        if (GameScenes::OnUpdate(ts))
            m_ActiveScene = GameScenes::InitNextScene();
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
    }

}