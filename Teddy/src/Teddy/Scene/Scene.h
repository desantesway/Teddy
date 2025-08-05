#pragma once

#include <entt/entt.hpp>

namespace Teddy 
{
	class Scene
	{
	public:
		Scene();
		~Scene();
		//void OnUpdate(Timestep ts);
		//void OnRender();
		//void OnResize(uint32_t width, uint32_t height);
		//void SetViewportSize(float width, float height) { m_ViewportSize = { width, height }; }
	private:
		entt::registry m_Registry;
		//float m_ViewportSize[2] = { 1280.0f, 720.0f };
		};
}