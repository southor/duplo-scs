
#ifndef _DUP_MIDI_FILE_PARSER_
#define _DUP_MIDI_FILE_PARSER_

#include "MidiEventReceiver.h"
#include "ModuleHandler.h"

namespace Dup
{

	class MidiFileParser
	{	
	protected:
		MidiEventReceiver *midiReceiver;
		ModuleHandler *mH;
		dup_moduleid mHID;

		int WaveFre;

		bool printMessages;
		unsigned int systemCommon[7];

		unsigned char *fileData;
		unsigned int length;

		char *errorString;
		unsigned char *parse;
		unsigned char *startPos;
		__int32 trackLength;
		unsigned short division;

		bool tempoSet;
		__int32 currentTime;

		int nChannels;

		// used by channelMessageParser
		int channel;
		int channelEventType;
	public:

		MidiFileParser(int WaveFre, int nChannels, MidiEventReceiver *midiReceiver, unsigned char *fileData, unsigned int length, bool showMessages, ModuleHandler *mH, dup_moduleid mHID)
		{
			this->midiReceiver = midiReceiver;
			this->mH = mH;
			this->mHID = mHID;
		
			this->WaveFre = WaveFre;
			this->printMessages = printMessages;
			this->fileData = NULL;
			this->length = 0;
			this->errorString = NULL;
			this->tempoSet = false;

			changeFile(fileData, length);
			this->currentTime = 0;
			 
			this->nChannels = nChannels;

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

		bool parseMidiEvent()
		{
			if (*parse >= 0xF8) // system Real-Time
			{
				std::cout << "--   system real-time: " << std::endl;
				//wait();
				
				parse++;
				return true;
			}
			else if (*parse >= 0xF1) // system common
			{
				std::cout << "--   system common: " << std::endl;
				//wait();
				
				parse = parse + systemCommon[*parse - 0xF1];
				return true;
			}		
			else // normal message
			{
				int size = midiReceiver->receiveEvent(parse, 0, nChannels);
				parse = parse+size;
				return (size>0);
			}
			

		}

		void parseMTrkEventTime()
		{
			unsigned int deltaTime = parseVLQ(parse);
			currentTime = currentTime+deltaTime;
		}

		bool parseMTrkEvent()
		{
			parseMTrkEventTime();
			if (needParseMore()) return parseMTrkEventContinue();			
			else return true;
		}

		bool parseMTrkEventContinue()
		{

			if ((*(parse) == 0xF0) || (*(parse) == 0xF7)) // sysex event
			{
				std::cout << "--   found some sysex: " << std::endl;
				//wait();

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
				return parseMidiEvent();			
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
				
				
				//if (!tempoSet)// old code
				if (true) // new code
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


					dup_pos newBPM = Tempo;
					MessageConstructor msgC(sizeof(dup_uint8)+sizeof(newBPM));
					msgC.addData8(0x9F);
					msgC.addData(&newBPM, sizeof(newBPM));				
					mH->postMessage(0.0, mHID, sizeof(newBPM)+sizeof(dup_uint8), msgC.getMessage(), msgC.getFormat());
	

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
				//wait();
							
			}
			
			parse = parse+dataLength;
			return true;
		}

		bool parseTrackChunk()
		{
			tempoSet = false;
			trackLength = 0;

			if (memcmp((char *)parse, (char *)"MTrk", 4) != 0)
			{
				setErrorString("Error in *parseTrackChunk*: Track Chunk Type of wrong type.");
				return false;
			}
			else
			{
				
				swapCopy(parse+4, (unsigned char *)(&trackLength), 4);			
				parse = parse+8;
				
				unsigned char *startPos = parse;
				return parseMore();

			}
		}

		bool parseContinue()
		{
			if(fileEnd() || (!needParseMore())) return true;
			else
			{
				if (parseMTrkEventContinue()) return parseMore();
				else return false;
			}
		}

		bool needParseMore()
		{
			return (currentTime <= calcMidiTime(beatPos));
		}

		bool parseMore()
		{
			while((!fileEnd()) && needParseMore())
			{
				//std::cout << "new MTrkEvent at:" << (int)(parse-startPos) << std::endl;
				if (!parseMTrkEvent()) return false;
			}			
			return true;
		}

		bool fileEnd()
		{

			return (parse >= startPos+trackLength);
		}

		__int32 calcMidiTime(dup_pos beatPos)
		{
			return beatPos*((dup_pos)division);
		}

		bool parseHeaderChunk()
		{
			printMessage("parsing header file");

			if (memcmp((char *)parse, (char *)"MThd", 4) != 0)
			{
				setErrorString("Error in *parseHeaderChunk*: Header Chunk Type of wrong type.");
				//setErrorString((char *)parse);
				return false;
			}
			else if (*(parse+7) != 0x06)
			{
				setErrorString("Error in *parseHeaderChunk*: Header Chunk Length of wrong number.");
				return false;
			}
			else if (*(parse+9) != 0x00)
			{
				setErrorString("Error in *parseHeaderChunk*: Midifile of wrong format.");
				return false;
			}
			else if (*(parse+11) != 0x01)
			{
				setErrorString("Error in *parseHeaderChunk*: Midifile must have exactly one track.");
				return false;
			}
			else if ((*(parse+12) | 0x80) == (*(parse+12)))
			{			
				setErrorString("Error in *parseHeaderChunk*: division must be of format ticks per quarter-note.");
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
					setErrorString("Error in *parseHeaderChunk*: ticks per quarter-note is set to zero!");
					return false;
				}
				else
				{
					this->division = division;
					std::cout << "value of division: " << division << std::endl;				
					//wait();
					parse = parse + 14;
					return true;
				}
			}
		}


		
		bool changeFile(unsigned char *fileData, unsigned int length)
		{		
			if (fileData == NULL)
			{
				setErrorString("Error in *changeFile*: FileData pointer passed to changeFile was null!");
				return false;
			}
			else
			{					
				this->fileData = fileData;
				this->length = length;
				return true;
			}
		}

		bool parseFile()
		{
			currentTime = 0;
			
			if (fileData == NULL)
			{
				setErrorString("Error in *parseFile*: FileData pointer is null!");
				return false;
			}
			if (length <= 0)
			{
				setErrorString("Error in *parseFile*: FileLength was <= 0");
				return false;
			}
			else
			{
				parse = fileData;
				if (!parseHeaderChunk()) return false;
				if(!parseTrackChunk()) return false;				
				return true;				
				
				
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
				delete[] errorString;
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

		void setErrorString(const char *string)
		{
			resetErrorString();
			int stringLength = strlen(string);
			errorString = new char[stringLength+5];
			strcpy(errorString, string);
		}


		void printMessage(char *message)
		{
			if (printMessages)
			{
				std::cout << message << std::endl;
			}
		}

	};
};

#endif