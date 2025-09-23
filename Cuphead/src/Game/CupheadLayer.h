#pragma once

#include <Teddy.h>

#include "Platform/OpenGL/OpenGLShader.h"
#include "Teddy/Renderer/Shader.h"

#include <imgui.h>

#include <glm/gtc/type_ptr.hpp>

class CupheadLayer : public Teddy::Layer
{
	public:
		CupheadLayer();
		virtual ~CupheadLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		void OnUpdate(Teddy::Timestep ts) override;
		void OnEvent(Teddy::Event& event) override;
		virtual void OnImGuiRender() override;

		void OnOverlayRender();
	protected:
		Teddy::Ref<Teddy::Scene> m_ActiveScene;
};