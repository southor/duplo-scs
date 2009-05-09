#ifndef _ENVELOPET_
#define _ENVELOPET_

#include "Standards.h"
#include "Curves.h"

class EnvelopeT : public STInstrument
{
public:
	
	

	// internal

	double ReleaseLevel;


	// changables

	double Attack;

	double Hold;
	
	double Decay;
	
	double Sustain;

	double Release;
	
	EnvelopeT(double A, double H, double D, double S, double R) : STInstrument()
	{
		Attack = A;
		Hold = H;
		Decay = D;
		Sustain = S;
		Release = R;

		//AttackFunc = Rise2;
		//ReleaseFunc= Fall3;
	}

	EnvelopeT() : STInstrument()
	{
		Attack = 0.01;
		Hold = 0.0;
		Decay = 0.0;
		Sustain = 1.0;
		Release = 0.03;

		//AttackFunc = Rise2;
		//ReleaseFunc= Fall3;
	}
	
	bool Run()
	{

		
		//(SPInstrument*)this->Run();
		STInstrument::Run();

		//cout << "skf" << endl;

		if (On)
		{
			if (TimePos < Attack)
			{
				Amp = Rise2S(TimePos/Attack,1);
			}
			else if (TimePos < Attack+Hold)
			{
				Amp = 1;
			}
			else if (TimePos < Attack+Hold+Decay)
			{
				Amp = Fall3((TimePos-(Attack+Hold))/Decay) * (1.0-Sustain) + Sustain;
			}
			else
			{
				Amp = Sustain;
			}
			//cout << "baby" << endl;
		}
		else if (ReleaseTime < Release)
		{
			//cout << "baby" << endl;
			Amp = Fall2S(ReleaseTime/Release, 1.0)*ReleaseLevel;
		}
		else
		{
			Playing = false;
		}
		
		return Playing;
	}

	void NoteOff()
	{
		STInstrument::NoteOff();

		ReleaseLevel = Amp;
	}
};

#endif