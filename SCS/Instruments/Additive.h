#ifndef _ADDITIVE_
#define _ADDITIVE_

#include "Standards.h"
#include "Curves.h"
#include "Image.h"
#include "limits.h"

class Additive : public Instrument
{
public:
	
	// Functional
	double TimePos;
	double Pos;
	double ReleaseStart;
	double Presence;
	double AttackSpeed;

	// Modules
	Image *Harmonic;

	// Output
	double Amp1, Pan1;
	double Amp2, Pan2;
	double Amp3, Pan3;

	// Modifications
	//double HighMute;
	double Apply1Bool; // bool value, > 0.5 means true
	double HighAmplify; // should not be zero but can be close to zero
	double HarmonicScaling; // can only span from -3.0 to 3.0
	double CutOff; // 0.0 - 1.0
	double Vol;	
	double Resolution;
	bool ConstantSpeed;
	double Release;
	

	Additive(char *filename) : Instrument()
	{
		Harmonic = new Image(filename);
		

		Pos = 0;
		Presence = 0.0;
		AttackSpeed = 1.0; // do not change this, not a constant.
		
		Amp1 = 0; Pan1 = -1;
		Amp2 = 0; Pan2 = 0;
		Amp3 = 0; Pan3 = 1;

		CutOff = 1.0;

		Resolution = 100;
		//HighMute = 1;
		HighAmplify = 1;
		HarmonicScaling = 0;
		ConstantSpeed = true;

		Release = 0.03;
		ReleaseStart = 1000000000.0;
		Vol = 1.0;

		//Apply1Bool = 1.0;
		
	}

	void ChangeImage(char *filename)
	{
		delete Harmonic;
		Harmonic = new Image(filename);
	}

	void NoteOn()
	{
		TimePos = 0;
		Pos = Loop(Pos);
		Playing = true;
		ReleaseStart = 1000000000.0;
	}

	bool Run()
	{
		TimePos = TimePos+TimeStep;
		//PlayPos = PlayPos+PlayStep;
		//double Env = Shot(PlayPos*30);
		//double Env = 1.0;
		Pos = Pos+TimeStep*Fre;	

		
		
		//Mod = Fall2(TimePos*8)*100+2;
		//Amp = Sine(Pos);

		//Amp = Sine(TriSaw(Pos,Sine(PlayPos*4)*0.1))*Sine(TriSqr(Pos*1,Sine(PlayPos*3)*0.1+0.2))*Sine(Pos+Sine(PlayPos*5)*0.15);
		//Amp = Tri((Tri(Pos*2)*Tri(Pos*7)*Tri(Pos*3)))+Tri(Pos*5)*Tri(Pos*11)*Tri(Pos*12);
		Amp1 = 0;
		Amp2 = 0;
		Amp3 = 0;
		
		double x;		
		double P = TimePos * Resolution / (double)Harmonic->Width();

		if (ConstantSpeed)
		{
				
			if (P > 2.0/3.0)
			{
				x = (Rise2(P-2.0/3.0)/3.0+2.0/3.0)*(double)Harmonic->Width();
			}
			else
			{
				x =  P * (double)Harmonic->Width();
			}
		}
		else
		{
			x = Rise2(P)*(double)Harmonic->Width();
		}
		int xi = x;
		double xRest = x - xi;

		double HighMute;
		if (HighAmplify > 0.0) HighMute = 1.0 / HighAmplify;
		else HighMute = 1.0;

		double HarmonicMod;
		if (HarmonicScaling >= 0.0) HarmonicMod = (HarmonicScaling + 1.0);
		else HarmonicMod = 0.25 * (HarmonicScaling + 4.0);


		//bool Apply1 = Apply1Bool >= 0.5;

		//if (Apply1) ApplyHarmonics1(x, HighMute, HarmonicMod);
		//else ApplyHarmonics2(x, HighMute, HarmonicMod);
		
		ApplyHarmonics1(xi, xRest, HighMute, HarmonicMod);
		
		
		
		//Amp = Fall2(Loop(Pos));

		
		if (TimePos > ReleaseStart+Release)
		{
			Playing = false;
			Presence = 0.0;
			AttackSpeed = 1.0;
		}
		else if (TimePos > ReleaseStart)
		{
			Presence = Fall2S((TimePos-ReleaseStart)/Release, 1.0);
			AttackSpeed = 1.0 - Presence;
			if (AttackSpeed > 1.0) AttackSpeed = 1.0;
		}
		else if (Presence < 1.0)
		{
			//MessageBox(NULL, "attack phase", "ok", MB_ICONEXCLAMATION | MB_OK);
			Presence += AttackSpeed * TimeStep * 1000.0; // change attack speed constant here
			if (Presence > 1.0) Presence = 1.0;
		}

		double ActualVol = Presence * Vol;
		Amp1 = Amp1*ActualVol;
		Amp2 = Amp2*ActualVol;
		Amp3 = Amp3*ActualVol;
				
		
		
		
		return Playing;
	}

	void NoteOff()
	{
		ReleaseStart = TimePos;		
		Playing = true;
	}

	void End()
	{
	}

	void ApplyHarmonics1(int x, double xRest, double HighMute, double HarmonicMod)
	{
		double Amp;
		double Wave;


		// CutOff calculation
		int max;		
		double rest2 = 1.0;
		if (CutOff == 1.0) max = Harmonic->Height();
		//else if (CutOff == 0.0) max = 1;
		else
		{
			double value = 1 + CutOff*CutOff*(Harmonic->Height()-1) + 0.5;
			
			max = value;			
			if (max > Harmonic->Height()) max = Harmonic->Height();
			else if (max < 1) max = 1;
			
			rest2 = value - max;			
			if (rest2 < 0.0) rest2 = 0.0;
			else if (rest2 > 1.0) rest2 = 1.0;
		}

		for (int i=1;i<=max;i++)
		{
			//cout << Harmonic->getValue(Rise2(TimePos*Harmonic->Width()),i) << endl;			
			//Rise2(TimePos)*(double)Harmonic->Width()
			int y = i-1;

			//double Phase = Wave*Harmonic->getValue2(x,y);
			int HarmonicNum = static_cast<int>((i-1)*HarmonicMod) + 1;
			
			Amp = 0.15 / ((double)HarmonicNum*HighMute+(1.0-HighMute));
			if (i == max) Amp *= rest2;
			Wave = rSin(Pos*HarmonicNum);

			double x0Amp = Amp *(1.0-xRest);
			double x1Amp = Amp * xRest;
			
			Amp1 = Amp1 + Wave*Harmonic->getValue1(x,y)*x0Amp;
			Amp2 = Amp2 + Wave*Harmonic->getValue2(x,y)*x0Amp;
			Amp3 = Amp3 + Wave*Harmonic->getValue3(x,y)*x0Amp;


			if (x < (Harmonic->Width()-1)) // do we have room to add second x value?
			{
				Amp1 = Amp1 + Wave*Harmonic->getValue1(x+1,y)*x1Amp;
				Amp2 = Amp2 + Wave*Harmonic->getValue2(x+1,y)*x1Amp;
				Amp3 = Amp3 + Wave*Harmonic->getValue3(x+1,y)*x1Amp;
			}

			//if (i% == 1) 
			//Amp = Amp + Fall2(i)*0.3*rSin(Pos*i)*(i%3);
		}
	}

	/*
	void ApplyHarmonics2(int x, double HighMute, double HarmonicMod)
	{
		double Amp;
		double Wave;

		int MaxHarmonicNum = (Harmonic->Height()-1)*HarmonicMod + 1;

		for(int i=0;i<MaxHarmonicNum;i++)
		{
			int y = i*HarmonicMod;
			if ((y >= 0) && (y < Harmonic->Height()))
			{
				//double Phase = Wave*Harmonic->getValue2(x,y);

				Amp = 0.15 / ((double)i*HighMute+(1.0-HighMute));
				Wave = rSin(Pos*i);

				Amp1 = Amp1 + Wave*Harmonic->getValue1(x,y)*Amp;
				Amp2 = Amp2 + Wave*Harmonic->getValue2(x,y)*Amp;
				Amp3 = Amp3 + Wave*Harmonic->getValue3(x,y)*Amp;
			}
		}
	}
	*/

};

#endif