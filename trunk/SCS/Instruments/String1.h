#ifndef _STRING1_
#define _STRING1_

#include "Standards.h"
#include "Curves.h"

class String1 : public STInstrument
{
public:

	double Pos;

	EnvelopeT *Env1;
	EnvelopeT *Env2;
	EchoModule *Echo;

	double BaseFre;
	double HighFreAmplify;
	double BaseAmp;

	// mods

	double ExtraReverb;
	

	String1() : STInstrument()
	{
		Env1 = new EnvelopeT(0.005, 0.1, 0.15, 0.9, 0.17);
		Env2 = new EnvelopeT(0.005, 0.1, 0.1, 0.4, 0.17);
		Pos = 0;
	
		Echo = new EchoModule(0.01, 4);
		
		BaseFre = 100;
		HighFreAmplify = 1700.0;
		BaseAmp = 0.05;

		ExtraReverb = 0.7;
	}

	void NoteOn()
	{
		Env1->NoteOn();
		Env2->NoteOn();

		Echo->NoteOn();

		STInstrument::NoteOn();
		Pos = 0;
	}

	bool Run()
	{
		STInstrument::Run();
		
		
		

		Amp = 0;

		if (Env1->Playing == true)
		{
			if(Env1->Run() == false)
			{
				Echo->NoteOff();
			}
		
			Env2->Run();

			Pos = Pos + TimeStep*Fre;
		
		

			Amp = Amp + Crossfade(TriSqr(Pos+0.2,Fall3(TimePos*20)-0.01), NegSine((Pos+0.2)*3), NegSine(Pos*2)*0.7);
			Amp = Crossfade(Amp, TriSaw(Pos*2, Fall2(TimePos*10)-0.1), Tri(Pos+0.3)*SoftSqr(Pos)*0.6);
			Amp = Amp * Fall2(TimePos*2)*Env1->Amp*0.5;

			Amp = Amp * (Fall2((BaseFre+Fre*Fre/290.0)/HighFreAmplify)+BaseAmp);
		
		}
		
		
	
		//if (Echo->Out)

		// Echo
		Echo->In = Amp;
		Playing = Echo->Run();
		
		//if (TimePos < 0.5) cout << Echo->Out << endl;
		Amp = Amp + Echo->Out * ExtraReverb;
	
		return Playing;
	}
	
	void NoteOff()
	{
		Env1->NoteOff();
		Env2->NoteOff();
		
		STInstrument::NoteOff();
	}

	void End()
	{
		Echo->End();
	}

};

#endif