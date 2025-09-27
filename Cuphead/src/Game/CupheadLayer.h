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

		bool OnKeyPressed(Teddy::KeyPressedEvent& e);

		void StartTransition(Teddy::Timestep);
		void LoadMainMenu(Teddy::Timestep ts);
	protected:
		Teddy::Ref<Teddy::Scene> m_ActiveScene;

		bool m_ProceedToMainMenu = false;
		bool m_Transitioning = false;
		bool m_LoadMainMenu = false;

		Teddy::Ref<Teddy::Entity> m_TransitionCircle;
		Teddy::Ref<Teddy::Entity> m_TransitionQuad;
};