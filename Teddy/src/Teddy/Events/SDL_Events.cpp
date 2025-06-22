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
				//KeyEvent e(event.key.scancode);
				//EventCallback(e);
				TED_CORE_INFO("Key pressed");
				break;
			} case  SDL_EVENT_KEY_UP: {
				TED_CORE_INFO("Key released");
				break;

			}case SDL_EVENT_MOUSE_BUTTON_DOWN: {
				//MouseButtonPressedEvent e(true, event);
				TED_CORE_INFO("Mouse pressed");
				break;
			} case SDL_EVENT_MOUSE_BUTTON_UP: {
				//MouseButtonReleasedEvent(false, event);
				TED_CORE_INFO("Mouse released");
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
