#include "teddy_pch.h"
#include "Application.h"

#include "Teddy/Events/ApplicationEvent.h"
#include "Teddy/Events/MouseEvent.h"
#include "Teddy/Log.h"

#include <SDL3/SDL.h>
#include <glad/glad.h>

namespace Teddy {

	Application::Application(){
		SDL_Window = std::unique_ptr<Window>(new Window());
	}
	Application::~Application(){
	
	}

	void Application::Run() {
		while (SDL_Running)
		{
			glClearColor(1, 0, 1, 1);
			glClear(GL_COLOR_BUFFER_BIT);
			SDL_Window->OnUpdate();
		}
	}
}