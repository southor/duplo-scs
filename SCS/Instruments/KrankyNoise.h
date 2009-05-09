#ifndef _KRANKYNOISE_
#define _KRANKYNOISE_

#include "Standards.h"
#include "Curves.h"

class KrankyNoise : public Instrument
{
public:
	double Amp;
	double Pos;

	//double PlayPos;

	double Mod1, Mod2;

	KrankyNoise() : Instrument()
	{
		Amp = 0;
		Pos = 0;
		Mod1 = 2.0;
		Mod2 = 4.0;
	}

	void NoteOn()
	{
		//PlayPos = 0;
		Playing = true;
		Amp = 0;
		Pos = 0;
	}

	bool Run()
	{
		//PlayPos = PlayPos+PlayStep;
		//double Env = Shot(PlayPos*30);
		//double Env = 1.0;
		Pos = Pos+TimeStep*Fre;	
		Amp = rSin(Pos*(rSin(Pos*Mod1)/Mod2+1))*SoftSqr(Pos);
		
		return Playing;
	}

	void NoteOff()
	{
		Playing = false;
	}

	void End()
	{
	}

};

#endif