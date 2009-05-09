#ifndef _CPRAW_
#define _CPRAW_

#include "Standards.h"
#include "Curves.h"
#include "EnvelopeT.h"

class CPRaw : public STInstrument
{
public:

	double Pos;

	EnvelopeT *Env1;
	//EnvelopeT *Env2;

	double VoblAmount;
	double VoblFre;

	CPRaw() : STInstrument()
	{
		Env1 = new EnvelopeT(0.05, 0.1, 0.15, 0.9, 0.1);
		//Env2 = new EnvelopeT(0.005, 0.1, 0.1, 0.4, 0.1);
		Pos = 0;
		VoblFre = 0.02;
		VoblAmount = 0.6;
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

		Pos = Pos + TimeStep*Fre*2.0/1.142462048;
		
			
		Amp = 0;
		double Vobl = Vobl1(TimePos*VoblFre/SqrRoot(Fre))*VoblAmount;
		Amp = Amp+Sine(Pos+Tri(Pos+0.5 + TriSaw(Pos*(1.0+Vobl+0.13), 0.3 + Sine(TimePos*Vobl)/16.0)*0.15));
		//Amp = Merge(Pos, Amp, TriSqr(Pos, 0.68));
		
		
		Amp = Amp * Env1->Amp;

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