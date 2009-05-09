#ifndef _DRUM2_
#define _DRUM2_

#include "Standards.h"
#include "Curves.h"

class Drum2 : public Instrument
{
public:
	double Pos;
	double TimePos;
	double PlayPos;
	double Length;
	
	double Mod;
	double LowMod;
	double NoiseMod;

	double Amp;
	double CurrentFre;

	Drum2() : Instrument()
	{
		Pos = 0;
		TimePos = 0;
		Length = 0.5;

	

		Mod = 60;
		LowMod = 1.0;
		NoiseMod = 1.0;


		Amp = 0;
	}

	void NoteOn()
	{
		TimePos = 0;
		PlayPos = 0;
		Pos = 0;
		Playing = true;
		CurrentFre = Fre;
	}

	bool Run()
	{
		TimePos = TimePos+TimeStep;
		PlayPos = PlayPos+PlayStep;
		//double Env = Shot(PlayPos*30);
		//double Env = 1.0;
		//Pos = Pos+TimeStep*Fre*Fall2(TimePos*30);	
		

		double HPos = PlayPos*CurrentFre/10;
		
		CurrentFre = CurrentFre/(PlayStep*40.0+1.0);
		
				

		//double HPos = 3*Rise2(PlayHPos*2)/(PlayHPos*0.3);
		Amp = 0;
		Amp = Amp + LowMod*SoftSqr(HPos*20.8/4.0)*TriSqr(HPos*55.0/4, 0.5-Rise2(PlayPos))*(NegSine(HPos*31.5/4)*NegSine(HPos*13.45/4)*0.2+0.8);
		Amp = Amp + (0.5/((TimePos+0.5)*12.0)-0.1) * NoiseMod*0.5*NegSine(Rise2(NegSine(HPos*52.56)*NegSine(HPos*150)*NegSine(HPos*137.8)*NegSine(HPos*95.43)*NegSine(HPos*113.6784)*Tri(HPos*116.9)*8));
		

		Amp = Amp * (Fall2(TimePos*Mod)-Fall2(Length*Mod));
		
		if (TimePos/Length > 1.0) {
			Playing = false;
			//std::cout << "okey drum ends" << std::endl;
		}
		return Playing;

		//Amp = Sine(Pos);

		//Amp = Sine(TriSaw(Pos,Sine(PlayPos*4)*0.1))*Sine(TriSqr(Pos*1,Sine(PlayPos*3)*0.1+0.2))*Sine(Pos+Sine(PlayPos*5)*0.15);
		//Amp = Tri((Tri(Pos*2)*Tri(Pos*7)*Tri(Pos*3)))+Tri(Pos*5)*Tri(Pos*11)*Tri(Pos*12);
		//Amp = 0;
		//Amp = Amp + rSin(Pos*1)*0.2+rSin(Pos*2)*0.1+rSin(Pos*3)*0.1+rSin(Pos*4)*0.05+rSin(Pos*5)*0.025+rSin(Pos*6)*0.025;
		
		//Amp = Amp + rSin(Pos*2+Tri(Pos+0.35)*0.0+Tri(Pos*3.7+0.6)*0.0)*0.7;
		/*
		Amp = Amp + rSin(Pos*20+Tri(Pos+0.35)*0.2+Tri(Pos*3.7+0.6)*0.35)*0.2*Fall2(TimePos*32/Length);

		//Amp = Amp + rSin(2.41)*0.1 + rSin(3.236)*rSin(1.15)*rSin(4.9)*0.3;
		
		Amp = Amp*Fall2S(TimePos/Length,1);
		if (TimePos/Length > 1) Playing = false;

		return Playing;
	*/
	}

	void NoteOff()
	{
	}

	void End()
	{
	}

};

#endif