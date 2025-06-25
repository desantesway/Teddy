#include "OpenGLWindow.h"

#include "Teddy/Events/ApplicationEvent.h"
#include "Teddy/Events/MouseEvent.h"
#include "Teddy/Events/KeyEvent.h"

#include "Platform/OpenGL/OpenGLContext.h"

namespace Teddy {
	
	static bool SDL_Initialized = false;

	Window* Window::Create(const WindowProps& props)
	{
		return new OpenGLWindow(props);
	}

	OpenGLWindow::OpenGLWindow(const WindowProps& props)
	{
		Init(props);
	}

	OpenGLWindow::~OpenGLWindow()
	{
		Shutdown();
	}

	void OpenGLWindow::Init(const WindowProps& props)
	{
		m_Data.Title = props.Title;
		m_Data.Width = props.Width;
		m_Data.Height = props.Height;

		TED_CORE_INFO("Creating window {0} ({1}, {2})", props.Title, props.Width, props.Height);

		if (!SDL_Initialized)
		{
			SDL_SetAppMetadata(m_Data.Title.c_str(), "0.01", "com.teddy.window");
			int success = SDL_Init(SDL_INIT_VIDEO);
			TED_CORE_ASSERT(success, "Could not intialize SDL!");

			SDL_Initialized = true;
		}

		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

		m_Window = SDL_CreateWindow(m_Data.Title.c_str(), (int)props.Width, (int)props.Height, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

		m_Context = new OpenGLContext(m_Window);
		m_Context->Init();

		SetVSync(true);
		
	}

	void OpenGLWindow::Shutdown()
	{	
		m_Context->Shutdown();

		if (m_Window) {
			SDL_DestroyWindow(m_Window);
			m_Window = nullptr;
		}

		SDL_Quit();

	}

	void OpenGLWindow::OnUpdate()
	{
		Events();
		m_Context->SwapBuffers();
	}

    void OpenGLWindow::SetVSync(bool enabled)
    {
		if (enabled)
			SDL_GL_SetSwapInterval(1);
		else
			SDL_GL_SetSwapInterval(0);

		m_Data.VSync = enabled;
    }

	bool OpenGLWindow::IsVSync() const
	{
		return m_Data.VSync;
	}

}
