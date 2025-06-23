#pragma once	

#include "teddy_pch.h"
#include "Teddy/Window.h"

#include "Teddy/Events/ApplicationEvent.h"
#include "Teddy/Events/KeyEvent.h"
#include "Teddy/Events/MouseEvent.h"

namespace Teddy {
	void Window::Events() {
        SDL_Event event;
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_EVENT_QUIT: {
				WindowCloseEvent e;
				EventCallback(e);
				break;
			}
			case SDL_EVENT_WINDOW_RESIZED: {
				WindowResizeEvent e(event.window.data1, event.window.data2);
				EventCallback(e);
				break;
			}case SDL_EVENT_KEY_DOWN: {
				KeyPressedEvent e(event.key.key, event.key.repeat);
				EventCallback(e);
				break;
			} case  SDL_EVENT_KEY_UP: {
				KeyReleasedEvent e(event.key.key);
				EventCallback(e);
				break;
			}case SDL_EVENT_MOUSE_BUTTON_DOWN: {
				MouseButtonPressedEvent e(event.button.button);
				EventCallback(e);
				break;
			} case SDL_EVENT_MOUSE_BUTTON_UP: {
				MouseButtonReleasedEvent e(event.button.button);
				EventCallback(e);
				break;
			} case SDL_EVENT_MOUSE_MOTION: {
				MouseMovedEvent e(event.motion.x, event.motion.y);
				EventCallback(e);
				break;
			} case SDL_EVENT_MOUSE_WHEEL: {
				MouseScrolledEvent e(event.wheel.x, event.wheel.y);
				EventCallback(e);
				break;
			}default:
				break;
			}
		}
	}
}
