#ifndef _TOBE_1_
#define _TOBE_1_

#include "Standards.h"
#include "Curves.h"

class Tobe1 : public Instrument
{
public:
	

	double Amp;
	EnvelopeT *Env;
	double  Pos;

	double TobeSpeed;
	double TobeAmount;
	double Brightness;
	
	//double (*AmpFunc)(double);

	double Vol;

	Tobe1() : Instrument()
	{
		Env = new EnvelopeT();
		Pos = 0;
		Vol = 1.0;

		TobeSpeed = 0.1171875;
		Brightness = 0.5;
		TobeAmount = 1.0;
	}

	void NoteOn()
	{
		Amp = 0;
	
		Pos = 0;
		Env->NoteOn();
		
		Playing = true;
	}

	bool Run()
	{
		

		Playing = Env->Run();
		
		Pos = Pos + TimeStep*Fre;

		//if (AmpFunc != NULL) Amp = AmpFunc(Pos) * Env->Amp * Vol;
		//else Amp = 0;

		double EnvAmp = Env->Amp;

		// calc HillValue
		double HillValue = Sine(Pos * (256.0 * 4.0 * TobeSpeed) / (60.0 * Fre) + 0.25)/(-2.0) + 0.5;
		

		double SawValue;	
			
		double TmpPos = Loop(Pos,0.0,1.0);

		// calc SawValue

		if (TmpPos > 0.75) SawValue = (TmpPos-0.75)*4.0-1.0;
		else if (TmpPos < 0.25) SawValue = TmpPos*4.0;
		else SawValue = (TmpPos-0.25)*(-4.0)+1.0;

		

		// calc Amp
		Amp = SawSine(Pos + TobeAmount * SawValue / (3.0 * HillValue + 2.5) - 0.5, Brightness) * EnvAmp * Vol;


		return Playing;
	}

	void NoteOff()
	{
		Env->NoteOff();
	}

	void End()
	{
	}
};

#endif