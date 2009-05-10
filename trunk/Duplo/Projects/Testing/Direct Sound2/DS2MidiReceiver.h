#include "MidiEventReceiver.h"
#include "Queue.h"

#define USE_CC_ACCUMULATING true


Dup::ModuleHandler *mH;


LPDIRECTSOUNDBUFFER m_pDSbuffer;

int playDSBuffer(LPDIRECTSOUNDBUFFER m_pDSbuffer);


int transpose;


class DS2MidiReceiver : Dup::MidiEventReceiver
{
public:
	static const int N_CONTROLLER_LAYERS = 2; // Channel pressure can also be set to a  specific controller (one more layer in this case)
	static const int CHANNEL_PRESSURE_CONTROLLER = N_CONTROLLER_LAYERS;

	static const int DEFAULT_CHANNEL_PRESSURE_CONTROLLER = 1; // Modulation controller

	static const dup_uint8 FEEDBACK_CHANNEL = 0;

	enum
	{
		DEBUG_ALL,
		DEBUG_ALL_BUT_ONE,
		DEBUG_ONE,
		DEBUG_NOTHING
	};

	static const int DEBUG_MODE_SOURCE = DEBUG_ONE;
	static const int DEBUG_MODE_TYPE = DEBUG_ONE;
	static const int DEBUG_MODE_CHANNEL = DEBUG_ALL;

	static const int DEBUG_SOURCE = 0;
	static const int DEBUG_TYPE = 0xC0;
	static const int DEBUG_CHANNEL = 0;
	

private:

	// midi feedback trigger
	int midiFeedbackTriggerChannel;

	int drumChannel;
	int midiReceiver;
	const int *nVoices;
	int nDrums;

	// real time or not?
	bool realTime;

	// midi parsing
	dup_uint8 channel;
	dup_uint8 type;

	dup_uint8 controllerValue[16][128][N_CONTROLLER_LAYERS];
	dup_uint8 channelPressureController[16]; // channel pressure controlling a controller
	dup_uint8 channelPressureControllerValue[16];

	// Channel filter
	bool filterChannels; // true if we should filter all channels but one
	char acceptedChannel; // the channel to let through the filter
	bool playedAnyMsg; // True if any message was recieved and not filtered

	//wave frequency
	int waveFre;

	// debugging	
	static const int DEBUG_QUEUE_SIZE = 1024;
	static const bool DEBUG_INCOMMING = true;
	Dup::Queue<dup_uint8 *, NULL> queue;

public:

	DS2MidiReceiver(int drumChannel, int midiReceiver, int waveFre, const int *nVoices, int nDrums, bool realTime, bool filterChannels, char acceptedChannel = 1)
		: Dup::MidiEventReceiver(), queue(DEBUG_QUEUE_SIZE)
	{
		this->drumChannel = drumChannel;
		this->midiReceiver = midiReceiver;
		this->nVoices = nVoices;
		this->nDrums = nDrums;
		this->realTime = realTime;
		this->waveFre = waveFre;
		this->midiFeedbackTriggerChannel = 0;

		type = 0xA0; // noteoff
		channel = 0x00; // channel 1

		this->filterChannels = filterChannels;
		this->acceptedChannel = acceptedChannel;
		this->playedAnyMsg = false;

		for(int i=0; i<16; ++i)
		{
			this->channelPressureController[i] = DEFAULT_CHANNEL_PRESSURE_CONTROLLER;
			this->channelPressureControllerValue[i] = 0;
			
			for(int j=0; j<128; ++j)
			{
				for(int k=0; k<N_CONTROLLER_LAYERS; ++k)
				{
					controllerValue[i][j][k] = 0;
				}
			}
		}
	}

	//int getMidiFeedbackChannel()
	//{
	//	return this->midiFeedbackTriggerChannel;
	//}

	void setAcceptedChannel(char acceptedChannel)
	{
		this->acceptedChannel = acceptedChannel;
	}

	void resetPlayedAnyMsg()
	{
		this->playedAnyMsg = false;
	}

	bool getPlayedAnyMsg()
	{
		return playedAnyMsg;
	}

	void debugPrintLastMsg(HWND hwnd)
	{
		if ((DEBUG_MODE_CHANNEL == DEBUG_NOTHING) || (DEBUG_MODE_TYPE == DEBUG_NOTHING) || (DEBUG_MODE_SOURCE == DEBUG_NOTHING) || (!DEBUG_INCOMMING))
		{
			MessageBox(hwnd, "debugging is not activated!", "midi msg",MB_OK);
		}
		else
		{
			if (queue.empty())
			{
				MessageBox(hwnd, "no more messages", "midi msg",MB_OK);
				return;
			}

			dup_uint8 *tmpMsg = queue.pop();
			dup_uint8 msg[3];
			msg[0] = *(tmpMsg+0);
			msg[1] = *(tmpMsg+1);

			int type = (msg[0] & 0xF0);
			int channel = (msg[0] & 0x0F);

			char *typeStr;
			char *channelStr;
							
			if (DEBUG_MODE_TYPE == DEBUG_ALL) typeStr = "   type: ";
			else if (DEBUG_MODE_TYPE == DEBUG_ONE) typeStr = "   (Only One) type: ";
			else if (DEBUG_MODE_TYPE == DEBUG_ALL_BUT_ONE) typeStr = "   (All but One) type: ";
			else assert(false);

			if (DEBUG_MODE_CHANNEL == DEBUG_ALL) channelStr = "   channel: ";
			else if (DEBUG_MODE_CHANNEL == DEBUG_ONE) channelStr = "   (Only One) channel: ";
			else if (DEBUG_MODE_CHANNEL == DEBUG_ALL_BUT_ONE) channelStr = "   (All but One) channel: ";
			else assert(false);

			MessageBox(hwnd, unitestrings(typeStr, inttostring(type), channelStr, inttostring(channel), "   value 1: ", inttostring(msg[1])), "midi msg",MB_OK);
		}
	}

	void replayBuffer()
	{
		if (realTime)
		{
			m_pDSbuffer->Stop();
			playDSBuffer(m_pDSbuffer);
		}
	}

	double getControllerValue(dup_uint8 channel, dup_uint8 controller)
	{
		double value = 0.0;
		for(int i=0; i<N_CONTROLLER_LAYERS; ++i)
		{
			value += (((double)(controllerValue[channel][controller][i])) / 128.0) * (1.0 - value);
		}
		if (controller == channelPressureController[channel]) value += (((double)channelPressureControllerValue[channel]) / 128.0) * (1.0 - value);
		return value;
	}

	void setControllerValue(dup_uint8 channel, dup_uint8 controller, int controllerLayer, dup_uint8 value)
	{		
		controllerValue[channel][controller][controllerLayer] = value;
	}

	inline int receiveEvent(dup_uint8 *data, int source, int nChannels)
	{
		bool dummy;
		return receiveEvent(data, source, nChannels, dummy);
	}

	/**
	 * @param data In out parameter. The channel field in the message is changed when param 'sendFeedback' is true
	 * @param source Range: 0..N_CONTROLLER_LAYERS-1 or CHANNEL_PRESSURE_CONTROLLER
	 */
	int receiveEvent(dup_uint8 *data, int source, int nChannels, bool &sendFeedback)
	{
		assert(data != NULL);
		if (data == NULL) MessageBox(NULL, "midi data null","error",MB_OK);
		
		bool debugIncoming = DEBUG_INCOMMING && (*(data +0) != 0xFE);
		
		int headSize = 0;
		
		if (*(data+0) >= 0x80) // not same type and channel
		{
			type = (*(data+0) & 0xF0);
			channel = (*(data+0) & 0x0F);

			++headSize;
			//++data;

			//if (channel > nChannels) MessageBox(NULL, "channel number not supported","message",MB_OK);
		}

		// debugging
		if (debugIncoming)
		{
			if (((DEBUG_MODE_SOURCE == DEBUG_ALL) ||
				((DEBUG_MODE_SOURCE == DEBUG_ONE) && (source == DEBUG_SOURCE)) ||
				((DEBUG_MODE_SOURCE == DEBUG_ALL_BUT_ONE) && (source != DEBUG_SOURCE)))
				&&
				((DEBUG_MODE_TYPE == DEBUG_ALL) ||
				((DEBUG_MODE_TYPE == DEBUG_ONE) && (type == DEBUG_TYPE)) ||
				((DEBUG_MODE_TYPE == DEBUG_ALL_BUT_ONE) && (type != DEBUG_TYPE)))
				&&
				((DEBUG_MODE_CHANNEL == DEBUG_ALL) ||
				((DEBUG_MODE_CHANNEL == DEBUG_ONE) && (channel == DEBUG_CHANNEL)) ||
				((DEBUG_MODE_CHANNEL == DEBUG_ALL_BUT_ONE) && (channel != DEBUG_CHANNEL))))
			{
				dup_uint8 *debugSaveMsg = new dup_uint8[3];
				debugSaveMsg[0] = type + channel;
				debugSaveMsg[1] = data[1];
				debugSaveMsg[2] = data[2];

				queue.push(debugSaveMsg);
			}	
		}

		

		int size = headSize + recieveEvent2(type, channel, data+1, source, sendFeedback) - 1;

		sendFeedback = sendFeedback && realTime;
		
		if (sendFeedback)
		{
			if (*(data+0) >= 0x80) // not same type and channel
			{
				// channel is changed to feedback channel
				*(data+0) = ((*(data+0)) & 0xF0) | FEEDBACK_CHANNEL;
				
			}
		}
		
		return size;
	}

	int recieveEvent2(dup_uint8 type, dup_uint8 channel, const dup_uint8 *restData, int source, bool &sendFeedback)
	{
		sendFeedback = false;
		assert(restData != NULL);
		if (restData == NULL) MessageBox(NULL, "midi data null","error",MB_OK);
		
		SCSPlayer *SCSP;
		int eventSize = 1;

		
		static int debug_counter = 0;
		++debug_counter;
		const static int COUNTER = 39;

		//if (debug_counter > COUNTER) MessageBox(NULL,"event0","ok",MB_OK);

		// Perhaps ignore message if the channelFilter is active
		if (!filterChannels || (channel == acceptedChannel))
		{
		
			if (type == 0x90) // note on
			{


				//if (debug_counter > COUNTER) MessageBox(NULL,"event1","ok",MB_OK);

				//dup_uint8 msgType = 0x80;
				dup_uint8 note = *(restData+0) + transpose; 
				dup_val vel = ((double)(*(restData+1)))/127.0;

				if (note - transpose == 127) // special.. triggers replay buffer
				{
					//char str[32];
					//str[0] = type;
					//str[1] = '0' + channel;
					//str[2] = *(restData+0);
					//str[3] = *(restData+1);
					//str[4] = 0;
					//MessageBox(NULL,str,"msg", MB_OK);
					//if (debug_counter > COUNTER) MessageBox(NULL,"event1.12","ok",MB_OK);
					
					replayBuffer();
				}
				//else if (note - transpose == 126) // special.. triggers reset midi-feedback-trigger-channel
				//{
				//	midiFeedbackTriggerChannel = channel;
				//}

				//if (debug_counter > COUNTER) MessageBox(NULL,"event1.2","ok",MB_OK);

				//MessageBox(NULL, "midi mess noteon","message",MB_OK);

				if (vel == 0.0)
				{
					//if (debug_counter > COUNTER) MessageBox(NULL,"event1.31","ok",MB_OK);
					
					Dup::MessageConstructor *msgC = new Dup::MessageConstructor(sizeof(double)+2, 128);
					msgC->addData8(0x82);
					msgC->addData8(note);
					msgC->addDataVal(vel);

					Dup::MessageParser msgP = msgC->getParser();

					//if (debug_counter > COUNTER) MessageBox(NULL,"event1.32","ok",MB_OK);
					mH->postMessage(0.0, channel*100+midiReceiver, msgP.getSize(), msgP.getMessage(), msgP.getFormat());
				}
				else
				{
					//if (debug_counter > COUNTER) MessageBox(NULL,"event1.41","ok",MB_OK);


					Dup::MessageConstructor *msgC = new Dup::MessageConstructor(sizeof(double)+2, 128);
					msgC->addData8(0x80);
					msgC->addData8(note);
					msgC->addDataVal(vel);

					Dup::MessageParser msgP = msgC->getParser();

					//if (debug_counter > COUNTER) MessageBox(NULL,"event1.42","ok",MB_OK);
					mH->postMessage(0.0, channel*100+midiReceiver, msgP.getSize(), msgP.getMessage(), msgP.getFormat());
				}

				//if (debug_counter > COUNTER) MessageBox(NULL,"event1.5","ok",MB_OK);

				eventSize = 3;
				//if (realTime) replayBuffer();
			}
			else if (type == 0xA0) // after touch
			{
				//MessageBox(NULL, "midi mess aftertouch","message",MB_OK);

				dup_uint8 note = *(restData+0) + transpose;
				double vel = ((double)(*(restData+1)))/127.0;

				Dup::MessageConstructor *msgC = new Dup::MessageConstructor(sizeof(double)+2, 128);
				msgC->addData8(0x83);
				msgC->addData8(note);
				msgC->addDataVal(vel);

				Dup::MessageParser msgP = msgC->getParser();

				mH->postMessage(0.0, channel*100 + midiReceiver, msgP.getSize(), msgP.getMessage(), msgP.getFormat());

				eventSize = 3;
			}
			else if (type == 0xD0) // channel pressure
			{
				//MessageBox(NULL, "midi mess pressure","message",MB_OK);
				dup_uint8 newData[2] = {channelPressureController[channel], restData[0]};
				bool dummy;
				recieveEvent2(0xB0, channel, newData, CHANNEL_PRESSURE_CONTROLLER, dummy); // send controller midi message

				eventSize = 2;
			}
			else if (type == 0x80) // note off
			{
				//MessageBox(NULL, "midi mess noteoff","message",MB_OK);
				//if (debug_counter > COUNTER) MessageBox(NULL,"midi mess noteoff","ok",MB_OK);
				
				//dup_uint8 msgType = 0x80;									
				dup_uint8 note = *(restData+0) + transpose;
				double vel = ((double)(*(restData+1)))/127.0;

				Dup::MessageConstructor *msgC = new Dup::MessageConstructor(sizeof(double)+2, 128);
				msgC->addData8(0x82);
				msgC->addData8(note);
				msgC->addDataVal(vel);

				Dup::MessageParser msgP = msgC->getParser();

				mH->postMessage(0.0, channel*100+midiReceiver, msgP.getSize(), msgP.getMessage(), msgP.getFormat());

				eventSize = 3;
			}
			else if (type == 0xC0) // patch select
			{
				//MessageBox(NULL, "patch select","message",MB_OK);
				//if (debug_counter > COUNTER) MessageBox(NULL,"patch select","ok",MB_OK);
				
				if (channel != 9) // not the drum channel
				{
					
					dup_uint8 patch = *(restData+0);

					SCSPlayer *SCSP;
					SoundCollector *soundCollector;
					
					//double Pan = 0.0;
					
					//// debug
					//if (*(nVoices+channel) == 3)
					//{
					//	//MessageBox(hwnd, "3 voices patch select","message",MB_OK);
					//	Pan = Pan;
					//}
					
					for(int voice = 1;voice<=(*(nVoices+channel));voice++)
					{
						SCSP = (SCSPlayer *)(mH->getModule(channel*100+10*voice+5));
						if (SCSP != NULL)
						{
							soundCollector = SCSP->getSoundCollector();
							if (soundCollector != NULL)
							{
								delete soundCollector;
							}
							soundCollector = new SoundCollector();

							//SCSCreateInstrument(soundCollector, patch);
							SCSCreateInstrument(SCSP, soundCollector, patch);					
						}
							
						//MessageBox(NULL, "inside voice loop","message",MB_OK);	
					}
				}
				eventSize = 2;
				
				if (realTime) replayBuffer();
			}
			else if (type == 0xB0) // CC controller
			{
				//if (debug_counter > COUNTER) MessageBox(NULL,"controller 01","ok",MB_OK);

				//char str[32];
				//str[0] = type;
				//str[1] = '0' + channel;
				//str[2] = itoa(*(restData+0));
				//str[3] = *(restData+1);
				//str[4] = 0;
				//if (debug_counter > COUNTER) MessageBox(NULL,itoa(*(restData+0), str, 10),"msg", MB_OK);
				//if (debug_counter > COUNTER && *(restData+0) == 7) MessageBox(NULL,str,"volume", MB_OK);
				//__asm {
				//	int 3
				//}

				dup_uint8 cntrl = *(restData+0);
				dup_uint8 data2 = *(restData+1);

				if (cntrl == 21) // special.. triggers reset midi-feedback-trigger-channel
				{ 
					//midiFeedbackTriggerChannel = channel;
					midiFeedbackTriggerChannel = 7-1;
					sendFeedback = false;

					//if (debug_counter > COUNTER) MessageBox(NULL,"controller 02","ok",MB_OK);
				}
				else
				{
					// find out if feedback should be sent
					sendFeedback = (channel == midiFeedbackTriggerChannel);

					//if (debug_counter > COUNTER) MessageBox(NULL,"controller 03","ok",MB_OK);
				}

				


				//MessageBox(NULL, "controller","message",MB_OK);

				// update the controller
				if (source == CHANNEL_PRESSURE_CONTROLLER)
				{
					assert(cntrl == channelPressureController[channel]);
					channelPressureControllerValue[channel] = data2;
				}
				else
				{
					assert(source >= 0 && source < N_CONTROLLER_LAYERS);
					setControllerValue(channel, cntrl, source, data2);					
				}

				//if (debug_counter > COUNTER) MessageBox(NULL,"controller 04","ok",MB_OK);

				
				double value = USE_CC_ACCUMULATING ? getControllerValue(channel, cntrl) : (data2 / 128.0);
				
				SoundCollector *soundCollector;

				

				if (channel == 9) // drum channel
				{

					if (cntrl == 0x07) // set vol
					{
						for(int id=1; id <= nDrums; id++)
						{
							SCSP = (SCSPlayer *)(mH->getModule(drumChannel+id+5));							
							SCSP->setAmp(1.5*value);
						}
					}
					else if (cntrl == 0x0B) // set Expression
					{
						for(int id=1; id <= nDrums; id++)
						{
							SCSP = (SCSPlayer *)(mH->getModule(drumChannel+id+5));
							SCSP->setExpr(value);
						}
					}
					else if (cntrl == 0x0A) // set Pan
					{
						double pan = data2 / 64.0 - 1.0;
						for(int id=1; id <= nDrums; id++)
						{
							((SCSPlayer *)(mH->getModule(drumChannel+id+5)))->setPan(pan);
						}
					}

					//if (debug_counter > COUNTER) MessageBox(NULL,"controller 05","ok",MB_OK);

				}
				else
				{

					//if (debug_counter > COUNTER) MessageBox(NULL,"controller 06","ok",MB_OK);

					
					
					//if (cntrl == 0x07) // set vol
					if (cntrl == 52) // set compress in gain
					{
						for(int voice = 1;voice<=*(nVoices+channel);voice++)
						{
							SCSP = (SCSPlayer *)(mH->getModule(100*channel+10*voice+5));
							if (SCSP != NULL) SCSP->setAmp(1.5*value);
						}

						//Compressor *compressor = (Compressor*)(mH->getModule(2000+channel*100+21));
						//if ((compressor != NULL) && (value != 0.0)) compressor->setGain1(1.0/value);
					}
					else if (cntrl == 0x0A) // set Pan
					{
						//if (debug_counter > COUNTER) MessageBox(NULL,"controller 06 pan","ok",MB_OK);
						
						double pan = data2 / 64.0 - 1.0;
						for(int voice = 1;voice<=*(nVoices+channel);voice++)
						{
							
							SCSP = (SCSPlayer *)(mH->getModule(100*channel+10*voice+5));
							if (SCSP != NULL)
							{
								//soundCollector =  SCSP->getSoundCollector();					
								//if (soundCollector != NULL) //soundCollector->Pan = pan;
								//	soundCollector->setPan(pan);
								SCSP->setPan(pan);
							}

						}
					}
					else if (cntrl == 0x0B) // set Expression
					{
						for(int voice = 1;voice<=*(nVoices+channel);voice++)
						{
							SCSP = (SCSPlayer *)(mH->getModule(100*channel+10*voice+5));
							if (SCSP != NULL) SCSP->setExpr(value);
						}

						//Compressor *compressor = (Compressor*)(mH->getModule(2000+channel*100+21));
						//if ((compressor != NULL) && (value != 0.0)) compressor->setGain2(1.0/value);
					}
					//else if (cntrl == 14) // set Local Volume
					//{
					//	for(int voice = 1;voice<=*(nVoices+channel);voice++)
					//	{
					//		SCSP = (SCSPlayer *)(mH->getModule(100*channel+10*voice+5));
					//		if (SCSP != NULL) SCSP->setLocalVol(value);
					//	}

					//	//Compressor *compressor = (Compressor*)(mH->getModule(2000+channel*100+21));
					//	//if ((compressor != NULL) && (value != 0.0)) compressor->setGain2(1.0/value);
					//}
					else if (cntrl == 0x01) // set modulation
					{
						for(int voice = 1;voice<=*(nVoices+channel);voice++)
						{	
							SCSP = (SCSPlayer *)(mH->getModule(100*channel+10*voice+5));
							if (SCSP != NULL)
							{
								SCSP->setModulation(value/5.0);
							}
						}
					}
					else if (cntrl == 0x1A) // set modulation depth
					{
						for(int voice = 1;voice<=*(nVoices+channel);voice++)
						{	
							SCSP = (SCSPlayer *)(mH->getModule(100*channel+10*voice+5));
							if (SCSP != NULL)
							{
								SCSP->setModulationDepth(value*value*128.0);
							}
						}
					}
					else if (cntrl == 0x1B) // set modulation frequency
					{
						for(int voice = 1;voice<=*(nVoices+channel);voice++)
						{	
							SCSP = (SCSPlayer *)(mH->getModule(100*channel+10*voice+5));
							if (SCSP != NULL)
							{
								SCSP->setModulationFre(value*value*value*96.0);
							}
						}
					}
					else if (cntrl == 0x5C) // set tremolo
					{
						for(int voice = 1;voice<=*(nVoices+channel);voice++)
						{
							
							SCSP = (SCSPlayer *)(mH->getModule(100*channel+10*voice+5));
							if (SCSP != NULL)
							{
								SCSP->setTremolo(value);
							}

						}
					}
					else if (cntrl == 9) // set inner compress
					{
						for(int voice = 1;voice<=*(nVoices+channel);voice++)
						{
							
							SCSP = (SCSPlayer *)(mH->getModule(100*channel+10*voice+5));
							if (SCSP != NULL)
							{
								SCSP->setCompress(value);
								SCSP->setCompressMod(value);
							}

						}
					}
					//else if (cntrl == 15) // set compress mod
					//{
					//	for(int voice = 1;voice<=*(nVoices+channel);voice++)
					//	{	
					//		SCSP = (SCSPlayer *)(mH->getModule(100*channel+10*voice+5));
					//		if (SCSP != NULL)
					//		{
					//			SCSP->setCompressMod(value);
					//		}

					//	}
					//}
					else if (cntrl == 0x27) // set pitch wheel sensitivity
					{
						for(int voice = 1;voice<=*(nVoices+channel);voice++)
						{
							
							SCSP = (SCSPlayer *)(mH->getModule(100*channel+10*voice+5));
							if (SCSP != NULL)
							{
								SCSP->setWheelSensitivity(data2); // not using the controller layers
							}

						}
					}
					else if (cntrl == 51) // set Lowpass filter frequency (buffersize)
					{
						//MultiLowPass *chanFilter = (MultiLowPass *)(mH->getModule(2000+channel*100+20));
						//if (chanFilter != NULL)
						//{
						//	double fre = 20.0;

						//	for(int i=0; i<data2; ++i)
						//	{
						//		fre *= 1.065;
						//	}

						//	Dup::MessageConstructor *msgC = new Dup::MessageConstructor(sizeof(dup_uint32));
						//	msgC->addData32(16000 / 22050);
						//	//msgC->addData32((dup_uint32)(waveFre / (int)fre));
						//	msgC->addData32(2);
						//	chanFilter->reset(msgC->getParser());
						//	delete msgC;
						//}
					}
					else if (cntrl == 60) // set Reverb Reflection
					{
						//Delay *delL1 = (Delay *)(mH->getModule(2000+channel*100+6));
						//Delay *delR1 = (Delay *)(mH->getModule(2000+channel*100+7));
						//Delay *delL2 = (Delay *)(mH->getModule(2000+channel*100+8));
						//Delay *delR2 = (Delay *)(mH->getModule(2000+channel*100+9));
						Passer *delL1 = (Passer *)(mH->getModule(2000+channel*100+6));
						Passer *delR1 = (Passer *)(mH->getModule(2000+channel*100+7));
						Passer *delL2 = (Passer *)(mH->getModule(2000+channel*100+8));
						Passer *delR2 = (Passer *)(mH->getModule(2000+channel*100+9));
						
						if ( ! ((delL1 == NULL) || (delL2 == NULL) || (delR1 == NULL) || (delR2 == NULL)))
						{
							delL1->setInputBaseVal(delL1->AMP_INPUT, 0.25 * 1.15 * value);
							delR1->setInputBaseVal(delR1->AMP_INPUT, 0.3 * 1.15 * value);
							delL2->setInputBaseVal(delL2->AMP_INPUT, 0.13 * 1.15 * value);
							delR2->setInputBaseVal(delR2->AMP_INPUT, 0.085 * 1.15 * value);
						}
					}
					else if (cntrl == 61) // set Reverb Delay
					{
						// not implemented yet
					}
					else if (cntrl == 62) // set Reverb Softness
					{
						// not implemented yet
					}
					else if (cntrl == 53) // compressor threshold
					{
						Compressor *compressor = (Compressor*)(mH->getModule(2000+channel*100+21));
						if (compressor != NULL) compressor->setThreshold(value);						
					}
					else if (cntrl == 54) // compressor ratio
					{
						Compressor *compressor = (Compressor*)(mH->getModule(2000+channel*100+21));
						if (compressor != NULL) compressor->setRatio(value);						
					}
					else if (cntrl == 55) // compressor attack
					{
						Compressor *compressor = (Compressor*)(mH->getModule(2000+channel*100+21));
						if (compressor != NULL) compressor->setAttack(value);						
					}
					else if (cntrl == 56) // compressor release
					{
						Compressor *compressor = (Compressor*)(mH->getModule(2000+channel*100+21));
						if (compressor != NULL) compressor->setRelease(value);						
					}
					else if (cntrl == 7) // out volume
					{
						Compressor *compressor = (Compressor*)(mH->getModule(2000+channel*100+21));
						if (compressor != NULL) compressor->setOutGain1(value);						
					}
					else if (cntrl == 14) // out local volume
					{
						Compressor *compressor = (Compressor*)(mH->getModule(2000+channel*100+21));
						if (compressor != NULL) compressor->setOutGain2(value);						
					}
					else if (cntrl == 63) // set Reverb Distance
					{
						Delay *delL3 = (Delay *)(mH->getModule(2000+channel*100+16));
						Delay *delR3 = (Delay *)(mH->getModule(2000+channel*100+17));
						Delay *delL4 = (Delay *)(mH->getModule(2000+channel*100+18));
						Delay *delR4 = (Delay *)(mH->getModule(2000+channel*100+19));

						delL3->setInputBaseVal(delL3->AMP_INPUT, value * 2.0);
						delR3->setInputBaseVal(delR3->AMP_INPUT, value * 2.0);
						delL4->setInputBaseVal(delL4->AMP_INPUT, (1.0 - value) * 2.0);
						delR4->setInputBaseVal(delR4->AMP_INPUT, (1.0 - value) * 2.0);
					}					
					else
					{
						//if (debug_counter > COUNTER) MessageBox(NULL,"else clause","ok",MB_OK);

						for(int voice = 1;voice<=*(nVoices+channel);voice++)
						{
							
							SCSP = (SCSPlayer *)(mH->getModule(100*channel+10*voice+5));
							if (SCSP != NULL)
							{
								SCSP->changeController(cntrl, value*2.0 - 1.0);
							}

						}
					}

				}
				eventSize = 3;
			}
			else if (type == 0xE0) // pitch wheel
			{
				if (channel != 9) // not the drum channel
				{			

					dup_int16 pitchVal = *(restData+0) + (*(restData+1)-64)*128;

					for(int voice = 1;voice<=*(nVoices+channel);voice++)
					{
						SCSP = (SCSPlayer *)(mH->getModule(100*channel+10*voice+5));
						if (SCSP != NULL) SCSP->setWheel(pitchVal/4096.0);
					}
				}
				eventSize = 3;
			}
			else // not done yet, SYSEX?
			{
				eventSize = 3;
				
			}
		}

		//if (debug_counter > COUNTER) MessageBox(NULL,"event2","ok",MB_OK);

		//MessageBox(NULL, "midi data handled","ok",MB_OK);

		return eventSize;
	}
};