#ifndef _MSTRING_
#define _MSTRING_

#include "Standards.h"
#include "Curves.h"

class MString : public STInstrument
{
public:

	double Pos;

	EnvelopeT *Env1;
	EnvelopeT *Env2;
	EchoModule *Echo;

	// modulations
	double Decay;
	double EchoAmount;
	

	MString() : STInstrument()
	{
		Env1 = new EnvelopeT(0.005, 0.1, 0.15, 0.9, 0.17);
		Env2 = new EnvelopeT(0.005, 0.1, 0.1, 0.4, 0.17);
		Pos = 0;
	
		Decay = 2.0;
		EchoAmount = 0.2;
		Echo = new EchoModule(0.01, 2);
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
		

			Amp = MultiCurve(Pos, 0.1, 0.5, 0.4, -0.5, 0.9, -1.0);
			Amp = Amp * Fall2(TimePos*Decay)*Env1->Amp*0.5;
		
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