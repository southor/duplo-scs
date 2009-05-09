#ifndef _NOISEFLANGING_
#define _NOISEFLANGING_

#include "Instrument.h"
#include "Flanging.h"
#include "Curves.h"

class NoiseFlanging : public SPInstrument
{

protected:
	double Pos;
	double CurrentFre;

public:

	// flanging output
	Flanging *Flang;

	double (*FlangFunc)(double); // takes PlayPos returns Phase
	double FreMod;

	NoiseFlanging(double Wideness) : SPInstrument()
	{
		Flang = new Flanging(Wideness);
		Flang->Phase = 0.0;
		FlangFunc = NULL;
		FreMod = 1.0;
	}

	void NoteOn()
	{
		SPInstrument::NoteOn();
		
		Flang->NoteOn();
		
		Pos = 0.0;
		CurrentFre = Fre*Random(0.5, 2.0); 
	}

	bool Run()
	{
		SPInstrument::Run();
		
		
		Pos = Pos+TimeStep*CurrentFre;
		
		if (Pos > 1.0)
		{
			Pos = Pos-1.0;
			CurrentFre = FreMod*Fre*Random(0.5, 2.0);
			
		}
		//std::cout << "Pos: " << Pos;
		if (FlangFunc != NULL)
		{
			Flang->Phase = FlangFunc(PlayPos);
		}

		Flang->Input = Sine(Pos);
		
		Playing = Flang->Run();


		return Playing;
	}

	void NoteOff()
	{
		SPInstrument::NoteOff();
		
		Flang->NoteOff();
		
	}

};

#endif