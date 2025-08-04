#include "teddyPch.h"
#include "Teddy/Core/Input.h"

#include "Teddy/Core/Application.h"
#include <SDL3/SDL.h>

#include "Teddy/Core/CodeConverter.h"

namespace Teddy
{

	bool m_MidiPedal = false;
	std::unordered_map<int, bool> m_MidiKeysPressed;

	void Input::OnEvent(Event& e) 
	{
		EventDispatcher dispatcher(e);
		
		dispatcher.Dispatch<MidiKeyPressedEvent>(TED_BIND_EVENT_FN(Input::OnMidiKeyPressed));
		dispatcher.Dispatch<MidiKeyReleasedEvent>(TED_BIND_EVENT_FN(Input::OnMidiKeyReleased));
		dispatcher.Dispatch<MidiPedalPressedEvent>(TED_BIND_EVENT_FN(Input::OnMidiPedalPressed));
		dispatcher.Dispatch<MidiPedalReleasedEvent>(TED_BIND_EVENT_FN(Input::OnMidiPedalReleased));
	}

	bool Input::OnMidiKeyPressed(MidiKeyPressedEvent &e) 
	{
		m_MidiKeysPressed[static_cast<uint32_t>(e.GetKey())] = true;
		return false;
	}

	bool Input::OnMidiKeyReleased(MidiKeyReleasedEvent& e) 
	{
		m_MidiKeysPressed[static_cast<uint32_t>(e.GetKey())] = false;
		return false;
	}

	bool Input::OnMidiPedalPressed(MidiPedalPressedEvent& e) 
	{
		m_MidiPedal = true;
		return false;
	}

	bool Input::OnMidiPedalReleased(MidiPedalReleasedEvent& e) 
	{
		m_MidiPedal = false;
		return false;
	}

	bool Input::IsKeyPressed(KeyCode keycode) 
	{
		auto window = static_cast<SDL_Window*>(Application::Get().GetWindow().GetNativeWindow());
		const bool* state = SDL_GetKeyboardState(nullptr);
		return state[SDL_GetScancodeFromKey(TeddyToSDLKey(keycode), nullptr)];
	}

	bool Input::IsMouseButtonPressed(MouseCode button) 
	{
		Uint32 buttons = SDL_GetMouseState(nullptr, nullptr);
		return (buttons & (1 << ((TeddyToSDLMouse(button)) - 1))) != 0; // from definition of SDL_BUTTON
	}

	bool Input::IsMidiKeyPressed(MidiCode keycode) 
	{
		if (m_MidiKeysPressed.count(static_cast<uint32_t>(keycode))) 
			return m_MidiKeysPressed[static_cast<uint32_t>(keycode)];
		else return false;
	}

	bool Input::IsMidiPedalPressed() 
	{
		return m_MidiPedal;
	}

	std::pair<float, float> Input::GetMousePosition() 
	{
		float x, y;
		SDL_GetMouseState(&x, &y);

		return { x, y };
	}

	float Input::GetMouseX() 
	{
		auto [x, y] = GetMousePosition();
		return x;
	}

	float Input::GetMouseY() 
	{
		auto [x, y] = GetMousePosition();
		return y;
	}
}