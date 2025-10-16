#pragma once

#include <Teddy.h>

class OverlayLayer : public Teddy::Layer
{
public:
	OverlayLayer();
	virtual ~OverlayLayer() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	void OnUpdate(Teddy::Timestep& ts) override;
	void OnEvent(Teddy::Event& event) override;
	virtual void OnImGuiRender() override;
protected:
	Teddy::Ref<Teddy::Scene> m_ActiveScene;
};