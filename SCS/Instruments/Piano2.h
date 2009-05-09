#ifndef _PIANO2_
#define _PIANO2_

#include "Standards.h"
#include "Curves.h"
#include "EnvelopeT.h"
#include "EchoModule.h"

class Piano2 : public STInstrument
{
public:

	double Pos;

	EnvelopeT *Env1;
	EnvelopeT *Env2;
	EchoModule *Echo;

	
	
	// modulations
	double EchoAmount;

	double Mod1Low; // Value at bottom of Env2
	double Mod1High; // Value at top of Env2
	double FreAffMod1; // how frequensy should affect Mod1
	//double BassAmount;
	double RoundOffFre;

	Piano2() : STInstrument()
	{
		Env1 = new EnvelopeT(0.005, 0.1, 0.15, 1.0, 0.17);
		Env2 = new EnvelopeT(0.005, 0.1, 0.1, 0.4, 0.17);
		Pos = 0;
	
		Echo = new EchoModule(0.03, 3);
		Mod1Low = 75;
		Mod1High = 75;
		FreAffMod1 = 0.8;
		//BassAmount = 1.5;
		RoundOffFre = 300.0;

		EchoAmount = 0.2;
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
		
		//double Fre = this->Fre*2.0;
		

		Amp = 0;

		if (Env1->Playing == true)
		{
			if(Env1->Run() == false)
			{
				Echo->NoteOff();
			}
		
			Env2->Run();

			Pos = Pos + TimeStep*Fre;

			double ActualMod1 = Mod1Low + (Mod1High-Mod1Low)*Env2->Amp;
		
			double FreEff = ActualMod1/150 + (FreAffMod1*Fre)/150 - (FreAffMod1*ActualMod1)/150;
			double PosEff = Bounce(Pos,0,1);
			double Vobl = Vobl1(PosEff/(FreEff*32.0)+13);
		
			Amp = Amp + rSin(Pos+Tri(Pos+0.7)*0.1+rSin(Pos*2+0.8)*0.05)*0.3+rSin(Pos+Tri(Pos*3+0.52)*0.12+rSin(Pos+0.15)*0.08+0.33)*0.16;
			Amp = Amp + (rSin(Pos+rSin(PosEff*1/FreEff)*0.5+Vobl*0.3*Fall2(TimePos*2.0)) + TriSqr(Pos,0.88)*0.4)*Env2->Amp*350.0/(Fre+400);
			Amp = Amp * Fall2(TimePos*2.0)*Env1->Amp*0.8*Rise2(Fre/RoundOffFre);
		
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