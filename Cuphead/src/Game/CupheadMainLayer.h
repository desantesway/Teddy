#pragma once

#include <Teddy.h>

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

	Teddy::Ref<Teddy::Scene> GetActiveScene() { return m_ActiveScene; }
private:
	bool OnKeyPressed(Teddy::KeyPressedEvent& e);
	bool OnWindowResize(Teddy::WindowResizeEvent& e);

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