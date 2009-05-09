#ifndef _HELICOPTER_
#define _HELICOPTER_

#include "Instrument.h"
#include "Doppler.h"
#include "Curves.h"

class Helicopter : public Instrument
{

protected:
	double Pos;

public:
	double Mod;
	double Amp;
	double Pan;

	double TimePos;

	Doppler *Dop;
	//double StartAngle; // angle from 0 to 1
	//double StartDistance; // distance, 1 = 350m
	//double Direction; // angle from 0 to 1
	//double Speed; // mac

	Helicopter(double Mod) : Instrument()
	{
		this->Mod = Mod;
		this->Pos = 0;
		this->TimePos = 0;
	
		this->Dop = new Doppler();
		
		Dop->StartAngle = 0.257;
		Dop->StartDistance = 0.5;
		Dop->Direction = 0.25;
		Dop->Speed = 0.1;
	}

	void NoteOn()
	{
		Pos = 0;
		TimePos = 0;
		Playing = true;
	
		
		Dop->NoteOn();
	}

	bool Run()
	{
		Dop->Run();
		Pan = Dop->Pan;
		
		//cout << Dop->FreMod << endl;
		
		double ResFre = Fre*(Dop->FreMod);
		Pos = Pos+TimeStep*ResFre;
		
		Amp = 0;
		double B = Bounce(Pos/2.8,0,0.90);
		Amp = Amp + 0.5 * TriSqr(Pos,B);
		Amp = Amp * B;
		
		B = Bounce(Pos/2.8,-0.90,0.90);
		double SAmp  = 0.5 * TriSaw(Pos,B);
		SAmp = SAmp * B;
		
		Amp = (Amp + SAmp) * Dop->Vol;
		
		
		//Amp = TriSqr(Pos,0);

		/*
		double LPos = Loop(Pos);
		if (LPos<1.0/3.0)
		{
			Amp = TriSaw(LPos*3,Bounce((Pos/2-1)/2,-0.90,0.90));
		}
		else if (LPos<1.5/3.0)
		{
			//cout << "t" << endl;
			//Amp = Tri(LPos*3-1);
			Amp = 0;
		}
		else if (LPos<2.5/3.0)
		{
			Amp = TriSaw(LPos*3-1.5,Bounce((Pos/2-1)/2,-0.90,0.90));
		}
		else
		{
			Amp = 0;
		}
		*/

		//Amp = Tri(Pos);
		
		//Amp = TriSaw(Pos,Bounce(240,-0.9,0.9));
		//Amp = TriSaw(Pos,3.0);
		

		//Amp = Saw(Bounce(TimePos*Fre,0,1));
		//Amp = Saw(Pos);
		
		TimePos = TimePos+TimeStep;
		//Amp = 0.6;//test
		return Playing;
	}

	void NoteOff()
	{
		Dop->NoteOff();
		Playing = false;
	}

	void End()
	{
	}
};

#endif