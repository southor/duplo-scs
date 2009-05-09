
#ifndef _CONT_PHASE_OSCILLATOR_1_
#define _CONT_PHASE_OSCILLATOR_1_

#include "Dup_Module.h"
#include "Dup_Curves.h"
#include "Dup_Base.h"

class ContPhaseOscillator1 : public BaseOscillator1
{

private:

	dup_val lastFre;
	dup_val posOffset;

public:

	ContPhaseOscillator1(Dup::ModuleHandler *moduleH, dup_uint32 id,
					dup_uint32 nInputs,
					dup_uint32 nOutputs) :
	BaseOscillator1(moduleH,
		id,
		nInputs,
		nOutputs)
	{	
		lastFre = 1.0;
		posOffset = 0.0;

	}

	bool checkFreChanged()
	{
		if (lastFre != fre) // fre changed!
		{
			posOffset = posOffset + Dup::loop(Dup::timePos*lastFre + posOffset, 0.0, 1.0)-Dup::loop(Dup::timePos*fre + posOffset, 0.0, 1.0);			
			lastFre = fre;
			return true;		
		}
		lastFre = fre;
		return false;
	}

	dup_pos contPhasePos()
	{
		return Dup::timePos*fre + posOffset;
	}
};


#endif