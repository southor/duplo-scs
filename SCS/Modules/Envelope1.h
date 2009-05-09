




#ifndef _ENVELOPE1_
#define _ENVELOPE1_

#include "Standards.h"

class Envelope1 : public StandardModule
{

protected:
	double OffPos;

public:
	double AttackLength;
	double AttackSize;
	double ReleaseLength;
	double HoldAmp;

	bool On;
	bool Playing;
	
	double Output;
	Envelope1() : StandardModule()
	{		
		AttackLength = 0.05;
		AttackSize = 0.4;
		ReleaseLength = 0.1;
		HoldAmp = 0.8;
	
		Playing = false;
		On = false;
	}

	void NoteOn()
	{
		On = true;
		Playing = true;

		TimePos = 0;
		PlayPos = 0;
	}

	bool Run()
	{
		TimePos = TimePos+TimeStep;
		PlayPos = PlayPos+PlayStep;

		if (On)
		{
			if (TimePos < AttackLength) // attack period
			{
				double APos = Rise2S(TimePos/AttackLength, 1.0);
				Output = (rSin(APos*0.75)+APos*(1.0+HoldAmp))*((1-AttackSize)*APos+AttackSize);
			}
			else
			{
				Output = HoldAmp;
			}
		}
		else if (PlayPos < OffPos+CovFunc(ReleaseLength,Tempo)) // release period
		{
			double RPos = (TimeFunc(PlayPos,Tempo)-TimeFunc(OffPos,Tempo))/ReleaseLength;
			Output = HoldAmp*Fall2S(RPos,2.0);
		}
		else
		{
			Playing = false;
		}
		
		return Playing;
	}
	

	void NoteOff()
	{
		OffPos = PlayPos;
		On = false;
	}
};


#endif