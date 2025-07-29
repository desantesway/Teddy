#pragma once	

#include "teddyPch.h"
#include "Teddy/Core/Window.h"

#include "Teddy/Events/ApplicationEvent.h"
#include "Teddy/Events/KeyEvent.h"
#include "Teddy/Events/MouseEvent.h"

#include "Teddy/Core/CodeConverter.h"

namespace Teddy {

	void Window::SDLEvents() {
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
				KeyPressedEvent e(SDLToTeddyKey(event.key.key), event.key.repeat);
				EventCallback(e);
				break;
			} case  SDL_EVENT_KEY_UP: {
				KeyReleasedEvent e(SDLToTeddyKey(event.key.key));
				EventCallback(e);
				break;
			}case SDL_EVENT_MOUSE_BUTTON_DOWN: {
				int button = SDLToTeddyMouse(event.button.button);
				TED_CORE_ASSERT(button != -1, "Unknown mouse button pressed");
				MouseButtonPressedEvent e(button);
				EventCallback(e);
				break;
			} case SDL_EVENT_MOUSE_BUTTON_UP: {
				int button = SDLToTeddyMouse(event.button.button);
				TED_CORE_ASSERT(button != -1, "Unknown mouse button pressed");
				MouseButtonReleasedEvent e(button);
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
			}case SDL_EVENT_TEXT_INPUT: {
				KeyTypedEvent e(event.text.text);
				EventCallback(e);
				break;
			}default:
				break;
			}
		}
	}
}
