#ifndef _TAMB_
#define _TAMB_

#include "Standards.h"
#include "Curves.h"

class Tamb : public StandardInstrument
{
public:
	double RPos[12];
	double Pos;
	double MinLength;

	Tamb() : StandardInstrument()
	{
		
		//TimePos = 2.0;
		for(int i=0;i<12;i++)
		{
			RPos[i] = rand()/15634.0;;
		}
		Pos = 0;

		MinLength = 0.4;
		
	}


	bool Run()
	{
		TimePos = TimePos+TimeStep;
		Pos = Pos+TimeStep*Fre;
		for(int i=0;i<12;i++)
		{
			RPos[i] = RPos[i]+TimeStep*Fre*rand()/(20000.0);
			//RPos[i] = RPos[i]+TimeStep*Fre;
		}
		
		Amp = Sqr(RPos[1]);
		
		Amp = Amp*Fall2S(TimePos*8,1);

		return (Playing || (TimePos < MinLength));
	}

};

#endif