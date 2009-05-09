#ifndef _FLUTE_
#define _FLUTE_

#include "Instrument.h"
#include "DelayModule.h"
#include "EnvelopeT.h"
#include "Curves.h"

class Flute : public Instrument
{
public:
	
	// internal
	double Pos, CPos;
	double TimePos;
	double Length;

	// modules
	DelayModule *Echo1;
	DelayModule *Echo2;
	EnvelopeT *Env;

	// modification
	double Mod1, Mod2;
	double LeftEcho;
	double RightEcho;

	// output
	double Amp1, Amp2, Pan1, Pan2;

	Flute() : Instrument()
	{
		Pos = 0;
		CPos = 0;
		TimePos = 0;
		Length = 0.01;
	
		Echo1 = new DelayModule(0.007);
		Echo2 = new DelayModule(0.015);
		Env = new EnvelopeT(0.05, 0, 0.07, 0.9, 0.05);


		Mod1 = 0.8;
		Mod2 = 0.8;

		LeftEcho = 0.3;
		RightEcho = 0.15;

		Amp1 = 0;
		Amp2 = 0;
		Pan1 = -1;
		Pan2 = 1;
	}

	void NoteOn()
	{
		TimePos = 0;
		Pos = 0;
		Playing = true;
		CPos = Mod2;


		Echo1->NoteOn();
		Echo2->NoteOn();
		Env->NoteOn();
	}

	bool Run()
	{
		TimePos = TimePos+TimeStep;
		//PlayPos = PlayPos+PlayStep;
		//double Env = Shot(PlayPos*30);
		//double Env = 1.0;
		//Pos = Pos+TimeStep*Fre*Fall2(TimePos*30);	
		Pos = Pos + TimeStep*Fre;

		double Amp = 0;
		
		if (Env->Playing)
		{
			if (Env->Run() == false)
			{
				Echo1->NoteOff();
				Echo2->NoteOff();
			}
		
			//Amp = Sine(Pos);

			//Amp = Sine(TriSaw(Pos,Sine(PlayPos*4)*0.1))*Sine(TriSqr(Pos*1,Sine(PlayPos*3)*0.1+0.2))*Sine(Pos+Sine(PlayPos*5)*0.15);
			//Amp = Tri((Tri(Pos*2)*Tri(Pos*7)*Tri(Pos*3)))+Tri(Pos*5)*Tri(Pos*11)*Tri(Pos*12);
		

		
			//Amp = Amp + BSine(Pos + (Sine(Pos)+Sine(Pos*2.0)/2+Sine(Pos*3.0)/3)*0.1 + Sine(Pos + Sine(Pos + 0.3)*0.2)*0.4*Env->Amp,0.3);
			
			//Amp = Amp + Sine(Pos + Sine(Pos*3.0)*0.6);
			
			//Amp = Amp + Crossfade(TriSaw(Pos*2, 0.9-Env->Amp*0.3), Sine(Pos), SineSqr(Pos));
	
			/*
			double Env1 = Tot(TimePos<0.15, 0.9-TimePos*0.1/0.15, 0.8);
			Amp = Amp + Crossfade(TriSaw(Pos*2, Env1), Sine(Pos), SineSqr(Pos));
			Amp = Amp + Tot(Inside(Loop(Pos),0.7,0.8), TriSaw(Pos*5, 0.9)*Env1*0.5, 0);
			Amp = Amp * Tot(TimePos<0.07, TimePos/0.07, 1.0);
			Amp = Amp * Env->Amp;
			*/
			Amp = 0;
			
			CPos = CPos+PosFunc(Tempo,PlayStep,Fre+Mod1*Fre*rSin(Pos));
			Amp = Amp + NegSine(CPos);  
			//Amp = Amp + SineSqr(Pos);
			Amp = Amp + Sqr(Pos) * Fall2(TimePos*15+0.2) * 0.6;
			Amp = Amp - NegSine(CPos) * Fall2(TimePos*15+0.2) * 0.6;
			Amp = Amp * Env->Amp;
		
			//Output = Fall2(TimePos*25)*Rise2(Vobl1(TimePos*10)); // add noise
			//Output = Fall2(TimePos*50)*(Vobl1(TimePos*10)-0.5);
			
		}
		
		Echo1->In = Amp;
		Echo2->In = Amp;

		Echo1->Run();
		Playing = Echo2->Run();


		Amp1 = Amp*0.5;
		Amp2 = Amp*0.6;
	

		Amp1 = Amp1 + Echo1->Out*LeftEcho;
		Amp2 = Amp2 + Echo2->Out*RightEcho;

		Amp = Amp* 300/(200-Fre);
		//if (TimePos/Length > 1) Playing = false;

		return Playing;
	}

	void NoteOff()
	{
		
		Env->NoteOff();
	}

	void End()
	{
		Echo1->End();
		Echo2->End();
		Env->End();
	}

};

#endif