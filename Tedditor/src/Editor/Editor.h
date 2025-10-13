#pragma once

#include "../Panels/SceneHierarchyPanel.h"
#include "../Panels/ContentBrowserPanel.h"
#include "Teddy/Renderer/Framebuffer.h"
#include "Teddy/Scene/Scene.h"
#include "Teddy/Physics/ContactEvent.h"
#include "Teddy/Physics/SensorEvent.h"

namespace Teddy
{
	class Editor
	{
	public:

		Editor() = default;
		~Editor() = default;

		void Init(Ref<Scene> activeScene);
		void OnUpdate(Timestep ts);
		void OnEvent(Event& event);
		void OnImGuiRender();

		void ResizeFramebuffer();

		void BindFramebuffer() { m_PostProcessedFramebuffer->Bind(); }
		void UnbindFramebuffer() { m_PostProcessedFramebuffer->Unbind(); }

		enum class SceneState { Edit = 0, Play = 1, Simulate = 2 };
		SceneState GetState() { return m_SceneState; }
		void SetActiveScene(Ref<Scene> scene);
	private:
		bool OnKeyPressed(KeyPressedEvent& e);
		bool OnMouseButtonPressed(MouseButtonPressedEvent& e);
		bool OnContactBegin(ContactBeginEvent& e);
		bool OnContactEnd(ContactEndEvent& e);
		bool OnSensorBegin(SensorBeginEvent& e);
		bool OnSensorEnd(SensorEndEvent& e);

		void OnOverlayRender();

		void UI_Toolbar();

		void GenerateAtlas();
		void GenerateAtlas(const int width, const int height, const int toleration, const std::string name);
		void GenerateAtlas(const std::vector<std::string>& filepaths, const int width, const int height, const int toleration, const std::string name);

		void NewScene();

		void OpenScene();
		void OpenScene(const std::filesystem::path& path);

		void SaveScene();
		void SaveSceneAs();

		void SerializeScene(Ref<Scene> scene, const std::filesystem::path& path);

		void OnScenePlay();
		void OnSceneStop();
		void OnSceneSimulate();

		void OnDuplicateEntity();

		void RemoveSceneAssetBypass();
		void SceneAssetBypass();
	private:
		Ref<Framebuffer> m_PostProcessedFramebuffer;

		Ref<Scene> m_ActiveScene;
		Ref<Scene> m_EditorScene;
		std::filesystem::path m_EditorScenePath;
		Entity m_SquareEntity;
		Entity m_CameraEntity;
		Entity m_SecondCamera;

		Entity m_HoveredEntity;
		int m_GizmoType = -1;
		bool m_ShowPhysicsColliders = true;

		bool m_PrimaryCamera = true;

		bool m_AtlasPopup = false;

		EditorCamera m_EditorCamera;

		Ref<Texture2D> m_BoardTexture;

		bool m_ViewportFocused = false, m_ViewportHovered = false;
		glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
		glm::vec2 m_ViewportBounds[2] = { {0.0f, 0.0f}, {0.0f, 0.0f} };

		SceneState m_SceneState = SceneState::Edit;

		// Panels
		SceneHierarchyPanel m_SceneHierarchyPanel;
		ContentBrowserPanel m_ContentBrowserPanel;

		// Editor Resources
		Ref<Texture2D> m_IconPlay, m_IconStop, m_IconSimulate;
	};
}