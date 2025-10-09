#include "TransitionScenes.h"

#include <Teddy.h>

namespace Cuphead
{

    void TransitionScenes::Init()
    {
        InitFadeScene();
		InitCircleScene();
	}

    void TransitionScenes::OnUpdate(Teddy::Timestep ts)
    {
        TED_PROFILE_FUNCTION();
        
        if (m_FadeIn && !m_Faded)
        {
            if(FadeIn(2.5f, ts))
            {
                m_Faded = true;
			}
        }
        else if (m_FadeOut && m_Faded)
        {
            if (FadeOut(2.5f, ts))
            {
                m_Faded = false;
            }
        }

        if (m_CircleIn && !m_CircleClosed)
        {
            if (CircleIn(2.0f, ts))
            {
                m_CircleClosed = true;
            }
        }
        else if (m_CircleOut && m_CircleClosed)
        {
            if (CircleOut(2.0f, ts))
            {
                m_CircleClosed = false;
            }
        }

        m_Scene->AlwaysOnUpdate();
        m_Scene->OnUpdateRuntime(ts);
	}

    void TransitionScenes::CheckAndCreateScene()
    {
        if (!m_Scene)
        {
            m_Scene = Teddy::CreateRef<Teddy::Scene>();
            m_Scene->OnRuntimeStart();

            auto camEntt = m_Scene->CreateEntity("Camera");
            camEntt.GetComponent<Teddy::TransformComponent>().Translation = { 0.0f, 0.0f, 9.0f };
            auto& cam = camEntt.AddComponent<Teddy::CameraComponent>();
            cam.Camera.SetProjectionType(Teddy::SceneCamera::ProjectionType::Perspective);
            cam.Camera.SetViewportSize(1920, 1080);
        }
    }

	void TransitionScenes::InitFadeScene()
	{
        CheckAndCreateScene();

        auto transitionQuadAnimation = m_Scene->CreateEntity("Title Transition Quad");
        m_TransitionQuad = Teddy::CreateRef<Teddy::Entity>(transitionQuadAnimation);
        auto& transitionQuad = m_TransitionQuad->AddComponent<Teddy::SpriteRendererComponent>();
        transitionQuad.Color = glm::vec4(0.0f);
        transitionQuad.IsBackground = true;
        auto& transitionQuadTransform = transitionQuadAnimation.GetComponent<Teddy::TransformComponent>();
        transitionQuadTransform.Translation = glm::vec3(0.0f, 0.0f, 6.0f);
	}

    bool TransitionScenes::FadeIn()
    {
        TED_PROFILE_FUNCTION();

        m_FadeOut = false;
        m_FadeIn = true;

        return false;
    }

    bool TransitionScenes::FadeOut()
    {
        TED_PROFILE_FUNCTION();

        m_FadeIn = false;
        m_FadeOut = true;

        return false;
    }

    bool TransitionScenes::FadeIn(const float& velocity, const Teddy::Timestep& ts)
    {
        TED_PROFILE_FUNCTION();

        auto& transitionQuad = m_TransitionQuad->GetComponent<Teddy::SpriteRendererComponent>();
        transitionQuad.Color.a += velocity * ts;
        if (transitionQuad.Color.a > 1.0f)
        {
            transitionQuad.Color.a = 1.0f;
            return true;
        }

        return false;
    }

    bool TransitionScenes::FadeOut(const float& velocity, const Teddy::Timestep& ts)
    {
        TED_PROFILE_FUNCTION();

        auto& transitionQuad = m_TransitionQuad->GetComponent<Teddy::SpriteRendererComponent>();
        transitionQuad.Color.a -= velocity * ts;
        if (transitionQuad.Color.a < 0.0f)
        {
            transitionQuad.Color.a = 0.0f;
            return true;
        }

        return false;
    }

    bool TransitionScenes::IsFadedIn()
    {
        bool ret = m_Faded && m_FadeIn;

        return ret;
    }

    void TransitionScenes::SetFadeAlpha(const float& alpha)
    {
        auto& transitionQuad = m_TransitionQuad->GetComponent<Teddy::SpriteRendererComponent>();
        transitionQuad.Color.a = alpha;
    }

    void TransitionScenes::InitCircleScene()
    {
        CheckAndCreateScene();

        // Transition Circle
        auto transitionAnimation = m_Scene->CreateEntity("Title Transition Circle");
        m_TransitionCircle = Teddy::CreateRef<Teddy::Entity>(transitionAnimation);
        auto& transitionCircle = m_TransitionCircle->AddComponent<Teddy::CircleRendererComponent>();
        transitionCircle.Color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
        transitionCircle.Thickness = 0.0f;
        auto& transitionTransform = transitionAnimation.GetComponent<Teddy::TransformComponent>();
        transitionTransform.Scale = glm::vec3(7.0f, 7.0f, 1.0f);
        transitionTransform.Translation = glm::vec3(0.0f, 0.0f, 5.0f);
    }

    bool TransitionScenes::CircleIn()
    {
        TED_PROFILE_FUNCTION();

		m_CircleOut = false;
        m_CircleIn = true;

        return false;
    }

    bool TransitionScenes::CircleIn(const float& velocity, const Teddy::Timestep& ts)
    {
        TED_PROFILE_FUNCTION();

        auto& transitionCircle = m_TransitionCircle->GetComponent<Teddy::CircleRendererComponent>();
        transitionCircle.Thickness += velocity * ts;
        if (transitionCircle.Thickness > 1.0f)
        {
            transitionCircle.Thickness = 1.0f;
            return true;
        }

        return false;
    }

    bool TransitionScenes::IsCircleIn()
    {
        return m_CircleClosed && m_CircleIn;
    }

    bool TransitionScenes::IsCircleOut()
    {
        return !m_CircleClosed && m_CircleOut;
    }

    bool TransitionScenes::CircleOut()
    {
        TED_PROFILE_FUNCTION();

        m_CircleIn = false;
        m_CircleOut = true;

        return false;
    }

    bool TransitionScenes::CircleOut(const float& velocity, const Teddy::Timestep& ts)
    {
        TED_PROFILE_FUNCTION();

        auto& transitionCircle = m_TransitionCircle->GetComponent<Teddy::CircleRendererComponent>();
        transitionCircle.Thickness -= velocity * ts;
        if (transitionCircle.Thickness < 0.0f)
        {
            transitionCircle.Thickness = 0.0f;
            return true;
        }

        return false;
    }

    void TransitionScenes::SetCircleAlpha(const float& alpha)
    {
        auto& transitionCircle = m_TransitionCircle->GetComponent<Teddy::CircleRendererComponent>();
		transitionCircle.Color.a = alpha;
    }
}