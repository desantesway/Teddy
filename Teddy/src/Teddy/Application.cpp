#include "teddy_pch.h"
#include "Application.h"

#include "Teddy/Log.h"

#include <SDL3/SDL.h>
#include <glad/glad.h>

namespace Teddy {

	#define BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

	Application::Application(){
		SDL_Window = std::unique_ptr<Window>(new Window());
	}
	Application::~Application(){
	
	}

	void Application::Run() {
		while (isRunning)
		{	
			SDL_Window->PollEvents();
			
			glClearColor(1, 0, 1, 1);
			glClear(GL_COLOR_BUFFER_BIT);
			
			SDL_Window->OnUpdate();
			isRunning = SDL_Window->IsRunning();
		}

		SDL_Window.reset();
	}
}