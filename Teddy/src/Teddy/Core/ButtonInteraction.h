#pragma once

#include <entt/entt.hpp>

namespace Teddy
{
	class ButtonInteractionSystem
	{
	public:
		ButtonInteractionSystem();
		~ButtonInteractionSystem() = default;

		static void OnUpdate();

		static void OnWindowResize(uint32_t width, uint32_t height);

		static ButtonInteractionSystem& Get() { return *s_Instance; }

		static entt::entity GetHoveredEntity() { return m_HoveredEntity; }
		static void SetHoveredEntity(entt::entity entity) { m_HoveredEntity = entity; }
		static void HoveredHandled(bool hoveredHandled) { m_Enabled = !hoveredHandled; };
	private:
		static bool m_Enabled;

		static entt::entity m_HoveredEntity;
		static uint32_t m_Width, m_Height; // TODO: Get from application

	private:
		static ButtonInteractionSystem* s_Instance;
	};
}