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
#ifdef TED_ENABLE_MIDI
		m_Data.MidiDriver.SetEventCallback(std::make_shared<EventCallbackFn>(m_Data.EventCallback));
#endif
	}

	SDLWindow::SDLWindow(const WindowProps& props)
	{
		TED_PROFILE_CAT(InstrumentorCategory::Streaming);

		Init(props);
	}

	SDLWindow::~SDLWindow()
	{
		TED_PROFILE_CAT(InstrumentorCategory::Streaming);

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
#ifdef TED_ENABLE_MIDI
			m_Data.MidiDriver.Init();
			m_Data.MidiDriver.InitIn(0);
			m_Data.MidiDriver.InitOut(2);
#endif

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

		SDL_AddEventWatch(&EventWatcher, this);

		m_Context = GraphicsContext::Create(m_Window->GetWindow());
		m_Context->Init(); 

		SetVSync(true);

		TED_CORE_INFO("Detected {} threads!", GetThreadCount());
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

	void SDLWindow::CalculateFPS(Timestep ts)
	{
		static double timer = 0.0;
		timer += ts;

		float fps = (ts > 0.0f) ? (1.0f / ts) : 0.0f;
		m_Stats.FrameTimes.push_back(fps);

		if (m_Stats.FrameTimes.size() > 5)
			m_Stats.FrameTimes.erase(m_Stats.FrameTimes.begin());

		if (timer > 0.5)
		{
			timer = 0.0;
			m_Stats.FPS = 0;
			for (float time : m_Stats.FrameTimes)
				m_Stats.FPS += time;
			m_Stats.FPS = m_Stats.FPS / m_Stats.FrameTimes.size();
		}
	}

	void SDLWindow::OnUpdate(Timestep ts)
	{
		TED_PROFILE_FUNCTION();

		CalculateFPS(ts);

#ifdef TED_ENABLE_MIDI
			m_Data.MidiDriver.OnUpdate();
#endif

		SDLEvents();

		m_Context->SwapBuffers();
	}

    void SDLWindow::SetVSync(bool enabled)
    {
		TED_PROFILE_FUNCTION();

		m_Window->VSync(enabled);

		m_Data.VSync = enabled;
    }

	bool SDLWindow::IsVSync() const
	{
		return m_Data.VSync;
	}

	void SDLWindow::SetScreenMode(ScreenMode mode)
	{
		switch (mode)
		{
		case Teddy::ScreenMode::Windowed:
			SDL_SetWindowFullscreen((SDL_Window*)m_Window->GetWindow(), false);
			SDL_SetWindowBordered((SDL_Window*)m_Window->GetWindow(), true);
			break;
		case Teddy::ScreenMode::Borderless:
			SDL_SetWindowFullscreen((SDL_Window*)m_Window->GetWindow(), false);
			SDL_SetWindowBordered((SDL_Window*)m_Window->GetWindow(), false);
			break;
		case Teddy::ScreenMode::Fullscreen:
			SDL_SetWindowFullscreen((SDL_Window*)m_Window->GetWindow(), true);
			SDL_SetWindowBordered((SDL_Window*)m_Window->GetWindow(), false);
			break;
		default:
			break;
		}
	}

	ScreenMode SDLWindow::GetScreenMode() const
	{
		Uint32 flags = SDL_GetWindowFlags((SDL_Window*)m_Window->GetWindow());
		if (flags & SDL_WINDOW_FULLSCREEN)
			return ScreenMode::Fullscreen;
		else if (flags & SDL_WINDOW_BORDERLESS)
			return ScreenMode::Borderless;
		else
			return ScreenMode::Windowed;
	}

	unsigned int SDLWindow::GetThreadCount() const
	{ 
		return std::thread::hardware_concurrency();
	}
}
