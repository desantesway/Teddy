#pragma once

#include <Teddy.h>

#include "Platform/OpenGL/OpenGLShader.h"
#include "Teddy/Renderer/Shader.h"

#include <imgui.h>

#include <glm/gtc/type_ptr.hpp>

class KeyStar2D : public Teddy::Layer{

public:
	KeyStar2D();
	virtual ~KeyStar2D() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	void OnUpdate(Teddy::Timestep ts) override;
	void OnEvent(Teddy::Event& event) override;
	virtual void OnImGuiRender() override;
private:
	Teddy::Ref<Teddy::VertexArray> m_SquareVA;

	Teddy::Ref<Teddy::Shader> m_FlatColorShader;
	Teddy::OrthographicCameraController m_CameraController;

	Teddy::Ref<Teddy::Texture2D> m_SustainTexture;

	struct ProfileResult {
		const char* name;
		float time;
	};

	std::vector<ProfileResult> m_ProfileResults;

	glm::vec4 m_SquareColor = { 0.2f, 0.1f, 1.0f, 1.0f };
};