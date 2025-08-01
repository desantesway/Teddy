#pragma once

#include "teddyPch.h"

#include "Teddy/Core/Core.h"
#include "Teddy/Events/Event.h"
#include "Teddy/Events/MidiDriver.h"

#include <SDL3/SDL.h>
#include <glad/glad.h>	

namespace Teddy 
{

	struct WindowProps
	{
		std::string Title;
		unsigned int Width;
		unsigned int Height;

		WindowProps(const std::string& title = "Teddy Engine",
			unsigned int width = 1920,
			unsigned int height = 1080)
			: Title(title), Width(width), Height(height)
		{
		}
	};

	// Interface representing a desktop system based Window
	class Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;
		
		virtual ~Window() = default;

		virtual void OnUpdate() = 0;
		virtual void SDLEvents() = 0;

		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;

		virtual void SetWidth(int width) = 0;
		virtual void SetHeight(int height) = 0;

		// Window attributes
		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		virtual void* GetNativeWindow() const = 0;
		virtual void* GetNativeContext() const = 0;

		static Scope<Window> Create(const WindowProps& props = WindowProps());
	};
}