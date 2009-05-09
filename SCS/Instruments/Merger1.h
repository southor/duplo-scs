#ifndef _MERGER1_
#define _MERGER1_

#include "Standards.h"
#include "Curves.h"
#include "WaveSound.h"


class Merger1 : public SPTInstrument
{
public:


	WaveSound *WS;

	Merger1(WaveSound *WS) : SPTInstrument()
	{

		this->WS = WS;
	}

	void NoteOn()
	{
		SPTInstrument::NoteOn();

	}

	bool Run()
	{
		SPTInstrument::Run();
		

		if (TimePos >= WS->GetTimeLength())
		{
			Playing = false;
		}
		double Pos = Rise2S(Abs(WS->GetAmp(PlayPos, true, true)), 0.8);

		Amp = 0;
		
		double Curve1 = NegSine(Pos + NegSine(Pos*2.0+0.23 + Tri(Pos)*0.4)*0.4)*0.8 + NegSine(Pos)*0.5 + NegSine(Pos*4)/8 + NegSine(Pos*5)/10 + BSine(Pos+0.5,0.1)*0.15;
		double Curve2 = NegSine(Pos+TriSaw(Pos + Rise2(PlayPos*2.0)*0.2, 0.5)*0.3+0.3)*NegSine(Pos*2.0)*0.8*Fall2(PlayPos*1.5);
		Amp = Merge( Curve2, NegSine(TriSaw(Pos,Shot1(PlayPos*30)*0.8)), Curve1);
		
		//Pos = TimePos*Fre;
		//Amp = Merge(Tri(Pos), SoftSqr(Pos), TriSaw(Pos, 0.8));

		return Playing;
	}
	
	void NoteOff()
	{
		SPTInstrument::NoteOff();
	}

	void End()
	{
		SPTInstrument::End();	
	}

};

#endif