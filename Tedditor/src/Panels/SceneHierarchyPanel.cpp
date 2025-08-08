#include "SceneHierarchyPanel.h"

#include <imgui.h>

#include "Teddy/Scene/Components.h"

namespace Teddy
{
	SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& context)
	{
		SetContext(context);
	}

	void SceneHierarchyPanel::SetContext(const Ref<Scene>& context)
	{
		m_Context = context;
	}

	void SceneHierarchyPanel::OnImGuiRender() 
	{
		ImGui::Begin("Scene Hierarchy");

		// TODO: group entities to their components
		auto view = m_Context->m_Registry.view<entt::entity>();
		for (auto entityID : view)
		{
			Entity entity = Entity(entityID, m_Context.get());
			DrawEntityNode(entity);

		}

		ImGui::End();
	}

	void SceneHierarchyPanel::DrawEntityNode(Entity entity)
	{
		auto& tag = entity.GetComponent<TagComponent>();
		ImGuiTreeNodeFlags flags = ((m_SelectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;

		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.Tag.c_str());
		if (ImGui::IsItemClicked())
		{
			m_SelectionContext = entity;
		}

		if (opened)
		{
			ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
			bool opened = ImGui::TreeNodeEx((void*)9817239, flags, tag.Tag.c_str());
			if (opened)
				ImGui::TreePop();
			ImGui::TreePop();
		}
	}
}