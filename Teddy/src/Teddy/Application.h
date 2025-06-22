#pragma once

#include "Core.h"
#include "Window.h"

namespace Teddy{

	class TED_API Application
	{
	public:
			Application();
			virtual ~Application();

			void Run();
	private:
		std::unique_ptr<Window> SDL_Window;
		bool isRunning = true;
	};

	// to be defined in CLIENT
	Application* CreateApplication();
}