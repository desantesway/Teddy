#include "CupheadMainLayer.h"

#include <chrono>

#include <imgui.h>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Teddy/Scene/SceneSerializer.h"
#include "Teddy/Utils/PlatformUtils.h"

#include "GameScenes.h"

CupheadLayer::CupheadLayer()
	: Layer("Main Layer")
{

}

void CupheadLayer::OnAttach()
{
	TED_PROFILE_FUNCTION();

    //TODO: change this to a gamestate singleton

	m_ActiveScene = GameScenes::Get()->InitTitleStart();

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

    if (GameScenes::Get()->OnUpdate(ts))
        m_ActiveScene = GameScenes::Get()->InitNextScene();

    {
        TED_PROFILE_SCOPE("Renderer Draw (CPU)");

        m_ActiveScene->AlwaysOnUpdate();
        m_ActiveScene->OnUpdateRuntime(ts);
    }
}

bool CupheadLayer::OnKeyPressed(Teddy::KeyPressedEvent& e)
{
    TED_PROFILE_FUNCTION();

    GameScenes::Get()->ProceedToMainMenu();

    return true;
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
    dispatcher.Dispatch<Teddy::WindowResizeEvent>(TED_BIND_EVENT_FN(CupheadLayer::OnWindowResize));

    m_ActiveScene->OnEvent(event);
}

void CupheadLayer::OnImGuiRender()
{
	TED_PROFILE_FUNCTION();
}