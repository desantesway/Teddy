#pragma once
#include "TeddyPch.h"

#include "Teddy/Core/Window.h"

#include <SDL3/SDL.h>

#include "Teddy/Renderer/GraphicsContext.h"
#include "Teddy/Renderer/GraphicsWindow.h"

namespace Teddy 
{

	class SDLWindow : public Window
	{
	public:
		SDLWindow(const WindowProps& props);
		virtual ~SDLWindow();

		void OnUpdate() override;
		virtual void SDLEvents() override;

		unsigned int GetWidth() const override { return m_Data.Width; }
		unsigned int GetHeight() const override { return m_Data.Height; }

		void SetWidth(int width) override { m_Data.Width = width; }
		void SetHeight(int height) override { m_Data.Height = height; }

		virtual void* GetNativeWindow() const override  { return m_Window->GetWindow(); }
		virtual void* GetNativeContext() const override { return m_Context->GetContext(); }

		// Window attributes
		void SetEventCallback(const EventCallbackFn& callback) override;

		void SetVSync(bool enabled) override;
		bool IsVSync() const override;
	private:
		virtual void Init(const WindowProps& props);
		virtual void Shutdown();
	private:

		Scope<GraphicsWindow> m_Window;
		Scope<GraphicsContext> m_Context;

		struct WindowData
		{
			std::string Title;
			unsigned int Width, Height;
			bool VSync;

			EventCallbackFn EventCallback;
			MidiDriver MidiDriver;
		};

		WindowData m_Data;
	};

}