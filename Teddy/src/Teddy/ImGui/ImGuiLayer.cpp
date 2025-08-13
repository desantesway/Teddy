#include "TeddyPch.h"
#include "Teddy/ImGui/ImGuiLayer.h"

#include <imgui.h>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_opengl3.h>

#include <SDL3/SDL.h>
#include <glad/glad.h>

#include "Teddy/Core/Application.h"

#include "Teddy/Core/CodeConverter.h"
#include "Teddy/Core/KeyCodes.h"
#include "Teddy/Core/Input.h"

namespace Teddy
{

	ImGuiLayer::ImGuiLayer()
		:Layer("ImGuiLayer"){}

	void ImGuiLayer::OnAttach() 
	{
		TED_PROFILE_FUNCTION();

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();

		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;	      // Enable Multi-Viewport / Platform Windows
		// TODO : Multi viewport stopped working(?)

		io.Fonts->AddFontFromFileTTF(
			"../Teddy/assets/fonts/instrument-sans/ttf/InstrumentSans-Bold.ttf", 18.0f);
		io.FontDefault = io.Fonts->AddFontFromFileTTF(
			"../Teddy/assets/fonts/instrument-sans/ttf/InstrumentSans-Regular.ttf", 18.0f);

		ImGui::StyleColorsDark();

		SetDarkThemeColors();

		ImGuiStyle& style = ImGui::GetStyle();

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		Application& app = Application::Get();
		SDL_Window* window = static_cast<SDL_Window*>(app.GetWindow().GetNativeWindow());
		SDL_GLContext glcontext = static_cast<SDL_GLContext>(app.GetWindow().GetNativeContext());

		ImGui_ImplSDL3_InitForOpenGL(window, glcontext);
		ImGui_ImplOpenGL3_Init("#version 410");
	}
	
	void ImGuiLayer::OnDetach() 
	{
		TED_PROFILE_FUNCTION();

		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplSDL3_Shutdown();
		ImGui::DestroyContext();
	}

	void ImGuiLayer::Begin() 
	{
		TED_PROFILE_FUNCTION();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL3_NewFrame();
		ImGui::NewFrame();
	}

	void ImGuiLayer::End() 
	{

		TED_PROFILE_FUNCTION();

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

	void ImGuiLayer::OnEvent(Event& event) 
	{
		TED_PROFILE_FUNCTION();

		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<MouseButtonPressedEvent>(TED_BIND_EVENT_FN(ImGuiLayer::OnMouseButtonPressedEvent));
		dispatcher.Dispatch<MouseButtonReleasedEvent>(TED_BIND_EVENT_FN(ImGuiLayer::OnMouseButtonReleasedEvent));
		dispatcher.Dispatch<MouseMovedEvent>(TED_BIND_EVENT_FN(ImGuiLayer::OnMouseMovedEvent));
		dispatcher.Dispatch<MouseScrolledEvent>(TED_BIND_EVENT_FN(ImGuiLayer::OnMouseScrolledEvent));

		dispatcher.Dispatch<KeyPressedEvent>(TED_BIND_EVENT_FN(ImGuiLayer::OnKeyPressedEvent));
		dispatcher.Dispatch<KeyReleasedEvent>(TED_BIND_EVENT_FN(ImGuiLayer::OnKeyReleasedEvent));
		dispatcher.Dispatch<KeyTypedEvent>(TED_BIND_EVENT_FN(ImGuiLayer::OnKeyTypedEvent));

		dispatcher.Dispatch<WindowResizeEvent>(TED_BIND_EVENT_FN(ImGuiLayer::OnWindowResizeEvent));

		if (m_BlockEvents)
		{
			ImGuiIO& io = ImGui::GetIO();

			event.Handled |= event.IsInCategory(EventCategoryMouse) & io.WantCaptureMouse;
			event.Handled |= event.IsInCategory(EventCategoryKeyboard) & io.WantCaptureKeyboard;
		}
	}

	bool ImGuiLayer::OnMouseButtonPressedEvent(MouseButtonPressedEvent& e) 
	{
		TED_PROFILE_FUNCTION();

		ImGuiIO& io = ImGui::GetIO();
		io.AddMouseButtonEvent(TeddyToImGuiMouse(e.GetMouseButton()), true);
		
		return false;
	}

	bool ImGuiLayer::OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e)
	{
		TED_PROFILE_FUNCTION();

		ImGuiIO& io = ImGui::GetIO();
		io.AddMouseButtonEvent(TeddyToImGuiMouse(e.GetMouseButton()), false);

		return false;
	}

	bool ImGuiLayer::OnMouseMovedEvent(MouseMovedEvent& e)
	{
		TED_PROFILE_FUNCTION();

		ImGuiIO& io = ImGui::GetIO();
		io.MousePos = ImVec2(e.GetX(), e.GetY());

		return false;
	}

	bool ImGuiLayer::OnMouseScrolledEvent(MouseScrolledEvent& e)
	{ 
		TED_PROFILE_FUNCTION();

		ImGuiIO& io = ImGui::GetIO();
		io.AddMouseWheelEvent(-e.GetXOffset(), e.GetYOffset());
		return false; 
	}

	bool ImGuiLayer::OnKeyPressedEvent(KeyPressedEvent& e)
	{ 
		TED_PROFILE_FUNCTION();

		ImGuiIO& io = ImGui::GetIO();
		io.AddKeyEvent((ImGuiKey)TeddyToImGuiKey(e.GetKeyCode()), true);

		if (e.GetKeyCode() == Key::LCtrl	|| e.GetKeyCode() == Key::RCtrl	) io.AddKeyEvent(ImGuiMod_Ctrl, true);
		if (e.GetKeyCode() == Key::LShift	|| e.GetKeyCode() == Key::RShift	) io.AddKeyEvent(ImGuiMod_Shift, true);
		if (e.GetKeyCode() == Key::LAlt		|| e.GetKeyCode() == Key::RAlt	) io.AddKeyEvent(ImGuiMod_Alt, true);
		if (e.GetKeyCode() == Key::LGui		|| e.GetKeyCode() == Key::RGui	) io.AddKeyEvent(ImGuiMod_Super, true);

		return false; 
	}

	bool ImGuiLayer::OnKeyReleasedEvent(KeyReleasedEvent& e)
	{ 
		TED_PROFILE_FUNCTION();

		ImGuiIO& io = ImGui::GetIO();
		io.AddKeyEvent((ImGuiKey)TeddyToImGuiKey(e.GetKeyCode()), false);
		
		if (!Input::IsKeyPressed(Key::LCtrl		) && !Input::IsKeyPressed(Key::RCtrl	)) io.AddKeyEvent(ImGuiMod_Ctrl, false);
		if (!Input::IsKeyPressed(Key::LShift	) && !Input::IsKeyPressed(Key::RShift	)) io.AddKeyEvent(ImGuiMod_Shift, false);
		if (!Input::IsKeyPressed(Key::LAlt		) && !Input::IsKeyPressed(Key::RAlt		)) io.AddKeyEvent(ImGuiMod_Alt, false);
		if (!Input::IsKeyPressed(Key::LGui		) && !Input::IsKeyPressed(Key::RGui		)) io.AddKeyEvent(ImGuiMod_Super, false);

		return false; 
	}

	bool ImGuiLayer::OnKeyTypedEvent(KeyTypedEvent& e)
	{
		
		TED_PROFILE_FUNCTION();

		ImGuiIO& io = ImGui::GetIO();
		io.AddInputCharactersUTF8(e.GetKey());
		return false;
	}

	bool ImGuiLayer::OnWindowResizeEvent(WindowResizeEvent& e)
	{ 
		TED_PROFILE_FUNCTION();

		ImGuiIO& io = ImGui::GetIO();

		io.DisplaySize = ImVec2(e.GetWidth(), e.GetHeight());
		io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);
		glViewport(0,0, e.GetWidth(), e.GetHeight());

		return false; 
	}

	void ImGuiLayer::SetDarkThemeColors()
	{
		auto& colors = ImGui::GetStyle().Colors;

		colors[ImGuiCol_WindowBg] = ImVec4{ 0.1f, 0.1f, 0.1f, 1.0f };

		// Headers
		colors[ImGuiCol_Header] = ImVec4{ 0.2f, 0.2f, 0.2f, 1.0f };
		colors[ImGuiCol_HeaderHovered] = ImVec4{ 0.25f, 0.25f, 0.25f, 1.0f };
		colors[ImGuiCol_HeaderActive] = ImVec4{ 0.15f, 0.15f, 0.15f, 1.0f };

		// Buttons
		colors[ImGuiCol_Button] = ImVec4{ 0.2f, 0.2f, 0.2f, 1.0f };
		colors[ImGuiCol_ButtonHovered] = ImVec4{ 0.25f, 0.25f, 0.25f, 1.0f };
		colors[ImGuiCol_ButtonActive] = ImVec4{ 0.2f, 0.2f, 0.2f, 1.0f };

		// Frame BG
		colors[ImGuiCol_FrameBg] = ImVec4{ 0.1f, 0.1f, 0.1f, 1.0f };
		colors[ImGuiCol_FrameBgHovered] = ImVec4{ 0.294f, 0.294f, 0.294f, 1.0f };
		colors[ImGuiCol_FrameBgActive] = ImVec4{ 0.294f, 0.294f, 0.294f, 1.0f };

		// Tabs
		colors[ImGuiCol_Tab] = ImVec4{ 0.2f, 0.2f, 0.2f, 1.0f };
		colors[ImGuiCol_TabHovered] = ImVec4{ 0.25f, 0.25f, 0.25f, 1.0f };
		colors[ImGuiCol_TabActive] = ImVec4{ 0.2f, 0.2f, 0.2f, 1.0f };
		colors[ImGuiCol_TabUnfocused] = ImVec4{ 0.1f, 0.1f, 0.1f, 1.0f };
		colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.1f, 0.1f, 0.1f, 1.0f };
		colors[ImGuiCol_TabSelectedOverline] = ImVec4{ 0.5f, 0.5f, 0.5f, 1.0f };

		// Title
		colors[ImGuiCol_TitleBg] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
		colors[ImGuiCol_TitleBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
	}
}