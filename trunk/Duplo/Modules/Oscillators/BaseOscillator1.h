
#ifndef _BASE_OSCILLATOR_1_
#define _BASE_OSCILLATOR_1_

#include "Dup_Module.h"
#include "Dup_Curves.h"
#include "Dup_Base.h"

//using namespace Dup;

class BaseOscillator1 : public Dup::Dup_Module
{
	

protected:



	dup_val fre;
	dup_val out;

public:

	/*
	 * constants for determining at
	 *  which position a certain output
	 *  is placed
	 */
	static const dup_uint16 FRE_INPUT = 0;
	static const dup_uint16 Y_OUTPUT = 0;

	
	BaseOscillator1(Dup::ModuleHandler *moduleH, dup_uint32 id,
					dup_uint32 nInputs,
					dup_uint32 nOutputs) :
	Dup_Module(moduleH,
		id,
		nInputs,
		nOutputs)
	{		
		out = 0.0;
		fre = 1.0;
		
		// set input and output
		if (nInputs >= 1)
		{
			setInputDestination(FRE_INPUT, &fre);
			setInputBaseVal(FRE_INPUT, 0.0);
		}
		if (nOutputs >= 1) setOutputSourcePointer(Y_OUTPUT, &out);		

	}

	dup_val getFre()
	{
		return fre;
	}

	dup_pos phasePos()
	{
		return Dup::timePos*fre;
	}
};


#endif