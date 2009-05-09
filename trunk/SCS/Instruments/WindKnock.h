#ifndef _WINDKNOCK_
#define _WINDKNOCK_

#include "Standards.h"
#include "Curves.h"

class WindKnock : public StandardInstrument
{
public:
	double Pos;
	

	WindKnock() : StandardInstrument()
	{
		Pos = 0;
	}


	bool Run()
	{
		TimePos = TimePos+TimeStep;
		Pos = Pos+TimeStep*Fre;	
		
		Amp = rSin(Pos*2+Tri(Pos+0.35)*0.2+Tri(Pos*3.7+0.6)*0.35)*0.7;
		
		Amp = Amp*Fall2S(TimePos*20,1);

		return Playing;
	}

};

#endif