#include "teddyPch.h"
#include "ImGuiLayer.h"

#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdl3.cpp"

#include <SDL3/SDL.h>
#include <glad/glad.h>

#include "Teddy/Application.h"

#include "Teddy/CodeConverter.h"
#include "Teddy/KeyCodes.h"
#include "Teddy/Input.h"

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

		Application& app = Application::Get();
		SDL_Window* window = static_cast<SDL_Window*>(app.GetWindow().GetNativeWindow());
		SDL_GLContext glcontext = static_cast<SDL_GLContext>(app.GetWindow().GetNativeContext());

		ImGui_ImplSDL3_InitForOpenGL(window, glcontext);
		ImGui_ImplOpenGL3_Init("#version 410");
	}
	void ImGuiLayer::OnDetach() {}
	void ImGuiLayer::OnUpdate() {
		
		ImGuiIO& io = ImGui::GetIO();
		Application& app = Application::Get();
		io.DisplaySize = ImVec2(app.GetWindow().GetWidth(), app.GetWindow().GetHeight());

		static uint64_t s_LastCounter = SDL_GetPerformanceCounter();
		uint64_t currentCounter = SDL_GetPerformanceCounter();
		uint64_t frequency = SDL_GetPerformanceFrequency();

		double delta = (double)(currentCounter - s_LastCounter) / (double)frequency;
		if (delta <= 0.0) delta = 1.0 / 60.0; 

		io.DeltaTime = static_cast<float>(delta);
		s_LastCounter = currentCounter;

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL3_NewFrame();
		ImGui::NewFrame();

		static bool show = true;
		ImGui::ShowDemoWindow(&show);

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}
	void ImGuiLayer::OnEvent(Event& event) {
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<MouseButtonPressedEvent>(BIND_EVENT_FN(ImGuiLayer::OnMouseButtonPressedEvent));
		dispatcher.Dispatch<MouseButtonReleasedEvent>(BIND_EVENT_FN(ImGuiLayer::OnMouseButtonReleasedEvent));
		dispatcher.Dispatch<MouseMovedEvent>(BIND_EVENT_FN(ImGuiLayer::OnMouseMovedEvent));
		dispatcher.Dispatch<MouseScrolledEvent>(BIND_EVENT_FN(ImGuiLayer::OnMouseScrolledEvent));

		dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT_FN(ImGuiLayer::OnKeyPressedEvent));
		dispatcher.Dispatch<KeyReleasedEvent>(BIND_EVENT_FN(ImGuiLayer::OnKeyReleasedEvent));
		dispatcher.Dispatch<KeyTypedEvent>(BIND_EVENT_FN(ImGuiLayer::OnKeyTypedEvent));

		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(ImGuiLayer::OnWindowResizeEvent));
		
	}

	bool ImGuiLayer::OnMouseButtonPressedEvent(MouseButtonPressedEvent& e) {
		ImGuiIO& io = ImGui::GetIO();
		io.AddMouseButtonEvent(TeddyToImGuiMouse(e.GetMouseButton()), true);
		
		return false;
	}

	bool ImGuiLayer::OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e){

		ImGuiIO& io = ImGui::GetIO();
		io.AddMouseButtonEvent(TeddyToImGuiMouse(e.GetMouseButton()), false);

		return false;
	}

	bool ImGuiLayer::OnMouseMovedEvent(MouseMovedEvent& e){
		ImGuiIO& io = ImGui::GetIO();
		io.MousePos = ImVec2(e.GetX(), e.GetY());

		return false;
	}

	bool ImGuiLayer::OnMouseScrolledEvent(MouseScrolledEvent& e){ 
		ImGuiIO& io = ImGui::GetIO();
		io.AddMouseWheelEvent(-e.GetXOffset(), e.GetYOffset());
		return false; 
	}

	bool ImGuiLayer::OnKeyPressedEvent(KeyPressedEvent& e){ 
		ImGuiIO& io = ImGui::GetIO();
		io.AddKeyEvent((ImGuiKey)TeddyToImGuiKey(e.GetKeyCode()), true);

		if (e.GetKeyCode() == TED_KEY_LCTRL || e.GetKeyCode() == TED_KEY_RCTRL) io.AddKeyEvent(ImGuiMod_Ctrl, true);
		if (e.GetKeyCode() == TED_KEY_LSHIFT || e.GetKeyCode() == TED_KEY_RSHIFT) io.AddKeyEvent(ImGuiMod_Shift, true);
		if (e.GetKeyCode() == TED_KEY_LALT || e.GetKeyCode() == TED_KEY_RALT) io.AddKeyEvent(ImGuiMod_Alt, true);
		if (e.GetKeyCode() == TED_KEY_LGUI || e.GetKeyCode() == TED_KEY_RGUI) io.AddKeyEvent(ImGuiMod_Super, true);

		return false; 
	}

	bool ImGuiLayer::OnKeyReleasedEvent(KeyReleasedEvent& e){ 
		ImGuiIO& io = ImGui::GetIO();
		io.AddKeyEvent((ImGuiKey)TeddyToImGuiKey(e.GetKeyCode()), false);
		
		if (!Input::IsKeyPressed(TED_KEY_LCTRL) && !Input::IsKeyPressed(TED_KEY_RCTRL)) io.AddKeyEvent(ImGuiMod_Ctrl, false);
		if (!Input::IsKeyPressed(TED_KEY_LSHIFT) && !Input::IsKeyPressed(TED_KEY_RSHIFT)) io.AddKeyEvent(ImGuiMod_Shift, false);
		if (!Input::IsKeyPressed(TED_KEY_LALT) && !Input::IsKeyPressed(TED_KEY_RALT)) io.AddKeyEvent(ImGuiMod_Alt, false);
		if (!Input::IsKeyPressed(TED_KEY_LGUI) && !Input::IsKeyPressed(TED_KEY_RGUI)) io.AddKeyEvent(ImGuiMod_Super, false);

		return false; 
	}

	bool ImGuiLayer::OnKeyTypedEvent(KeyTypedEvent& e){
		
		ImGuiIO& io = ImGui::GetIO();
		io.AddInputCharactersUTF8(e.GetKey());
		return false;
	}

	bool ImGuiLayer::OnWindowResizeEvent(WindowResizeEvent& e){ 
		ImGuiIO& io = ImGui::GetIO();

		io.DisplaySize = ImVec2(e.GetWidth(), e.GetHeight());
		io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);
		glViewport(0,0, e.GetWidth(), e.GetHeight());

		return false; 
	}
}