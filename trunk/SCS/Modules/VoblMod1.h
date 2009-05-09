#ifndef _VOBLMOD1_
#define _VOBLMOD1_

#include "Standards.h"

class VoblMod1 : public StandardModule
{
public:

	double Speed;
	double Output;

	VoblMod1() : StandardModule()
	{
		Speed = 10;
		
	}

	bool Run()
	{		
		PlayPos = PlayPos + PlayStep;
		Output = Vobl1(PlayPos*Speed/20);
	
		return true;
	}
	
};

#endif