#pragma once

#include "rtmidi.h"
#include "Teddy/Events/Event.h"

namespace Teddy {

	class MidiDriver 
	{
	public:
		MidiDriver() = default;
		~MidiDriver() = default;

		using EventCallbackFn = std::function<void(Event&)>;

		void SetEventCallback(Ref<EventCallbackFn> callback);

		void Init();

		void InitIn(unsigned int port);
		void InitOut(unsigned int port);

		void OpenInPort(unsigned int port);
		void OpenOutPort(unsigned int port);

		unsigned int GetInPorts();
		unsigned int GetOutPorts();

		std::string GetInPortName(unsigned int port);
		std::string GetOutPortName(unsigned int port);

		std::unordered_map<unsigned int, std::string> GetInPortsMap();
		std::unordered_map<unsigned int, std::string> GetOutPortsMap();

		void OnUpdate();

	private:
		Scope<RtMidiIn> midiIn;
		Scope<RtMidiOut> midiOut;

		Ref<EventCallbackFn> m_EventCallback;
	};
}
