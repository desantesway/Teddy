#pragma once

#ifdef TED_PLATFORM_WINDOWS

extern Teddy::Application* Teddy::CreateApplication();

int main(int argc, char** argv) {
	printf("Bomboclaat");
	auto app = Teddy::CreateApplication();
	app->Run();
	delete app;

	return 0;
}

#endif
