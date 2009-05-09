
#ifndef _STEREO_WAVE_BUFFER_
#define _STEREO_WAVE_BUFFER_

#include "Dup_Module.h"
#include "Dup_WaveFile.h"

//using namespace Dup;

static const dup_val VAL_SCALE = 1.0;


class StereoWaveBuffer : public Dup::Dup_Module
{
	

private:
	

	dup_val inputL;
	dup_val inputR;

	dup_val outputL;
	dup_val outputR;

	dup_int16 *bufferL;
	dup_int16 *bufferR;

	dup_val vol;

	dup_uint32 bufferSize;

	long writeHead;

	bool recording;

	void setPortsZero()
	{
		inputL = 0.0;
		inputR = 0.0;
		outputL = 0.0;
		outputR = 0.0;
	}

public:

	static const dup_uint32 PATHID = 101;


	static const dup_uint16 N_INPUTS = 2;
	static const dup_uint16 N_OUTPUTS = 2;

	/*
	 * constants for determining at
	 *  which position a certain output
	 *  is placed
	 */
	static const dup_uint16 WAVE_INPUT_L = 0;
	static const dup_uint16 WAVE_INPUT_R = 1;	
	static const dup_uint16 WAVE_OUTPUT_L = 0;
	static const dup_uint16 WAVE_OUTPUT_R = 1;	
	
	StereoWaveBuffer(Dup::ModuleHandler *moduleH, dup_uint32 id, Dup::MessageParser msgParser) :
	Dup_Module(moduleH,
		id,
		N_INPUTS,
		N_OUTPUTS)
	{		
		setPortsZero();
		
		// set input and output
		setInputDestination(WAVE_INPUT_L, &inputL);
		setInputDestination(WAVE_INPUT_R, &inputR);
		setOutputSourcePointer(WAVE_OUTPUT_L, &outputL);
		setOutputSourcePointer(WAVE_OUTPUT_R, &outputR);
		

		msgParser.getData(&bufferSize, sizeof(bufferSize));
		bufferL = new dup_int16[bufferSize];
		bufferR = new dup_int16[bufferSize];
		
		writeHead = 0;
		vol = 1.0;

		recording = false;
	}	

	virtual bool recieveMessage(dup_message *msg, int size, dup_uint8 format)
	{
		if (Dup::Dup_Module::receiveMessage(msg, size, format))
		{
			return true;
		}
		else
		{
			if (size <= 0)
			{
				return false;
			}
			else
			{
				
				Dup::MessageParser parser(msg, size, format);			
				dup_uint8 type;
				parser.getData(&type, sizeof(type));
				
				switch(type)
				{				
					case 0xB1: // stop
						stop();
					break;
					case 0xB2: // record
						record();
					break;
					case 0xB3: // rewind
						rewind();
					break;
					default:
						return false;
					break;
				}
				return true;
			}
			
		}
		
	}

	dup_val getVol()
	{
		return this->vol;
	}

	void setVol(dup_val vol)
	{
		this->vol = vol;
	}

	virtual void run()
	{
		if (recording && (writeHead < bufferSize))
		{
			//inputL = 0.8;

			*(bufferL+writeHead) = (((double)DUP_INT16_MAX)*vol*((double)inputL))/VAL_SCALE;
			*(bufferR+writeHead) = (((double)DUP_INT16_MAX)*vol*((double)inputR))/VAL_SCALE;
			outputL = inputL;
			outputR = inputR;
			writeHead++;

			//std::cout << inputL << std::endl;
		}
		
	}

	void record()
	{
		recording = true;
	}

	void stop()
	{
		recording = false;
		setPortsZero();
	}

	void rewind()
	{
		writeHead = 0;		
	}

	void saveToDisk(char* fileName, dup_int32 waveFre)
	{
		Dup::saveWave(fileName, waveFre, bufferSize, bufferL, bufferR);
	}

};


#endif