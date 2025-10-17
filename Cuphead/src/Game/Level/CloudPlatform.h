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
		void SetMovementSpeed(float speed) { m_MovementSpeed = speed; }

		void CloudContactBegin(const b2ShapeId& cloudShape);
		void CloudContactEnd(const b2ShapeId& cloudShape);
	private:
		struct Cloud
		{
			Teddy::Entity Entity;
			Teddy::Entity Overlay;
			int Type; // 0 = A, 1 = B, 2 = C
			bool Active = false;

			Cloud(Teddy::Entity entity, int type)
				: Entity(entity), Type(type) {
			}
		};

		void StartCloudA(float x, float y);
		void StartCloudB(float x, float y);
		void StartCloudC(float x, float y);
		void StartCloudA(float x, float y, bool isActivate);
		void ActivateCloud(Cloud& cloud);

		void GeneratePattern(int cloudNum);

		void UpdatePostions();
		void UpdateCollisionFilters();
		void UpdateAnimations();
	private:
		Teddy::Ref<Teddy::Scene> m_Scene = nullptr;

		struct CloudToSpawn
		{
			float X;
			float Y;
			bool Type; // 0 = A, 1 = C

			CloudToSpawn(float x, float y, bool type)
				: X(x), Y(y), Type(type) {}
		};

		std::vector<Cloud> m_Clouds;
		std::vector<CloudToSpawn> m_CloudsToSpawn;
		std::vector<CloudToSpawn> m_CurrentCloudsSpawning;
		std::vector<Teddy::Ref<Teddy::Texture2D>> m_CloudTextures;

		float m_PlayerY = 0.0f;
		float m_MovementSpeed = 0.0f;

		glm::vec2 m_LastSpawn;
		std::mt19937 m_Rng;
		std::uniform_real_distribution<float> m_XDistribution;
		std::uniform_real_distribution<float> m_YDistribution;	   // TODO: change this to levelScenes
		std::bernoulli_distribution m_TypeDistribution;	   // TODO: change this to levelScenes
	};
}