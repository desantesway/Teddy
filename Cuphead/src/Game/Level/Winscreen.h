#pragma once

#include "Teddy/Scene/Entity.h"
#include "Teddy/Core/Timestep.h"
#include "Teddy/Scene/Scene.h"
#include "Teddy/Events/Event.h"
#include "Teddy/Events/KeyEvent.h"

namespace Cuphead
{
	class Winscreen
	{
	public:
		Winscreen() = default;
		Winscreen(float time, int hp, int parry, int super, int skill)
			: m_Hp(hp), m_Parry(parry), m_Super(super), m_Skill(skill), m_Time(time) {}
		~Winscreen() = default;

		void OnUpdate(Teddy::Timestep ts);
		void OnEvent(Teddy::Event& event);

		Teddy::Ref<Teddy::Scene> Init();
	private:

		Teddy::Ref<Teddy::Texture2D> m_BackgroundTexture;
		Teddy::Ref<Teddy::Texture2D> m_BoardTexture;
		std::vector<Teddy::Ref<Teddy::Texture2D>> m_TheResultsTextures;
		std::vector<Teddy::Ref<Teddy::Texture2D>> m_GradeTextures;

		const glm::vec4 m_YellowColor = glm::vec4(254.0f / 255.0f, 203.0f / 255.0f, 70.0f / 255.0f, 1.0f);

		Teddy::Ref<Teddy::Font> m_BoardFont;

		float m_Time = 0.0f;
		int m_Hp = 0;
		int m_Parry = 0;
		int m_Super = 0;
		int m_Skill = 0;

		Teddy::Entity m_TimeEntity;
		Teddy::Entity m_HpEntity;
		Teddy::Entity m_ParryEntity;
		Teddy::Entity m_SuperEntity;

		Teddy::Ref<Teddy::Scene> m_Scene;
	};
};