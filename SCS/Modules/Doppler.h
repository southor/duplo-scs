#ifndef _DOPPLER_
#define _DOPPLER_


#include "math.h"
#include "Module.h"


class Doppler : public Module
{
protected:
	double X;
	double Y;
	
	double Dist;


public:

	double StartAngle;
	double StartDistance;
	// --------------
	double Direction;
	double Speed;
	double DistAffectVol;

	double Pan;
	double Vol;
	double FreMod;

	bool Playing;

	Doppler() : Module()
	{
		StartAngle = 0.25;
		StartDistance = 1.0;
		Direction = 0.25;
		Speed = 0.1;
		
		this->DistAffectVol = 12;	
		Playing = false;
	
	}

	void NoteOn()
	{
		X = StartDistance*rSin(StartAngle+0.5);
		Y = StartDistance*rSin(StartAngle+0.75);
		Dist = StartDistance;
		Playing = true; 
	}

	bool Run()
	{
		double Move = (this->Speed)*TimeStep;
		X = X + Move*rSin(Direction);
		Y = Y + Move*rSin(Direction+0.25);
		double OldDist = Dist;
		Dist = sqrt(X*X+Y*Y);
		FreMod = 1/(1-(OldDist-Dist)/TimeStep);
		Vol = 1/(1+Dist*DistAffectVol);

		double AY = Abs(Y);
		if (X>=0) Pan = 1-AY/(X+AY);
		else Pan = AY/(Abs(X)+AY)-1;
	
		return Playing;
	}
	
	void NoteOff()
	{
		Playing = false;
	}


};

#endif