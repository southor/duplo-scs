
#ifndef _TRI_SAW_OSCILLATOR_
#define _TRI_SAW_OSCILLATOR_

#include "BaseOscillator1.h"
#include "Dup_Curves.h"


class TriSawOscillator : public BaseOscillator1
{
private:

	// input
	dup_val sym;

	// fre change handling
	dup_pos changePos;
	dup_val changeVal;
	bool freChanged;
	dup_val oldFre;


public:

	static const dup_int16 PATHID = 107;


	static const dup_uint16 N_INPUTS = 2;
	static const dup_uint16 N_OUTPUTS = 1;

	static const dup_uint16 SYM_INPUT = 1;

	TriSawOscillator(Dup::ModuleHandler *moduleH, dup_uint32 id, Dup::MessageParser msgParser) :
	BaseOscillator1(moduleH,
		id,
		N_INPUTS,
		N_OUTPUTS)
	{
		sym = 0.0;

		changePos = 0.0;
		changeVal = 0.0;
		freChanged = false;
		oldFre = fre;
		
		// set input and output
		setInputDestination(SYM_INPUT, &sym);
		setInputBaseVal(SYM_INPUT, 0.0);
	}

	virtual void run()
	{
				
		if (fre != oldFre) // fre changed
		{
			freChanged = true;
			changePos = Dup::timePos;
			changeVal = out;
		}

		if (freChanged)
		{
			dup_val newOut = Dup::trisaw(phasePos(), sym);
			dup_val changeCurve = (Dup::timePos-changePos)*1000.0;
			
			if (changeVal >= newOut)
			{
				if ((changeVal-changeCurve) < newOut) // passed!
				{
					freChanged = false;
					out = newOut;
					
				}
				else
				{
					out = changeVal-changeCurve;
				}
			}
			else
			{
				if ((changeVal+changeCurve) >= newOut) // passed!
				{
					freChanged = false;
					out = newOut;
					return;
				}
				else
				{
					out = changeVal+changeCurve;
				}
			}
			
			
		}
		else
		{
			out = Dup::trisaw(phasePos(), sym);
		}

		oldFre = fre;
	}

};


#endif