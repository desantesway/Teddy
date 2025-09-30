#include "StartLayer.h"

StartLayer::StartLayer()
	: Layer("Starting Layer")
{
	TED_PROFILE_FUNCTION();
}

void StartLayer::OnUpdate(Teddy::Timestep ts)
{
	TED_PROFILE_FUNCTION();

	Teddy::Renderer2D::ResetStats();

	{
		TED_PROFILE_SCOPE("Renderer Prep");

		Teddy::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Teddy::RenderCommand::Clear();
	}
}

