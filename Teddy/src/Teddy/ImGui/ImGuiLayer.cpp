#include "teddy_pch.h"
#include "ImGuiLayer.h"

#include "imgui.h"
#include "imgui_impl_opengl3.h"
// "imgui_impl_opengl3.cpp"

#include <SDL3/SDL.h>

#include "Teddy/Application.h"

namespace Teddy
{
	ImGuiLayer::ImGuiLayer()
		:Layer("ImGuiLayer"){}
	ImGuiLayer::~ImGuiLayer() {}
	void ImGuiLayer::OnAttach() {
		ImGui::CreateContext();
		ImGui::StyleColorsDark();

		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

		ImGui_ImplOpenGL3_Init("#version 410");
	}
	void ImGuiLayer::OnDetach() {}
	void ImGuiLayer::OnUpdate() {
		
		ImGuiIO& io = ImGui::GetIO();
		Application& app = Application::Get();
		io.DisplaySize = ImVec2(app.GetWindow().GetWidth(), app.GetWindow().GetHeight());

		float time = (float)SDL_GetTicks() / 1000.0f;
		io.DeltaTime = m_Time > 0.0f ? (time - m_Time) : (1.0f / 60.0f);
		m_Time = time;

		ImGui_ImplOpenGL3_NewFrame();
		ImGui::NewFrame();

		static bool show = true;
		ImGui::ShowDemoWindow(&show);

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}
	void ImGuiLayer::OnImGuiRender() {}
	void ImGuiLayer::OnEvent(Event& event) {}
	void Begin() {}
	void End() {}
}