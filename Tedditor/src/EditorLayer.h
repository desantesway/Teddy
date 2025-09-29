#pragma once

#include <Teddy.h>

#include "Editor/Editor.h"

namespace Teddy
{
	class EditorLayer : public Layer
	{
	public:
		EditorLayer();
		virtual ~EditorLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		void OnUpdate(Timestep ts) override;
		void OnEvent(Event& event) override;
		virtual void OnImGuiRender() override;
	private:

		Editor m_Editor;
	};
}