#ifndef _FIR_
#define _FIR_

#include "Standards.h"
#include "Curves.h"
#include "EnvelopeT.h"

class Fir : public STInstrument
{
public:

	double Pos;

	EnvelopeT *Env1;
	//EnvelopeT *Env2;

	Fir() : STInstrument()
	{
		Env1 = new EnvelopeT(0.35, 0.1, 0.15, 0.9, 0.35);
		//Env2 = new EnvelopeT(0.005, 0.1, 0.1, 0.4, 0.1);
		Pos = 0;
	}

	void NoteOn()
	{
		Env1->NoteOn();
		//Env2->NoteOn();

		STInstrument::NoteOn();
		Pos = 0;
	}

	bool Run()
	{
		STInstrument::Run();
		
		Playing = Env1->Run();
		//Env2->Run();

		Pos = Pos + TimeStep*Fre;
		
		
			
		Amp = 0;
		Amp = Amp + rSin(Pos+Tri(Pos+0.7)*0.2+rSin(Pos*2+0.8)*0.05)*0.33;
		Amp = Amp + rSin(Pos+Tri(Pos+0.7)*0.2+rSin(Pos*2+0.8)*0.05 + 0.48)*0.33;
		Amp = Amp + rSin(Pos*1.000001+Tri(Pos*1.000001+0.7)*0.1+rSin(Pos*1.000001*2+0.8)*0.05 + 0.6)*0.33;
		Amp = Amp + TriSaw(Pos + 0.3,0.9)*0.2 + TriSaw(Pos + 0.5,-0.9)*0.2 + TriSaw(Pos + 0.8,0.8)*0.2;
		Amp = Amp - TriSaw(Pos + 0.4,0.9)*0.3 + TriSaw(Pos + 0.6,-0.9)*0.2;
		//Pos = Pos*2;
		//Amp = Amp + (rSin(Pos+rSin(PosEff*1/FreEff)*0.5+Vobl*0.3*Fall2(TimePos*2.0)) + TriSqr(Pos,0.88)*0.4)*Env2->Amp*350.0/(Fre+400);
		//Pos = Pos/2;
		Amp = Amp * Env1->Amp * 0.5;


		return Playing;
	}
	
	void NoteOff()
	{
		Env1->NoteOff();
		//Env2->NoteOff();
		
		STInstrument::NoteOff();
	}

};



#endif