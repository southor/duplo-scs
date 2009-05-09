#ifndef _BRASS1_
#define _BRASS1_

#include "Standards.h"
#include "Curves.h"
#include "EnvelopeT.h"
#include "EchoModule.h"

class Brass1 : public STInstrument
{
public:

	double Pos;

	EnvelopeT *Env1;
	EnvelopeT *Env2;
	EchoModule *Echo;
	
	// modulations
	double Mod1;
	double ThinMod;
	double EchoAmount;

	Brass1() : STInstrument()
	{
		Env1 = new EnvelopeT(0.12, 0.05, 0.1, 0.85, 0.08);
		Env2 = new EnvelopeT(0.12, 0.01, 0.08, 0.7, 0.1);
		Pos = 0;
	
		Echo = new EchoModule(0.005, 3);
		Mod1 = 0.2;
		EchoAmount = 0.3;

		ThinMod = 1.0;
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
					
			//Mod = Env2->Amp*Mod1*+0.05;
			double EvolveValue = Rise2(TimePos*8)*0.4 + Env2->Amp;
			double Mod = EvolveValue*Mod1;
			//double ActualThinMod = Bounce(ThinMod + EvolveValue, 0.0, 1.0);
			double ActualThinMod = ThinMod;

			double ActualPos = Loop(Pos);
			if (ActualPos > ActualThinMod) ActualPos = 1.0;
			else ActualPos = ActualPos / ActualThinMod;

			Amp = Amp + NegSine(ActualPos + BSine(ActualPos, 0.4)*0.4*Mod + NegSine(ActualPos + TriSaw(ActualPos, Env2->Amp*0.5)*0.25)*0.3*Mod + NegSine(ActualPos)*0.17)*Env1->Amp;

		
		}
		
		
	
		//if (Echo->Out)

		// Echo
		Echo->In = Amp;
		Playing = Echo->Run();
		
		//if (TimePos < 0.5) cout << Echo->Out << endl;
		Amp = Amp + Echo->Out * EchoAmount;
	
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