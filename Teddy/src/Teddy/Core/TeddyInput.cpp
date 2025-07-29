#include "teddyPch.h"
#include "Teddy/Core/TeddyInput.h"

#include "Teddy/Core/Application.h"
#include <SDL3/SDL.h>

#include "Teddy/Core/CodeConverter.h"

namespace Teddy
{
	Scope<Input> Input::s_Instance = CreateScope<TeddyInput>();

	void TeddyInput::OnEventImpl(Event& e) {
		EventDispatcher dispatcher(e);
		
		dispatcher.Dispatch<MidiKeyPressedEvent>(TED_BIND_EVENT_FN(TeddyInput::OnMidiKeyPressed));
		dispatcher.Dispatch<MidiKeyReleasedEvent>(TED_BIND_EVENT_FN(TeddyInput::OnMidiKeyReleased));
		dispatcher.Dispatch<MidiPedalPressedEvent>(TED_BIND_EVENT_FN(TeddyInput::OnMidiPedalPressed));
		dispatcher.Dispatch<MidiPedalReleasedEvent>(TED_BIND_EVENT_FN(TeddyInput::OnMidiPedalReleased));
	}

	bool TeddyInput::OnMidiKeyPressed(MidiKeyPressedEvent &e) {
		m_MidiKeysPressed[e.GetKey()] = true;
		return false;
	}

	bool TeddyInput::OnMidiKeyReleased(MidiKeyReleasedEvent& e) {
		m_MidiKeysPressed[e.GetKey()] = false;
		return false;
	}

	bool TeddyInput::OnMidiPedalPressed(MidiPedalPressedEvent& e) {
		m_MidiPedal = true;
		return false;
	}

	bool TeddyInput::OnMidiPedalReleased(MidiPedalReleasedEvent& e) {
		m_MidiPedal = false;
		return false;
	}

	bool TeddyInput::IsKeyPressedImpl(int keycode) {
		auto window = static_cast<SDL_Window*>(Application::Get().GetWindow().GetNativeWindow());
		const bool* state = SDL_GetKeyboardState(nullptr);
		return state[SDL_GetScancodeFromKey(TeddyToSDLKey(keycode), nullptr)];
	}

	bool TeddyInput::IsMouseButtonPressedImpl(int button) {
		Uint32 buttons = SDL_GetMouseState(nullptr, nullptr);
		return (buttons & (1 << ((TeddyToSDLMouse(button)) - 1))) != 0; // from definition of SDL_BUTTON
	}

	bool TeddyInput::IsMidiKeyPressedImpl(int keycode) {
		if (m_MidiKeysPressed.count(keycode)) return m_MidiKeysPressed[keycode];
		else return false;
	}

	bool TeddyInput::IsMidiPedalPressedImpl() {
		return m_MidiPedal;
	}

	std::pair<float, float> TeddyInput::GetMousePositionImpl() {

		float x, y;
		SDL_GetMouseState(&x, &y);

		return { x, y };
	}

	float TeddyInput::GetMouseXImpl() {
		auto [x, y] = GetMousePositionImpl();
		return x;
	}

	float TeddyInput::GetMouseYImpl() {
		auto [x, y] = GetMousePositionImpl();
		return y;
	}
}