#ifndef _FREEDELAYMODULE_
#define _FREEDELAYMODULE_

#include "Module.h"

class FreeDelayModule : public Module
{
protected:
	int Size;
	double *AmpArray;

	bool On;

	int CountDown;

	double MaxDelay;

	int Pos;
	//int ReadPos;
	//int WritePos;
public:
	//double Delay; // number of bars

	//double In;
	//double Out;
	
	void Reset()
	{
		//cout << "reset" << endl;
		for(int i=0;i<Size;i++)
		{
			*(AmpArray+i) = 0;
			//cout << "fan" << endl;
		}
	
		Pos = Size-1;
		//ReadPos = WritePos-Delay/PlayStep;
	}

	FreeDelayModule(double MaxDelay, bool Rythmic = true) : Module() // MaxDealy in bars
	{
		//if (MaxDelay = 2.0) cout << "jasdasdaaaa";
		if (Rythmic)
		{
			this->MaxDelay = MaxDelay + PlayStep;
		}
		else
		{
			this->MaxDelay = (MaxDelay + PlayStep)*((Tempo/60.0)/4.0);
		}
		this->Size = (this->MaxDelay)/PlayStep+1;
		AmpArray = new double[Size];
		//Delay = MaxDelay;
		CountDown = this->MaxDelay/PlayStep;

		On = false;

		Reset();
	}

	int getMaxDelay()
	{
		return (Size-1)*PlayStep;
	}

	int getSize()
	{
		return Size;
	}

	

	void NoteOn()
	{
		Reset();
		
		Pos = Size-1;
		On = true;
		Playing = true;
		
	}

	bool Run()
	{		
		Pos++;
		//WritePos++;
		//ReadPos++;
	
		if (On == false)
		{
			CountDown = CountDown - 1;
			if (CountDown <= 0) Playing = false;
		}

		return Playing;
	}

	void Input(double In, int WavePos)
	{
		//cout << "input " << PlayPos << endl;

		//int Pos = (Loop(this->Pos*PlayStep + PlayPos,0,MaxDelay))/PlayStep;
		
		int Pos = (this->Pos+WavePos)%Size;
		//cout << "input " << Pos << endl;
		
		*(AmpArray+Pos) = In;
	}

	void MixInput(double In, int WavePos)
	{
		int Pos = (this->Pos+WavePos)%Size;
		//cout << "input " << Pos << endl;
		
		*(AmpArray+Pos) = *(AmpArray+Pos) + In;
	}



	double Output(int WavePos)
	{
		//cout << "output " << PlayPos << endl;
		//int Pos = (Loop(this->Pos*PlayStep + PlayPos,0,MaxDelay))/PlayStep;

		int Pos = (this->Pos+WavePos);
		if (Pos < 0)
		{
			Pos = (Size-Pos)%Size;
		}
		else
		{
			Pos = Pos%Size;
		}

		//cout << "output " << Pos << endl;
		
		return *(AmpArray+Pos);
	}

	void NoteOff()
	{
		On = false;
		CountDown = 2.0*MaxDelay/PlayStep;
	}


};

#endif