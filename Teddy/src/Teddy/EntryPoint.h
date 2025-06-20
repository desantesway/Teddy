#pragma once

#ifdef TED_PLATFORM_WINDOWS

extern Teddy::Application* Teddy::CreateApplication();

int main(int argc, char** argv) {

	Teddy::Log::Init();
	TED_CORE_WARN("Teddy Engine Initialized!");
	TED_INFO("Teddy Engine Initialized!");

	auto app = Teddy::CreateApplication();
	app->Run();
	delete app;

	return 0;
}

#endif
