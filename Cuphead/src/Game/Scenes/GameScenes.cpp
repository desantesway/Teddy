#include "GameScenes.h"

namespace Cuphead
{
    void GameScenes::Init()
    {
        m_TransitionScenes.Init();
    }

    bool GameScenes::OnUpdate(Teddy::Timestep& ts)
    {
        {
            TED_PROFILE_SCOPE("Game Scene Preparation");

            m_TransitionScenes.OnUpdate(ts);

            switch (m_CurrentScene)
            {
                case 1:
                    return m_TransitionScenes.IsFadedIn() && m_TransitionScenes.IsCircleIn();
                case 2:
                    if (!m_MainMenuScene) return false;

                    m_MainMenuScene->OnUpdate(ts);
                    if (m_MainMenuScene->LoadLevel())
                    {
                        m_TransitionScenes.FadeIn();
                        m_TransitionScenes.CircleIn();

						m_IsCuphead = (m_MainMenuScene->LoadLevel() == 1);
                    }
                    return m_MainMenuScene->LoadLevel() && m_TransitionScenes.IsFadedIn() && m_TransitionScenes.IsCircleIn();
                case 3:
                    if (!m_LevelScene) return false;

                    if (!m_TransitionScenes.IsCircleOut())
                    {
                        m_TransitionScenes.CircleOut();
                        m_LevelScene->LoadIntro();
                    }
                    else if (!m_LevelScene->HasIntroStarted())
                    {
                        m_LevelScene->StartIntro();
                    }
                    if(m_LevelScene->WantsToRetry() || m_LevelScene->WantsToExit())
                    {
                        m_TransitionScenes.FadeIn();
                        m_TransitionScenes.CircleIn();
                        return m_TransitionScenes.IsFadedIn() && m_TransitionScenes.IsCircleIn();
					}

                    m_LevelScene->OnUpdate(ts);
                    return false;
                default:
                    TED_CORE_INFO("No scene loaded for index {0}", m_CurrentScene);
                    break;
            }
        }

        return false;
    }

    Teddy::Ref<Teddy::Scene> GameScenes::InitTitle()
    {
		m_MainTitleScene = MainTitleScene();

        m_ActiveScene = m_MainTitleScene.Init();
        m_LevelScene = nullptr;
        m_MainMenuScene = nullptr;
        return m_ActiveScene;
    }

    Teddy::Ref<Teddy::Scene> GameScenes::InitMainMenu() // TODO: assets bypass
    {
        m_MainMenuScene = Teddy::CreateRef<MainMenuScene>();

        m_TransitionScenes.FadeOut();
        m_TransitionScenes.SetCircleAlpha(0.0f); 
        m_TransitionScenes.CircleOut();
        m_ActiveScene = m_MainMenuScene->Init();
        m_LevelScene = nullptr;
		return m_ActiveScene;
    }

    Teddy::Ref<Teddy::Scene> GameScenes::InitLevel()
    {
        m_LevelScene = Teddy::CreateRef<LevelScene>();

        m_TransitionScenes.SetFadeAlpha(0.0f);
        m_TransitionScenes.FadeOut();
        m_TransitionScenes.SetCircleAlpha(1.0f);
        m_ActiveScene = m_LevelScene->Init(m_IsCuphead);
		m_MainMenuScene = nullptr;
        return m_ActiveScene;
    }

    void GameScenes::FreeScenes()
    {
        if (m_CurrentScene != 1)
            m_MainTitleScene.~MainTitleScene();
        
        Teddy::AssetManager::Get().RemoveExpiredAll();
    }

    Teddy::Ref<Teddy::Scene> GameScenes::InitNextScene()
    {
        switch (++m_CurrentScene)
        {
            case 1:
                return InitTitle();
            case 2:
                return InitMainMenu();
            case 3:
                return InitLevel();
            case 4:
                if (m_LevelScene->WantsToExit())
                {
					m_CurrentScene = 2;
                    return InitMainMenu();
                }
                else if (m_LevelScene->WantsToRetry())
                {
					m_CurrentScene = 3;
                    return InitLevel();
                }
                else
                {
                    TED_CORE_INFO("No scene change requested after level");
                    return nullptr;
				}
            default:
                TED_CORE_INFO("No scene found for index {0}", m_CurrentScene);
                return nullptr;
		}
    }

    bool GameScenes::OnKeyPressed(Teddy::KeyPressedEvent& e)
    {
        TED_PROFILE_FUNCTION();

        m_TransitionScenes.FadeIn();
        m_TransitionScenes.CircleIn();

        return true;
    }

    void GameScenes::OnEvent(Teddy::Event& event)
    {
        TED_PROFILE_FUNCTION();

        Teddy::EventDispatcher dispatcher(event);

        switch (m_CurrentScene)
        {
        case 1:
            dispatcher.Dispatch<Teddy::KeyPressedEvent>(TED_BIND_EVENT_FN(GameScenes::OnKeyPressed));
            break;
        case 2:
            if (m_MainMenuScene)
			    m_MainMenuScene->OnEvent(event);
            break;
		case 3:
            if (m_LevelScene)
                m_LevelScene->OnEvent(event);
			break;
        default:
            break;
        }
    }
}