#ifndef _MIXLEAD2_
#define _MIXLEAD2_

#include "Standards.h"
#include "Curves.h"
#include "Echo3D.h"

class MixLead2 : public SPTInstrument
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

	EnvelopeT *Env1;
	
	Echo3D *Echo;
	double EchoQuantity;

	MixLead2() : SPTInstrument()
	{
		Env1 = new EnvelopeT(0.008, 0, 0, 1, 0.07);
		Pos = 0;
		SlowPos = Random(0, 1.0);
	
		this->EchoQuantity = 1.0;
		Echo = new Echo3D(3, 6, 4, 30.0, 8.0);

		Pos3D Pos;
		for(int i=0;i<3;i++)
		{
			Pos.X = Random(0.0, 15.0);
			Pos.Y = Random(0.0, 15.0);
			Pos.Z = Random(0.0, 15.0);
			Echo->SetInPos(i, Pos);
		}

		for(int i=0;i<4;i++)
		{
			Pos.X = Random(0.0, 15.0);
			Pos.Y = Random(0.0, 15.0);
			Pos.Z = Random(0.0, 15.0);
			Echo->SetOutPos(i, Pos);
		}

		for(int i=0;i<6;i++)
		{
			Pos.X = Random(0.0, 15.0);
			Pos.Y = Random(0.0, 15.0);
			Pos.Z = Random(0.0, 15.0);
			Echo->SetReflectPos(i, Pos);

			*(Echo->ReflectSize+i) = Random(0.1, 0.5);
			//*(Echo->ReflectSize+i) = 4.0;
		}

		Echo->RandomizeDist(15);

		Pan1 = -0.7;
		Pan2 = -0.1;
		Pan3 = 0.9;
		Pan4 = 0.1;
	}

	void NoteOn()
	{
		Env1->NoteOn();
		

		Echo->NoteOn();

		SPTInstrument::NoteOn();
		
		Pos = 0;
		SlowPos = Random(0, 1.0);
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
			
			SlowPos = SlowPos + TimeStep*8.0;

			Amp1 = Amp1 + Crossfade(SqrTriSaw(Pos+0.20+Sine(SlowPos*0.63+0.13)*0.15*Fre/800.0, 0.97, 0.2), SqrTriSaw(Pos+0.87+Sine(SlowPos*0.88+0.705)*0.2*Fre/800.0, 0.95, 0.6), Tri(Pos+0.4))  * Env1->Amp * 0.3;
			Amp2 = Amp2 + Crossfade(SqrTriSaw(Pos+0.89+Sine(SlowPos*0.4+0.73)*0.35*Fre/800.0, 0.94, 0.0), SqrTriSaw(Pos+0.57+Sine(SlowPos*0.27+0.3333)*0.1*Fre/800.0, 0.91, 0.15), Sine(Pos+0.35+Sine(SlowPos)*0.19)) * Env1->Amp * 0.54;
			Amp3 = Amp3 + Crossfade(SqrTriSaw(Pos+1.01583+Sine(SlowPos*0.75+0.2876)*0.2*Fre/800.0, 0.985, 0.4), SqrTriSaw(Pos+0.17+Sine(SlowPos*0.266+0.55119)*0.27*Fre/800.0, 0.99, 0.3), SoftSqr(Pos+0.15))  * Env1->Amp * 0.4;
			
		}
		
		/*
		if (Abs(Amp1) < 0.001) Pan1 = Random(-1.0, 1.0);
		if (Abs(Amp2) < 0.001) Pan2 = Random(-1.0, 1.0);
		if (Abs(Amp3) < 0.001) Pan3 = Random(-1.0, 1.0);
		*/
		
		// Echo
		
		Amp1 = Amp1*(1.0+Sine(SlowPos*1.0791+0.23));
		Amp2 = Amp2*(1.0+Sine(SlowPos*1.44+0.87));
		Amp3 = Amp3*(1.0+Sine(SlowPos*1.24001));
		
		/*
		if (TimePos < 0.0004)
		{
			Amp1 = 0.8;
			Amp2 = 0.8;
			Amp3 = 0.8;
		}
		else
		{
			Amp1 = 0.0;
			Amp2 = 0.0;
			Amp3 = 0.0;
		}
		*/

		*(Echo->In+0) = Amp1;
		*(Echo->In+1) = Amp2;
		*(Echo->In+2) = Amp3;
		Playing = Echo->Run();
		
		//if (TimePos < 0.5) cout << Echo->Out << endl;
		

		Amp1 = Amp1 + EchoQuantity * (*(Echo->Out+0));
		Amp2 = Amp2 + EchoQuantity * (*(Echo->Out+1));
		Amp3 = Amp3 + EchoQuantity * (*(Echo->Out+2));
		Amp4 = Amp4 + EchoQuantity * (*(Echo->Out+3));

		
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