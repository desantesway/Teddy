#pragma once

#include "Teddy/Core/Base.h"
#include "Teddy/Events/Event.h"
#ifdef TED_ENABLE_MIDI
#include "Teddy/Events/MidiDriver.h"
#endif
#include "Teddy/Core/Timestep.h"

#include <SDL3/SDL.h>
#include <glad/glad.h>	

#include <sstream>

namespace Teddy 
{

	struct WindowStats
	{
		float FPS = 0;
		std::vector<float> FrameTimes = { 0.0f };

		WindowStats() = default;
	};

	struct WindowProps
	{
		std::string Title;
		uint32_t Width;
		uint32_t Height;

		WindowProps(const std::string& title = "Teddy Engine",
					uint32_t width = 1920,
					uint32_t height = 1080)
			: Title(title), Width(width), Height(height)
		{
		}
	};

	// Interface representing a desktop system based Window
	class Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;
		
		virtual ~Window() = default;

		virtual void OnUpdate(Timestep ts) = 0;
		virtual void SDLEvents() = 0;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		virtual void SetWidth(int width) = 0;
		virtual void SetHeight(int height) = 0;

		// Window attributes
		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		virtual void* GetNativeWindow() const = 0;
		virtual void* GetNativeContext() const = 0;

		virtual unsigned int GetThreadCount() const = 0;

		WindowStats GetStats() { return m_Stats; }

		virtual void CalculateFPS(Timestep ts) = 0;

		static Scope<Window> Create(const WindowProps& props = WindowProps());

	protected:
		WindowStats m_Stats;
	};
}