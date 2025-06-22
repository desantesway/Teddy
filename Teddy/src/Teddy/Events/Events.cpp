#include "Teddy/Window.h"

namespace Teddy {

	void Window::MouseWheel(SDL_Event event) {
		TED_CORE_INFO("Mouse Wheel: {0}", event.wheel.y);
	}

	void Window::MouseMovement(SDL_Event event) {
		//TED_CORE_INFO("Mouse Moved: {0}, {1}", event.motion.x, event.motion.y);
	}

	void Window::MouseEvent(bool pressed, SDL_Event event) {
		switch (event.button.button) {
		case SDL_BUTTON_LEFT:
			TED_CORE_INFO("LEFT Mouse Button Pressed: {0}", pressed);
			break;
		case SDL_BUTTON_RIGHT:
			TED_CORE_INFO("RIGHT Mouse Button Pressed : {0}", pressed);
			break;
		case SDL_BUTTON_MIDDLE:
			TED_CORE_INFO("MIDDLE Mouse Button Pressed : {0}", pressed);
			break;
		}
		
	}

	void Window::KeyEvent(bool pressed, SDL_Event event) {
		switch (event.key.scancode) {
		case SDL_SCANCODE_ESCAPE:
			TED_CORE_INFO("Key Pressed: {0} ESC", pressed);
			break;
		}
	}

	void Window::PollEvents() {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_EVENT_QUIT: {
					isRunning = false;
					break;
				}
				case SDL_EVENT_WINDOW_RESIZED: {
					TED_CORE_INFO("Resized {0}x{1}", event.window.data1, event.window.data2);
					this->SetWidth(event.window.data1);
					this->SetHeight(event.window.data2);
					break;
				}case SDL_EVENT_KEY_DOWN: {
					KeyEvent(true, event);
					break;
				} case  SDL_EVENT_KEY_UP: {
					KeyEvent(false, event);
					break;

				}case SDL_EVENT_MOUSE_BUTTON_DOWN: {
					MouseEvent(true, event);
					break;
				} case SDL_EVENT_MOUSE_BUTTON_UP: {
					MouseEvent(false, event);
					break;
				} case SDL_EVENT_MOUSE_MOTION: {
					MouseMovement(event);
					break;
				} case SDL_EVENT_MOUSE_WHEEL: {
					MouseWheel(event);
					break;
				}default:
					break;
			}
		}
	}
}