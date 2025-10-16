#pragma once

#include <Teddy.h>

namespace Cuphead
{
	class CupheadLayer : public Teddy::Layer
	{
	public:
		CupheadLayer();
		virtual ~CupheadLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		void OnUpdate(Teddy::Timestep& ts) override;
		void OnEvent(Teddy::Event& event) override;
		virtual void OnImGuiRender() override;

		Teddy::Ref<Teddy::Scene> GetActiveScene() { return m_ActiveScene; }

	private:
		bool OnKeyPressed(Teddy::KeyPressedEvent& e);
		bool OnWindowResize(Teddy::WindowResizeEvent& e);
	protected:
		bool new_Scene = false;
		Teddy::Ref<Teddy::Scene> m_ActiveScene;
	};
}