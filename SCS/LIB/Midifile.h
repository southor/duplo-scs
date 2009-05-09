

#ifndef _MIDIFILE_
#define _MIDIFILE_

#include "Base.h"
#include "MidiInport.h"
#include "fstream"
#include "iostream"
#include "StrFunc.h"


static const bool WAIT = false;

void wait()
{
	if(WAIT)
	{
		char wait[256];
		std::cout << "press a key and enter " << std::endl;
		std::cin >> wait;
	}
}



struct KeyStatus
{
	__int32 pressedAt; // time
	bool pressed; // current pressed or not
	int velocity; // attack velocity
	int vol; // volume
	int pan; // pan
};


// can handle midifile format 0
// but only load
class MidiFile
{
protected:

	int WaveFre;

	bool printMessages;
	unsigned int systemCommon[7];

	KeyStatus keyStatus[16][128];

	unsigned char *fileData;
	unsigned int length;

	char *errorString;
	unsigned char *parse;
	unsigned short division;

	bool tempoSet;
	__int32 currentTime;


	// used by channelMessageParser
	int channel;
	int channelEventType;


public:


	MidiFile(int WaveFre, unsigned char *fileData, unsigned int length, bool printMessages)
	{		
		this->WaveFre = WaveFre;
	
		this->printMessages = printMessages;
		this->fileData = NULL;
		this->length = 0;
		this->errorString = NULL;
		this->tempoSet = false;

		changeFile(fileData, length);
		this->currentTime = 0;
			
		for(int i=0;i<16;i++)
		{
			for(int j=0;j<128;j++)
			{
				keyStatus[i][j].pressed = false;
				keyStatus[i][j].pressedAt = 0;
				keyStatus[i][j].velocity = 0;
				keyStatus[i][j].vol = 0;
				keyStatus[i][j].pan = 0;
				
			}
		}
		 
		

		systemCommon[0] = 1;
		systemCommon[1] = 2;
		systemCommon[2] = 1;
		systemCommon[3] = 0;
		systemCommon[4] = 0;
		systemCommon[5] = 0;
		systemCommon[6] = 0;

	}



	/*
	 * copys data and swaps the bytes
	 */
	void swapCopy(unsigned char *from, unsigned char *to, int l)
	{
		for(int i=0;i<l;i++)
		{
			*((unsigned char*)(to+l-(i+1))) = *(from+i);
		}		
	}


	/*
	 * translates any number saved in variable length
	 * quantity representation to normal form.
	 */
	unsigned int parseVLQ(unsigned char *num)
	{
		
		if ((0x80 | *num) == *num)
		{			
			if ((0x80 | *(num+1)) == *(num+1))
			{
				if ((0x80 | *(num+2)) == *(num+2))
				{
					parse = parse+4;
					return ((*num) & 0x7F)*128*128*128 +
						((*(num+1)) & 0x7F)*128*128 +
						((*(num+2)) & 0x7F)*128 +
						(*(num+3));
					
				}
				else
				{
					parse = parse+3;
					return ((*num) & 0x7F)*128*128 +
						((*(num+1)) & 0x7F)*128 +
						(*(num+2));
				}
			}
			else
			{
				parse = parse+2;
				return ((*num) & 0x7F)*128 +
					(*(num+1));
			}
		}		
		else
		{
			parse = parse+1;
			return *num;
		}
		
	}

	bool parseNewChannelMessage(MidiInport *midiInport)
	{	
		channel = (*parse) & 0x0F;		
		channelEventType = *parse;

		parse++;

		return parseSameChannelMessage(midiInport);

	}

	void processNoteOff(MidiInport *midiInport, int data1, int data2)
	{

		if (keyStatus[channel][data1].pressed)
		{
			int tmpVol = midiInport->getVol(channel);
			int tmpPan = midiInport->getPan(channel);
			
			midiInport->setVol(channel, keyStatus[channel][data1].vol);
			midiInport->setPan(channel, keyStatus[channel][data1].pan);
			keyStatus[channel][data1].pressed = false;
			
			double startPos = ((double)(keyStatus[channel][data1].pressedAt))/(((double)division)*4.0);
			double playLength = ((double)(currentTime-keyStatus[channel][data1].pressedAt))/(((double)division)*4.0);
			midiInport->playNote(channel,
								data1,
								keyStatus[channel][data1].velocity,
								startPos,
								playLength);
			midiInport->setVol(channel, tmpVol);
			midiInport->setPan(channel, tmpPan);
		}			

	}


	bool parseSameChannelMessage(MidiInport *midiInport)
	{		
				
		int data1 = *parse;
		int data2;

		if (channelEventType < 0x90) // Note Off
		{
			int data2 = *(parse+1);

			processNoteOff(midiInport, data1, data2);

					
			parse = parse+2;
		}
		else if (channelEventType < 0xA0) // Note On
		{
			
			
			data2 = *(parse+1);
			
			if (data2 == 0)
			{
				//std::cout << "found note off" << std::endl;
				
				processNoteOff(midiInport, data1, data2);
			}
			else
			{
				//std::cout << "found note on" << std::endl;
				
				keyStatus[channel][data1].pressed = true;
				keyStatus[channel][data1].pressedAt = currentTime;
				keyStatus[channel][data1].velocity = data2;
				keyStatus[channel][data1].vol = midiInport->getVol(channel);
				keyStatus[channel][data1].pan = midiInport->getPan(channel);
			}

			parse = parse+2;
		}
		else if (channelEventType < 0xB0) // poly key pressure
		{
			data2 = *(parse+1);
			parse = parse+2;
		}
		else if (channelEventType < 0xC0) // control change
		{
			data2 = *(parse+1);
			
			if (data1 == 0x07)			// volume change
			{
				midiInport->setVol(channel, data2);
			}
			else if (data1 == 0x0A)	// pan change
			{
				
				midiInport->setPan(channel, data2);
				
			}
			else
			{
				std::cout << "--   unknown control change: " << std::endl;
				wait();
			}
			
			parse = parse+2;
		}
		else if (channelEventType < 0xD0) // program change
		{
			parse = parse+1;
		}
		else if (channelEventType < 0xE0) // channel pressure
		{	
			parse = parse+1;
		}
		else if (channelEventType < 0xF0) // pitch wheel change
		{
			data2 = *(parse+1);
			parse = parse+2;
		}
		
		return true;
	}

	bool parseMidiEvent(MidiInport *midiInport)
	{
		if (*parse >= 0xF8) // system Real-Time
		{
			std::cout << "--   system real-time: " << std::endl;
			wait();
			
			parse++;
			return true;
		}
		else if (*parse >= 0xF1) // system common
		{
			std::cout << "--   system common: " << std::endl;
			wait();
			
			parse = parse + systemCommon[*parse - 0xF1];
			return true;
		}		
		else if (*parse < 0x80) // same channel and type
		{			
			return parseSameChannelMessage(midiInport);
		}
		else
		{
			return parseNewChannelMessage(midiInport);
		}
		

	}


	

	bool parseMTrkEvent(MidiInport *midiInport)
	{
		unsigned int deltaTime = parseVLQ(parse);
		currentTime = currentTime+deltaTime;

		if ((*(parse) == 0xF0) || (*(parse) == 0xF7)) // sysex event
		{
			std::cout << "--   found some sysex: " << std::endl;
			wait();

			parse++;
			parse = parse + parseVLQ(parse);
			return true;
		}
		else if (*(parse) == 0xFF) // meta event
		{
			return parseMetaEvent();			
			
		}
		else // normal midi event
		{
			parseMidiEvent(midiInport);			
		}
		
	}


	bool parseMetaEvent()
	{
		__int8 type = *(parse+1);
		
		
		// jump to <length>
		parse = parse+2;
		
		
		unsigned int dataLength = parseVLQ(parse);
		

		if (type == 0x51) // tempo change
		{
						
			unsigned long msq;
						
			if (!tempoSet)
			{
				std::cout << "--   tempo before: " << Tempo << std::endl;
				
				msq = 0;
				swapCopy(parse, (unsigned char *)(&msq), dataLength);
				
				std::cout << "-- tempochange --" << std::endl;
				std::cout << "--   dataLength: " <<  dataLength << std::endl;
				std::cout << "--   msq: " <<  msq << std::endl;
				
				

				Tempo = 60000000.0/((double)msq);
				PlayStep = CovFunc(1/(double)WaveFre,Tempo);
				TimeStep = 1.0/WaveFre;

				

				std::cout << "--   tempo after: " << Tempo << std::endl;
				tempoSet = true;
			}			
			
			
		}
		else
		{
			std::cout << "--   found some meta: " << (unsigned short)type << std::endl;
			
			for(int i = 0;i<dataLength;i++)
			{
				std::cout << "      number: " << (unsigned short)(*(parse+i))  << "     character: " << *(parse+i) << std::endl;
			}
			wait();
						
		}
		
		parse = parse+dataLength;
		return true;
	}

	bool parseTrackChunk(MidiInport *midiInport)
	{
		tempoSet = false;
		__int32 trackLength = 0;

		if (!eqchararray((char *)parse, "MTrk", 4))
		{
			errorString = copystring("Error in *parseTrackChunk*: Track Chunk Type of wrong type.");
			return false;
		}
		else
		{
			
			

			swapCopy(parse+4, (unsigned char *)(&trackLength), 4);			
			parse = parse+8;
			
			unsigned char *startPos = parse;			
			while(parse < startPos+trackLength)
			{
				//std::cout << "new MTrkEvent at:" << (int)(parse-startPos) << std::endl;
				if (!parseMTrkEvent(midiInport)) return false;				
			}
			return true;
			
		}
	}

	bool parseHeaderChunk()
	{
		printMessage("parsing header file");

		if (!eqchararray((char *)parse, "MThd", 4))
		{
			errorString = copystring("Error in *parseHeaderChunk*: Header Chunk Type of wrong type.");
			return false;
		}
		else if (*(parse+7) != 0x06)
		{
			errorString = copystring("Error in *parseHeaderChunk*: Header Chunk Length of wrong number.");
			return false;
		}
		else if (*(parse+9) != 0x00)
		{
			errorString = copystring("Error in *parseHeaderChunk*: Midifile of wrong format.");
			return false;
		}
		else if (*(parse+11) != 0x01)
		{
			errorString = copystring("Error in *parseHeaderChunk*: Midifile must have exactly one track.");
			return false;
		}
		else if ((*(parse+12) | 0x80) == (*(parse+12)))
		{			
			errorString = copystring("Error in *parseHeaderChunk*: division must be of format ticks per quarter-note.");
			return false;
		}
		else
		{		

			unsigned short division;

			// swap bytes, system should use LSB
			swapCopy(parse+12, (unsigned char *)(&division), 2);
			division = division & 0x7FFF;

			if (division == 0)
			{
				errorString = copystring("Error in *parseHeaderChunk*: ticks per quarter-note is set to zero!");
				return false;
			}
			else
			{
				this->division = division;
				std::cout << "value of division: " << division << std::endl;				
				wait();
				parse = parse + 14;
				return true;
			}
		}



	}


	
	bool changeFile(unsigned char *fileData, unsigned int length)
	{		
		if (fileData == NULL)
		{
			errorString = copystring("Error in *changeFile*: FileData pointer passed to changeFile was null!");
			return false;
		}
		else
		{					
			this->fileData = fileData;
			this->length = length;
			return true;
		}
	}

	bool parseFile(MidiInport *midiInport)
	{
		currentTime = 0;
		

		if (fileData == NULL)
		{
			errorString = copystring("Error in *parseFile*: FileData pointer is null!");
			return false;
		}
		if (length <= 0)
		{
			errorString = copystring("Error in *parseFile*: FileLength was <= 0");
			return false;
		}
		else if (midiInport == NULL)
		{
			errorString = copystring("Error in *parseFile*: midiInport pointer passed to parseFile was null!");
			return false;
		}
		else
		{
			parse = fileData;
			if (!parseHeaderChunk()) return false;
			else if(!parseTrackChunk(midiInport)) return false;
			else
			{
				return true;				
			}
			
		}
	}




	/*
	 * returns true if error String was set
	 */
	bool resetErrorString()
	{
		if (errorString == NULL)
		{
			return false;
		}
		else
		{
			errorString = NULL;
			return true;
		}
	}

	char *getErrorString()
	{
		return errorString;
	}

	/*
	 * returns true if error String is set
	 */
	bool errorStringExists()
	{
		if (errorString == NULL) return false;
		else return true;
	}


	void printMessage(char *message)
	{
		if (printMessages)
		{
			std::cout << message << std::endl;
		}
	}

};

#endif