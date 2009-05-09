#ifndef _DELAYMODULE_
#define _DELAYMODULE_

#include "Module.h"

class DelayModule : public Module
{
protected:
	int Size;
	double *AmpArray;

	bool On;

	int CountDown;

	double MaxDelay;

	int ReadPos;
	int WritePos;
public:
	double Delay; // number of bars

	double In;
	double Out;
	
	void Reset()
	{
		//cout << "reset" << endl;
		for(int i=0;i<Size;i++)
		{
			*(AmpArray+i) = 0;
		}
	
		WritePos = Size-1;
		ReadPos = WritePos-Delay/PlayStep;
	}

	DelayModule(double MaxDelay) : Module() // MaxDelay in bars
	{
		this->MaxDelay = MaxDelay+PlayStep;
		this->Size = (this->MaxDelay)/PlayStep+1;
		AmpArray = new double[Size];
		Delay = MaxDelay;
		CountDown = Delay/PlayStep;

		On = false;
		In = 0;
		Out = 0;

		Reset();
	}

	int getMaxDelay()
	{
		return (Size-1)*PlayStep;
	}

	

	void NoteOn()
	{
		On = true;
		Playing = true;
	
		In = 0;
		Out = 0;
		Reset();
	}

	bool Run()
	{
		
		
		WritePos++;
		ReadPos++;
		if (ReadPos >= Size) ReadPos = 0;
		if (WritePos >= Size) WritePos = 0;
		
		if (On)
		{
			*(AmpArray+WritePos) = In;
		}
		else
		{
			//*(AmpArray+WritePos) = In; // test
			//cout << "countdown" << endl;
			CountDown = CountDown - 1;
			*(AmpArray+WritePos) = 0;
			if (CountDown <= 0) Playing = false;
		}
		
		Out = *(AmpArray+ReadPos);
		

		return Playing;
	}

	void NoteOff()
	{
		On = false;
		CountDown = Delay/PlayStep;
	}

	void End()
	{
		
	}

};

#endif