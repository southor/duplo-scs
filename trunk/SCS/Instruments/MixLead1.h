#ifndef _MIXLEAD1_
#define _MIXLEAD1_

#include "Standards.h"
#include "Curves.h"

class MixLead1 : public SPTInstrument
{
public:

	double Pos;
	double SlowPos;
	
	double Amp1;
	double Amp2;
	double Amp3;
	double Amp4;
	double Pan1;
	double Pan2;
	double Pan3;
	double Pan4;


	double Whine;
	double WhineSpeed;
	double WhineBehavour;
	double Brightness;	

	EnvelopeT *Env1;
	
	EchoModule *Echo;
	double EchoQuantity;

	MixLead1() : SPTInstrument()
	{
		Env1 = new EnvelopeT(0.008, 0, 0, 1, 0.07);
		Pos = 0;
		SlowPos = Random(0, 1.0);
	
		EchoQuantity = 0.2;
		Echo = new EchoModule(0.25/4.0, 4);

		Pan1 = 0.8;
		Pan2 = -0.6;
		Pan3 = -0.2;
		Pan4 = 0.1;

		Whine = 1.0;
		Brightness = 1.0;
		WhineSpeed = 2.8;
		WhineBehavour = 0.0;

		SlowPos = Random(0.0, 1.0);
	}

	void NoteOn()
	{
		Env1->NoteOn();
		

		Echo->NoteOn();

		SPTInstrument::NoteOn();
		
		Pos = 0;

		if (WhineBehavour < (1.0/3.0)) SlowPos = 0.0;
		else if (WhineBehavour > (2.0/3.0)) SlowPos = Random(0.0, 1.0);
		
	}

	bool Run()
	{
		SPTInstrument::Run();
		
		Amp1 = 0;
		Amp2 = 0;
		Amp3 = 0;
		Amp4 = 0;

		if (Env1->Playing == true)
		{
			if(Env1->Run() == false)
			{
				Echo->NoteOff();
			}
		
			

			Pos = Pos + TimeStep*Fre;
			
			SlowPos = SlowPos + TimeStep*WhineSpeed*WhineSpeed;

			Amp1 = Amp1 + Crossfade(SqrTriSaw(Pos+0.20+Whine*Sine(SlowPos*0.63+0.13)*0.15*Fre/800.0, 0.97*Brightness, 0.2*Brightness), SqrTriSaw(Pos+0.87+Whine*Sine(SlowPos*0.88+0.705)*0.2*Fre/800.0, 0.95*Brightness, 0.6*Brightness), Tri(Pos+0.4))  * Env1->Amp * 0.3;
			Amp2 = Amp2 + Crossfade(SqrTriSaw(Pos+0.89+Whine*Sine(SlowPos*0.4+0.73)*0.35*Fre/800.0, 0.94*Brightness, 0.0), SqrTriSaw(Pos+0.57+Whine*Sine(SlowPos*0.27+0.3333)*0.1*Fre/800.0, 0.91*Brightness, 0.15*Brightness), Sine(Pos+0.35+Whine*Sine(SlowPos)*0.19)) * Env1->Amp * 0.54;
			Amp3 = Amp3 + Crossfade(SqrTriSaw(Pos+1.01583+Whine*Sine(SlowPos*0.75+0.2876)*0.2*Fre/800.0, 0.985*Brightness, 0.4*Brightness), SqrTriSaw(Pos+0.17+Whine*Sine(SlowPos*0.266+0.55119)*0.27*Fre/800.0, 0.99*Brightness, 0.3*Brightness), SoftSqr(Pos+0.15))  * Env1->Amp * 0.4;
			
		}
		
		/*
		if (Abs(Amp1) < 0.001) Pan1 = Random(-1.0, 1.0);
		if (Abs(Amp2) < 0.001) Pan2 = Random(-1.0, 1.0);
		if (Abs(Amp3) < 0.001) Pan3 = Random(-1.0, 1.0);
		*/
		
		// Echo
		Echo->In = (1.0+Sine(SlowPos*1.0791))*Amp1 + (1.0+Sine(SlowPos*1.44))*Amp2 + (1.0+Sine(SlowPos*1.24001))*Amp3;
		Playing = Echo->Run();
		
		//if (TimePos < 0.5) cout << Echo->Out << endl;
		Amp4 = Amp4 + Echo->Out * EchoQuantity;
		
		//if (Abs(Amp4) < 0.001) Pan4 = Random(-1.0, 1.0);

		return Playing;
	}
	
	void NoteOff()
	{
		Env1->NoteOff();
		
		
		SPTInstrument::NoteOff();
	}

	void End()
	{
		Echo->End();
	}

};

#endif