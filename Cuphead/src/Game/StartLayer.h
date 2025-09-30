#pragma once

#include <Teddy.h>

class StartLayer : public Teddy::Layer
{
public:
	StartLayer();
	virtual ~StartLayer() = default;

	void OnUpdate(Teddy::Timestep ts) override;
};