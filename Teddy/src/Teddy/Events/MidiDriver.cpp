#include "TeddyPch.h"
#include "Teddy/Events/MidiDriver.h"

#include "Teddy/Events/MidiEvent.h"
#include "Teddy/Core/MidiCodes.h"

#include "Teddy/Core/CodeConverter.h"

namespace Teddy 
{

	void MidiDriver::SetEventCallback(Ref<EventCallbackFn> callback) 
	{
		m_EventCallback = callback;
	}

	void MidiDriver::Init() 
	{
		TED_PROFILE_FUNCTION();

		try 
		{
			midiIn = CreateScope<RtMidiIn>(RtMidi::UNSPECIFIED, "Teddy MidiIn");
			midiIn->ignoreTypes(false, false, false);
		}
		catch (RtMidiError& error) 
		{
			TED_CORE_ERROR(error.getMessage());
		}

		try 
		{
			midiOut = CreateScope<RtMidiOut>();
		}
		catch (RtMidiError& error) 
		{
			TED_CORE_ERROR(error.getMessage());
		}

	}

	void MidiDriver::InitIn(unsigned int port) 
	{
		TED_PROFILE_FUNCTION();

		try 
		{
			midiIn->openPort(port);
			TED_CORE_ASSERT(midiIn->isPortOpen(), "Could not initialize Midi In");
		}
		catch (RtMidiError& error) 
		{
			TED_CORE_ERROR(error.getMessage());
		}
	}

	void MidiDriver::InitOut(unsigned int port) 
	{
		TED_PROFILE_FUNCTION();

		try 
		{
			midiOut->openPort(port);
			TED_CORE_ASSERT(midiOut->isPortOpen(), "Error initializing Midi Output");
		}
		catch (RtMidiError& error) 
		{
			TED_CORE_ERROR(error.getMessage());
		}
	}

	void MidiDriver::OpenInPort(unsigned int port) 
	{
		TED_PROFILE_FUNCTION();

		if (midiIn) 
		{
			midiIn->openPort(port);
		}
	}

	void MidiDriver::OpenOutPort(unsigned int port) 
	{
		TED_PROFILE_FUNCTION();

		if (midiOut) {
			midiOut->openPort(port);
		}
	}

	unsigned int MidiDriver::GetInPorts() 
	{
		TED_PROFILE_FUNCTION();

		return midiIn->getPortCount();
	}

	unsigned int MidiDriver::GetOutPorts() 
	{
		TED_PROFILE_FUNCTION();

		return midiOut->getPortCount();
	}

	std::string MidiDriver::GetInPortName(unsigned int port) 
	{
		TED_PROFILE_FUNCTION();

		return midiIn->getPortName(port);
	}

	std::string MidiDriver::GetOutPortName(unsigned int port) 
	{
		TED_PROFILE_FUNCTION();

		return midiOut->getPortName(port);
	}

	std::unordered_map<unsigned int, std::string> MidiDriver::GetInPortsMap() 
	{
		TED_PROFILE_FUNCTION();

		std::unordered_map<unsigned int, std::string> ports;
		for (unsigned int i = 0; i < midiIn->getPortCount(); ++i) 
		{
			ports[i] = midiIn->getPortName(i);
		}
		return ports;
	}

	std::unordered_map<unsigned int, std::string> MidiDriver::GetOutPortsMap() 
	{
		TED_PROFILE_FUNCTION();

		std::unordered_map<unsigned int, std::string> ports;
		for (unsigned int i = 0; i < midiOut->getPortCount(); ++i) 
		{
			ports[i] = midiOut->getPortName(i);
		}
		return ports;
	}

	void MidiDriver::OnUpdate() 
	{
		TED_PROFILE_FUNCTION();

		if (!(*m_EventCallback) || !midiIn->isPortOpen()) return;
		static std::vector<unsigned char> message;
		int nBytes;

		bool done = false;
		double stamp;
		
		while (!done) 
		{
			stamp = midiIn->getMessage(&message);
			nBytes = static_cast<int>(message.size());

			if (nBytes > 0) 
			{

				if (message[0] == 144) 
				{
					MidiKeyPressedEvent e(RtmidiToTeddy(message[1]), message[2]);
					(*m_EventCallback)(e);
				}
				else if (message[0] == 128) 
				{
					MidiKeyReleasedEvent e(RtmidiToTeddy(message[1]), message[2]);
					(*m_EventCallback)(e);
				}
				else if (message[0] == 176) 
				{
					if (message[2] == 0) 
					{
						MidiPedalReleasedEvent e;
						(*m_EventCallback)(e);
					}
					else if (message[2] <= 127) 
					{
						MidiPedalPressedEvent e;
						(*m_EventCallback)(e);
					}
					else 
					{
						TED_CORE_TRACE("Pedal Event unrecognized: {0}", message[2]);
					}
				}
				else 
				{
					TED_CORE_TRACE("Midi Event unrecognized: {0}", message[0]);
				}

				if (midiOut->isPortOpen()) 
				{

					midiOut->sendMessage(&message);
				}
			}
			else 
			{
				done = true;
			}
		}
	}

}