#ifndef _FMSTRING1_
#define _FMSTRING1_

#include "Standards.h"
#include "EchoModule.h"
#include "Curves.h"
#include "Func.h"

class FMString1 : public STInstrument
{
public:

	double Pos;

	// modules
	EnvelopeT *Env1;
	EnvelopeT *Env2;
	EchoModule *Echo;
	
	// modification
	double EchoQuantity;

	FMString1() : STInstrument()
	{
		Env1 = new EnvelopeT(0.005, 0.0, 1.5, 0.0, 0.01);
		
		Env2 = new EnvelopeT(0.001, 0.0, 0.05, 0.0, 4.0);

		
		Pos = 0;
	
		EchoQuantity = 0.1;
		Echo = new EchoModule(0.01, 4);
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
			
			Amp = Amp + SqrSine(Pos + 0.68 + Sine(Pos*2.0 + 0.35)*0.23, Greatest(1.0-TimePos*Fre/10.0, 0.1)) * 0.5;
			Amp = Amp + (BSine(Pos*1.0+0.2, 0.01)+BSine(Pos*1.0+0.2, 0.022)) * Env2->Amp * 0.5;
			
			//Amp = Sine(Pos);

			//Amp = Amp + Sine(Pos + Sine(Pos*7.3061036)*0.14*Env2->Amp + Sine(Pos * 0.345 + Sine(Pos*0.8261036)*0.7*Env2->Amp)*0.52) * 0.5;
			
			Amp = Amp * Env1->Amp;
			
		
		}
		
		// Echo
		Echo->In = Amp;
		Playing = Echo->Run();
		
		//if (TimePos < 0.5) cout << Echo->Out << endl;
		Amp = Amp + Echo->Out * EchoQuantity;
		Amp = Amp * 500/(300 + Fre);

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