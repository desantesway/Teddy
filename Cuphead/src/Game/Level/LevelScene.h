#pragma once

#include "Teddy/Scene/Scene.h"

namespace Cuphead
{
	class LevelScene
	{
	public:
		LevelScene() = default;
		~LevelScene() = default;
		
		Teddy::Ref<Teddy::Scene> Init(unsigned int character);
	private:
		Teddy::Ref<Teddy::Scene> m_Scene;
	};
}