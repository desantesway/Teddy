#pragma once

#include <Teddy.h>

class StartLayer : public Teddy::Layer
{
public:
	StartLayer();
	virtual ~StartLayer() = default;

	void OnUpdate(Teddy::Timestep& ts) override;

	void OnEvent(Teddy::Event& event) override;
private:
	bool OnWindowResize(Teddy::WindowResizeEvent& e);
private:
	glm::vec2 m_ViewportSize = { 1920, 1080 };
};