#pragma once

#include <Teddy.h>

#include "Platform/OpenGL/OpenGLShader.h"
#include "Teddy/Renderer/Shader.h"

#include <imgui.h>

#include <glm/gtc/type_ptr.hpp>

class Cuphead2D : public Teddy::Layer
{
	public:
		Cuphead2D();
		virtual ~Cuphead2D() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		void OnUpdate(Teddy::Timestep ts) override;
		void OnEvent(Teddy::Event& event) override;
		virtual void OnImGuiRender() override;

		void OnOverlayRender();
	private:

		Teddy::Ref<Teddy::Scene> m_ActiveScene;

		Teddy::Ref<Teddy::VertexArray> m_SquareVA;
		Teddy::Ref<Teddy::Shader> m_FlatColorShader;

		Teddy::Ref<Teddy::Texture2D> m_BoardTexture;

		Teddy::EditorCamera m_EditorCamera;

		glm::vec4 m_SquareColor = { 0.2f, 0.1f, 1.0f, 1.0f };
};