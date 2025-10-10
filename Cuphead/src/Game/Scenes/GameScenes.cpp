#include "GameScenes.h"

namespace Cuphead
{
    GameScenes* GameScenes::instance = nullptr;

    Teddy::Ref<Teddy::Scene> GameScenes::m_ActiveScene = nullptr;

    TransitionScenes GameScenes::m_TransitionScenes;
    MainTitleScene GameScenes::m_MainTitleScene;
    MainMenuScene GameScenes::m_MainMenuScene;
	LevelScene GameScenes::m_LevelScene;

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
                case 2:
                    m_MainMenuScene.OnUpdate(ts);
                    if (m_MainMenuScene.LoadLevel())
                    {
                        m_TransitionScenes.FadeIn();
                        m_TransitionScenes.CircleIn();
                    }
                    return m_MainMenuScene.LoadLevel() && m_TransitionScenes.IsFadedIn() && m_TransitionScenes.IsCircleIn();
                case 3:
                    if (!m_TransitionScenes.IsCircleOut())
                    {
                        m_TransitionScenes.CircleOut();
                    }
                    m_LevelScene.OnUpdate(ts);
                    return false;
                default:
                    TED_CORE_INFO("No scene loaded for index {0}", m_CurrentScene);
                    break;
            }
        }

        return false;
    }

    Teddy::Ref<Teddy::Scene> GameScenes::InitMainTitle()
    {
        m_MainMenuScene.~MainMenuScene();
        Teddy::Ref<Teddy::Scene> scene = m_MainTitleScene.Init();
        m_ActiveScene = scene;
        return m_ActiveScene;
    }

    Teddy::Ref<Teddy::Scene> GameScenes::InitTitleMenu()
    {
        m_TransitionScenes.FadeOut();
        m_TransitionScenes.SetCircleAlpha(0.0f);
        m_TransitionScenes.CircleOut();
		Teddy::Ref<Teddy::Scene> scene = m_MainMenuScene.Init();
		m_ActiveScene = scene;
		return m_ActiveScene;
    }

    Teddy::Ref<Teddy::Scene> GameScenes::InitLevel()
    {
        m_TransitionScenes.SetFadeAlpha(0.0f);
        m_TransitionScenes.FadeOut();
        m_TransitionScenes.SetCircleAlpha(1.0f);
        Teddy::Ref<Teddy::Scene> scene = m_LevelScene.Init(m_MainMenuScene.LoadLevel());
        m_ActiveScene = scene;
        return m_ActiveScene;
    }

    void GameScenes::FreeScenes()
    {
        if (m_CurrentScene != 2)
            m_MainTitleScene.~MainTitleScene();
		if (m_CurrentScene != 3)
            m_MainMenuScene.~MainMenuScene();
        if (m_CurrentScene != 4)
            m_LevelScene.~LevelScene();
        Teddy::AssetManager::Get().RemoveExpiredAll();
    }

    Teddy::Ref<Teddy::Scene> GameScenes::InitNextScene()
    {
        //m_CurrentScene = 2;
        switch (++m_CurrentScene)
        {
            case 1:
                return InitMainTitle();
            case 2:
                return InitTitleMenu();
            case 3:
                return InitLevel();
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
            //TED_CORE_INFO("No scene found for index {0}", m_CurrentScene);
            return false;
        }

        return false;
    }

    void GameScenes::OnEvent(Teddy::Event& event)
    {
        TED_PROFILE_FUNCTION();

        Teddy::EventDispatcher dispatcher(event);
        dispatcher.Dispatch<Teddy::KeyPressedEvent>(TED_BIND_EVENT_FN(GameScenes::OnKeyPressed));

        switch (m_CurrentScene)
        {
        case 2:
			m_MainMenuScene.OnEvent(event);
            break;
        default:
            break;
        }
    }
}