#pragma once

#include "Teddy/Renderer/GraphicsContext.h"

#include <SDL3/SDL.h>

namespace Teddy {

	class OpenGLContext : public GraphicsContext
	{
	public:
		OpenGLContext(SDL_Window* windowHandle);

		virtual void* GetContext() override { return m_Context; }

		virtual void Init() override;
		virtual void SwapBuffers() override;
		virtual void Shutdown() override;
	private:
		SDL_Window* m_WindowHandle;
		SDL_GLContext m_Context;
	};

}