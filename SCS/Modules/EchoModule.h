#ifndef _EchoModule_
#define _EchoModule_

#include "FreeDelayModule.h"

class EchoModule : public FreeDelayModule
{
public:

	// Properties
	double Interval;
	int nEcho;
	double Dim;

	// Input
	double In;

	// Output
	double Out;

	EchoModule(double Interval, int nEcho) : FreeDelayModule(Interval * nEcho)
	{
		this->Interval = Interval;
		this->nEcho = nEcho;

		In = 0;
		Dim = 1;
	}

	void NoteOn()
	{
		FreeDelayModule::NoteOn();
		In = 0;
		Out = 0;
	}
	
	bool Run()
	{
		FreeDelayModule::Run();
		
		Input(In, 0);

		Out = 0;
		for(int i=0;i<nEcho;i++)
		{
			//cout << i << endl;
			
			Out = Out + Output((-1.0)*(i+1)*Interval/PlayStep)*Fall2S((double)i/(double)nEcho, Dim);
		}
		

		return Playing;
	}

};

#endif