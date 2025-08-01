#include "teddyPch.h"
#include "Teddy/Core/TeddyInput.h"

#include "Teddy/Core/Application.h"
#include <SDL3/SDL.h>

#include "Teddy/Core/CodeConverter.h"

namespace Teddy
{

	void TeddyInput::OnEventImpl(Event& e) 
	{
		EventDispatcher dispatcher(e);
		
		dispatcher.Dispatch<MidiKeyPressedEvent>(TED_BIND_EVENT_FN(TeddyInput::OnMidiKeyPressed));
		dispatcher.Dispatch<MidiKeyReleasedEvent>(TED_BIND_EVENT_FN(TeddyInput::OnMidiKeyReleased));
		dispatcher.Dispatch<MidiPedalPressedEvent>(TED_BIND_EVENT_FN(TeddyInput::OnMidiPedalPressed));
		dispatcher.Dispatch<MidiPedalReleasedEvent>(TED_BIND_EVENT_FN(TeddyInput::OnMidiPedalReleased));
	}

	bool TeddyInput::OnMidiKeyPressed(MidiKeyPressedEvent &e) 
	{
		m_MidiKeysPressed[static_cast<uint32_t>(e.GetKey())] = true;
		return false;
	}

	bool TeddyInput::OnMidiKeyReleased(MidiKeyReleasedEvent& e) 
	{
		m_MidiKeysPressed[static_cast<uint32_t>(e.GetKey())] = false;
		return false;
	}

	bool TeddyInput::OnMidiPedalPressed(MidiPedalPressedEvent& e) 
	{
		m_MidiPedal = true;
		return false;
	}

	bool TeddyInput::OnMidiPedalReleased(MidiPedalReleasedEvent& e) 
	{
		m_MidiPedal = false;
		return false;
	}

	bool TeddyInput::IsKeyPressedImpl(KeyCode keycode) 
	{
		auto window = static_cast<SDL_Window*>(Application::Get().GetWindow().GetNativeWindow());
		const bool* state = SDL_GetKeyboardState(nullptr);
		return state[SDL_GetScancodeFromKey(TeddyToSDLKey(keycode), nullptr)];
	}

	bool TeddyInput::IsMouseButtonPressedImpl(MouseCode button) 
	{
		Uint32 buttons = SDL_GetMouseState(nullptr, nullptr);
		return (buttons & (1 << ((TeddyToSDLMouse(button)) - 1))) != 0; // from definition of SDL_BUTTON
	}

	bool TeddyInput::IsMidiKeyPressedImpl(MidiCode keycode) 
	{
		if (m_MidiKeysPressed.count(static_cast<uint32_t>(keycode))) 
			return m_MidiKeysPressed[static_cast<uint32_t>(keycode)];
		else return false;
	}

	bool TeddyInput::IsMidiPedalPressedImpl() 
	{
		return m_MidiPedal;
	}

	std::pair<float, float> TeddyInput::GetMousePositionImpl() 
	{
		float x, y;
		SDL_GetMouseState(&x, &y);

		return { x, y };
	}

	float TeddyInput::GetMouseXImpl() 
	{
		auto [x, y] = GetMousePositionImpl();
		return x;
	}

	float TeddyInput::GetMouseYImpl() 
	{
		auto [x, y] = GetMousePositionImpl();
		return y;
	}
}