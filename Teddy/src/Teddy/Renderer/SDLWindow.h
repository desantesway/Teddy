#pragma once
#include "teddyPch.h"

#include "Teddy/Core/Window.h"

#include <SDL3/SDL.h>

#include "Teddy/Renderer/GraphicsContext.h"
#include "Teddy/Renderer/GraphicsWindow.h"

namespace Teddy {

	class SDLWindow : public Window
	{
	public:
		SDLWindow(const WindowProps& props);
		virtual ~SDLWindow();

		void OnUpdate() override;
		virtual void SDLEvents() override;

		inline unsigned int GetWidth() const override { return m_Data.Width; }
		inline unsigned int GetHeight() const override { return m_Data.Height; }

		inline void SetWidth(int width) override { m_Data.Width = width; }
		inline void SetHeight(int height) override { m_Data.Height = height; }

		inline virtual void* GetNativeWindow() const override  { return m_Window->GetWindow(); }
		inline virtual void* GetNativeContext() const override { return m_Context->GetContext(); }

		// Window attributes
		inline void SetEventCallback(const EventCallbackFn& callback) override;

		void SetVSync(bool enabled) override;
		bool IsVSync() const override;
	private:
		virtual void Init(const WindowProps& props);
		virtual void Shutdown();
	private:

		Scope<GraphicsWindow> m_Window;
		Scope<GraphicsContext> m_Context;
		MidiDriver m_MidiDriver;

		struct WindowData
		{
			std::string Title;
			unsigned int Width, Height;
			bool VSync;
		};

		WindowData m_Data;

		EventCallbackFn EventCallback;
	};

}