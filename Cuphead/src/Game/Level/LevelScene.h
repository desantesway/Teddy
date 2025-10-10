#pragma once

#include "Teddy/Scene/Scene.h"
#include "Teddy/Core/Timestep.h"
#include "Teddy/Scene/Entity.h"

namespace Cuphead
{
	class LevelScene
	{
	public:
		LevelScene() = default;
		~LevelScene() = default;
		
		Teddy::Ref<Teddy::Scene> Init(unsigned int character);
		Teddy::Ref<Teddy::Scene> GetScene() { return m_Scene; }

		void OnUpdate(Teddy::Timestep ts);

	private:
		void InitPhase1();
		void InitPhase1Background();
		void InitPhase1Foreground();
		void OnUpdatePhase1();
	private:
		Teddy::Ref<Teddy::Scene> m_Scene = nullptr;

		const float m_MovementVelocity = 1.1f;
		float m_MovementSpeed = 0.0f;

		struct Background
		{
			Teddy::Entity LeftBackground;
			Teddy::Entity RightBackground;

			Teddy::Entity Spire;

			Teddy::Entity Cloud1Left;
			Teddy::Entity Cloud1Right;
			Teddy::Entity Cloud2Left;
			Teddy::Entity Cloud2Right;
			Teddy::Entity Cloud3Left;
			Teddy::Entity Cloud3Right;
			Teddy::Entity Cloud4Left;
			Teddy::Entity Cloud4Right;
			Teddy::Entity Cloud5Left;
			Teddy::Entity Cloud5Right;
		};

		struct Foreground
		{
			Teddy::Entity Cloud1Left;
			Teddy::Entity Cloud1Right;
			Teddy::Entity Cloud2Left;
			Teddy::Entity Cloud2Right;
			Teddy::Entity Cloud3Left;
			Teddy::Entity Cloud3Right;
		};

		Background m_Background;
		Foreground m_Foreground;

		float m_WorldWidth = 0.0f, m_WorldHeight = 0.0f;
		int m_Phase = 1;
	};
}