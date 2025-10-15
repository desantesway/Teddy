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

		void SetPlayerY(float y);

		void CloudContactBegin(const b2ShapeId& cloudShape);
		void CloudContactEnd(const b2ShapeId& cloudShape);
	private:
		void StartCloudA(float x, float y);
		void StartCloudB(float x, float y);
		void StartCloudC(float x, float y);

		void UpdateCollisionFilters();
	private:
		Teddy::Ref<Teddy::Scene> m_Scene = nullptr;

		std::vector<Teddy::Entity> m_Clouds;
		std::vector<Teddy::Ref<Teddy::Texture2D>> m_CloudTextures;

		float m_PlayerY = 0.0f;
	};
}