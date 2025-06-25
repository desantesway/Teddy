#include "teddyPch.h"
#include "ImGuiLayer.h"

#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_opengl3.h"

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
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();

		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;	      // Enable Multi-Viewport / Platform Windows

		ImGui::StyleColorsDark();

		ImGuiStyle& style = ImGui::GetStyle();

		Application& app = Application::Get();
		SDL_Window* window = static_cast<SDL_Window*>(app.GetWindow().GetNativeWindow());
		SDL_GLContext glcontext = static_cast<SDL_GLContext>(app.GetWindow().GetNativeContext());

		ImGui_ImplSDL3_InitForOpenGL(window, glcontext);
		ImGui_ImplOpenGL3_Init("#version 410");
	}
	
	void ImGuiLayer::OnDetach() {
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplSDL3_Shutdown();
		ImGui::DestroyContext();
	}

	void ImGuiLayer::Begin() {
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL3_NewFrame();
		ImGui::NewFrame();
	}

	void ImGuiLayer::End() {

		ImGuiIO& io = ImGui::GetIO();

		ImGui::Render();

		glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			SDL_Window* backup_current_window = SDL_GL_GetCurrentWindow();
			SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			SDL_GL_MakeCurrent(backup_current_window, backup_current_context);
		}

		Application& app = Application::Get();
		SDL_Window* window = static_cast<SDL_Window*>(app.GetWindow().GetNativeWindow());
	}

	void ImGuiLayer::OnImGuiRender() {
		static bool show = true;
		ImGui::ShowDemoWindow(&show);
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