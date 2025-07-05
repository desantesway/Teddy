#include "CodeConverter.h"
#include "KeyCodes.h"
#include "MouseCodes.h"
#include "MidiCodes.h"

#include <SDL3/SDL.h>
#include <imgui.h>

namespace Teddy {
	
	int SDLToTeddyKey(int keycode) {
		return keycode;
	}

	int TeddyToSDLKey(int keycode) {
		return keycode;
	}

	int SDLToTeddyMouse(int keycode){
		if (keycode == SDL_BUTTON_LEFT) return TED_MOUSE_LEFT;
		if (keycode == SDL_BUTTON_RIGHT) return TED_MOUSE_RIGHT;
		if (keycode == SDL_BUTTON_MIDDLE) return TED_MOUSE_MIDDLE;
		if (keycode == SDL_BUTTON_X1) return TED_MOUSE_X1;
		if (keycode == SDL_BUTTON_X2) return TED_MOUSE_X2;
		return -1;
	}
	
	int TeddyToSDLMouse(int keycode){
		if (keycode == TED_MOUSE_LEFT) return SDL_BUTTON_LEFT;
		if (keycode == TED_MOUSE_RIGHT) return SDL_BUTTON_RIGHT;
		if (keycode == TED_MOUSE_MIDDLE) return SDL_BUTTON_MIDDLE;
		if (keycode == TED_MOUSE_X1) return SDL_BUTTON_X1;
		if (keycode == TED_MOUSE_X2) return SDL_BUTTON_X2;
		return -1;
	}

	int TeddyToImGuiKey(int keycode){
        switch (keycode)
        {
        case TED_KEY_TAB: return ImGuiKey_Tab;
        case TED_KEY_LEFT: return ImGuiKey_LeftArrow;
        case TED_KEY_RIGHT: return ImGuiKey_RightArrow;
        case TED_KEY_UP: return ImGuiKey_UpArrow;
        case TED_KEY_DOWN: return ImGuiKey_DownArrow;
        case TED_KEY_PAGEUP: return ImGuiKey_PageUp;
        case TED_KEY_PAGEDOWN: return ImGuiKey_PageDown;
        case TED_KEY_HOME: return ImGuiKey_Home;
        case TED_KEY_END: return ImGuiKey_End;
        case TED_KEY_INSERT: return ImGuiKey_Insert;
        case TED_KEY_DELETE: return ImGuiKey_Delete;
        case TED_KEY_BACKSPACE: return ImGuiKey_Backspace;
        case TED_KEY_SPACE: return ImGuiKey_Space;
        case TED_KEY_RETURN: return ImGuiKey_Enter;
        case TED_KEY_ESCAPE: return ImGuiKey_Escape;
        case TED_KEY_APOSTROPHE: return ImGuiKey_Apostrophe;
        case TED_KEY_COMMA: return ImGuiKey_Comma;
        case TED_KEY_MINUS: return ImGuiKey_Minus;
        case TED_KEY_PERIOD: return ImGuiKey_Period;
        case TED_KEY_SLASH: return ImGuiKey_Slash;
        case TED_KEY_SEMICOLON: return ImGuiKey_Semicolon;
        case TED_KEY_EQUALS: return ImGuiKey_Equal;
        case TED_KEY_LEFTBRACKET: return ImGuiKey_LeftBracket;
        case TED_KEY_BACKSLASH: return ImGuiKey_Backslash;
        case TED_KEY_RIGHTBRACKET: return ImGuiKey_RightBracket;
        case TED_KEY_GRAVE: return ImGuiKey_GraveAccent;
        case TED_KEY_CAPSLOCK: return ImGuiKey_CapsLock;
        case TED_KEY_SCROLLLOCK: return ImGuiKey_ScrollLock;
        case TED_KEY_NUMLOCKCLEAR: return ImGuiKey_NumLock;
        case TED_KEY_PRINTSCREEN: return ImGuiKey_PrintScreen;
        case TED_KEY_PAUSE: return ImGuiKey_Pause;
        case TED_KEY_LCTRL: return ImGuiKey_LeftCtrl;
        case TED_KEY_LSHIFT: return ImGuiKey_LeftShift;
        case TED_KEY_LALT: return ImGuiKey_LeftAlt;
        case TED_KEY_LGUI: return ImGuiKey_LeftSuper;
        case TED_KEY_RCTRL: return ImGuiKey_RightCtrl;
        case TED_KEY_RSHIFT: return ImGuiKey_RightShift;
        case TED_KEY_RALT: return ImGuiKey_RightAlt;
        case TED_KEY_RGUI: return ImGuiKey_RightSuper;
        case TED_KEY_APPLICATION: return ImGuiKey_Menu;
        case TED_KEY_0: return ImGuiKey_0;
        case TED_KEY_1: return ImGuiKey_1;
        case TED_KEY_2: return ImGuiKey_2;
        case TED_KEY_3: return ImGuiKey_3;
        case TED_KEY_4: return ImGuiKey_4;
        case TED_KEY_5: return ImGuiKey_5;
        case TED_KEY_6: return ImGuiKey_6;
        case TED_KEY_7: return ImGuiKey_7;
        case TED_KEY_8: return ImGuiKey_8;
        case TED_KEY_9: return ImGuiKey_9;
        case TED_KEY_A: return ImGuiKey_A;
        case TED_KEY_B: return ImGuiKey_B;
        case TED_KEY_C: return ImGuiKey_C;
        case TED_KEY_D: return ImGuiKey_D;
        case TED_KEY_E: return ImGuiKey_E;
        case TED_KEY_F: return ImGuiKey_F;
        case TED_KEY_G: return ImGuiKey_G;
        case TED_KEY_H: return ImGuiKey_H;
        case TED_KEY_I: return ImGuiKey_I;
        case TED_KEY_J: return ImGuiKey_J;
        case TED_KEY_K: return ImGuiKey_K;
        case TED_KEY_L: return ImGuiKey_L;
        case TED_KEY_M: return ImGuiKey_M;
        case TED_KEY_N: return ImGuiKey_N;
        case TED_KEY_O: return ImGuiKey_O;
        case TED_KEY_P: return ImGuiKey_P;
        case TED_KEY_Q: return ImGuiKey_Q;
        case TED_KEY_R: return ImGuiKey_R;
        case TED_KEY_S: return ImGuiKey_S;
        case TED_KEY_T: return ImGuiKey_T;
        case TED_KEY_U: return ImGuiKey_U;
        case TED_KEY_V: return ImGuiKey_V;
        case TED_KEY_W: return ImGuiKey_W;
        case TED_KEY_X: return ImGuiKey_X;
        case TED_KEY_Y: return ImGuiKey_Y;
        case TED_KEY_Z: return ImGuiKey_Z;
        case TED_KEY_F1: return ImGuiKey_F1;
        case TED_KEY_F2: return ImGuiKey_F2;
        case TED_KEY_F3: return ImGuiKey_F3;
        case TED_KEY_F4: return ImGuiKey_F4;
        case TED_KEY_F5: return ImGuiKey_F5;
        case TED_KEY_F6: return ImGuiKey_F6;
        case TED_KEY_F7: return ImGuiKey_F7;
        case TED_KEY_F8: return ImGuiKey_F8;
        case TED_KEY_F9: return ImGuiKey_F9;
        case TED_KEY_F10: return ImGuiKey_F10;
        case TED_KEY_F11: return ImGuiKey_F11;
        case TED_KEY_F12: return ImGuiKey_F12;
        case TED_KEY_F13: return ImGuiKey_F13;
        case TED_KEY_F14: return ImGuiKey_F14;
        case TED_KEY_F15: return ImGuiKey_F15;
        case TED_KEY_F16: return ImGuiKey_F16;
        case TED_KEY_F17: return ImGuiKey_F17;
        case TED_KEY_F18: return ImGuiKey_F18;
        case TED_KEY_F19: return ImGuiKey_F19;
        case TED_KEY_F20: return ImGuiKey_F20;
        case TED_KEY_F21: return ImGuiKey_F21;
        case TED_KEY_F22: return ImGuiKey_F22;
        case TED_KEY_F23: return ImGuiKey_F23;
        case TED_KEY_F24: return ImGuiKey_F24;
        case TED_KEY_AC_BACK: return ImGuiKey_AppBack;
        case TED_KEY_AC_FORWARD: return ImGuiKey_AppForward;
        default: break;
        }
        return ImGuiKey_None;
	}

	int TeddyToImGuiMouse(int keycode){
		if (keycode == TED_MOUSE_LEFT) return 0;
		if (keycode == TED_MOUSE_RIGHT) return 1;
		if (keycode == TED_MOUSE_MIDDLE) return 2;
		if (keycode == TED_MOUSE_X1) return 3;
		if (keycode == TED_MOUSE_X2) return 4;
		return -1;
	}

    int TeddyToRtmidi(int keycode) {
        // Teddy MIDI note codes are defined to match the MIDI note numbers (0-127)
        // If the keycode is in the valid MIDI note range, return it as RtMidi code
        if (keycode >= 0x00 && keycode <= 0x77) {
            return keycode;
        }
        // Special case for pedal (sustain) control, if you use TED_MIDI_PEDAL
        if (keycode == TED_MIDI_PEDAL) {
            return 0x78;
        }
        // Not a valid MIDI note or mapped control
        return 999;
    }

    int RtmidiToTeddy(int keycode) {
        // Teddy MIDI note codes are defined to match the MIDI note numbers (0-127)
        // If the keycode is in the valid MIDI note range, return it as Teddy code
        if (keycode >= 0x00 && keycode <= 0x77) {
            return keycode;
        }
        // Special case for pedal (sustain) control, if you use 0x78 for TED_MIDI_PEDAL
        if (keycode == 0x78) {
            return TED_MIDI_PEDAL;
        }
        // Not a valid MIDI note or mapped control
        return 999;
    }

    std::string KeyToNote(unsigned int key) {
        if (key < 0 || key > 119) {
            return "Invalid note number";
        }

        const char* note;
        switch (key % 12) {
        case 0: note = "C"; break;
        case 1: note = "C#"; break;
        case 2: note = "D"; break;
        case 3: note = "D#"; break;
        case 4: note = "E"; break;
        case 5: note = "F"; break;
        case 6: note = "F#"; break;
        case 7: note = "G"; break;
        case 8: note = "G#"; break;
        case 9: note = "A"; break;
        case 10: note = "A#"; break;
        case 11: note = "B"; break;
        default: note = "?"; break;
        }

        int octave = key / 12;
        return std::string(note) + std::to_string(octave);
    }
}