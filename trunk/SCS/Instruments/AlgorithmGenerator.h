#ifndef _ALGORITHMGENERATOR_
#define _ALGORITHMGENERATOR_

#include "Standards.h"
#include "Curves.h"

class AlgorithmGenerator : public STInstrument
{
public:

	double Pos;

	int Data[256];


	AlgorithmGenerator() : STInstrument()
	{

		Pos = 0;

	}


	bool Run()
	{
		STInstrument::Run();
		
		
		

		Amp = 0;

	

		Pos = Pos + TimeStep*Fre;
	
		
		

	
		return Playing;
	}

};

#endif