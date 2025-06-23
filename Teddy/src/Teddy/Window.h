#pragma once

#include "teddy_pch.h"

#include "Teddy/Core.h"
#include "Teddy/Events/Event.h"

#include <SDL3/SDL.h>
#include <glad/glad.h>	

namespace Teddy {

	struct WindowProps
	{
		std::string Title;
		unsigned int Width;
		unsigned int Height;

		WindowProps(const std::string& title = "Teddy Engine",
			unsigned int width = 1920,
			unsigned int height = 1080)
			: Title(title), Width(width), Height(height)
		{}
	};

	// Interface representing a desktop system based Window
	class TED_API Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;
		
		virtual ~Window() {}

		virtual void OnUpdate() = 0;
		void Events();

		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;

		virtual SDL_Window* GetWindow() const = 0;
		virtual SDL_GLContext GetGLContext() const = 0;

		// Window attributes
		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		static Window* Create(const WindowProps& props = WindowProps());

		EventCallbackFn EventCallback;
	};
}