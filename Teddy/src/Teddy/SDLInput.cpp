#include "teddyPch.h"
#include "SDLInput.h"

#include "Teddy/Application.h"
#include <SDL3/SDL.h>

#include "Teddy/CodeConverter.h"

namespace Teddy
{
	Input* Input::s_Instance = new SDLInput();

	bool SDLInput::IsKeyPressedImpl(int keycode) { // SHIFT!!!
		auto window = static_cast<SDL_Window*>(Application::Get().GetWindow().GetNativeWindow());
		const bool* state = SDL_GetKeyboardState(nullptr);
		return state[SDL_GetScancodeFromKey(TeddyToSDLKey(keycode), nullptr)];
	}

	bool SDLInput::IsMouseButtonPressedImpl(int button) {
		Uint32 buttons = SDL_GetMouseState(nullptr, nullptr);
		return (buttons & (1 << ((TeddyToSDLMouse(button))-1))) != 0; // from definition of SDL_BUTTON
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