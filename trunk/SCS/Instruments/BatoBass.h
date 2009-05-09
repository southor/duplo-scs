#ifndef _BATOBASS_
#define _BATOBASS_

//#include "Standards.h"
#include "Instrument.h"
#include "EnvelopeT.h"
#include "Curves.h"

class BatoBass : public Instrument
{
public:
	double Amp;
	double Pos;

	// modifications
	double MaxDist;
	double DistSpeed;

	double PlayPos;

	EnvelopeT *Envelope;

	BatoBass() : Instrument()
	{
		Envelope = new EnvelopeT();
		Envelope->Attack = 1.0/40.0;
		Envelope->Hold = 0;
		Envelope->Decay = 1.0/20.0;
		Envelope->Sustain = 0.8;
		Envelope->Release = 1.0/4.0;
		
		Amp = 0;
		Pos = 0;

		MaxDist = 0.8;
		DistSpeed = 3.0;
	}

	void NoteOn()
	{

		PlayPos = 0;
		Playing = true;
		Amp = 0;
	
		Envelope->NoteOn();


	}

	bool Run()
	{
	
		Playing = Envelope->Run();
		

		PlayPos = PlayPos+PlayStep;
		//double Env = Shot(PlayPos*30);
		//double Env = 1.0;
		
		Pos = Pos+TimeStep*Fre;

		double FreDistMod = 500.0 / (300.0 + Fre);
		if (FreDistMod > 1.0) FreDistMod = 1.0;

		Amp = NegSine(TriSaw(Pos,Shot1(PlayPos*DistSpeed*DistSpeed*DistSpeed)*MaxDist*FreDistMod)) * Envelope->Amp;
		
		return Playing;

	}

	void NoteOff()
	{
		Envelope->NoteOff();
	}

	void End()
	{
	}

};

#endif