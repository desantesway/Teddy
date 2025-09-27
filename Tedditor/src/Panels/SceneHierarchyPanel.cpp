#include "SceneHierarchyPanel.h"

#include <imgui.h>
#include <imgui_internal.h>

#include "Teddy/Scene/Components.h"
#include <cstring>
#include <algorithm>

#include <glm/gtc/type_ptr.hpp>

/* The Microsoft C++ compiler is non-compliant with the C++ standard and needs
 * the following definition to disable a security warning on std::strncpy().
 */
#ifdef _MSVC_LANG
#define _CRT_SECURE_NO_WARNINGS
#endif


namespace Teddy
{

	extern const std::filesystem::path g_AssetPath;

	SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& context)
	{
		SetContext(context);
	}

	void SceneHierarchyPanel::SetContext(const Ref<Scene>& context)
	{
		m_Context = context;
		m_SelectionContext = {};
	}

	template<typename T>
	void SceneHierarchyPanel::AddToComponentTree(Entity& entity, bool& empty, std::string title)
	{
		if (entity.HasComponent<T>())
		{
			m_ComponentToEntityTree[title].push_back(entity);
			empty = false;
		}
	}

	void SceneHierarchyPanel::CalculateComponentTree()
	{
		m_ComponentToEntityTree = {};

		auto view = m_Context->m_Registry.view<entt::entity>();
		for (auto entityID : view)
		{
			Entity entity = Entity(entityID, m_Context.get());

			bool isEmpty = true;

			AddToComponentTree<CameraComponent>(entity, isEmpty, "Cameras");
			AddToComponentTree<SpriteRendererComponent>(entity, isEmpty, "Sprites");
			AddToComponentTree<SpriteAtlasComponent>(entity, isEmpty, "Sprites with atlas");
			AddToComponentTree<SpriteAnimationComponent>(entity, isEmpty, "Sprites with animations");
			AddToComponentTree<CircleRendererComponent>(entity, isEmpty, "Circles");
			AddToComponentTree<TextComponent>(entity, isEmpty, "Text");

			if (isEmpty)
				m_ComponentToEntityTree["Empty"].push_back(entity);
		}
	}

	void SceneHierarchyPanel::OnImGuiRender() 
	{
		ImGui::Begin("Scene Hierarchy");

		if (m_Context) 
		{
			CalculateComponentTree();

			int i = 0;
			bool entityDeleted = false;
			for (auto& components : m_ComponentToEntityTree)
			{
				ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen |
					ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_SpanAvailWidth
					| ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_FramePadding;
				bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)i, flags, components.first.c_str());

				if (opened)
				{
					for (const auto& entity : components.second)
					{
						entityDeleted = DrawEntityNode(entity);
						if (entityDeleted) break;
					}

					ImGui::TreePop();
				}

				i++;

				if (entityDeleted) break;
			}

			if (ImGui::IsWindowHovered() && ImGui::IsMouseDown(0))
			{
				m_SelectionContext = {};
			}

			if (ImGui::BeginPopupContextWindow(0, 1 | ImGuiPopupFlags_NoOpenOverItems))
			{
				if (ImGui::MenuItem("Create Empty Entity"))
					m_Context->CreateEntity("Empty Entity");

				if (m_SelectionContext)
				{
					if (ImGui::MenuItem("Delete Entity"))
						entityDeleted = true;
				}

				ImGui::EndPopup();
			}

			if (entityDeleted)
			{
				m_Context->DestroyEntity(m_SelectionContext);
				m_SelectionContext = {};
			}

		}

		ImGui::End();

		ImGui::Begin("Properties");

		if (m_SelectionContext)
		{
			// ?
			DrawComponents(m_SelectionContext);
		}

		ImGui::End();		
	}

	void SceneHierarchyPanel::SetSelectedEntity(Entity entity)
	{
		m_SelectionContext = entity;
	}

	bool SceneHierarchyPanel::DrawEntityNode(Entity entity)
	{
		auto& tag = entity.GetComponent<TagComponent>();
		ImGuiTreeNodeFlags flags = ((m_SelectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		flags |= ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap;

		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.Tag.c_str());
		if (ImGui::IsItemClicked())
		{
			m_SelectionContext = entity;
		}

		if (opened)
		{
			ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
			bool opened = ImGui::TreeNodeEx((void*)9817239, flags, tag.Tag.c_str());
			if (opened)
				ImGui::TreePop();
			ImGui::TreePop();
		}

		bool entityDeleted = false;
		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Delete Entity"))
				entityDeleted = true;
		
			ImGui::EndPopup();
		}

		return entityDeleted;
	}

	static void DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f)
	{
		ImGuiIO& io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[0];

		ImGui::PushID(label.c_str());

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label.c_str());
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

		float lineHeight = GImGui->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("X", buttonSize))
			values.x = resetValue;
		ImGui::PopStyleColor(3);
		ImGui::PopFont();
		ImGui::SameLine();
		ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Y", buttonSize))
			values.y = resetValue;
		ImGui::PopStyleColor(3);
		ImGui::PopFont();
		ImGui::SameLine();
		ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Z", buttonSize))
			values.z = resetValue;
		ImGui::PopStyleColor(3);
		ImGui::PopFont();
		ImGui::SameLine();
		ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();

		ImGui::PopStyleVar();

		ImGui::Columns(1);

		ImGui::PopID();
	}

	template<typename T>
	bool DrawComponentPart(auto& component, const std::string& name, bool canDelete, Entity ent)
	{
		const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen |
			ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_SpanAvailWidth |
			ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_FramePadding;

		ImVec2 contentRegionAvail = ImGui::GetContentRegionAvail();
		
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
		float lineHeight = GImGui->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImGui::Separator();

		bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeNodeFlags, name.c_str());

		ImGui::PopStyleVar();

		ImGui::SameLine(contentRegionAvail.x - lineHeight * 0.5f);
		if (ImGui::Button(("+##" + name).c_str(), ImVec2{ lineHeight, lineHeight }))
		{
			ImGui::OpenPopup((name + "Settings").c_str());
		}

		if (canDelete)
		{
			bool removeComponent = false;

			if (ImGui::BeginPopup((name + "Settings").c_str()))
			{
				if (ImGui::MenuItem("Remove component"))
					removeComponent = true;
				ImGui::EndPopup();
			}

			if (removeComponent)
				ent.RemoveComponent<T>();
		}

		return open;
	}

	template<typename T, typename UIFunction>
	static void DrawComponent(const std::string& name, bool canDelete, Entity entity, UIFunction uiFunction)
	{
		if (entity.HasComponent<T>())
		{
			auto& component = entity.GetComponent<T>();

			if (DrawComponentPart<T>(component, name, canDelete, entity))
			{
				uiFunction(component);
				ImGui::TreePop();
			}
		}
	}

	template<typename T, typename UIFunction>
	static void DrawComponentWithEntity(const std::string& name, bool canDelete, Entity entity, UIFunction uiFunction)
	{
		if (entity.HasComponent<T>())
		{
			auto& component = entity.GetComponent<T>();

			if (DrawComponentPart<T>(component, name, canDelete, entity))
			{
				uiFunction(entity, component);
				ImGui::TreePop();
			}
		}
	}

	template<typename T, typename UIFunction>
	static void DrawSecondComponent(const std::string& name, bool canDelete, Entity entity, auto& scndComponent, UIFunction uiFunction)
	{
		if (entity.HasComponent<T>())
		{
			auto& component = entity.GetComponent<T>();

			if (DrawComponentPart<T>(component, name, canDelete, entity))
			{
				uiFunction(component, scndComponent);
				ImGui::TreePop();
			}
		}
	}

	template<typename T>
	void SceneHierarchyPanel::DisplayAddComponentEntry(const std::string& entryName) {
		if (!m_SelectionContext.HasComponent<T>())
		{
			if (ImGui::MenuItem(entryName.c_str()))
			{
				m_SelectionContext.AddComponent<T>();
				ImGui::CloseCurrentPopup();
			}
		}
	}

	std::vector<int> ParseIndicesString(const int size, const std::string& str, bool& valid) {
		std::vector<int> result;
		valid = true;
		std::stringstream ss(str);
		std::string token;
		while (std::getline(ss, token, ',')) {
			token.erase(0, token.find_first_not_of(" \t\n\r"));
			token.erase(token.find_last_not_of(" \t\n\r") + 1);
			if (token.empty()) continue;
			try {
				int value = std::stoi(token);
				if(value <= size-1 && value >= 0)
					result.push_back(value);
				else
				{
					valid = false;
					break;
				}
			}
			catch (...) {
				valid = false;
				break;
			}
		}
		return result;
	}

	void SceneHierarchyPanel::DrawComponents(Entity entity)
	{
		if (entity.HasComponent<TagComponent>())
		{
			auto& tag = entity.GetComponent<TagComponent>().Tag;

			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			std::strncpy(buffer, tag.c_str(), sizeof(buffer));

			if (ImGui::InputText("##Tag", buffer, sizeof(buffer)))
			{
				tag = std::string(buffer);
			}
		}

		ImGui::SameLine();

		if (ImGui::Button("Add Component"))
			ImGui::OpenPopup("AddComponent");

		if (ImGui::BeginPopup("AddComponent"))
		{
			DisplayAddComponentEntry<CameraComponent>("Camera");
			DisplayAddComponentEntry<SpriteRendererComponent>("Sprite Renderer");
			DisplayAddComponentEntry<SpriteAtlasComponent>("Sprite Atlas");
			DisplayAddComponentEntry<SpriteAnimationComponent>("Sprite Animation");
			DisplayAddComponentEntry<CircleRendererComponent>("Circle Renderer");
			DisplayAddComponentEntry<TextComponent>("Text");
			DisplayAddComponentEntry<Rigidbody2DComponent>("Rigidbody 2D");
			DisplayAddComponentEntry<BoxCollider2DComponent>("Box Collider 2D");
			DisplayAddComponentEntry<CircleCollider2DComponent>("Circle Collider 2D");

			ImGui::EndPopup();
		}

		DrawComponent<TransformComponent>("Transform", false, entity, [](auto& component)
			{
				DrawVec3Control("Translation", component.Translation);
				glm::vec3 rotation = glm::degrees(component.Rotation);
				DrawVec3Control("Rotation", rotation);
				component.Rotation = glm::radians(rotation);
				DrawVec3Control("Scale", component.Scale, 1.0f);
			});

		DrawComponent<CameraComponent>("Camera Renderer", true, entity, [](auto& component)
			{
				auto& camera = component.Camera;

				ImGui::Checkbox("Primary", &component.Primary);

				const char* projectionTypeStrings[] = { "Perspective", "Orthographic" };
				const char* currentProjectionTypeString = projectionTypeStrings[(int)camera.GetProjectionType()];

				if (ImGui::BeginCombo("Projection", currentProjectionTypeString))
				{
					for (int i = 0; i < 2; i++)
					{
						bool isSelected = currentProjectionTypeString == projectionTypeStrings[i];

						if (ImGui::Selectable(projectionTypeStrings[i], isSelected))
						{
							currentProjectionTypeString = projectionTypeStrings[i];
							camera.SetProjectionType((SceneCamera::ProjectionType)i);
						}
						if (isSelected)
							ImGui::SetItemDefaultFocus();
					}
					ImGui::EndCombo();
				}

				if (camera.GetProjectionType() == SceneCamera::ProjectionType::Perspective)
				{
					float perspectiveVerticalFov = glm::degrees(camera.GetPerspectiveVerticalFOV());

					if (ImGui::DragFloat("Vertical FOV", &perspectiveVerticalFov))
						camera.SetPerspectiveVerticalFOV(glm::radians(perspectiveVerticalFov));

					float perspectiveNear = camera.GetPerspectiveNearClip();
					if (ImGui::DragFloat("Near", &perspectiveNear))
						camera.SetPerspectiveNearClip(perspectiveNear);

					float perspectiveFar = camera.GetPerspectiveFarClip();
					if (ImGui::DragFloat("Far", &perspectiveFar))
						camera.SetPerspectiveFarClip(perspectiveFar);
				}

				if (camera.GetProjectionType() == SceneCamera::ProjectionType::Orthographic)
				{
					float orthoSize = camera.GetOrthographicSize();
					if (ImGui::DragFloat("Size", &orthoSize))
						camera.SetOrthographicSize(orthoSize);

					float orthoNear = camera.GetOrthographicNearClip();
					if (ImGui::DragFloat("Near", &orthoNear))
						camera.SetOrthographicNearClip(orthoNear);

					float orthoFar = camera.GetOrthographicFarClip();
					if (ImGui::DragFloat("Far", &orthoFar))
						camera.SetOrthographicFarClip(orthoFar);

					ImGui::Checkbox("Fixed Aspect Ratio", &component.FixedAspectRatio);
				}
			});

		DrawComponentWithEntity<SpriteRendererComponent>("Sprite Renderer", true, entity, [](Entity ent, auto& component)
			{
				ImGui::Checkbox("Original Aspect Ratio", &component.OriginalAspectRatio);
				ImGui::Checkbox("Background", &component.IsBackground);
				ImGui::ColorEdit4("Color", glm::value_ptr(component.Color));

				ImGui::Button("Texture", ImVec2(100.0f, 0.0f));

				if (ImGui::BeginDragDropTarget())
				{
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("TEXTURE_CONTENT_BROWSER_ITEM"))
					{
						const wchar_t* path = (const wchar_t*)payload->Data;
						std::filesystem::path texturePath = std::filesystem::path(g_AssetPath) / path;
						Ref<Texture2D> texture = AssetManager::Get().Load<Texture2D>(texturePath.string(), Boolean::True);
						if (texture->IsLoaded())
							component.Texture = texture;
						else
							TED_WARN("Could not load texture {0}", texturePath.filename().string());
					}
					ImGui::EndDragDropTarget();
				}

				ImGui::DragFloat("Tiling Factor", &component.TilingFactor, 0.1f, 0.0f, 100.0f);

				DrawSecondComponent<SpriteAtlasComponent>("Atlas", true, ent, component,
					[](auto& component, auto& secndComponent)
					{
						int w=0;
						int h=0;
						if (secndComponent.Texture)
						{
							int w = secndComponent.Texture->GetWidth();
							int h = secndComponent.Texture->GetHeight();
						}
						
						ImGui::DragInt("X position", &component.X, 0.05f, 0.0f, (w / (component.SpriteWidth == 0 ? 1 : component.SpriteWidth)  ) - 1);
						ImGui::DragInt("Y position", &component.Y, 0.05f, 0.0f, (h / (component.SpriteHeight == 0 ? 1 : component.SpriteHeight)) - 1);
						ImGui::DragInt("Sprite Width", &component.SpriteWidth, 1.0f, 0.0f, w);
						ImGui::DragInt("Sprite Height", &component.SpriteHeight, 1.0f, 0.0f, h);
					});
			});

		DrawComponentWithEntity<SpriteAnimationComponent>("Sprite Animation", true, entity, [](Entity ent, auto& component)
			{
				ImGui::Checkbox("Original Aspect Ratio", &component.OriginalAspectRatio);
				ImGui::Checkbox("Background", &component.IsBackground);
				ImGui::ColorEdit4("Color", glm::value_ptr(component.Color));

				int texturesPerRow = 3;
				for (int i = 0; i < component.Textures.size(); i++)
				{
					ImGui::Button(("Texture#" + std::to_string(i)).c_str(), ImVec2(100.0f, 0.0f));
					if (ImGui::BeginDragDropTarget())
					{
						if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("TEXTURE_CONTENT_BROWSER_ITEM"))
						{
							const wchar_t* path = (const wchar_t*)payload->Data;
							std::filesystem::path texturePath = std::filesystem::path(g_AssetPath) / path;
							Ref<Texture2D> texture = AssetManager::Get().Load<Texture2D>(texturePath.string(), Boolean::True);
							if (texture->IsLoaded())
								component.Textures[i] = texture;
							else
								TED_WARN("Could not load texture {0}", texturePath.filename().string());
						}
						ImGui::EndDragDropTarget();
					}
					if (i < component.Textures.size()-1 && (((i % texturesPerRow) != 1)))
						ImGui::SameLine();
				}
				
				ImGui::Button("Add Texture", ImVec2(100.0f, 0.0f));
				if (ImGui::BeginDragDropTarget())
				{
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("TEXTURE_CONTENT_BROWSER_ITEM"))
					{
						const wchar_t* path = (const wchar_t*)payload->Data;
						std::filesystem::path texturePath = std::filesystem::path(g_AssetPath) / path;
						Ref<Texture2D> texture = AssetManager::Get().Load<Texture2D>(texturePath.string(), Boolean::True);
						if (texture->IsLoaded())
							component.Textures.push_back(texture);
						else
							TED_WARN("Could not load texture {0}", texturePath.filename().string());
					}
					ImGui::EndDragDropTarget();
				}

				ImGui::SameLine();
				static bool openInputPopup = false;
				static char buf[256] = { 0 };

				if (ImGui::Button("Remove Texture")) {
					openInputPopup = true;
					ImGui::OpenPopup("RemoveTexturePopup");
				}

				if (openInputPopup) {
					ImGui::SetNextWindowSize(ImVec2(300, 0));
					if (ImGui::BeginPopupModal("RemoveTexturePopup", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
						ImGui::Text("Enter texture indices to remove:");
						ImGui::Separator();
						ImGui::SetKeyboardFocusHere();
						if (ImGui::InputText("##indices", buf, sizeof(buf), ImGuiInputTextFlags_EnterReturnsTrue)) {
							bool validIndices = true;
							auto parsed = ParseIndicesString(component.Textures.size(), std::string(buf), validIndices);
							if (validIndices) {
								int toRemove = parsed[0];
								component.Textures.erase(component.Textures.begin() + toRemove);
								component.TextureIndex = 0;
							}
							else {
								ImGui::TextColored(ImVec4(1, 0, 0, 1), "Invalid indices input!");
							}
							openInputPopup = false;
							ImGui::CloseCurrentPopup();
						}
						if (ImGui::Button("Cancel")) {
							openInputPopup = false;
							ImGui::CloseCurrentPopup();
						}
						ImGui::EndPopup();
					}
				}

				ImGui::DragFloat("Tiling Factor", &component.TilingFactor, 0.1f, 0.0f, 100.0f);

				std::string indicies = "";
				for (const int& index : component.PlayableIndicies)
				{
					indicies += std::to_string(index) + ", ";
				}

				char buffer[256];
				memset(buffer, 0, sizeof(buffer));
				std::strncpy(buffer, indicies.c_str(), sizeof(buffer));
				if (ImGui::InputText("Playable Indicies", buffer, sizeof(buffer)))
				{
					indicies = std::string(buffer);
					bool validIndices = true;
					auto parsed = ParseIndicesString(component.Textures.size(), indicies, validIndices);
					if (validIndices) {
						component.PlayableIndicies = parsed;
					}
					else {
						ImGui::TextColored(ImVec4(1, 0, 0, 1), "Invalid indices input!");
					}
				}

				ImGui::DragInt("Texture Index", &component.TextureIndex, 1, 0, component.Textures.size() - 1);
				ImGui::DragFloat("Frame Time", &component.FrameTime, 0.01f, 0.0f, 10.0f);
				ImGui::DragFloat("Initial Frame Time", &component.InitialFrameTime, 0.01f, 0.0f, 10.0f);
				ImGui::DragFloat("Final Frame Time", &component.FinalFrameTime, 0.01f, 0.0f, 10.0f);
				ImGui::Checkbox("Loop", &component.Loop);
				if (ImGui::Checkbox("Ping Pong", &component.PingPong))
				{
					component.Reverse = false;
				}
				ImGui::Checkbox("Pause", &component.Pause);

				DrawSecondComponent<SpriteAtlasComponent>("Atlas", true, ent, component,
					[](auto&component, auto& secndComponent)
					{
						int w = 0;
						int h = 0;
						for (int i = 0; i < secndComponent.Textures.size(); i++)
						{
							w = std::max(w, static_cast<int>(secndComponent.Textures[i]->GetWidth()));
							h = std::max(h, static_cast<int>(secndComponent.Textures[i]->GetHeight()));
						}

						ImGui::DragInt("X position", &component.X, 0.05f, 0.0f, (w / (component.SpriteWidth == 0 ? 1 : component.SpriteWidth)) - 1);
						ImGui::DragInt("Y position", &component.Y, 0.05f, 0.0f, (h / (component.SpriteHeight == 0 ? 1 : component.SpriteHeight)) - 1);
						ImGui::DragInt("Sprite Width", &component.SpriteWidth, 1.0f, 0.0f, w);
						ImGui::DragInt("Sprite Height", &component.SpriteHeight, 1.0f, 0.0f, h);
					});
			});

		
		DrawComponent<CircleRendererComponent>("Circle Renderer", true, entity, [](auto& component)
			{
				ImGui::ColorEdit4("Color", glm::value_ptr(component.Color));
				ImGui::DragFloat("Thickness", &component.Thickness, 0.025f, 0.0f, 1.0f);
				ImGui::DragFloat("Fade", &component.Fade, 0.00025f, 0.0f, 1.0f);
			});

		DrawComponent<Rigidbody2DComponent>("Rigidbody 2D", true, entity, [](auto& component)
			{
				const char* bodyTypeStrings[] = { "Static", "Kinematic", "Dynamic" };
				const char* currentBodyTypeString = bodyTypeStrings[(int)component.Type];

				if (ImGui::BeginCombo("BodyType", currentBodyTypeString))
				{
					for (int i = 0; i < 3; i++)
					{
						bool isSelected = currentBodyTypeString == bodyTypeStrings[i];

						if (ImGui::Selectable(bodyTypeStrings[i], isSelected))
						{
							currentBodyTypeString = bodyTypeStrings[i];
							component.Type = ((Rigidbody2DComponent::BodyType)i);
						}
						if (isSelected)
							ImGui::SetItemDefaultFocus();
					}
					ImGui::EndCombo();
				}

				ImGui::Checkbox("Fixed Rotation", &component.FixedRotation);
			});

		DrawComponent<BoxCollider2DComponent>("Box Collider 2D", true, entity, [](auto& component)
			{
				ImGui::DragFloat2("Offset", glm::value_ptr(component.Offset));
				ImGui::DragFloat2("Size", glm::value_ptr(component.Size), 0.1f, 0.0f, 100.0f);
				ImGui::DragFloat("Density", &component.Density, 0.01f, 0.0f, 1.0f);
				ImGui::DragFloat("Friction", &component.Friction, 0.01f, 0.0f, 1.0f);
				ImGui::DragFloat("Restitution", &component.Restitution, 0.01f, 0.0f, 1.0f);
			});
	
		DrawComponent<CircleCollider2DComponent>("Circle Collider 2D", true, entity, [](auto& component)
			{
				ImGui::DragFloat2("Offset", glm::value_ptr(component.Offset));
				ImGui::DragFloat("Radius", &component.Radius, 0.1f, 0.0f, 100.0f);
				ImGui::DragFloat("Density", &component.Density, 0.01f, 0.0f, 1.0f);
				ImGui::DragFloat("Friction", &component.Friction, 0.01f, 0.0f, 1.0f);
				ImGui::DragFloat("Restitution", &component.Restitution, 0.01f, 0.0f, 1.0f);
			});

		DrawComponent<TextComponent>("Text", true, entity, [](auto& component)
			{
				bool recalculate = false;

				static char textBuffer[1024 * 4];
				memset(textBuffer, 0, sizeof(textBuffer));
				strncpy(textBuffer, component.TextString.c_str(), sizeof(textBuffer) - 1);
				if (ImGui::InputTextMultiline("Text", textBuffer, sizeof(textBuffer))) {
					component.TextString = std::string(textBuffer);
					recalculate = true;
				}

				int textAlignment = (int)component.TextAlignment;
				if(ImGui::DragInt("Text Alignment", &textAlignment, 0.01f, 0, 6))
				{
					recalculate = true;
					component.TextAlignment = (TextComponent::AlignmentType)textAlignment;
				}

				ImGui::Button("Font", ImVec2(100.0f, 0.0f));
				if (ImGui::BeginDragDropTarget())
				{
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("FONT_CONTENT_BROWSER_ITEM"))
					{
						const wchar_t* path = (const wchar_t*)payload->Data;
						std::filesystem::path fontPath = std::filesystem::path(g_AssetPath) / path;

						Ref<Font> font;
						font = AssetManager::Get().Load<Font>(fontPath.string(), Boolean::True);

						if (font)
							component.FontAsset = font;
						else
							TED_WARN("Could not load font {0}", fontPath.filename().string());
						recalculate = true;
					}
					ImGui::EndDragDropTarget();
				}

				recalculate = recalculate || ImGui::ColorEdit4("Color", glm::value_ptr(component.Color));

				recalculate = recalculate || ImGui::DragFloat("Kerning", &component.Kerning, 0.01f, 0.0f, 10.0f);
				recalculate = recalculate || ImGui::DragFloat("Line Spacing", &component.LineSpacing, 0.1f, 0.0f, 100.0f);

				recalculate = recalculate || ImGui::ColorEdit4("Background Color", glm::value_ptr(component.BackgroundColor));

				recalculate = recalculate || ImGui::ColorEdit4("Outline Color", glm::value_ptr(component.OutlineColor));
				recalculate = recalculate || ImGui::DragFloat("Outline Thickness", &component.OutlineThickness, 0.01f, 0.0f, 50.0f);

				if (recalculate)
					component.CalculateTextQuad();
			});
	}
}