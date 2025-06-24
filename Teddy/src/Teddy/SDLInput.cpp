#include "teddyPch.h"
#include "SDLInput.h"

#include "Teddy/Application.h"
#include <SDL3/SDL.h>

namespace Teddy
{
	Input* Input::s_Instance = new SDLInput();

	bool SDLInput::IsKeyPressedImpl(int keycode) { // SHIFT!!!
		auto window = static_cast<SDL_Window*>(Application::Get().GetWindow().GetNativeWindow());
		const bool* state = SDL_GetKeyboardState(nullptr);
		return state[SDL_GetScancodeFromKey(keycode, nullptr)];
	}

    bool SDLInput::IsMouseButtonPressedImpl(int button) {
		Uint32 buttons = SDL_GetMouseState(NULL, NULL);
		return buttons&button;
    }

	std::pair<float, float> SDLInput::GetMousePositionImpl() {
		
		float x, y;
		SDL_GetMouseState(&x,&y);

		return { x, y };
	}

	float SDLInput::GetMouseXImpl() {
		auto [x, y] = GetMousePositionImpl();
		return x;
	}

	float SDLInput::GetMouseYImpl() {
		auto [x, y] = GetMousePositionImpl();
		return y;
	}
}