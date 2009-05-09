#ifndef _SYNTHBASS1_
#define _SYNTHBASS1_

#include "Standards.h"
//#include "Instrument.h"
#include "EnvelopeT.h"
#include "Curves.h"

class SynthBass1 : public StandardInstrument
{
public:
	double Pos;

	

	EnvelopeT *Envelope;

	double Mod1; // controls decay and synthy sound
	double ThinMod;

	SynthBass1() : StandardInstrument()
	{
		Envelope = new EnvelopeT();
		Envelope->Attack = 1.0/40.0;
		Envelope->Hold = 0;
		Envelope->Decay = 1.0/20.0;
		Envelope->Sustain = 0.8;
		Envelope->Release = 1.0/64.0;

		ThinMod = 0.5;

		Pos = 0;

		Mod1 = 3.0;
	}

	void NoteOn()
	{

		StandardInstrument::NoteOn();
	
		Envelope->NoteOn();
	}

	bool Run()
	{
		
		
		Playing = Envelope->Run();
		
		

		PlayPos = PlayPos+PlayStep;
		TimePos = TimePos+TimeStep;
		//double Env = Shot(PlayPos*30);
		//double Env = 1.0;
		
		
		
		Pos = Pos+TimeStep*Fre;	
		

		double SlowDecayValue = Fall1(TimePos*Mod1);
		double FastDecayValue = Fall1(TimePos*20.0);
		Amp = BSine(Pos+Sine(Pos+0.34)*SlowDecayValue*0.3, Fall1(TimePos*FastDecayValue)*ThinMod);
		Amp = Amp*Envelope->Amp;
	
			
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