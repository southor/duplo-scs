#ifndef _PLUTO_
#define _PLUTO_

#include "Standards.h"
#include "Curves.h"

class Pluto : public Instrument
{



public:
	
	bool On;
	double Pos;
	double PlayPos;

	// output channels
	double Amp1; double Pan1;
	double Amp2; double Pan2;

	

	// Modifications
	double Mod1;
	double ModSpeed;
	bool ModForward;
	double DropSpeed;
	double ModMin;
	double ModMax;
	

	EnvelopeT *Envelope;
	EnvelopeT *Env2;

	DelayModule *Delay;

	

	Pluto() : Instrument()
	{
		Amp1 = 0; Amp2 = 0;
		Pan1 = -1;
		Pan2 = 1;
		
		Pos = 0;
		Mod1 = -1.6;
		ModMin = -1.6;
		ModMax = -1.4;
	
		On = false;
		Delay = new DelayModule(0.25);
		Delay->Delay = 0.02;

		Envelope = new EnvelopeT(0.12,0.005,0.03,0.8,0.2);
		Env2 = new EnvelopeT(0.02,0.005,0.005,0.8,0.15);

		ModForward = true;
		ModSpeed = 0.02;
		DropSpeed = 1.0;	
	}

	void NoteOn()
	{
		PlayPos = 0;
		Playing = true;
		
		Pos = 0;
		
		On = true;
		Delay->NoteOn();
		Envelope->NoteOn();
		Env2->NoteOn();
	}

	
	bool Run()
	{
		
		PlayPos = PlayPos+PlayStep;
		if (Envelope->Playing)
		{
			if (Envelope->Run() == false)
			{
				Delay->NoteOff();
			}
			
		}
		
		double Amp;
		Env2->Run();
		
		//cout << "o";
			
		double Env = Envelope->Amp;
		//double Env = 1.0;
		Pos = Pos+TimeStep*Fre;
		
		if (ModForward)
		{
			Mod1 = Mod1 + ModSpeed * PlayStep;
			if (Mod1 > ModMax) ModForward = false;
		}
		else
		{
			Mod1 = Mod1 - ModSpeed  * PlayStep;
			if (Mod1 < ModMin) ModForward = true;
		}
		
		
		//Mod1 = -1.2;
		double StartEnv = Limit(Bounce(Mod1,-1.6,1.4)+0.65,-1.0,1.0);
		double EndEnv = Limit(Bounce(Mod1,-1.6,1.4),-1.0,1.0);
		
		
		double NewAmp1 = NegSine(Pos+TriSaw(Pos + Delay->Out*Rise2(PlayPos*2.0)*0.2, Env2->Amp*(EndEnv-StartEnv)+StartEnv)*0.3+0.3)*NegSine(Pos*2.0)*Env*Fall2(PlayPos*DropSpeed);
		
		// fix problem with sound distortion
		double Amp1Diff = NewAmp1 - Amp1;
		double AllowedDiff = TimeStep*Fre*8.0;
		if (Amp1Diff > AllowedDiff)
		{
			Amp1 = Amp1 + AllowedDiff;
		}
		else if (Amp1Diff < -AllowedDiff)
		{
			Amp1 = Amp1 - AllowedDiff;
		}
		else
		{
			Amp1 = NewAmp1;
		}

		

		Delay->In = Amp1;
		Delay->Run();
		Amp2 = Delay->Out*0.7;

		
		
		Playing = Delay->Playing;
		return Playing;
		
	}

	void NoteOff()
	{
		//Playing = false;
	
		On = false;
		//Delay->NoteOff();
		Envelope->NoteOff();
		Env2->NoteOff();
	}

	void End()
	{
		Delay->End();
	}

};

#endif