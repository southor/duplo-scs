#ifndef _BASDRUM2_
#define _BASDRUM2_

#include "Instrument.h"
#include "Curves.h"

class BasDrum2 : public Instrument
{
public:
	double Pos;
	double TimePos;
	double Length;
	
	double Amp;
	

	BasDrum2() : Instrument()
	{
		Pos = 0;
		TimePos = 0;
		Length = 0.20;
	
		Amp = 0;
	}

	void NoteOn()
	{
		TimePos = 0;
		Pos = 0;
		Playing = true;
	}

	bool Run()
	{
		TimePos = TimePos+TimeStep;
		
		Pos = Pos+TimeStep*Fre*(Fall2(TimePos*150)*(Fall1(TimePos*20)*2+10));	
		
		Amp = 0;
		
		Amp = Amp + rSin(Pos);


		
		//Amp = Amp * Max((Shot2(TimePos/Length)*0.8 + Fall5S(TimePos/Length, 4.0)*0.2), 0);
		Amp = Amp * (Shot2(TimePos/Length)*0.8 + Fall5S(TimePos/Length, 4.0)*0.2);
		if (TimePos/Length > 1 && Amp == 0.0) Playing = false;

		return Playing;
	}

	void NoteOff()
	{
	}

	void End()
	{
	}

};

#endif