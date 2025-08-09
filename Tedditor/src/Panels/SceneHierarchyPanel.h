#pragma once

#include "Teddy/Core/Base.h"
#include "Teddy/Core/Log.h"
#include "Teddy/Scene/Scene.h"
#include "Teddy/Scene/Entity.h"

namespace Teddy
{ 
	class SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel() = default;	
		SceneHierarchyPanel(const Ref<Scene>& context);

		void SetContext(const Ref<Scene>& context);

		void OnImGuiRender();
	private:
		void DrawEntityNode(Entity entity);
		void DrawComponents(Entity entity);
	private:
		Ref<Scene> m_Context;
		Entity m_SelectionContext;
	};
}