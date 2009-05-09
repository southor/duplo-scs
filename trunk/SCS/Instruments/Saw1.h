#ifndef _SAW1_
#define _SAW1_

#include "Standards.h"
#include "Curves.h"

class Saw1 : public SPTInstrument
{
public:

	// operating, local
	double Pos;
	double SlowPos;



	// output
	double Amp1;
	double Amp2;

	double Pan1;
	double Pan2;



	
	// modules
	EnvelopeT *Env1;

	// modulation
	double Brightness1;
	double Brightness2;

	Saw1() : SPTInstrument()
	{
		Env1 = new EnvelopeT(0.006, 0, 0.05, 0.85, 0.02);
		Pos = 0;

		Pan1 = 0.8;
		Pan2 = -0.6;

		Brightness1 = 0.95;
		Brightness2 = 0.93;

	}

	void NoteOn()
	{
		Env1->NoteOn();


		SPTInstrument::NoteOn();
		
		Pos = 0;
		SlowPos = Random(0, 1.0);
	}

	bool Run()
	{
		SPTInstrument::Run();
		
		
		

		Amp1 = 0;
		Amp2 = 0;


		Playing = Env1->Run();
		if (Playing)
		{


			Pos = Pos + TimeStep*Fre;
			
			SlowPos = SlowPos + TimeStep/50.0;

			Amp1 = Amp1 + SawSine(Pos+Vobl1(SlowPos*1.043+0.13)*Fre/1000.0 + 0.496, Brightness1+Fall2(TimePos*150.0)*0.02-0.02)  * Env1->Amp * 0.5;
			Amp1 = Amp1 + TriSaw(Pos+ Vobl1(SlowPos*1.503+0.5)*Fre/1200.0 + 0.105, Brightness1+Fall2(TimePos*120.0)*0.1-0.1)  * Env1->Amp * 0.4;
			
			Amp2 = Amp2 + SawSine(Pos+Vobl1(SlowPos*1.893+56.28901)*Fre/1050.0 + 5.1503, Brightness2 + (Fall2(TimePos*100.0)*0.02)-0.02)  * Env1->Amp * 0.5;
			Amp2 = Amp2 + TriSaw(Pos+Vobl1(SlowPos*1.893+56.28901)*Fre/1766.0 + 2.0522, Brightness2 + (Fall2(TimePos*130.0)*0.1)-0.1)  * Env1->Amp * 0.6;
		}
		


		return Playing;
	}
	
	void NoteOff()
	{
		Env1->NoteOff();
		
		SPTInstrument::NoteOff();
	}


};

#endif