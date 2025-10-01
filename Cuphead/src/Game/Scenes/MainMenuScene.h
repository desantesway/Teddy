#pragma once

#include "Teddy/Scene/Scene.h"
#include "Teddy/Core/Timestep.h"

namespace Cuphead
{
	class MainMenuScene
	{
	public:
		MainMenuScene() = default;
		~MainMenuScene() = default;

		Teddy::Ref<Teddy::Scene> Init();
	};
}