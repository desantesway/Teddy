#pragma once
#include "Teddy/Core/Core.h"

#define SDL_MAIN_HANDLED
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#ifdef TED_PLATFORM_WINDOWS

extern Teddy::Application* Teddy::CreateApplication();

	#ifdef TED_DIST
		int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
			LPSTR lpCmdLine, int nCmdShow) 
		{
			(void)hInstance; (void)hPrevInstance;
			(void)lpCmdLine; (void)nCmdShow;

			SDL_SetMainReady();

			Teddy::Log::Init();

			TED_PROFILE_BEGIN_SESSION("Startup", "debug/TeddyProfile-Startup.json");
			auto app = Teddy::CreateApplication();
			TED_PROFILE_END_SESSION();

			TED_PROFILE_BEGIN_SESSION("Runtime", "debug/TeddyProfile-Runtime.json");
			app->Run();
			TED_PROFILE_END_SESSION();

			TED_PROFILE_BEGIN_SESSION("Shutdown", "debug/TeddyProfile-Shutdown.json");
			delete app;
			TED_PROFILE_END_SESSION();

			return 0;
		}
	#else
		int main(int argc, char** argv) 
		{

			Teddy::Log::Init();

			SDL_SetMainReady();

			TED_PROFILE_BEGIN_SESSION("Startup", "debug/TeddyProfile-Startup.json");
			auto app = Teddy::CreateApplication();
			TED_PROFILE_END_SESSION();

			TED_PROFILE_BEGIN_SESSION("Runtime", "debug/TeddyProfile-Runtime.json");
			app->Run();
			TED_PROFILE_END_SESSION();

			TED_PROFILE_BEGIN_SESSION("Shutdown", "debug/TeddyProfile-Shutdown.json");
			delete app;
			TED_PROFILE_END_SESSION();

			return 0;
		}
	#endif

#endif
