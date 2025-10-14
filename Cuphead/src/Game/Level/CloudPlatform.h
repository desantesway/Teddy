#pragma once

#include "teddyPch.h"

#include "Teddy/Scene/Entity.h"
#include "Teddy/Scene/Scene.h"
#include "Teddy/Core/Timestep.h"

namespace Cuphead
{
	class CloudPlatform
	{
	public:
		CloudPlatform() = default;
		~CloudPlatform() = default;

		void Init(Teddy::Ref<Teddy::Scene> scene);
		void InitPhase1();

		void OnUpdate(Teddy::Timestep ts);
	private:
		void StartCloudA(float X, float Y);
		void StartCloudB(float X, float Y);
		void StartCloudC(float X, float Y);

		Teddy::Ref<Teddy::Scene> m_Scene = nullptr;

		std::vector<Teddy::Entity> m_Clouds;
		std::vector<Teddy::Ref<Teddy::Texture2D>> m_CloudTextures;
	};
}