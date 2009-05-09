
#ifndef _SIMPLE_DIRECT_SOUND_
#define _SIMPLE_DIRECT_SOUND_

#include "Dup_Module.h"



//static const dup_val VAL_SCALE = 1.0;

class SimpleDirectSound : public Dup::Dup_Module
{
	

private:

	double vol;

	dup_val inputL;
	dup_val inputR;

	void *outputL;
	void *outputR;

	dup_uint32 bufferSize;

	int interval;

	long writeHead;

	void setPortsZero()
	{
		inputL = 0.0;
		inputR = 0.0;
	}

public:

	static const dup_uint32 PATHID = 105;

	static const dup_uint16 N_INPUTS = 2;
	static const dup_uint16 N_OUTPUTS = 0;

	/*
	 * constants for determining at
	 *  which position a certain output
	 *  is placed
	 */
	static const dup_uint16 WAVE_INPUT_L = 0;
	static const dup_uint16 WAVE_INPUT_R = 1;
	
	SimpleDirectSound(Dup::ModuleHandler *moduleH, dup_uint32 id, Dup::MessageParser msgParser, void *outputL, void *outputR) :
	Dup_Module(moduleH,
		id,
		N_INPUTS,
		N_OUTPUTS)
	{		
		this->bufferSize = bufferSize;
		this->interval = interval;
		setPortsZero();
		
		// set input and output
		setInputDestination(WAVE_INPUT_L, &inputL);
		setInputDestination(WAVE_INPUT_R, &inputR);



		msgParser.getData(&bufferSize, sizeof(bufferSize));
		msgParser.getData(&interval, sizeof(interval));
		
		writeHead = 0;


		this->outputL = outputL;
		this->outputR = outputR;

		vol = 1.0;
	}

	void setVol(double vol)
	{
		this->vol = vol;
	}

	double getVol()
	{
		return vol;
	}

	virtual void run()
	{
		
		dup_int32 lVali = (((double)DUP_INT16_MAX)*vol*((double)inputL))/1.0;
		dup_int32 rVali = (((double)DUP_INT16_MAX)*vol*((double)inputR))/1.0;

		if (lVali >= DUP_INT16_MAX) lVali = DUP_INT16_MAX-1;
		else if (lVali <= DUP_INT16_MIN) lVali = DUP_INT16_MIN+1;

		if (rVali >= DUP_INT16_MAX) rVali = DUP_INT16_MAX-1;
		else if (rVali <= DUP_INT16_MIN) rVali = DUP_INT16_MIN+1;

		dup_int16 lVal = lVali;
		dup_int16 rVal =rVali;
		
		*(((dup_int16 *)outputL)+(writeHead*interval)%(bufferSize*interval)) = lVal;
		*(((dup_int16 *)outputR)+(writeHead*interval)%(bufferSize*interval)) = rVal;
		
		
		
		
		writeHead++;
	}

};


#endif