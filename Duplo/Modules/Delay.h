
#ifndef _DUP_DELAY_
#define _DUP_DELAY_

#include "Dup_Module.h"
#include "Dup_Curves.h"
#include "Dup_Base.h"
#include "Dup_Func.h"


class Delay : public Dup::Dup_Module
{
private:

		
	// buffer
	dup_val *buffer;
	dup_uint32 bufferSize;
	dup_uint32 writePos;

	// input/output
	dup_val in;
	dup_val amp;
	dup_val out;

public:

	static const dup_int16 PATHID = 108;


	static const dup_uint16 N_INPUTS = 2;
	static const dup_uint16 N_OUTPUTS = 1;

	/*
	* constants for determining at
	*  which position a certain output or output
	*  is placed
	*/
	static const dup_uint16 Y_INPUT = 0;
	static const dup_uint16 AMP_INPUT = 1;
	static const dup_uint16 Y_OUTPUT = 0;

	
	Delay(Dup::ModuleHandler *moduleH, dup_uint32 id, Dup::MessageParser msgParser) :
	Dup_Module(moduleH,
		id,
		N_INPUTS,
		N_OUTPUTS)
	{		
		in = 0.0;
		out = 0.0;
		amp = 0.0;

		writePos = 0;
		msgParser.getData(&bufferSize, sizeof(bufferSize));
		bufferSize;
		buffer = new dup_val[bufferSize];		
		
		//Dup::clearMemory(buffer, bufferSize*sizeof(dup_val));
		// clears all samples
		for(int i=0;i<bufferSize;i++)
		{
			*(buffer+i) = 0.0;
		}


		// set input and output
		setInputDestination(Y_INPUT, &in);		
		setInputDestination(AMP_INPUT, &amp);
		setOutputSourcePointer(Y_OUTPUT, &out);

	}

	virtual void run()
	{		
		
		out = (*(buffer + writePos)) * amp;
		*(buffer + writePos) = in;
		if (writePos < bufferSize) writePos++;
		else writePos = 0;	
	}
};



#endif