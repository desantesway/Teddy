#pragma once
#include "teddyPch.h"

#include "Teddy/Window.h"

#include <SDL3/SDL.h>
#include <glad/glad.h>	

namespace Teddy {

	class OpenGLWindow : public Window
	{
	public:
		OpenGLWindow(const WindowProps& props);
		virtual ~OpenGLWindow();

		void OnUpdate() override;
		

		inline unsigned int GetWidth() const override { return m_Data.Width; }
		inline unsigned int GetHeight() const override { return m_Data.Height; }

		inline virtual void* GetNativeWindow() const override  { return m_Window; }
		inline virtual void* GetNativeContext() const override { return m_Context; }

		// Window attributes
		inline void SetEventCallback(const EventCallbackFn& callback) override {EventCallback = callback; }
		void SetVSync(bool enabled) override;
		bool IsVSync() const override;
	private:
		virtual void Init(const WindowProps& props);
		virtual void Shutdown();
	private:

		SDL_Window* m_Window;
		SDL_GLContext m_Context;

		struct WindowData
		{
			std::string Title;
			unsigned int Width, Height;
			bool VSync;
		};

		WindowData m_Data;
	};

}