#pragma once

#include "Teddy/Layer.h"

namespace Teddy
{
	class TED_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();
		void OnAttach() override;
		void OnDetach() override;
		void OnUpdate() override;
		void OnImGuiRender();
		void OnEvent(Event& event) override;
	private:
		void Begin();
		void End();

		float m_Time = 0.0f;
	};
} // namespace Teddy