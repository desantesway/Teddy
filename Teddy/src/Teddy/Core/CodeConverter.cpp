#include "CodeConverter.h"

#include <SDL3/SDL.h>
#include <imgui.h>

namespace Teddy 
{
	
    KeyCode SDLToTeddyKey(uint32_t keycode) 
    {
		return (KeyCode)keycode;
	}

    uint32_t TeddyToSDLKey(KeyCode keycode) 
    {
		return (uint32_t)keycode;
	}

    MouseCode SDLToTeddyMouse(uint32_t keycode)
    {
		if (keycode == SDL_BUTTON_LEFT) return      Mouse::Left;
		if (keycode == SDL_BUTTON_RIGHT) return     Mouse::Right;
		if (keycode == SDL_BUTTON_MIDDLE) return    Mouse::Middle;
		if (keycode == SDL_BUTTON_X1) return        Mouse::X1;
		if (keycode == SDL_BUTTON_X2) return        Mouse::X2;
		return Mouse::Unknown;
	}
	
    uint32_t TeddyToSDLMouse(MouseCode keycode)
    {
		if (keycode == Mouse::Left)     return SDL_BUTTON_LEFT;
		if (keycode == Mouse::Right)    return SDL_BUTTON_RIGHT;
		if (keycode == Mouse::Middle)   return SDL_BUTTON_MIDDLE;
		if (keycode == Mouse::X1)       return SDL_BUTTON_X1;
		if (keycode == Mouse::X2)       return SDL_BUTTON_X2;
		return -1;
	}

    uint32_t TeddyToImGuiKey(KeyCode keycode)
    {
        switch (keycode)
        {
        case Key::Tab: return ImGuiKey_Tab;
        case Key::Left: return ImGuiKey_LeftArrow;
        case Key::Right: return ImGuiKey_RightArrow;
        case Key::Up: return ImGuiKey_UpArrow;
        case Key::Down: return ImGuiKey_DownArrow;
        case Key::PageUp: return ImGuiKey_PageUp;
        case Key::PageDown: return ImGuiKey_PageDown;
        case Key::Home: return ImGuiKey_Home;
        case Key::End: return ImGuiKey_End;
        case Key::Insert: return ImGuiKey_Insert;
        case Key::Delete: return ImGuiKey_Delete;
        case Key::Backspace: return ImGuiKey_Backspace;
        case Key::Space: return ImGuiKey_Space;
        case Key::Return: return ImGuiKey_Enter;
        case Key::Escape: return ImGuiKey_Escape;
        case Key::Apostrophe: return ImGuiKey_Apostrophe;
        case Key::Comma: return ImGuiKey_Comma;
        case Key::Minus: return ImGuiKey_Minus;
        case Key::Period: return ImGuiKey_Period;
        case Key::Slash: return ImGuiKey_Slash;
        case Key::Semicolon: return ImGuiKey_Semicolon;
        case Key::Equals: return ImGuiKey_Equal;
        case Key::LeftBracket: return ImGuiKey_LeftBracket;
        case Key::Backslash: return ImGuiKey_Backslash;
        case Key::RightBracket: return ImGuiKey_RightBracket;
        case Key::Grave: return ImGuiKey_GraveAccent;
        case Key::CapsLock: return ImGuiKey_CapsLock;
        case Key::ScrollLock: return ImGuiKey_ScrollLock;
        case Key::NumLockClear: return ImGuiKey_NumLock;
        case Key::PrintScreen: return ImGuiKey_PrintScreen;
        case Key::Pause: return ImGuiKey_Pause;
        case Key::LCtrl: return ImGuiKey_LeftCtrl;
        case Key::LShift: return ImGuiKey_LeftShift;
        case Key::LAlt: return ImGuiKey_LeftAlt;
        case Key::LGui: return ImGuiKey_LeftSuper;
        case Key::RCtrl: return ImGuiKey_RightCtrl;
        case Key::RShift: return ImGuiKey_RightShift;
        case Key::RAlt: return ImGuiKey_RightAlt;
        case Key::RGui: return ImGuiKey_RightSuper;
        case Key::Application: return ImGuiKey_Menu;
        case Key::D0: return ImGuiKey_0;
        case Key::D1: return ImGuiKey_1;
        case Key::D2: return ImGuiKey_2;
        case Key::D3: return ImGuiKey_3;
        case Key::D4: return ImGuiKey_4;
        case Key::D5: return ImGuiKey_5;
        case Key::D6: return ImGuiKey_6;
        case Key::D7: return ImGuiKey_7;
        case Key::D8: return ImGuiKey_8;
        case Key::D9: return ImGuiKey_9;
        case Key::A: return ImGuiKey_A;
        case Key::B: return ImGuiKey_B;
        case Key::C: return ImGuiKey_C;
        case Key::D: return ImGuiKey_D;
        case Key::E: return ImGuiKey_E;
        case Key::F: return ImGuiKey_F;
        case Key::G: return ImGuiKey_G;
        case Key::H: return ImGuiKey_H;
        case Key::I: return ImGuiKey_I;
        case Key::J: return ImGuiKey_J;
        case Key::K: return ImGuiKey_K;
        case Key::L: return ImGuiKey_L;
        case Key::M: return ImGuiKey_M;
        case Key::N: return ImGuiKey_N;
        case Key::O: return ImGuiKey_O;
        case Key::P: return ImGuiKey_P;
        case Key::Q: return ImGuiKey_Q;
        case Key::R: return ImGuiKey_R;
        case Key::S: return ImGuiKey_S;
        case Key::T: return ImGuiKey_T;
        case Key::U: return ImGuiKey_U;
        case Key::V: return ImGuiKey_V;
        case Key::W: return ImGuiKey_W;
        case Key::X: return ImGuiKey_X;
        case Key::Y: return ImGuiKey_Y;
        case Key::Z: return ImGuiKey_Z;
        case Key::F1: return ImGuiKey_F1;
        case Key::F2: return ImGuiKey_F2;
        case Key::F3: return ImGuiKey_F3;
        case Key::F4: return ImGuiKey_F4;
        case Key::F5: return ImGuiKey_F5;
        case Key::F6: return ImGuiKey_F6;
        case Key::F7: return ImGuiKey_F7;
        case Key::F8: return ImGuiKey_F8;
        case Key::F9: return ImGuiKey_F9;
        case Key::F10: return ImGuiKey_F10;
        case Key::F11: return ImGuiKey_F11;
        case Key::F12: return ImGuiKey_F12;
        case Key::F13: return ImGuiKey_F13;
        case Key::F14: return ImGuiKey_F14;
        case Key::F15: return ImGuiKey_F15;
        case Key::F16: return ImGuiKey_F16;
        case Key::F17: return ImGuiKey_F17;
        case Key::F18: return ImGuiKey_F18;
        case Key::F19: return ImGuiKey_F19;
        case Key::F20: return ImGuiKey_F20;
        case Key::F21: return ImGuiKey_F21;
        case Key::F22: return ImGuiKey_F22;
        case Key::F23: return ImGuiKey_F23;
        case Key::F24: return ImGuiKey_F24;
        case Key::AcBack: return ImGuiKey_AppBack;
        case Key::AcForward: return ImGuiKey_AppForward;
        default: break;
        }
        return ImGuiKey_None;
	}

    uint32_t TeddyToImGuiMouse(MouseCode keycode)
    {
		if (keycode == Mouse::Left)     return 0;
		if (keycode == Mouse::Right)    return 1;
		if (keycode == Mouse::Middle)   return 2;
		if (keycode == Mouse::X1)       return 3;
		if (keycode == Mouse::X2)       return 4;
		return -1;
	}

    uint32_t TeddyToRtmidi(MidiCode keycode) 
    {
        // Teddy MIDI note codes are defined to match the MIDI note numbers (0-127)
        // If the keycode is in the valid MIDI note range, return it as RtMidi code
        if (static_cast<uint32_t>(keycode) >= 0x00 && static_cast<uint32_t>(keycode) <= 0x77) 
        {
            return static_cast<uint32_t>(keycode);
        }
        // Special case for pedal (sustain) control, if you use TED_MIDI_PEDAL
        if (keycode == Midi::Pedal) 
        {
            return 0x78;
        }
        // Not a valid MIDI note or mapped control
        return -1;
    }

    MidiCode RtmidiToTeddy(uint32_t keycode) 
    {
        // Teddy MIDI note codes are defined to match the MIDI note numbers (0-127)
        // If the keycode is in the valid MIDI note range, return it as Teddy code
        if (keycode >= 0x00 && keycode <= 0x77) 
        {
            return static_cast<MidiCode>(keycode);
        }
        // Special case for pedal (sustain) control, if you use 0x78 for TED_MIDI_PEDAL
        if (keycode == 0x78) 
        {
            return Midi::Pedal;
        }
        // Not a valid MIDI note or mapped control
        return Midi::Unknown;
    }

    std::string KeyToNote(MidiCode key) 
    {
        if (static_cast<uint32_t>(key) < 0 || static_cast<uint32_t>(key) > 120) 
        {
            return "Invalid note number";
        }

        const char* note;
        switch (static_cast<uint32_t>(key) % 12) 
        {
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