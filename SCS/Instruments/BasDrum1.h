#ifndef _BASDRUM1_
#define _BASDRUM1_

#include "Instrument.h"
#include "Curves.h"

class BasDrum1 : public Instrument
{
public:
	double Pos;
	double TimePos;
	double Length;
	
	double Amp;
	

	BasDrum1() : Instrument()
	{
		Pos = 0;
		TimePos = 0;
		Length = 0.09;
	
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
		//PlayPos = PlayPos+PlayStep;
		//double Env = Shot(PlayPos*30);
		//double Env = 1.0;
		Pos = Pos+TimeStep*Fre*Fall2(TimePos*30);	
		
		//Amp = Sine(Pos);

		//Amp = Sine(TriSaw(Pos,Sine(PlayPos*4)*0.1))*Sine(TriSqr(Pos*1,Sine(PlayPos*3)*0.1+0.2))*Sine(Pos+Sine(PlayPos*5)*0.15);
		//Amp = Tri((Tri(Pos*2)*Tri(Pos*7)*Tri(Pos*3)))+Tri(Pos*5)*Tri(Pos*11)*Tri(Pos*12);
		Amp = 0;
		//Amp = Amp + rSin(Pos*1)*0.2+rSin(Pos*2)*0.1+rSin(Pos*3)*0.1+rSin(Pos*4)*0.05+rSin(Pos*5)*0.025+rSin(Pos*6)*0.025;
		
		Amp = Amp + rSin(Pos*2+Tri(Pos+0.35)*0.0+Tri(Pos*3.7+0.6)*0.0)*0.7;
		
		Amp = Amp + rSin(Pos*20+Tri(Pos+0.35)*0.2+Tri(Pos*3.7+0.6)*0.35)*0.2*Fall2(TimePos*32/Length);

		//Amp = Amp + rSin(2.41)*0.1 + rSin(3.236)*rSin(1.15)*rSin(4.9)*0.3;
		
		Amp = Amp*Fall2S(TimePos/Length,1);
		if (TimePos/Length > 1) Playing = false;

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