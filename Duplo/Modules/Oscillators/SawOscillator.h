
#ifndef _SAW_OSCILLATOR_
#define _SAW_OSCILLATOR_

#include "BaseOscillator1.h"
#include "Dup_Curves.h"

class SawOscillator : public BaseOscillator1
{
public:

	static const dup_int16 PATHID = 104;


	static const dup_uint16 N_INPUTS = 1;
	static const dup_uint16 N_OUTPUTS = 1;

	SawOscillator(Dup::ModuleHandler *moduleH, dup_uint32 id, Dup::MessageParser msgParser) :
	BaseOscillator1(moduleH,
		id,
		N_INPUTS,
		N_OUTPUTS)
	{
	}

	virtual void run()
	{
		out = Dup::saw(phasePos());		

	}
};


#endif