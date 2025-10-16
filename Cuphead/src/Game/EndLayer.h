#pragma once

#include <Teddy.h>

class EndLayer : public Teddy::Layer
{
public:
	EndLayer();
	virtual ~EndLayer() = default;

	void OnUpdate(Teddy::Timestep& ts) override;
};