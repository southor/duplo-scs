#ifndef _BLIP1_
#define _BLIP1_

#include "Standards.h"
#include "EchoModule.h"
#include "Curves.h"

class Blip1 : public STInstrument
{
public:

	double Pos;

	// modules
	EnvelopeT *Env1;
	EchoModule *Echo1;
	EchoModule *Echo2;
	
	// modification
	double EchoQuantity;
	double MaxDist;
	double DistSpeed;

	double Amp1;
	double Amp2;
	double Pan1;
	double Pan2;

	Blip1() : STInstrument()
	{
		Env1 = new EnvelopeT();
		Env1->Attack = 0.005;
		Env1->Sustain = 0.8;
		Pos = 0;
	
		EchoQuantity = 0.8;
		Echo1 = new EchoModule(0.0344, 4);
		Echo2 = new EchoModule(0.02, 4);

		Amp1 = 0;
		Amp2 = 0;

		MaxDist = 0.5;
		DistSpeed = 0.15;
		
		Pan1 = 0.8;
		Pan2 = -0.8;
	}

	void NoteOn()
	{
		Env1->NoteOn();
		

		Echo1->NoteOn();
		Echo2->NoteOn();

		STInstrument::NoteOn();
		
		Pos = 0;

		Amp1 = 0;
		Amp2 = 0;
	}

	bool Run()
	{
		STInstrument::Run();
		
		
		

		Amp = 0;
		Echo2->In = 0.0;

		if (Env1->Playing == true)
		{
			if(Env1->Run() == false)
			{
				Echo1->NoteOff();
				Echo2->NoteOff();
			}
		
			

			Pos = Pos + TimeStep*Fre;
			
			double Shape = MaxDist/(1.0 + TimePos*DistSpeed) + 1.0 - MaxDist;
						
			Amp = Amp + TriSaw(Pos*3.0+0.35, Tri(Pos)*0.7*(1.0/Shape))*0.7*Env1->Amp;
			Echo2->In = Amp;
			Amp = Amp + TriSaw(Pos+(Echo2->Out)*0.15, 0.8*Shape)*0.6*Fall2(TimePos*1.0) * Env1->Amp;

			
		}
		
		// Echo
		Echo1->In = Amp;		
		Playing = (Echo1->Run() || Echo2->Run());
		
		//if (TimePos < 0.5) cout << Echo->Out << endl;
		Amp1 = Amp + Echo1->Out * EchoQuantity;
		Amp2 = Amp + Echo2->Out * EchoQuantity;
		//Amp = Amp * 500/(300 + Fre);

		

		return Playing;
	}
	
	void NoteOff()
	{
		Env1->NoteOff();

		
		
		STInstrument::NoteOff();
	}

	void End()
	{
		Echo1->End();
		Echo2->End();
	}

};

#endif