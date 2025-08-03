#pragma once

#include <Teddy.h>

#include "Platform/OpenGL/OpenGLShader.h"
#include "Teddy/Renderer/Shader.h"

#include <imgui.h>

#include <glm/gtc/type_ptr.hpp>

class Sandbox2D : public Teddy::Layer{

public:
	Sandbox2D();
	virtual ~Sandbox2D() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	void OnUpdate(Teddy::Timestep ts) override;
	void OnEvent(Teddy::Event& event) override;
	virtual void OnImGuiRender() override;
private:
	Teddy::Ref<Teddy::VertexArray> m_SquareVA;

	Teddy::Ref<Teddy::Shader> m_FlatColorShader;
	Teddy::OrthographicCameraController m_CameraController;

	Teddy::Ref<Teddy::Texture2D> m_BoardTexture;

	glm::vec4 m_SquareColor = { 0.2f, 0.1f, 1.0f, 1.0f };
};