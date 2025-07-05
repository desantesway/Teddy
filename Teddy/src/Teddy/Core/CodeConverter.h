#pragma once

namespace Teddy {

	int SDLToTeddyKey(int keycode);
	int SDLToTeddyMouse(int keycode);
	int TeddyToSDLKey(int keycode);
	int TeddyToSDLMouse(int keycode);

	int TeddyToImGuiKey(int keycode);
	int TeddyToImGuiMouse(int keycode);

	int TeddyToRtmidi(int keycode);
	int RtmidiToTeddy(int keycode);

	std::string KeyToNote(unsigned int key);
}