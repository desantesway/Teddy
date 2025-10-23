#pragma once

#include "Teddy/Scene/Entity.h"
#include "Teddy/Events/KeyEvent.h"

namespace Cuphead
{
	class Dragon
	{
	public:
		Dragon() = default;
		~Dragon() = default;

		void Init(Teddy::Ref<Teddy::Scene> scene);

		void OnUpdate(Teddy::Timestep ts);
		void OnEvent(Teddy::Event& event);

		Teddy::Entity GetEntity() { return m_Entity; }

		bool IsIntroDone() { return m_State != DragonState::Intro; }

		void StartIntro();
		void LoadIntro();

		bool IsDead() { return m_Health <= 0; }

		void Pause() { m_Entity.GetComponent<Teddy::SpriteAnimationComponent>().Pause = true; }
		void Unpause() { m_Entity.GetComponent<Teddy::SpriteAnimationComponent>().Pause = false; }

		bool IsSensor(b2ShapeId shape);

		void Hit(int damage);
		bool IsShooting() const { return m_Shooting; }

		void SetPlayerPosition(const glm::vec2& pos) { m_PlayerPosition = pos; }
	private:
		bool OnKeyPressed(Teddy::KeyPressedEvent& e);
		bool OnKeyReleased(Teddy::KeyReleasedEvent& e);

		void StartIdle();
		void Idle();

		void Intro();

		void StartPeashot();
		void Peashot(Teddy::Timestep ts);
		void PsychicEye(Teddy::Timestep ts);

		void Hitting(Teddy::Timestep ts);

		void LoadTextures();
	private:
		Teddy::Ref<Teddy::Scene> m_Scene = nullptr;
		Teddy::Entity m_Entity;

		std::vector<Teddy::Ref<Teddy::Texture2D>> m_IntroTextures;
		std::vector<Teddy::Ref<Teddy::Texture2D>> m_IdleTextures;
		std::vector<Teddy::Ref<Teddy::Texture2D>> m_PeashotTextures;
		Teddy::Ref<Teddy::Texture2D> m_PsychicEyeTexture;

		enum class DragonState
		{
			Idle = 0,
			Intro,
			Peashot
		};

		DragonState m_State = DragonState::Intro;

		int m_Phase = 1;

		glm::vec2 m_PlayerPosition = { 0.0f, 0.0f };

		bool m_IntroDone = false;
		bool m_Shooting = false;

		int m_Health = 1700; // 629, 510, 561 /// 1700, 1071, 561
		// grim firebubble has 5 health
		bool m_Hit = false;

		float m_PeashotTimer = 0.0f;

		Teddy::Entity m_PsychicEyeEntity;
		std::vector<Teddy::Entity> m_PeashotEntities;
	};

}