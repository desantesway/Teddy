#include "TeddyPch.h"
#include "Platform/OpenGL/OpenGLWindow.h"

#include <glad/glad.h>

namespace Teddy 
{

	OpenGLWindow::OpenGLWindow(const char* title, int width, int height)
		: m_Title(title), m_Width(width), m_Height(height), m_Window(nullptr)
	{
	}

	void OpenGLWindow::Init()
	{
		TED_PROFILE_FUNCTION();

		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);

		#ifdef TED_DEBUG
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
		#endif

		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

		m_Window = SDL_CreateWindow(m_Title, m_Width, m_Height, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
	}

	void OpenGLWindow::Shutdown() 
	{
		TED_PROFILE_FUNCTION();

		if (m_Window) 
		{
			SDL_DestroyWindow(m_Window);
			m_Window = nullptr;
		}
	}

	void OpenGLWindow::VSync(bool enabled)
	{
		if (enabled)
			SDL_GL_SetSwapInterval(1);
		else
			SDL_GL_SetSwapInterval(0);
	}
}