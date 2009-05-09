#ifndef _ORGANBASS_
#define _ORGANBASS_

#include "Instrument.h"
#include "Curves.h"

class OrganBass : public Instrument
{
public:
	double Pos;
	double TimePos;
	double Length;

	DelayModule *Echo1;
	DelayModule *Echo2;

	double Mod; // mod between -1.0 and 1.0

	double Amp1, Amp2, Pan1, Pan2;

	OrganBass() : Instrument()
	{
		Pos = 0;
		TimePos = 0;
		Length = 0.01;
	
		Echo1 = new DelayModule(0.007);
		Echo2 = new DelayModule(0.015);

		Mod = -1.0;
		Amp1 = 0; Amp2 = 0;
		Pan1 = -1; Pan2 = 1;
	}

	void NoteOn()
	{
		TimePos = 0;
		Pos = 0;
		Playing = true;
	
		Echo1->NoteOn();
		Echo2->NoteOn();
	}

	bool Run()
	{
		TimePos = TimePos+TimeStep;
		//PlayPos = PlayPos+PlayStep;
		//double Env = Shot(PlayPos*30);
		//double Env = 1.0;
		//Pos = Pos+TimeStep*Fre*Fall2(TimePos*30);	
		Pos = Pos + TimeStep*Fre;

		
		
		//Amp = Sine(Pos);

		//Amp = Sine(TriSaw(Pos,Sine(PlayPos*4)*0.1))*Sine(TriSqr(Pos*1,Sine(PlayPos*3)*0.1+0.2))*Sine(Pos+Sine(PlayPos*5)*0.15);
		//Amp = Tri((Tri(Pos*2)*Tri(Pos*7)*Tri(Pos*3)))+Tri(Pos*5)*Tri(Pos*11)*Tri(Pos*12);
		double Amp = 0;
		//Amp = Amp + rSin(Pos*1)*0.2+rSin(Pos*2)*0.1+rSin(Pos*3)*0.1+rSin(Pos*4)*0.05+rSin(Pos*5)*0.025+rSin(Pos*6)*0.025;
		
		//Amp = Amp + Rec(Pos,0.1);
		
		//Amp = Amp + Rec(Pos*0.78,Mod)*0.7+Rec(Pos*0.67+0.8,Mod)*0.5+Rec(Pos*0.95+0.34,Mod)*0.3+Rec(Pos*0.556+0.9,Mod)*0.8+Rec(Pos*0.955001+0.2,Mod)*0.3;
		
		//Amp = Amp + Crossfade(Tri(Pos*4.0),SineSqr(Pos*2.0),Sine(Pos)+Echo1->Out*0.10);
		
		Amp = Amp + NegSine(Pos + Crossfade(Tri(Pos*4.0), SoftSqr(Pos*2.0), Crossfade(Echo1->Out, Sine(Pos), Mod))*TimePos*Fall2(TimePos));
		
		//cout << Amp << endl;
		
		Amp = Amp*Fall2(TimePos);
		Amp = Amp*Rise2(TimePos*1000);

		Echo1->In = Amp;
		Echo2->In = Amp;
		
		Echo1->Run();
		Playing = Echo2->Run();
		
		
		//Amp = Amp + Echo1->Out*0.5;
		//Amp = Amp + Echo2->Out*0.2;

		//if (TimePos/Length > 1) Playing = false;

		Amp1 = Amp*0.5;
		Amp2 = Amp*0.6;

		Amp1 = Amp1 + Echo1->Out*0.3;
		Amp2 = Amp2 + Echo2->Out*0.4;

		return Playing;
	}

	void NoteOff()
	{
		Echo1->NoteOff();
		Echo2->NoteOff();
	}

	void End()
	{
		Echo1->End();
		Echo2->End();
	}

};

#endif