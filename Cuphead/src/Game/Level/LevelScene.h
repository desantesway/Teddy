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

		void OnUpdate(Teddy::Timestep ts);
	private:
		Teddy::Ref<Teddy::Scene> m_Scene;

		const float m_MovementVelocity = 1.1f;
		float m_MovementSpeed = 0.0f;

		struct Background
		{
			Teddy::Entity LeftBackground;
			Teddy::Entity RightBackground;
		};

		Background m_Background;

		float m_WorldWidth = 0.0f, m_WorldHeight = 0.0f;
	};
}