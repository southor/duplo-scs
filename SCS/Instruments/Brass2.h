#ifndef _BRASS2_
#define _BRASS2_

#include "Standards.h"
#include "Curves.h"
#include "EnvelopeT.h"
#include "EchoModule.h"

class Brass2 : public SPTInstrument
{
public:

	double Pos;	

	EnvelopeT *Env1;
	EnvelopeT *Env2;
	EchoModule *Echo;
	
	// modulations
	double Mod1;
	double EchoAmount;

	Brass2() : SPTInstrument()
	{
		Env1 = new EnvelopeT(0.12, 0.05, 0.1, 0.85, 0.08);
		Env2 = new EnvelopeT(0.12, 0.01, 0.08, 0.7, 0.1);
		Pos = 0;
	
		Echo = new EchoModule(0.005, 3);
		//Mod1 = 0.5;
		Mod1 = 0.987;
		EchoAmount = 0.3;
	}

	void NoteOn()
	{
		Env1->NoteOn();
		Env2->NoteOn();

		Echo->NoteOn();

		SPTInstrument::NoteOn();
		Pos = 0;
	}

	bool Run()
	{
		SPTInstrument::Run();
		
		Amp = 0;

		if (Env1->Playing == true)
		{
			if(Env1->Run() == false)
			{
				Echo->NoteOff();
			}
		
			//double AMod1 = Mod1*Mod1*Mod1;
			double AMod1 = 1.0/(1.0+(1.0-Mod1*Mod1)*40.0);

			Env2->Run();
			Pos = Pos + PosFunc(Tempo, PlayStep, Fre*(Min(Env2->Amp, 1.0) * rSin(PosFunc(Tempo,PlayPos,Fre))));
			Amp = Amp + BSine(Pos, AMod1) * Env1->Amp;	
		}

		// Echo
		Echo->In = Amp;
		Playing = Echo->Run();

		Amp = Amp + Echo->Out * EchoAmount;
	
		return Playing;
	}
	
	void NoteOff()
	{
		Env1->NoteOff();
		Env2->NoteOff();
		
		SPTInstrument::NoteOff();
	}

	void End()
	{
		Echo->End();
	}

};

#endif