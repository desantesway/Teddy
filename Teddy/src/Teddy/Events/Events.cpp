#include "Teddy/Window.h"

namespace Teddy {
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
			}

			}
		}
	}
}