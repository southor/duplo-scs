#ifndef _HARP_
#define _HARP_

#include "Standards.h"
#include "EnvelopeT.h"
#include "EchoModule.h"

class Harp : public STInstrument
{
public:

	double Pos;

	EnvelopeT *Env1;
	EnvelopeT *Env2;
	EchoModule *Echo1;
	EchoModule *Echo2;

	double Amp1, Amp2;
	double EchoAmount;
	bool useRingBuff;

	// to eliminate effect (high pass filter)
	double *RingBuffL;
	double *RingBuffR;
	int RingBuffSize;
	int RingBuffPos;
	double RingBuffSumL;
	double RingBuffSumR;

	Harp() : STInstrument()
	{
		Env1 = new EnvelopeT(0.005, 0.1, 0.15, 0.8, 0.25);
		Env2 = new EnvelopeT(0.005, 0.1, 0.1, 0.4, 0.17);
		Pos = 0;

		Amp1 = 0;
		Amp2 = 0;
	
		Echo1 = new EchoModule(0.03, 5); // the longer echo
		Echo2 = new EchoModule(0.04, 2);

		EchoAmount = 1.0;

		RingBuffSize = 1.0/(TimeStep*15.0);
		RingBuffL = new double[RingBuffSize];
		RingBuffR = new double[RingBuffSize];		
		ClearRingBuff();

		useRingBuff = true;

		Pos = 0.0;
		
	}

	void NoteOn()
	{
		Env1->NoteOn();
		Env2->NoteOn();

		//if ( ! Echo1->Playing) Echo1->NoteOn();
		//if ( ! Echo2->Playing) Echo2->NoteOn();
		Echo1->NoteOn();
		Echo2->NoteOn();

		STInstrument::NoteOn();
		Pos = Loop(Pos);
		Amp1 = 0;
		Amp2 = 0;
	}

	bool Run()
	{
		STInstrument::Run();
		
		
		

		Amp = 0;
		double EAmp = 0;

		if (Env1->Playing == true)
		{
			if(Env1->Run() == false)
			{
				Echo2->NoteOff();				
			}
		
			Env2->Run();

			Pos = Pos + TimeStep*Fre;		

			Amp = MultiCurve(Pos, 0.1, 0.7, 0.17+Rise2(TimePos*3.0)*0.25, -0.5, 0.5+Rise2(TimePos)*0.5, -1.0);
			Amp = Amp + BSine(Pos+0.45, 0.01)*0.2*Fall2(TimePos*30-0.01);
			EAmp = MultiCurve(Pos, 0.1, 0.7, 0.15+Rise2(TimePos*3.0+0.3)*0.25, -0.5, 0.5+Rise2(TimePos+0.3)*0.5, -1.0);
			EAmp = EAmp * Fall2(TimePos*2)*Env1->Amp*0.5;
			Amp = Amp * Fall2(TimePos*2)*Env1->Amp*0.5;
		
		}
		
		if(Echo2->Playing == true)
		{
			if(Echo2->Run() == false) Echo1->NoteOff();
		}
		
	
		//if (Echo->Out)

		// Echo
		Echo2->In = EAmp;
		Echo1->In = EAmp*0.7 + Echo2->Out*0.5;
		
	
		Playing = Echo1->Run();
		
		//if (TimePos < 0.5) cout << Echo->Out << endl;
		Amp1 = Echo1->Out * 0.5 * EchoAmount;
		Amp2 = Echo2->Out * 0.2 * EchoAmount + Amp + Echo1->Out * 0.2 * EchoAmount;

		// play with RingBuff
		if (useRingBuff)
		{
			RingBuffSumL = RingBuffSumL + Amp1 - *(RingBuffL + RingBuffPos);
			*(RingBuffL + RingBuffPos) = Amp1;
			Amp1 = Amp1 - (RingBuffSumL/RingBuffSize);
			RingBuffSumR = RingBuffSumR + Amp2 - *(RingBuffR + RingBuffPos);
			*(RingBuffR + RingBuffPos) = Amp2;
			Amp2 = Amp2 - (RingBuffSumR/RingBuffSize);
			RingBuffPos = (RingBuffPos+1)%RingBuffSize;
		}

		return Playing;
	}
	
	void NoteOff()
	{
		Env1->NoteOff();
		Env2->NoteOff();
		
		STInstrument::NoteOff();
	}

	void ClearRingBuff()
	{
		for(int i=0;i<RingBuffSize;i++)
		{
			*(RingBuffL+i) = 0.0;
			*(RingBuffR+i) = 0.0;
		}
		RingBuffPos = 0;
		RingBuffSumL = 0.0;
		RingBuffSumR = 0.0;
	}

	void End()
	{
		Echo1->End();
		Echo2->End();
		if (useRingBuff) ClearRingBuff();
	}

};

#endif