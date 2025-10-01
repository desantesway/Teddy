#pragma once

#include "Teddy/Scene/Scene.h"
#include "Teddy/Core/Timestep.h"

namespace Cuphead
{
	class MainTitleScene
	{
	public:
		MainTitleScene() = default;
		~MainTitleScene() = default;

		Teddy::Ref<Teddy::Scene> Init();
	private:
		bool m_Transitioning = false;
		bool m_LoadMainMenu = false;
	};
}