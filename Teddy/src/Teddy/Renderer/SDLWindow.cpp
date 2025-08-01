#include "Teddy/Renderer/SDLWindow.h"

#include "Teddy/Events/ApplicationEvent.h"
#include "Teddy/Events/MouseEvent.h"
#include "Teddy/Events/KeyEvent.h"

#include "Teddy/Renderer/GraphicsContext.h"

namespace Teddy 
{
	
	static uint8_t s_SDLWindowCount = 0;

	void SDLWindow::SetEventCallback(const EventCallbackFn& callback) 
	{
		m_Data.EventCallback = callback;
		m_Data.MidiDriver.SetEventCallback(std::make_shared<EventCallbackFn>(m_Data.EventCallback));
	}

	SDLWindow::SDLWindow(const WindowProps& props)
	{
		TED_PROFILE_FUNCTION();

		Init(props);
	}

	SDLWindow::~SDLWindow()
	{
		TED_PROFILE_FUNCTION();

		Shutdown();
	}

	void SDLWindow::Init(const WindowProps& props)
	{
		TED_PROFILE_FUNCTION();

		m_Data.Title = props.Title;
		m_Data.Width = props.Width;
		m_Data.Height = props.Height;

		TED_CORE_INFO("Creating window {0} ({1}, {2})", props.Title, props.Width, props.Height);

		if (s_SDLWindowCount == 0)
		{
			m_Data.MidiDriver.Init();

			m_Data.MidiDriver.InitIn(0);
			m_Data.MidiDriver.InitOut(2);

			SDL_SetAppMetadata(m_Data.Title.c_str(), "0.01", "com.teddy.window");
			int success = 0;

			{
				TED_PROFILE_SCOPE("SDL_Init");
				success = SDL_Init(SDL_INIT_VIDEO);
			}

			TED_CORE_ASSERT(success, "Could not initialize SDL!");

			++s_SDLWindowCount;

			
		}

		m_Window = GraphicsWindow::Create(m_Data.Title.c_str(), (int)props.Width, (int)props.Height);
		m_Window->Init();

		m_Context = GraphicsContext::Create(m_Window->GetWindow());
		m_Context->Init(); 

		SetVSync(true);
	}

	void SDLWindow::Shutdown()
	{	
		TED_PROFILE_FUNCTION();

		m_Context->Shutdown();

		m_Window->Shutdown();
		
		s_SDLWindowCount--;
		if (s_SDLWindowCount == 0) 
		{
			TED_CORE_INFO("Terminating SDL");
			SDL_Quit();
		}

	}

	void SDLWindow::OnUpdate()
	{
		TED_PROFILE_FUNCTION();

		m_Data.MidiDriver.OnUpdate();
		SDLEvents();

		m_Context->SwapBuffers();
	}

	// TODO: API ABSTRACTION
    void SDLWindow::SetVSync(bool enabled)
    {
		TED_PROFILE_FUNCTION();

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
