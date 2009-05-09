#ifndef _MERGER3_
#define _MERGER3_

#include "Standards.h"
#include "Curves.h"


class Merger3 : public SPTInstrument
{
public:


	WaveSound *WS;
	
	// modifications
	double sp; // special point
	double spDelay;
	double spFre;
	double spAccept;

	// private
	double spCount;

	Merger3(WaveSound *WS) : SPTInstrument()
	{
		sp = 0.2;
		spDelay = 1.0/16000.0;
		spAccept = 0.4;
		this->WS = WS;
		spFre = 200.0;
		
		spCount = 0.0;
		
	}

	void NoteOn()
	{
		SPTInstrument::NoteOn();
		spCount = 0.0;
	}

	bool Run()
	{
		SPTInstrument::Run();
		

		if (TimePos >= WS->GetTimeLength())
		{
			Playing = false;
		}

		
		if (Inside(WS->GetAmp(PlayPos, true, true), sp, sp+spAccept) && (spCount > spDelay)) {
			spCount = 0.0;
			//std::cout << "zeros" << std::endl;
		}


		spCount = spCount+TimeStep;
		

		Amp = 0;
		
		Amp = Amp + TriSaw(spCount*spFre, 0.85);

		/*
		double Curve1 = NegSine(Pos + NegSine(Pos*2.0+0.23 + Tri(Pos)*0.4)*0.4)*0.8 + NegSine(Pos)*0.5 + NegSine(Pos*4)/8 + NegSine(Pos*5)/10 + BSine(Pos+0.5,0.1)*0.15;
		double Curve2 = NegSine(Pos+TriSaw(Pos + Rise2(PlayPos*2.0)*0.2, 0.5)*0.3+0.3)*NegSine(Pos*2.0)*0.8*Fall2(PlayPos*1.5);
		Amp = Merge( Curve2, NegSine(TriSaw(Pos,Shot1(PlayPos*30)*0.8)), Curve1);
		
		//Pos = TimePos*Fre;
		//Amp = Merge(Tri(Pos), SoftSqr(Pos), TriSaw(Pos, 0.8));
		*/

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