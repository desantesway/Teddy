#pragma once

#include "teddy_pch.h"

#include "Teddy/Core.h"

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

		Window(const WindowProps& props = WindowProps());
		~Window();

		void OnUpdate();

		void SetVSync(bool enabled);
		bool IsVSync() const;

		inline unsigned int GetWidth() const { return SDL_Data.Width; }
		inline unsigned int GetHeight() const { return SDL_Data.Height; }
		void SetWidth(unsigned int w) { SDL_Data.Width = w; }
		void SetHeight(unsigned int h) { SDL_Data.Height = h; }

		void PollEvents();
		bool IsRunning() { return isRunning; }

		void KeyEvent(bool pressed, SDL_Event event);
		void MouseEvent(bool pressed, SDL_Event event);
		void MouseMovement(SDL_Event event);
		void MouseWheel(SDL_Event event);

	private:
		void Init(const WindowProps& props);
		void Shutdown();

		// vars

		SDL_Window* SDL_Window;
		bool isRunning;

		struct WindowData
		{
			std::string Title;
			unsigned int Width, Height;
			bool VSync;

		};

		WindowData SDL_Data;
	};
}