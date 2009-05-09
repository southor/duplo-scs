#ifndef _BRASS3_
#define _BRASS3_

#include "Standards.h"
#include "Curves.h"
#include "EnvelopeT.h"
#include "EchoModule.h"

class Brass3 : public SPTInstrument
{
public:

	double Pos;	

	EnvelopeT *Env1;
	EnvelopeT *Env2;
	EchoModule *Echo;
	
	// modulations
	double ThinMod;
	double EchoAmount;

	Brass3() : SPTInstrument()
	{
		Env1 = new EnvelopeT(0.12, 0.05, 0.1, 0.85, 0.08);
		Env2 = new EnvelopeT(0.12, 0.01, 0.08, 0.7, 0.1);
		Pos = 0;
	
		Echo = new EchoModule(0.005, 3);
		ThinMod = 0.0;
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

		

		double APos;
		if (Pos > 2.0) 
		{
			Pos = 0.0;
			APos = 0.0;
		}
		else if (Pos > 1.0)
		{
			APos = 2.0 - Pos;
		}
		else
		{
			APos = Pos;
		}

		if (Env1->Playing == true)
		{
			if(Env1->Run() == false)
			{
				Echo->NoteOff();
			}
		
			Env2->Run();

			
			Pos = Pos + TimeStep*Fre;
			

			Amp += SoftPrecipice(APos, ThinMod * ThinMod);
			//Amp = APos;

			
		}

		
		
		
	

		// Echo
		Echo->In = Amp;
		Playing = Echo->Run();
		

		//Amp = Amp + Echo->Out * EchoAmount;
	
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