#ifndef _NAKOS_1_
#define _NAKOS_1_

#include "Standards.h"
#include "Curves.h"


class Nakos : public STInstrument
{

protected:
	

public:
	

	EnvelopeT *Env;
	double  Pos;
	double VoblPos;
	double ColorPos;
	bool ColorPosRising;


	// modifications

	double Brightness;
	double Mod;
	double ThinMod;
	double VoblSpeed;
	
	/*
	 * 0.0 - 1.0 Divided into:
	 * 1. Static
	 * 2. Sine Curve
	 * 3. Vobl2
	 */
	double VoblType;



	Nakos() : STInstrument()
	{
		Env = new EnvelopeT();
		Env->Attack = 1.0/40.0;
		Env->Hold = 0;
		Env->Decay = 1.0/20.0;
		Env->Sustain = 0.8;
		Env->Release = 1.0/4.0;
		Pos = 0.0;
		VoblPos = 0.0;
		ColorPos = 0.0;

		VoblSpeed = 0.6;		
		Brightness = -0.3;
		VoblType = 0.99;
		ThinMod = 1.0;
		Mod = 0.1;

		ColorPosRising = true;
	}

	void NoteOn()
	{
	
		Pos = 0;
		Env->NoteOn();
		
		Playing = true;

		STInstrument::NoteOn();

		VoblPos = 0.0;
		//VoblPos = Loop(Fre);
	}

	bool Run()
	{
		STInstrument::Run();

		Playing = Env->Run();
		
		Pos = Pos + TimeStep*Fre;

		VoblPos += TimeStep*VoblSpeed*VoblSpeed;
		double ActualVoblPos = VoblPos + Fre/20.0;
		
		double CyclePos = Loop(Pos);

		double Color = 0.0;
		if (VoblType < 1.0/3.0) Color = VoblType * 3.0 * 4.0;			
		else if (VoblType < 5.0/6.0) Color = Sine(ActualVoblPos * 2.0) * 8.0;
		else Color = Vobl2(ActualVoblPos / 128.0);


		//double ColorStep = TimeStep*Fre*Color;

		//if (ColorPosRising)
		//{
		//	ColorPos += ColorStep;
		//	if (ColorPos > Color)
		//	{
		//		ColorPos = Bounce(ColorPos, 0.0, Color);
		//		ColorPosRising = false;
		//	}
		//}
		//else 
		//{
		//	ColorPos -= ColorStep;
		//	if (ColorPos < 0.0)
		//	{
		//		ColorPos = Bounce(ColorPos, 0.0, Color);
		//		ColorPosRising = true;
		//	}
		//}

		////double Part = BTriSqr(CyclePos, 0.5, 0.0);
		double Part = TriSqr(CyclePos, Mod);
		////Amp = Merge(CyclePos,Part,TriSaw(ColorPos, Brightness),0.0,0.5);
		Amp = Merge(CyclePos,Part,BTriSaw(CyclePos*Color, Brightness + rSin(TimePos*Color)/16.0, ThinMod),0.0,0.5);
		////Amp = TriSaw(CyclePos*Color, Brightness);
		Amp = Merge(CyclePos,Amp,Part,0.5,0.5);
		Amp = Amp * Env->Amp;


		return Playing;
	}

	void NoteOff()
	{
		Env->NoteOff();
		STInstrument::NoteOff();
	}

	void End()
	{
	}
};

#endif