#ifndef _NOISE_
#define _NOISE_

#include "Standards.h"
#include "Curves.h"


class Noise : public StandardInstrument
{
protected:
	double D;
	double E1;

	double FreFix;
	double AmpFix;
public:
	
	
	// Modifications
	double RandomMod;
	double ControlMod;

	// output
	double E;

	Noise() : StandardInstrument()
	{
		FreFix = 8.3;
		AmpFix = 7.7;

		RandomMod = 0.05;
		ControlMod = 5.0;
		E1 = 0;
		D = 0.3;
	}

	Noise(double RandomMod, double ControlMod) : StandardInstrument()
	{
		FreFix = 8.3;
		AmpFix = 7.7;
		
		this->RandomMod = RandomMod;
		this->ControlMod = ControlMod;
		E1 = 0;
		D = 0.2;
	}

	bool Run()
	{
		//cout << (rand()/32768.0-0.5) << endl;

		D = D+((rand()*rand()/(32768.0*32768.0)-0.5)*RandomMod-ControlMod*E1*TimeStep)*Fre/FreFix;
		E1 = E1+D/AmpFix;
		
		E = Rise2(E1);

		

		return Playing;
	}

};


#endif