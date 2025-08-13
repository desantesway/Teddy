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

			static bool ResizingWatcher(void* data, SDL_Event* event);
	private:
		bool OnKeyPressed(KeyPressedEvent& e);
		void NewScene();
		void OpenScene();
		void SaveSceneAs();
		private:
			Ref<VertexArray> m_SquareVA;
			Ref<Shader> m_FlatColorShader;
			Ref<Framebuffer> m_Framebuffer;

			Ref<Scene> m_ActiveScene;
			Entity m_SquareEntity;
			Entity m_CameraEntity;
			Entity m_SecondCamera;

			bool m_PrimaryCamera = true;

			EditorCamera m_EditorCamera;

			Ref<Texture2D> m_BoardTexture;

			bool m_ViewportFocused = false, m_ViewportHovered = false;
			glm::vec2 m_ViewportSize = {0.0f, 0.0f};

			// Panels
			SceneHierarchyPanel m_SceneHierarchyPanel;
			ContentBrowserPanel m_ContentBrowserPanel;
	};
}