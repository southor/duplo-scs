#ifndef _VOBLER_1_
#define _VOBLER_1_

#include "Standards.h"
#include "Curves.h"


class Vobler1 : public STInstrument
{

protected:
	

public:
	

	EnvelopeT *Env;
	double  Pos;
	double VoblPos;

	double SawMod;
	double SqrMod;
	double ThinMod;
	
	double SawVoblMod;
	double SqrVoblMod;
	double SineVoblMod;

	double VoblSpeed;

	double Vol;


	Vobler1() : STInstrument()
	{
		Env = new EnvelopeT();
		Pos = 0.0;
		VoblPos = 0.0;
		Vol = 1.0;

		SawMod = 0.0;
		SqrMod = 0.0;
		ThinMod = 1.0;
		
		SawVoblMod = 0.01;
		SqrVoblMod = 0.0;
		SineVoblMod = 0.95;

		VoblSpeed = 0.2;
	}

	void NoteOn()
	{
	
		Pos = 0;
		Env->NoteOn();
		
		Playing = true;

		STInstrument::NoteOn();
	}

	bool Run()
	{
		STInstrument::Run();

		Playing = Env->Run();
		
		Pos = Pos + TimeStep*Fre;

		VoblPos += TimeStep*VoblSpeed*VoblSpeed*VoblSpeed;

		//double Vobl = Sine(TimePos) * Sine(TimePos*0.56) * Sine(TimePos*4.2354) * Sine(TimePos*2.31);

		double Vobl1 = Sine(VoblPos) * Sine(VoblPos*0.56);
		double Vobl2 = Sine(VoblPos*1.31) * Sine(VoblPos*4.2354);
		double Vobl3 = Sine(VoblPos*2.31) * Sine(VoblPos*3.36);
		//double Vobl = SineSqr(TimePos, 0.8);
		//if (Vobl > 1.0) Vobl = 1.0 - Vobl;

		//double ActualSaw = SawMod + Vobl * SawVoblMod;
		//double ActualSqr = SqrMod + Vobl * SqrVoblMod;
		//double ActualThin = (1.0-ThinMod) + Vobl * ThinVoblMod;

		double ActualSaw = SawMod;
		double ActualSqr = SqrMod;
		double ActualThin = ThinMod;

		double ActualPos = Pos;
		ActualPos += Saw(Pos*3.0) * Vobl1*Vobl2 * SawVoblMod;
		ActualPos += Sqr(Pos*4.0) * Vobl2*Vobl3 * SqrVoblMod;
		ActualPos += Sine(Pos*2.0) * Vobl1*Vobl3 * SineVoblMod;

		if (ActualSaw > 1.0) ActualSaw = 1.0;
		else if (ActualSqr > 1.0) ActualSqr = 1.0;
		else if (ActualThin > 1.0) ActualThin = 1.0;
		//ActualThin = 0.5;

		Amp = BTriSawSineSqr(ActualPos, ((ActualSaw < 0.0) ? 0.0 : ActualSaw),
									((ActualSqr < 0.0) ? 0.0 : ActualSqr),
									0.0,
									((ActualThin < 0.0) ? 0.0 : ActualThin))
							* Env->Amp * Vol;

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