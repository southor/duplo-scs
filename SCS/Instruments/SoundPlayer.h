#ifndef _SOUND_PLAYER_
#define _SOUND_PLAYER_

#include "Standards.h"
#include "WaveSound.h"

class SoundPlayer : public Instrument
{
public:
	
	//bool On;
	double PlayPos;

	// output channels
	double AmpL;
	double AmpR;
	

	EnvelopeT *Envelope;
	WaveSound WSound;

	SoundPlayer(const char *Filename) : Instrument(), WSound(Filename)
	{
		AmpL = 0;
		AmpR = 0;
		//On = false;
		
		Envelope = new EnvelopeT(0.0001,0.0,1.0,1.0,0.1);
	}

	void NoteOn()
	{
		PlayPos = 0;
		Playing = true;
		
		//On = true;
		Envelope->NoteOn();
	}

	
	bool Run()
	{
		PlayPos = PlayPos+PlayStep;
		Envelope->Run();

		AmpL = WSound.GetLAmp(PlayPos) * Envelope->Amp;
		AmpR = WSound.GetLAmp(PlayPos) * Envelope->Amp;
		
		Playing = Envelope->Playing;
		return Playing;
		
	}

	void NoteOff()
	{
		Envelope->NoteOff();
	}

};

#endif