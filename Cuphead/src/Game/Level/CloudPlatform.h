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

		void SetPlayerPosition(glm::vec2 pos);
		void SetMovementSpeed(float speed) { m_MovementSpeed = speed; }

		void CloudContactBegin(const b2ShapeId& cloudShape);
		void CloudContactEnd(const b2ShapeId& cloudShape);

		void Pause();
		void Unpause();

		bool IsSensor(const b2ShapeId& shape);

		void SetColor(glm::vec4& color);
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
		void UpdateCollisionFilters(Teddy::Timestep ts);
		void UpdateAnimations();
	private:
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

		glm::vec2 m_PlayerPosition = { 0.0f, 0.0f };
		float m_MovementSpeed = 0.0f;

		glm::vec2 m_LastSpawn = {0.0f, 0.0f};

		glm::vec4 m_CloudColor = { 1.0f, 1.0f, 1.0f, 1.0f };

		Teddy::Ref<Teddy::Scene> m_Scene = nullptr;
	};
}