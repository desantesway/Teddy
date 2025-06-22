#pragma once

#include "teddy_pch.h"

#include "Window.h"

namespace Teddy {

	static bool SDL_Initialized = false;

	Window::Window(const WindowProps& props)
	{
		Init(props);
	}

	Window::~Window()
	{
		Shutdown();
	}

	void Window::Init(const WindowProps& props)
	{
		SDL_Data.Title = props.Title;
		SDL_Data.Width = props.Width;
		SDL_Data.Height = props.Height;

		TED_CORE_INFO("Creating window {0} ({1}, {2})", props.Title, props.Width, props.Height);

		if (!SDL_Initialized)
		{
			SDL_SetAppMetadata(SDL_Data.Title.c_str(), "0.01", "com.teddy.window");
			int success = SDL_Init(SDL_INIT_VIDEO);
			TED_CORE_ASSERT(success, "Could not intialize SDL!");

			SDL_Initialized = true;
		}

		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

		SDL_Window = SDL_CreateWindow(SDL_Data.Title.c_str(), (int)props.Width, (int)props.Height, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
		SDL_GL_CreateContext(SDL_Window);

		if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
			TED_CORE_ERROR("Could not intialize GLAD!");
		}

		//SDL_SetPointerProperty(SDL_GetWindowProperties(SDL_Window), "WindowData", &SDL_Data);

		SetVSync(true);

		isRunning = true;
	}

	void Window::Shutdown()
	{
		if (SDL_Window) {
			SDL_DestroyWindow(SDL_Window);
			SDL_Window = nullptr;
		}

		SDL_Quit();

	}

	void Window::OnUpdate()
	{
		SDL_GL_SwapWindow(SDL_Window);
	}

	void Window::SetVSync(bool enabled)
	{
		SDL_GL_SetSwapInterval(enabled ? 1 : 0);

		SDL_Data.VSync = enabled;
	}

	bool Window::IsVSync() const
	{
		return SDL_Data.VSync;
	}

	
}