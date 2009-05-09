#ifndef _HIHAT_
#define _HIHAT_

#include "Curves.h"
#include "Standards.h"

class HiHat : public StandardInstrument
{
protected:

public:

	double Mod;

	HiHat() : StandardInstrument()
	{
		Mod = 50; // attack hitlength
	}

	bool Run()
	{
		TimePos = TimePos+TimeStep;

			Amp = Fall2(TimePos*Mod)*(Vobl1(TimePos*10)-0.5);
			
			if (TimePos > 1/Mod)
			{
				Amp = Amp*Fall1((TimePos-1/Mod)*20.0);
			}
			
			if (TimePos > 1/Mod+1/20.0)
			{
				Playing = false;
			}
			//cout << "Amp " << Amp;

		return Playing;
	}
	
	void NoteOff()
	{
	}
};

#endif