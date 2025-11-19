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

		Teddy::Ref<Teddy::Scene> m_Scene;
	};
};