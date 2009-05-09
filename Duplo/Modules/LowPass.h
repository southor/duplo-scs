
#ifndef _LOW_PASS_
#define _LOW_PASS_

#include "Dup_Module.h"
#include "Dup_WaveFile.h"



class LowPass : public Dup::Dup_Module
{
	

private:
	

	dup_val inputL;
	dup_val inputR;

	dup_val outputL;
	dup_val outputR;

	dup_val *bufferL;
	dup_val *bufferR;

	dup_val bufferSumL;
	dup_val bufferSumR;

	dup_uint32 bufferSize;

	dup_uint32 head;

	bool on;

	void setPortsZero()
	{
		inputL = 0.0;
		inputR = 0.0;
		outputL = 0.0;
		outputR = 0.0;
	}

public:

	static const dup_uint32 PATHID = 114;


	static const dup_uint16 N_INPUTS = 2;
	static const dup_uint16 N_OUTPUTS = 2;

	/*
	 * constants for determining at
	 *  which position a certain output
	 *  is placed
	 */
	static const dup_uint16 Y_INPUT_L = 0;
	static const dup_uint16 Y_INPUT_R = 1;	
	static const dup_uint16 Y_OUTPUT_L = 0;
	static const dup_uint16 Y_OUTPUT_R = 1;	
	
	LowPass(Dup::ModuleHandler *moduleH, dup_uint32 id, Dup::MessageParser msgParser) :
	Dup_Module(moduleH,
		id,
		N_INPUTS,
		N_OUTPUTS)
	{		
		setPortsZero();
		
		// set input and output
		setInputDestination(Y_INPUT_L, &inputL);
		setInputDestination(Y_INPUT_R, &inputR);
		setOutputSourcePointer(Y_OUTPUT_L, &outputL);
		setOutputSourcePointer(Y_OUTPUT_R, &outputR);
		

		msgParser.getData(&bufferSize, sizeof(bufferSize));
		if (bufferSize < 1) bufferSize = 1;
		bufferL = new dup_val[bufferSize];
		bufferR = new dup_val[bufferSize];
		
		head = 0;

		on = true;

		for(int i=0;i<bufferSize;i++)
		{
			*(bufferL+i) = 0.0;
			*(bufferR+i) = 0.0;
		}
		bufferSumL = 0.0;
		bufferSumR = 0.0;
	}

	virtual void run()
	{
		bufferSumL = bufferSumL + inputL - *(bufferL + head);
		bufferSumR = bufferSumR + inputR - *(bufferR + head);
		*(bufferL + head) = inputL;
		*(bufferR + head) = inputR;
		outputL = bufferSumL/bufferSize;
		outputR = bufferSumR/bufferSize;
		head = (head+1)%bufferSize;
	}

};


#endif