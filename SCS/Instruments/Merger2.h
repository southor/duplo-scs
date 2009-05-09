#ifndef _MERGER2_
#define _MERGER2_

#include "Standards.h"
#include "Curves.h"


class Merger2 : public SPTInstrument
{
public:

	double Pos;
	double WavePos;
	WaveSound *WS;

	// modules
	EnvelopeT *Env1;

	Merger2(WaveSound *WS) : SPTInstrument()
	{
		Env1 = new EnvelopeT(0.006, 0, 0.05, 0.85, 0.02);
		Pos = 0;
		
		WavePos = 0.0;
		this->WS = WS;
	}

	void NoteOn()
	{
		SPTInstrument::NoteOn();
		Env1->NoteOn();
	}

	bool Run()
	{
		SPTInstrument::Run();
		Playing = Env1->Run();


		Pos = Pos + TimeStep*Fre;
		WavePos = WavePos+PlayStep;

		Amp = 0;
		
		Amp = Crossfade(TriSaw(Pos, 0.8), TriSqr(Pos+0.78843, 0.6), Sine(WS->GetAmp(WavePos, true, true)));


		return Playing;
	}
	
	void NoteOff()
	{
		SPTInstrument::NoteOff();
		Env1->NoteOff();
	}

	void End()
	{
		Env1->End();
		SPTInstrument::End();
		
	}

};

#endif