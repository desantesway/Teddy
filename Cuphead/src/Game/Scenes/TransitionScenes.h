#pragma once

#include "Teddy/Scene/Scene.h"
#include "Teddy/Core/Timestep.h"

namespace Cuphead
{
	class TransitionScenes
	{
	public:

		TransitionScenes() = default;
		~TransitionScenes() = default;

		void Init();

		void OnUpdate(Teddy::Timestep ts);

		void InitFadeScene();
		bool FadeIn();
		bool FadeIn(const float& velocity, const Teddy::Timestep& ts);
		bool IsFadedIn();
		bool FadeOut();
		bool FadeOut(const float& velocity, const Teddy::Timestep& ts);
		bool IsFadedOut();
		void SetFadeAlpha(const float& alpha);

		void InitCircleScene();
		bool CircleIn();
		bool CircleIn(const float& velocity, const Teddy::Timestep& ts);
		bool IsCircleIn();
		bool CircleOut();
		bool CircleOut(const float& velocity, const Teddy::Timestep& ts);
		bool IsCircleOut();
		void SetCircleAlpha(const float& alpha);
	private:
		void CheckAndCreateScene();
	private:

		bool m_Faded = false;
		bool m_FadeIn = false;
		bool m_FadeOut = false;

		bool m_CircleClosed = false;
		bool m_CircleIn = false;
		bool m_CircleOut = false;

		Teddy::Ref<Teddy::Scene> m_Scene = nullptr;

		Teddy::Ref<Teddy::Entity> m_TransitionQuad = nullptr;
		Teddy::Ref<Teddy::Entity> m_TransitionCircle = nullptr;
	};
}