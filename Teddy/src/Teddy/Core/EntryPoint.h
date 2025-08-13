#pragma once
#include "Teddy/Core/Base.h"

#define SDL_MAIN_HANDLED
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#ifdef TED_PLATFORM_WINDOWS

extern Teddy::Application* Teddy::CreateApplication(ApplicationCommandLineArgs args);

	#ifdef TED_DIST
		int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
			LPSTR lpCmdLine, int nCmdShow) 
		{
			(void)hInstance; (void)hPrevInstance;
			(void)nCmdShow;

			SDL_SetMainReady();

			int argc;
			LPWSTR* szArglist = CommandLineToArgvW(GetCommandLineW(), &argc);

			// Convert to char** for compatibility
			char** argv = new char* [argc];
			for (int i = 0; i < argc; i++) {
				int len = WideCharToMultiByte(CP_UTF8, 0, szArglist[i], -1, nullptr, 0, nullptr, nullptr);
				argv[i] = new char[len];
				WideCharToMultiByte(CP_UTF8, 0, szArglist[i], -1, argv[i], len, nullptr, nullptr);
			}
			LocalFree(szArglist);

			Teddy::Log::Init();

			TED_PROFILE_BEGIN_SESSION("Startup", "debug/TeddyProfile-Startup.json");
			auto app = Teddy::CreateApplication({ argc, argv });
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
			auto app = Teddy::CreateApplication({ argc, argv });
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
