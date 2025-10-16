#include "StartLayer.h"

#include "Teddy/PostProcessing/PostProcessing.h"

StartLayer::StartLayer()
	: Layer("Starting Layer")
{
	TED_PROFILE_FUNCTION();
}

void StartLayer::OnUpdate(Teddy::Timestep& ts)
{
	TED_PROFILE_FUNCTION();

	if (Teddy::FramebufferSpecification spec = Teddy::PostProcessing::GetFramebufferSpec();
		m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f &&
		(spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y))
	{
		Teddy::PostProcessing::Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
	}

	Teddy::Renderer2D::ResetStats();

	{
		TED_PROFILE_SCOPE("Renderer Prep");

		Teddy::PostProcessing::Bind();

		Teddy::RenderCommand::SetClearColor({ 1.0f, 0.1f, 0.1f, 1 });
		Teddy::RenderCommand::Clear();

		Teddy::PostProcessing::Clear();
	}
}

void StartLayer::OnEvent(Teddy::Event& event)
{
	Teddy::EventDispatcher dispatcher(event);
	dispatcher.Dispatch<Teddy::WindowResizeEvent>(TED_BIND_EVENT_FN(StartLayer::OnWindowResize));
}

bool StartLayer::OnWindowResize(Teddy::WindowResizeEvent& e)
{
	m_ViewportSize.x = (float)e.GetWidth();
	m_ViewportSize.y = (float)e.GetHeight();

	return false;
}