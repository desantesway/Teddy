#include "TeddyPch.h"
#include "Platform/OpenGL/OpenGLContext.h"

#include <SDL3/SDL.h>
#include <glad/glad.h>

namespace Teddy 
{

	OpenGLContext::OpenGLContext(SDL_Window* windowHandle)
		: m_WindowHandle(windowHandle)
	{
		TED_CORE_ASSERT(windowHandle != nullptr, "Window handle is null!")
	}

	void OpenGLContext::Init()
	{
		TED_PROFILE_FUNCTION();

		m_Context = SDL_GL_CreateContext(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress);
		TED_CORE_ASSERT(status, "Could not intialize GLAD!");

		TED_CORE_INFO("OpenGL Info:");
		TED_CORE_INFO("  Vendor: {0}", reinterpret_cast<const char*>(glGetString(GL_VENDOR)));
		TED_CORE_INFO("  Renderer: {0}", reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
		TED_CORE_INFO("  Version: {0}", reinterpret_cast<const char*>(glGetString(GL_VERSION)));

		TED_CORE_ASSERT(GLVersion.major > 4 || (GLVersion.major == 4 && GLVersion.minor >= 5), "Teddy requires at least OpenGL version 4.5!");
	}

	void OpenGLContext::SwapBuffers()
	{
		TED_PROFILE_FUNCTION();

		SDL_GL_SwapWindow(m_WindowHandle);
	}

	void OpenGLContext::Shutdown() 
	{
		TED_PROFILE_FUNCTION();

		SDL_GL_DestroyContext(m_Context);
	}

}