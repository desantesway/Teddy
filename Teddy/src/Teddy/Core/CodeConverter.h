#pragma once

#include "Teddy/Core/KeyCodes.h"
#include "Teddy/Core/MouseCodes.h"
#include "Teddy/Core/MidiCodes.h"

namespace Teddy {

	KeyCode SDLToTeddyKey(uint32_t keycode);
	MouseCode SDLToTeddyMouse(uint32_t keycode);
	uint32_t TeddyToSDLKey(KeyCode keycode);
	uint32_t TeddyToSDLMouse(MouseCode keycode);

	uint32_t TeddyToImGuiKey(KeyCode keycode);
	uint32_t TeddyToImGuiMouse(MouseCode keycode);

	uint32_t TeddyToRtmidi(MidiCode keycode);
	MidiCode RtmidiToTeddy(uint32_t keycode);

	std::string KeyToNote(MidiCode key);
}