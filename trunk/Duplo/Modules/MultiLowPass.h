
#ifndef _MULTI_LOW_PASS_
#define _MULTI_LOW_PASS_

#include "Dup_Module.h"
#include "Dup_WaveFile.h"



class MultiLowPass : public Dup::Dup_Module
{
	

private:
	

	dup_val inputL;
	dup_val inputR;

	dup_val outputL;
	dup_val outputR;

	dup_val *bufferL;
	dup_val *bufferR;

	dup_val *bufferSumL;
	dup_val *bufferSumR;

	dup_uint32 order;

	dup_uint32 bufferSize;

	//dup_uint32 currentBufferSize;

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

	static const dup_uint32 PATHID = 115;


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
	
	MultiLowPass(Dup::ModuleHandler *moduleH, dup_uint32 id, Dup::MessageParser msgParser) :
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

		bufferL = NULL;
		bufferR = NULL;
		bufferSumL = NULL;
		bufferSumR = NULL;

		reset(msgParser);

		
		
		
		

		

		//currentBufferSize = bufferSize;
	}

	void reset(Dup::MessageParser msgParser)
	{
		delete bufferL;
		delete bufferR;
		delete bufferSumL;
		delete bufferSumR;

		setPortsZero();

		msgParser.getData(&bufferSize, sizeof(bufferSize));
		msgParser.getData(&order, sizeof(order)); // WTF

		//order = 16; // WTF

		if (order < 1) order = 1;
		if (order > 4) order = 4;
		if (bufferSize < 1) bufferSize = 1;
		bufferL = new dup_val[bufferSize];
		bufferR = new dup_val[bufferSize];
		
		head = bufferSize;

		on = true;

		for(int i=0;i<bufferSize;i++)
		{
			*(bufferL+i) = 0.0;
			*(bufferR+i) = 0.0;
		}
		
		bufferSumL = new dup_val[order];
		bufferSumR = new dup_val[order];
		
		for(int i=0;i<order;i++)
		{
			*(bufferSumL+i) = 0.0;
			*(bufferSumR+i) = 0.0;
		}
	}

	virtual void run()
	{
		//// a local "bufferSize"
		//dup_uint32 bufferSize = (head > this->currentBufferSize) ? this->bufferSize; : this->currentBufferSize;
		
		dup_uint32 orderSize;
		
		outputL = 0;
		outputR = 0;
		for(int i=1;i<=order;i++)
		{
			orderSize = (2*bufferSize)/(i+order);
			
			*(bufferSumL+i) = *(bufferSumL+i) + inputL - *(bufferL + (head + orderSize)%bufferSize);
			*(bufferSumR+i) = *(bufferSumR+i) + inputR - *(bufferR + (head + orderSize)%bufferSize);
		
			outputL = outputL + (*(bufferSumL+i)/bufferSize)/order;
			outputR = outputR + (*(bufferSumR+i)/bufferSize)/order;
		}
		
		*(bufferL + head) = inputL;
		*(bufferR + head) = inputR;
		
		head = (head+1)%bufferSize;		
	}

	//void setCurrentBufferSize(dup_uint32 currentBufferSize)
	//{
	//	if (currentBufferSisze < 1) this->currentBufferSize = 1;
	//	else if (currentBufferSisze > bufferSize) this->currentBufferSize = bufferSize;
	//	else this->currentBufferSize = currentBufferSize;
	//}

};


#endif