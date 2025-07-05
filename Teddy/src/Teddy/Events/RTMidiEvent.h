#pragma once	

#include "teddyPch.h"
#include "Teddy/Window.h"
#include "rtmidi.h"

//#include "Teddy/Events/MidiEvent.h"

#include "Teddy/CodeConverter.h"

namespace Teddy {

    void RTMidiEvents() {
        try {
            RtMidiOut midiOut;

            // Check available ports.
            unsigned int nPorts = midiOut.getPortCount();
            if (nPorts == 0) {
                std::cout << "No MIDI output ports available.\n";
            }

            std::cout << "Available MIDI Output Ports:\n";
            for (unsigned int i = 0; i < nPorts; ++i) {
                std::cout << "  Port " << i << ": " << midiOut.getPortName(i) << "\n";
            }

        }
        catch (RtMidiError& error) {
            error.printMessage();
        }
    }
	
}
