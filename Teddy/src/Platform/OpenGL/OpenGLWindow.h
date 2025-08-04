#pragma once

#include "Teddy/Renderer/GraphicsWindow.h"

#include <SDL3/SDL.h>

namespace Teddy 
{

	class OpenGLWindow : public GraphicsWindow
	{
	public:
		OpenGLWindow(const char* title, int width, int height);
		virtual ~OpenGLWindow() = default;

		virtual void* GetWindow() override { return m_Window; }

		virtual void Init() override;
		virtual void Shutdown() override;
	private:
		SDL_Window* m_Window;
		const char* m_Title;
		int m_Width, m_Height;
	};

}