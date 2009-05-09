#ifndef _SOFTBASS_
#define _SOFTBASS_

#include "Standards.h"
#include "Curves.h"

class SoftBass : public SPTInstrument
{
public:

	double Pos;

	EnvelopeT *Env1;
	
	EchoModule *Echo;
	double EchoQuantity;

	SoftBass() : SPTInstrument()
	{
		Env1 = new EnvelopeT(0.05, 0, 0, 1, 0.125);
		Pos = 0;
	
		EchoQuantity = 0.2;
		Echo = new EchoModule(0.25/4.0, 4);
	}

	void NoteOn()
	{
		Env1->NoteOn();
		

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
		
			

			Pos = Pos + TimeStep*Fre;
			Amp = Amp + NegSine(Pos+Tri(Pos)/(1.5*Hill1(PlayPos*2)+2.5)) * Env1->Amp;
		}
		
		// Echo
		Echo->In = Amp;
		Playing = Echo->Run();
		
		//if (TimePos < 0.5) cout << Echo->Out << endl;
		Amp = Amp + Echo->Out * EchoQuantity;
	
		return Playing;
	}
	
	void NoteOff()
	{
		Env1->NoteOff();
		
		
		SPTInstrument::NoteOff();
	}

	void End()
	{
		Echo->End();
	}

};

#endif