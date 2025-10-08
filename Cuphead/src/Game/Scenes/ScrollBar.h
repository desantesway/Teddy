#pragma once

#include "teddyPch.h"
#include "Teddy/Scene/Scene.h"
#include "Teddy/Scene/Components.h"
#include "Teddy/Scene/Entity.h"

namespace Cuphead
{
	class ScrollBar
	{
	public:
		ScrollBar() = default;
		ScrollBar(Teddy::Ref<Teddy::Scene> scene, Teddy::TransformComponent& transform, int position, float spacing);
		~ScrollBar() = default;

		void Increment();
		void Decrement();
		void SetPosition(int position);
		void UpdateBar(glm::vec4& Color);
	private:
		std::vector<Teddy::Entity> m_Entities;
		Teddy::Entity m_CurrentPositionEntity;
		
		float m_Spacing = 0;	
		int m_CurrentPosition = 0;
	};
}