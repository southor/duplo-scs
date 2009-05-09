#ifndef _BRAMBO_
#define _BRAMBO_

#include "Standards.h"
#include "Curves.h"

class Brambo : public Instrument
{
public:
	double Pos;
	double FMPos;
	double TimePos;
	double PlayPos;
	double Length;
	
	// mods
	double Harm1;
	double Harm1Multiple;
	double InHarm;
	double Harm2;	
	double Harm2Multiple;
	
	double Amp;


	Brambo() : Instrument()
	{
		Pos = 0;
		FMPos = 0;
		TimePos = 0;
		Length = 0.5;

		Harm1 = 0.15;
		Harm2 = 0.05;

		Harm1Multiple = 2.5;
		Harm2Multiple = 3.5;

		InHarm = 0.001;

		Amp = 0;
	}

	void NoteOn()
	{
		TimePos = 0;
		PlayPos = 0;
		Pos = 0;
		FMPos = 0;
		Playing = true;
	}

	bool Run()
	{

		double AInHarm = InHarm*InHarm;
		double FMValue = rSin(FMPos*(((int)Harm1Multiple) + AInHarm))*Harm1 + rSin(FMPos*(((int)Harm2Multiple) + AInHarm))*Harm2;
		
		double APos = Pos + 0.5 + FMValue;
		//double APos = Pos;

		if (TimePos < Length)
		{
			Amp = 0.0;
			Amp = Amp + Top(APos)*Soft((APos)+Hill1((APos)*5)*Soft((APos)*3.0)/3);
			Amp = Amp + Dep(APos)*Soft((APos)+Hill1((APos)*5)*Soft((APos)*3.0)/3);
			Amp = Amp * 0.2;

		
			if (TimePos < 1.0/1024.0)
			{
				Amp = Amp*Rise1(TimePos/(1.0/1024.0))*2.0;
			}
			else
			{
				Amp = Amp*(TimePos/(Length/2.0)-2.0)*(TimePos/(Length/2.0)-2.0)/2.0;		
			}
		}



		TimePos = TimePos+TimeStep;
		PlayPos = PlayPos+TimeStep;

		FMPos = FMPos + TimeStep*Fre;

		


		//double ActualFre = Fre * (1.0 + FMValue * 4.0);
		//if (ActualFre <= 1.0) ActualFre = 1.0;
		double ActualFre = Fre;

		Pos = Pos + TimeStep*ActualFre;
		

		
		

		


		if (TimePos/Length > 1.0)
		{
			Playing = false;
			//std::cout << "okey drum ends" << std::endl;
		}
		return Playing;

	}

	void NoteOff()
	{
	}

	void End()
	{
	}

};

#endif