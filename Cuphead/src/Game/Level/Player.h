#pragma once

#include "Teddy/Scene/Scene.h"
#include "Teddy/Core/Timestep.h"
#include "Teddy/Scene/Entity.h"

namespace Cuphead
{
	class Player
	{
	public:
		Player() = default;
		~Player() = default;
		void InitCuphead(Teddy::Ref<Teddy::Scene> scene);
		void InitMugman(Teddy::Ref<Teddy::Scene> scene);
		void StartIntro();

		Teddy::Entity GetEntity() { return m_Entity; }

		void OnUpdate(Teddy::Timestep ts);
	private:
		Teddy::Entity m_Entity;
		const float m_MovementVelocity = 1.1f;
		float m_MovementSpeed = 0.0f;
	};
}