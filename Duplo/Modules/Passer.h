
#ifndef _DUP_PASSER_
#define _DUP_PASSER_

#include "Dup_Module.h"
#include "Dup_Curves.h"
#include "Dup_Base.h"
#include "Dup_Func.h"

// Passes input without transforming it (except for amp)
class Passer : public Dup::Dup_Module
{
private:

	// input/output
	dup_val in;
	dup_val amp;
	dup_val out;

public:

	static const dup_int16 PATHID = 116;


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

	
	Passer(Dup::ModuleHandler *moduleH, dup_uint32 id, Dup::MessageParser msgParser) :
	Dup_Module(moduleH,
		id,
		N_INPUTS,
		N_OUTPUTS)
	{		
		in = 0.0;
		out = 0.0;
		amp = 0.0;

		// set input and output
		setInputDestination(Y_INPUT, &in);		
		setInputDestination(AMP_INPUT, &amp);
		setOutputSourcePointer(Y_OUTPUT, &out);

	}

	virtual void run()
	{		
		out = in * amp;		
	}
};



#endif