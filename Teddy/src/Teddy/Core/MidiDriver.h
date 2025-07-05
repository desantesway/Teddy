#pragma once

#include "TeddyPch.h"
#include "rtmidi.h"

#include "Teddy/Events/MidiEvent.h"
#include "Teddy/Core/MidiCodes.h"

#include "Teddy/Core/CodeConverter.h"

namespace Teddy {

	class MidiDriver {
	public:
		MidiDriver() = default;
		~MidiDriver() = default;

		using EventCallbackFn = std::function<void(Event&)>;

		void SetEventCallback(EventCallbackFn* callback) {
			m_EventCallback = callback;
		}
		
		void Init(EventCallbackFn* callback) {

			SetEventCallback(callback);

			try {
				midiIn = CreateScope<RtMidiIn>(RtMidi::UNSPECIFIED, "Teddy MidiIn");
				midiIn->ignoreTypes(false, false, false);
			}
			catch (RtMidiError& error) {
				TED_CORE_ERROR(error.getMessage());
			}

			try {
				midiOut = CreateScope<RtMidiOut>();
			}
			catch (RtMidiError& error) {
				TED_CORE_ERROR(error.getMessage());
			}

		}

		void InitIn(unsigned int port) {
			try{
				midiIn->openPort(port);
				TED_CORE_ASSERT(midiIn->isPortOpen(), "Could not initialize Midi In");
			}
			catch (RtMidiError& error) {
				TED_CORE_ERROR(error.getMessage());
			}
		}

		void InitOut(unsigned int port) {
			try {
				midiOut->openPort(port);
				TED_CORE_ASSERT(midiOut->isPortOpen(), "Error initializing Midi Output");
			}
			catch (RtMidiError& error) {
				TED_CORE_ERROR(error.getMessage());
			}
		}

		void OpenInPort(unsigned int port) {
			if (midiIn) {
				midiIn->openPort(port);
			}
		}

		void OpenOutPort(unsigned int port) {
			if (midiOut) {
				midiOut->openPort(port);
			}
		}

		unsigned int GetInPorts() {
			return midiIn->getPortCount();
		}

		unsigned int GetOutPorts() {
			return midiOut->getPortCount();
		}

		std::string GetInPortName(unsigned int port) {
			return midiIn->getPortName(port);
		}

		std::string GetOutPortName(unsigned int port) {
			return midiOut->getPortName(port);
		}

		std::unordered_map<unsigned int, std::string> GetInPortsMap() {
			std::unordered_map<unsigned int, std::string> ports;
			for (unsigned int i = 0; i < midiIn->getPortCount(); ++i) {
				ports[i] = midiIn->getPortName(i);
			}
			return ports;
		}

		std::unordered_map<unsigned int, std::string> GetOutPortsMap() {
			std::unordered_map<unsigned int, std::string> ports;
			for (unsigned int i = 0; i < midiOut->getPortCount(); ++i) {
				ports[i] = midiOut->getPortName(i);
			}
			return ports;
		}

		void OnUpdate() {

			if (!(*m_EventCallback) || !midiIn->isPortOpen()) return;
			static std::vector<unsigned char> message;
			int nBytes;

			bool done = false;
			double stamp;

			while (!done) {
				stamp = midiIn->getMessage(&message);
				nBytes = static_cast<int>(message.size());

				if (nBytes > 0) {
					
					
					if (message[0] == 144) {
						MidiKeyPressedEvent e(RtmidiToTeddy(message[1]), message[2]);
						(*m_EventCallback)(e);
					}
					else if (message[0] == 128) {
						MidiKeyReleasedEvent e(RtmidiToTeddy(message[1]), message[2]);
						(*m_EventCallback)(e);
					}
					else if (message[0] == 176) {
						if (message[2] == 0) {
							MidiPedalReleasedEvent e;
							(*m_EventCallback)(e);
						}
						else if(message[2] <= 127){
							MidiPedalPressedEvent e;
							(*m_EventCallback)(e);
						}
						else {
							TED_CORE_TRACE("Pedal Event unrecognized: {0}", message[2]);
						}
					}
					else {
						TED_CORE_TRACE("Midi Event unrecognized: {0}", message[0]);
					}

					if (midiOut->isPortOpen()){
						
						midiOut->sendMessage(&message);
					}
				}
				else {
					done = true;
				}
			}
		}

	private:
		Scope<RtMidiIn> midiIn;
		Scope<RtMidiOut> midiOut;

		EventCallbackFn* m_EventCallback;
	};
}