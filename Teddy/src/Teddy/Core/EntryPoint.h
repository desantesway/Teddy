#pragma once
#include "Teddy/Core/Core.h"

#define SDL_MAIN_HANDLED
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#ifdef TED_PLATFORM_WINDOWS

extern Teddy::Application* Teddy::CreateApplication();

	#ifdef TED_DIST
		int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
			LPSTR lpCmdLine, int nCmdShow) {
			(void)hInstance; (void)hPrevInstance;
			(void)lpCmdLine; (void)nCmdShow;

			SDL_SetMainReady();

			Teddy::Log::Init();
			TED_CORE_WARN("Teddy Engine Initialized!");

			auto app = Teddy::CreateApplication();
			app->Run();
			delete app;

			return 0;
		}
	#else
		int main(int argc, char** argv) {

			Teddy::Log::Init();
			TED_CORE_WARN("Teddy Engine Initialized!");

			SDL_SetMainReady();

			auto app = Teddy::CreateApplication();
			app->Run();
			delete app;

			return 0;
		}
	#endif

#endif
