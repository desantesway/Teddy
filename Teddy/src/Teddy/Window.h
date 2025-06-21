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

		Window(const WindowProps& props = WindowProps());
		~Window();

		void OnUpdate();

		// Window attributes
		inline void SetEventCallback(const EventCallbackFn& callback) { SDL_Data.EventCallback = callback; }
		void SetVSync(bool enabled);
		bool IsVSync() const;

		inline unsigned int GetWidth() const { return SDL_Data.Width; }
		inline unsigned int GetHeight() const { return SDL_Data.Height; }

	private:
		void Init(const WindowProps& props);
		void Shutdown();

		// vars

		SDL_Window* SDL_Window;

		struct WindowData
		{
			std::string Title;
			unsigned int Width, Height;
			bool VSync;

			EventCallbackFn EventCallback;
		};

		WindowData SDL_Data;
	};
}