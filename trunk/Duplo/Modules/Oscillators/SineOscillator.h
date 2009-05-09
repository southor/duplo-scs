
#ifndef _SINE_OSCILLATOR_
#define _SINE_OSCILLATOR_

#include "ContPhaseOscillator1.h"

class SineOscillator : public ContPhaseOscillator1
{

public:

	static const dup_int16 PATHID = 102;


	static const dup_uint16 N_INPUTS = 1;
	static const dup_uint16 N_OUTPUTS = 1;
	
	SineOscillator(Dup::ModuleHandler *moduleH, dup_uint32 id, Dup::MessageParser msgParser) :
	ContPhaseOscillator1(moduleH,
		id,
		N_INPUTS,
		N_OUTPUTS)
	{
	}

	virtual void run()
	{
		checkFreChanged();
		out = Dup::sine(contPhasePos());
	}
};


#endif