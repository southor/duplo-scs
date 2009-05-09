
#ifndef _DUP_MIDI_EVENT_RECEIVER_
#define _DUP_MIDI_EVENT_RECEIVER_


namespace Dup
{

	class MidiEventReceiver
	{	
	public:

		MidiEventReceiver()
		{
		}

		// returns size of message
		virtual int receiveEvent(dup_uint8 *midiEvent, int source, int nChannels) = 0;

	};
};

#endif