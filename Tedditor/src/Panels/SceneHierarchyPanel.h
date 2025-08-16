#pragma once

#include "Teddy/Core/Base.h"
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

		Entity GetSelectedEntity() const { return m_SelectionContext; }
		void SetSelectedEntity(Entity entity);
	private:
		template<typename T>
		void DisplayAddComponentEntry(const std::string& entryName);

		void CalculateComponentTree();

		bool DrawEntityNode(Entity entity);
		void DrawComponents(Entity entity);
	private:
		Ref<Scene> m_Context;
		Entity m_SelectionContext;

		std::map<std::string, std::vector<Entity>> m_ComponentToEntityTree;
	};
}