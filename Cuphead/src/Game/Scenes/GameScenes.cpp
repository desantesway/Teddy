#include "GameScenes.h"

namespace Cuphead
{
    GameScenes* GameScenes::instance = nullptr;

    MainTitleScene GameScenes::m_MainTitleScene;
    TransitionScenes GameScenes::m_TransitionScenes;

    int GameScenes::m_CurrentScene = 0;

    GameScenes::GameScenes()
    {
        if (instance)
            TED_CORE_ASSERT(false, "GameScenes instance already exists!");
        instance = this;
    }

    void GameScenes::Init()
    {
        m_TransitionScenes.Init();
    }

    bool GameScenes::OnUpdate(Teddy::Timestep ts)
    {
        {
            TED_PROFILE_SCOPE("Game Scene Preparation");

            m_TransitionScenes.OnUpdate(ts);

            switch (m_CurrentScene)
            {
                case 1:
                    return m_TransitionScenes.IsFadedIn() && m_TransitionScenes.IsCircleIn();
                default:
                    TED_CORE_INFO("No scene loaded for index {0}", m_CurrentScene);
                    break;
            }
        }

        return false;
    }

    Teddy::Ref<Teddy::Scene> GameScenes::InitMainTitle()
    {
        return m_MainTitleScene.Init();
    }

    Teddy::Ref<Teddy::Scene> GameScenes::InitTitleMenu()
    {
		return nullptr;
    }

    Teddy::Ref<Teddy::Scene> GameScenes::InitNextScene()
    {
        switch (++m_CurrentScene)
        {
            case 1:
                return InitMainTitle();
            case 2:
                return InitMainTitle(); //InitTitleMenu() !!!!!!
            default:
                TED_CORE_INFO("No scene found for index {0}", m_CurrentScene);
                return nullptr;
		}
    }

    bool GameScenes::OnKeyPressed(Teddy::KeyPressedEvent& e)
    {
        TED_PROFILE_FUNCTION();

        switch (m_CurrentScene)
        {
        case 1:
            m_TransitionScenes.FadeIn();
            m_TransitionScenes.CircleIn();
            return true;
        default:
            TED_CORE_INFO("No scene found for index {0}", m_CurrentScene);
            return false;
        }

        return false;
    }

    void GameScenes::OnEvent(Teddy::Event& event)
    {
        TED_PROFILE_FUNCTION();

        Teddy::EventDispatcher dispatcher(event);
        dispatcher.Dispatch<Teddy::KeyPressedEvent>(TED_BIND_EVENT_FN(GameScenes::OnKeyPressed));
    }
}