#ifndef _VIOLIN_
#define _VIOLIN_

#include "Standards.h"
#include "EnvelopeT.h"

class Violin : public STInstrument
{


public:

	// private
	double Pos;
	double StringE;
	double NextTop;
	double NextBottom;
	bool Sliding;

	// modules
	EnvelopeT *Env1;

	// modulation
	double PullSpeed;
	double SlideSpeed;
	double TRandomness;
	double PRandomness;

	// output
	double Amp;

	Violin() : STInstrument()
	{
		Env1 = new EnvelopeT(0.01, 0.03, 0.05, 0.8, 0.1);
		
		

		TRandomness = 0.28;
		PRandomness = 0.12;
		
		NextTop = (rand()/32768.0 + rand()/32768.0) * TRandomness + 1-TRandomness;
		PullSpeed = 2.25;
		SlideSpeed = 6.0;

		//cout << "HEJ" << endl; // asdasd

		Pos = 0;
		StringE = 0;
		Sliding = false;
		Amp = 0;
	}

	void NoteOn()
	{
		Env1->NoteOn();

		//Playing = Env1->Playing;
		//cout << rand()/32768.0 << endl;

		NextTop = (rand()/32768.0 + rand()/32768.0) * TRandomness + 1-TRandomness;

		Sliding = false;
		STInstrument::NoteOn();
		Pos = 0;
		StringE = 0;
		Amp = 0;
	
		//cout << "   playing: " << Playing;
	}

	bool Run()
	{
		//cout << "run" << endl;
		STInstrument::Run();
	
		Amp = 0;


		if (Sliding)
		{
			StringE = StringE - SlideSpeed*TimeStep*Fre;
			if(StringE <= NextBottom)
			{
				Sliding = false;
			}
		}
		else
		{
			StringE = StringE + PullSpeed*TimeStep*Fre;
			if (StringE >= NextTop)
			{
				Sliding = true;
				NextBottom = (rand()/32768.0 + rand()/32768.0 - 1.0) * PRandomness - 1.0;
				NextTop = (rand()/32768.0 + rand()/32768.0) * TRandomness + 1-TRandomness;
			}
		}
			
		//cout << TimePos  << "sdfasdasdas" << endl;

		Playing = Env1->Run();
		
		Amp = StringE * Env1->Amp;

		return Playing;
	}
	
	void NoteOff()
	{
		Env1->NoteOff();

		
		STInstrument::NoteOff();
	}

	void End()
	{
	
	}

};

#endif
