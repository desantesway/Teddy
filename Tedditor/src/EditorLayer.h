#pragma once

#include <Teddy.h>

#include "Platform/OpenGL/OpenGLShader.h"
#include "Teddy/Renderer/Shader.h"

#include <imgui.h>

#include <glm/gtc/type_ptr.hpp>

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
			OrthographicCameraController m_CameraController;

			Ref<VertexArray> m_SquareVA;
			Ref<Shader> m_FlatColorShader;
			Ref<Framebuffer> m_Framebuffer;

			Ref<Scene> m_ActiveScene;
			Entity m_SquareEntity;
			Entity m_CameraEntity;
			Entity m_SecondCamera;

			bool m_PrimaryCamera = true;

			Ref<Texture2D> m_BoardTexture;

			bool m_ViewportFocused = false, m_ViewportHovered = false;
			ImVec2 m_ViewportSize = {0.0f, 0.0f};

			glm::vec4 m_SquareColor = { 0.2f, 0.1f, 1.0f, 1.0f };
	};
}