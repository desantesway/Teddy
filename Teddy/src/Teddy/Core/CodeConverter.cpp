#include "CodeConverter.h"

#include <SDL3/SDL.h>
#include <imgui.h>

namespace Teddy {
	
    KeyCode SDLToTeddyKey(uint32_t keycode) {
		return (KeyCode)keycode;
	}

    uint32_t TeddyToSDLKey(KeyCode keycode) {
		return (uint32_t)keycode;
	}

    MouseCode SDLToTeddyMouse(uint32_t keycode){
		if (keycode == SDL_BUTTON_LEFT) return      MouseCode::Left;
		if (keycode == SDL_BUTTON_RIGHT) return     MouseCode::Right;
		if (keycode == SDL_BUTTON_MIDDLE) return    MouseCode::Middle;
		if (keycode == SDL_BUTTON_X1) return        MouseCode::X1;
		if (keycode == SDL_BUTTON_X2) return        MouseCode::X2;
		return MouseCode::Unknown;
	}
	
    uint32_t TeddyToSDLMouse(MouseCode keycode){
		if (keycode == MouseCode::Left)     return SDL_BUTTON_LEFT;
		if (keycode == MouseCode::Right)    return SDL_BUTTON_RIGHT;
		if (keycode == MouseCode::Middle)   return SDL_BUTTON_MIDDLE;
		if (keycode == MouseCode::X1)       return SDL_BUTTON_X1;
		if (keycode == MouseCode::X2)       return SDL_BUTTON_X2;
		return -1;
	}

    uint32_t TeddyToImGuiKey(KeyCode keycode){
        switch (keycode)
        {
        case KeyCode::Tab: return ImGuiKey_Tab;
        case KeyCode::Left: return ImGuiKey_LeftArrow;
        case KeyCode::Right: return ImGuiKey_RightArrow;
        case KeyCode::Up: return ImGuiKey_UpArrow;
        case KeyCode::Down: return ImGuiKey_DownArrow;
        case KeyCode::PageUp: return ImGuiKey_PageUp;
        case KeyCode::PageDown: return ImGuiKey_PageDown;
        case KeyCode::Home: return ImGuiKey_Home;
        case KeyCode::End: return ImGuiKey_End;
        case KeyCode::Insert: return ImGuiKey_Insert;
        case KeyCode::Delete: return ImGuiKey_Delete;
        case KeyCode::Backspace: return ImGuiKey_Backspace;
        case KeyCode::Space: return ImGuiKey_Space;
        case KeyCode::Return: return ImGuiKey_Enter;
        case KeyCode::Escape: return ImGuiKey_Escape;
        case KeyCode::Apostrophe: return ImGuiKey_Apostrophe;
        case KeyCode::Comma: return ImGuiKey_Comma;
        case KeyCode::Minus: return ImGuiKey_Minus;
        case KeyCode::Period: return ImGuiKey_Period;
        case KeyCode::Slash: return ImGuiKey_Slash;
        case KeyCode::Semicolon: return ImGuiKey_Semicolon;
        case KeyCode::Equals: return ImGuiKey_Equal;
        case KeyCode::LeftBracket: return ImGuiKey_LeftBracket;
        case KeyCode::Backslash: return ImGuiKey_Backslash;
        case KeyCode::RightBracket: return ImGuiKey_RightBracket;
        case KeyCode::Grave: return ImGuiKey_GraveAccent;
        case KeyCode::CapsLock: return ImGuiKey_CapsLock;
        case KeyCode::ScrollLock: return ImGuiKey_ScrollLock;
        case KeyCode::NumLockClear: return ImGuiKey_NumLock;
        case KeyCode::PrintScreen: return ImGuiKey_PrintScreen;
        case KeyCode::Pause: return ImGuiKey_Pause;
        case KeyCode::LCtrl: return ImGuiKey_LeftCtrl;
        case KeyCode::LShift: return ImGuiKey_LeftShift;
        case KeyCode::LAlt: return ImGuiKey_LeftAlt;
        case KeyCode::LGui: return ImGuiKey_LeftSuper;
        case KeyCode::RCtrl: return ImGuiKey_RightCtrl;
        case KeyCode::RShift: return ImGuiKey_RightShift;
        case KeyCode::RAlt: return ImGuiKey_RightAlt;
        case KeyCode::RGui: return ImGuiKey_RightSuper;
        case KeyCode::Application: return ImGuiKey_Menu;
        case KeyCode::D0: return ImGuiKey_0;
        case KeyCode::D1: return ImGuiKey_1;
        case KeyCode::D2: return ImGuiKey_2;
        case KeyCode::D3: return ImGuiKey_3;
        case KeyCode::D4: return ImGuiKey_4;
        case KeyCode::D5: return ImGuiKey_5;
        case KeyCode::D6: return ImGuiKey_6;
        case KeyCode::D7: return ImGuiKey_7;
        case KeyCode::D8: return ImGuiKey_8;
        case KeyCode::D9: return ImGuiKey_9;
        case KeyCode::A: return ImGuiKey_A;
        case KeyCode::B: return ImGuiKey_B;
        case KeyCode::C: return ImGuiKey_C;
        case KeyCode::D: return ImGuiKey_D;
        case KeyCode::E: return ImGuiKey_E;
        case KeyCode::F: return ImGuiKey_F;
        case KeyCode::G: return ImGuiKey_G;
        case KeyCode::H: return ImGuiKey_H;
        case KeyCode::I: return ImGuiKey_I;
        case KeyCode::J: return ImGuiKey_J;
        case KeyCode::K: return ImGuiKey_K;
        case KeyCode::L: return ImGuiKey_L;
        case KeyCode::M: return ImGuiKey_M;
        case KeyCode::N: return ImGuiKey_N;
        case KeyCode::O: return ImGuiKey_O;
        case KeyCode::P: return ImGuiKey_P;
        case KeyCode::Q: return ImGuiKey_Q;
        case KeyCode::R: return ImGuiKey_R;
        case KeyCode::S: return ImGuiKey_S;
        case KeyCode::T: return ImGuiKey_T;
        case KeyCode::U: return ImGuiKey_U;
        case KeyCode::V: return ImGuiKey_V;
        case KeyCode::W: return ImGuiKey_W;
        case KeyCode::X: return ImGuiKey_X;
        case KeyCode::Y: return ImGuiKey_Y;
        case KeyCode::Z: return ImGuiKey_Z;
        case KeyCode::F1: return ImGuiKey_F1;
        case KeyCode::F2: return ImGuiKey_F2;
        case KeyCode::F3: return ImGuiKey_F3;
        case KeyCode::F4: return ImGuiKey_F4;
        case KeyCode::F5: return ImGuiKey_F5;
        case KeyCode::F6: return ImGuiKey_F6;
        case KeyCode::F7: return ImGuiKey_F7;
        case KeyCode::F8: return ImGuiKey_F8;
        case KeyCode::F9: return ImGuiKey_F9;
        case KeyCode::F10: return ImGuiKey_F10;
        case KeyCode::F11: return ImGuiKey_F11;
        case KeyCode::F12: return ImGuiKey_F12;
        case KeyCode::F13: return ImGuiKey_F13;
        case KeyCode::F14: return ImGuiKey_F14;
        case KeyCode::F15: return ImGuiKey_F15;
        case KeyCode::F16: return ImGuiKey_F16;
        case KeyCode::F17: return ImGuiKey_F17;
        case KeyCode::F18: return ImGuiKey_F18;
        case KeyCode::F19: return ImGuiKey_F19;
        case KeyCode::F20: return ImGuiKey_F20;
        case KeyCode::F21: return ImGuiKey_F21;
        case KeyCode::F22: return ImGuiKey_F22;
        case KeyCode::F23: return ImGuiKey_F23;
        case KeyCode::F24: return ImGuiKey_F24;
        case KeyCode::AcBack: return ImGuiKey_AppBack;
        case KeyCode::AcForward: return ImGuiKey_AppForward;
        default: break;
        }
        return ImGuiKey_None;
	}

    uint32_t TeddyToImGuiMouse(MouseCode keycode){
		if (keycode == MouseCode::Left)     return 0;
		if (keycode == MouseCode::Right)    return 1;
		if (keycode == MouseCode::Middle)   return 2;
		if (keycode == MouseCode::X1)       return 3;
		if (keycode == MouseCode::X2)       return 4;
		return -1;
	}

    uint32_t TeddyToRtmidi(MidiCode keycode) {
        // Teddy MIDI note codes are defined to match the MIDI note numbers (0-127)
        // If the keycode is in the valid MIDI note range, return it as RtMidi code
        if (static_cast<uint32_t>(keycode) >= 0x00 && static_cast<uint32_t>(keycode) <= 0x77) {
            return static_cast<uint32_t>(keycode);
        }
        // Special case for pedal (sustain) control, if you use TED_MIDI_PEDAL
        if (keycode == MidiCode::Pedal) {
            return 0x78;
        }
        // Not a valid MIDI note or mapped control
        return -1;
    }

    MidiCode RtmidiToTeddy(uint32_t keycode) {
        // Teddy MIDI note codes are defined to match the MIDI note numbers (0-127)
        // If the keycode is in the valid MIDI note range, return it as Teddy code
        if (keycode >= 0x00 && keycode <= 0x77) {
            return static_cast<MidiCode>(keycode);
        }
        // Special case for pedal (sustain) control, if you use 0x78 for TED_MIDI_PEDAL
        if (keycode == 0x78) {
            return MidiCode::Pedal;
        }
        // Not a valid MIDI note or mapped control
        return MidiCode::Unknown;
    }

    std::string KeyToNote(MidiCode key) {
        if (static_cast<uint32_t>(key) < 0 || static_cast<uint32_t>(key) > 120) {
            return "Invalid note number";
        }

        const char* note;
        switch (static_cast<uint32_t>(key) % 12) {
        case 0: note = "C"; break;
        case 1: note = "Db"; break;
        case 2: note = "D"; break;
        case 3: note = "Eb"; break;
        case 4: note = "E"; break;
        case 5: note = "F"; break;
        case 6: note = "Gb"; break;
        case 7: note = "G"; break;
        case 8: note = "Ab"; break;
        case 9: note = "A"; break;
        case 10: note = "Bb"; break;
        case 11: note = "B"; break;
        default: note = "?"; break;
        }

        int octave = static_cast<uint32_t>(key) / 12;
        return std::string(note) + std::to_string(octave);
    }
}