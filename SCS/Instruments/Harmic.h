#ifndef _HARMIC_
#define _HARMIC_

#include "Standards.h"
#include "EchoModule.h"
#include "Curves.h"

class Harmic : public STInstrument
{
public:

	double Pos;

	// modules
	EnvelopeT *Env1;
	EchoModule *Echo;
	
	// modification
	double EchoQuantity;
	double Decay;

	Harmic() : STInstrument()
	{
		Env1 = new EnvelopeT();
		Env1->Attack = 0.005;
		Env1->Sustain = 0.8;
		Pos = 0;
	
		EchoQuantity = 0.1;
		Decay = 6.0;
		Echo = new EchoModule(0.01, 4);
	}

	void NoteOn()
	{
		Env1->NoteOn();
		

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
		
			

			Pos = Pos + TimeStep*Fre;
			Amp = NegSine(Pos + Sine(Pos*2.0+0.23 + Tri(Pos)*0.4)*0.4*(1.2-Env1->Amp*0.3))*0.8 + NegSine(Pos)*0.5 + NegSine(Pos*4)/8 + NegSine(Pos*5)/10 + BSine(Pos+0.5,0.1)*0.15;
			//Amp = Amp + Crossfade((TriSqr(Pos, 0.9)+TriSqr(Pos+0.34, -0.9))*0.5, rSin(Pos*2 + Tri(Pos*8+0.2)*0.2), rSin(Pos + 0.87))*0.7;
			//Amp = Amp + (BSine(Pos,0.05)+BSine(Pos+0.2,0.1)+BSine(Pos+0.5,0.08)+BSine(Pos+0.3,0.2))*0.3/4.0;
			Amp = Amp * Fall2(TimePos*Decay) * Env1->Amp;
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
		
		
		STInstrument::NoteOff();
	}

	void End()
	{
		Echo->End();
	}

};

#endif