#pragma once

#include <Teddy.h>

#include "Panels/SceneHierarchyPanel.h"
#include "Panels/ContentBrowserPanel.h"

#include "Teddy/Renderer/EditorCamera.h"

namespace Teddy
{
	class EditorLayer : public Layer
	{
		public:
			EditorLayer();
			virtual ~EditorLayer() = default;

			virtual void OnAttach() override;
			virtual void OnDetach() override;
			void OnUpdate(Timestep ts) override;
			void OnEvent(Event& event) override;
			virtual void OnImGuiRender() override;
	private:
		bool OnMouseButtonPressed(MouseButtonPressedEvent& e);
		bool OnKeyPressed(KeyPressedEvent& e);

		void NewScene();
		void OpenScene();
		void OpenScene(const std::filesystem::path& path);
		void SaveScene();
		void SaveSceneAs();

		void SerializeScene(Ref<Scene> scene, const std::filesystem::path& path);

		void OnOverlayRender();

		void OnScenePlay();
		void OnSceneStop();
		void OnSceneSimulate();

		void OnDuplicateEntity();

		// UI Panels
		void UI_Toolbar();
		private:
			Ref<VertexArray> m_SquareVA;
			Ref<Shader> m_FlatColorShader;
			Ref<Framebuffer> m_Framebuffer;

			Ref<Scene> m_ActiveScene;
			Ref<Scene> m_EditorScene;
			std::filesystem::path m_EditorScenePath;
			Entity m_SquareEntity;
			Entity m_CameraEntity;
			Entity m_SecondCamera;

			Entity m_HoveredEntity;
			bool m_ShowPhysicsColliders = true;

			bool m_PrimaryCamera = true;

			EditorCamera m_EditorCamera;

			Ref<Texture2D> m_BoardTexture;

			bool m_ViewportFocused = false, m_ViewportHovered = false;
			glm::vec2 m_ViewportSize = {0.0f, 0.0f};
			glm::vec2 m_ViewportBounds[2] = { {0.0f, 0.0f}, {0.0f, 0.0f} };

			enum class SceneState
			{
				Edit = 0, Play = 1, Simulate = 2
			};
			SceneState m_SceneState = SceneState::Edit;

			// Panels
			SceneHierarchyPanel m_SceneHierarchyPanel;
			ContentBrowserPanel m_ContentBrowserPanel;

			// Editor Resources
			Ref<Texture2D> m_IconPlay, m_IconStop, m_IconSimulate;
	};
}