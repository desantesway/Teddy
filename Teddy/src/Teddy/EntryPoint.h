#pragma once

#ifdef TED_PLATFORM_WINDOWS

extern Teddy::Application* Teddy::CreateApplication();

	/*#ifdef defined(TED_PLATFORM_WINDOWS) && (defined(TED_RELEASE) || defined(TED_DIST))
		int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
			LPSTR lpCmdLine, int nCmdShow) {
			(void)hInstance; (void)hPrevInstance;
			(void)lpCmdLine; (void)nCmdShow;

			//SDL_SetMainReady();

			Teddy::Log::Init();
			TED_CORE_WARN("Teddy Engine Initialized!");

			auto app = Teddy::CreateApplication();
			app->Run();
			delete app;

			return 0;
		}
	#else*/
		int main(int argc, char** argv) {

			Teddy::Log::Init();
			TED_CORE_WARN("Teddy Engine Initialized!");

			auto app = Teddy::CreateApplication();
			app->Run();
			delete app;

			return 0;
		}
	//#endif

#endif
