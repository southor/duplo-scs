#ifndef _VOICE1_
#define _VOICE1_

#include "Standards.h"
#include "Curves.h"
#include "EnvelopeT.h"

class Voice1 : public STInstrument
{
public:

	double Pos;

	EnvelopeT *Env1;
	//EnvelopeT *Env2;
	DelayModule *Delay1;
	DelayModule *Delay2;

	double VoblAmount;
	double VoblFre;
	double Brightness;

	Voice1() : STInstrument()
	{
		Env1 = new EnvelopeT(0.02, 0.1, 0.15, 0.85, 0.1);
		//Env2 = new EnvelopeT(0.005, 0.1, 0.1, 0.4, 0.1);
		Pos = 0;
		VoblFre = 0.004;
		VoblAmount = 0.3;
		Delay1 = new DelayModule(1.0/256.0);
		Delay2 = new DelayModule(1.0/175.12);

		Brightness = 0.8;
	}

	void NoteOn()
	{
		Env1->NoteOn();
		//Env2->NoteOn();
		Delay1->NoteOn();
		Delay2->NoteOn();


		STInstrument::NoteOn();
		Pos = 0;


	}

	bool Run()
	{
		STInstrument::Run();
		
		Playing = Env1->Run();
		//Env2->Run();

		Pos = Pos + TimeStep*Fre;
		
			
		Amp = 0;

		
		//std::cout << Delay1->Out*0.12 << std::endl;

		Amp = Amp + Delay1->Out*0.2;
		Amp = Amp + TriSqr(Pos+0.75, 0.6)*0.07;
		
		

		Amp = Amp + TriSqr(Pos + Vobl1(TimePos*VoblFre)*VoblAmount + Crossfade(Sine(Pos*2.0+0.57)+Tri(Pos+0.5), Amp, Sine(Pos+0.12))*0.13, Rise1((Vobl1(TimePos*VoblFre*3.9036)*VoblAmount*3.0 + Sine(Pos*3.0+0.80)*0.13)*Brightness))*0.5;

		Delay2->In = Amp;
		Delay2->Run();
	
		Amp = Amp + Tri(Pos + Vobl1(TimePos*VoblFre*3.15)*VoblAmount + Sine(Pos*2.0+0.12)*0.1)*0.35;
		Amp = Amp*0.15 + Crossfade(Amp, Delay2->Out*0.12, Rise1(Delay1->Out*2.5));
		Delay1->In = Amp;

		Delay1->Run();

	
		Amp = Amp * Env1->Amp;

		return Playing;
	}
	
	void NoteOff()
	{
		Env1->NoteOff();
		//Env2->NoteOff();
		Delay1->NoteOff();
		Delay2->NoteOff();
		
		STInstrument::NoteOff();
	}

};



#endif