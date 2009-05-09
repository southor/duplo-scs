#ifndef _GONG1_
#define _GONG1_

#include "Curves.h"
#include "Standards.h"

class Gong1 : public Instrument
{
public:
	double Amp;
	double Pos;

	double PlayPos;

	Gong1() : Instrument()
	{
		Amp = 0;
		Pos = 0;
	}

	void NoteOn()
	{
		PlayPos = 0;
		Playing = true;
		Amp = 0;
		Pos = 0;
	}

	bool Run()
	{
		PlayPos = PlayPos+PlayStep;
		double Env = Shot1(PlayPos*30);
		Pos = Pos+TimeStep*Fre;
			
		Amp = rSin(Pos*(rSin(Pos)*Env*0.01+1)/3.0)*SoftSqr(Pos/5.0)*Env;
		
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