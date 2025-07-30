#include "Teddy/Renderer/SDLWindow.h"

#include "Teddy/Events/ApplicationEvent.h"
#include "Teddy/Events/MouseEvent.h"
#include "Teddy/Events/KeyEvent.h"

#include "Teddy/Renderer/GraphicsContext.h"

// TODO: SUBSTRACT OPENGL CREATION

namespace Teddy {
	
	static uint8_t s_SDLWindowCount = 0;

	Scope<Window> Window::Create(const WindowProps& props)
	{
		return CreateScope<SDLWindow>(props);
	}

	SDLWindow::SDLWindow(const WindowProps& props)
	{
		Init(props);
	}

	SDLWindow::~SDLWindow()
	{
		Shutdown();
	}

	void SDLWindow::Init(const WindowProps& props)
	{
		m_Data.Title = props.Title;
		m_Data.Width = props.Width;
		m_Data.Height = props.Height;

		TED_CORE_INFO("Creating window {0} ({1}, {2})", props.Title, props.Width, props.Height);

		if (s_SDLWindowCount == 0)
		{
			SDL_SetAppMetadata(m_Data.Title.c_str(), "0.01", "com.teddy.window");
			int success = SDL_Init(SDL_INIT_VIDEO);
			TED_CORE_ASSERT(success, "Could not intialize SDL!");

			++s_SDLWindowCount;
		}

		m_Window = GraphicsWindow::Create(m_Data.Title.c_str(), (int)props.Width, (int)props.Height);
		m_Window->Init();

		m_Context = GraphicsContext::Create(m_Window->GetWindow());
		m_Context->Init();
		
		midiDriver.Init(&EventCallback);
		midiDriver.InitIn(0);
		midiDriver.InitOut(2);

		SetVSync(true);
	}

	void SDLWindow::Shutdown()
	{	
		m_Context->Shutdown();

		m_Window->Shutdown();
		
		s_SDLWindowCount--;
		if (s_SDLWindowCount == 0) {
			TED_CORE_INFO("Terminating SDL");
			SDL_Quit();
		}

	}

	void SDLWindow::OnUpdate()
	{
		midiDriver.OnUpdate();
		SDLEvents();

		m_Context->SwapBuffers();
	}

    void SDLWindow::SetVSync(bool enabled)
    {
		if (enabled)
			SDL_GL_SetSwapInterval(1);
		else
			SDL_GL_SetSwapInterval(0);

		m_Data.VSync = enabled;
    }

	bool SDLWindow::IsVSync() const
	{
		return m_Data.VSync;
	}

}
