#include "Slider.h"

#include <Teddy.h>

namespace Cuphead
{
	Slider::Slider(Teddy::Ref<Teddy::Scene> scene, Teddy::TransformComponent& transform, int position, float spacing)
	{
		constexpr int barSize = 10;
		m_Spacing = spacing;
		m_CurrentPosition = position;
		for (int i = 0; i <= 10; i++)
		{
			auto entity = scene->CreateEntity("Scroll Bar" + std::to_string(i));
			entity.AddComponent<Teddy::SpriteRendererComponent>();

			auto& transf = entity.GetComponent<Teddy::TransformComponent>();
			transf.Scale *= glm::vec3(1.45f, 0.75f, 1.0f);
			transf.Translation += glm::vec3(m_Spacing * i, 0.0f, 0.0f);

			transf.Translation += transform.Translation;
			transf.Scale *= transform.Scale;

			m_Entities.push_back(entity);

			if (m_CurrentPosition == i)
			{
				auto curEntity = scene->CreateEntity("Scroll Bar Current Position");
				curEntity.AddComponent<Teddy::SpriteRendererComponent>();
				
				auto& curTransform = curEntity.GetComponent<Teddy::TransformComponent>();
				curTransform.Scale *= glm::vec3(0.5f, 4.0f, 1.0f);
				curTransform.Translation += glm::vec3(m_Spacing * i, 0.0f, 0.0f);
				
				curTransform.Translation += transform.Translation;
				curTransform.Scale *= transform.Scale;

				entity.GetComponent<Teddy::SpriteRendererComponent>().Color = glm::vec4(0.0f);

				m_CurrentPositionEntity = curEntity;
			}
		}
		
	}

	void Slider::Decrement()
	{
		SetPosition(m_CurrentPosition - 1);
	}

	void Slider::Increment()
	{
		SetPosition(m_CurrentPosition + 1);
	}

	void Slider::SetPosition(int position)
	{
		if (position == m_CurrentPosition)
			return;
		else if(position > 10)
			position = 10;
		else if(position < 0)
			position = 0;
		if (m_CurrentPositionEntity.HasComponent<Teddy::TransformComponent>())
		{
			auto& transform = m_CurrentPositionEntity.GetComponent<Teddy::TransformComponent>();
			transform.Translation.x = m_Spacing * position + transform.Translation.x - (m_Spacing * m_CurrentPosition);
		}
		m_CurrentPosition = position;
	}

	void Slider::UpdateBar(glm::vec4& Color)
	{
		for (int i = 0; i < m_Entities.size(); i++)
		{
			if(m_CurrentPosition != i)
				m_Entities[i].GetComponent<Teddy::SpriteRendererComponent>().Color = Color;
			else
				m_Entities[i].GetComponent<Teddy::SpriteRendererComponent>().Color = glm::vec4(0.0f);
		}

		m_CurrentPositionEntity.GetComponent<Teddy::SpriteRendererComponent>().Color = Color;
	}
}