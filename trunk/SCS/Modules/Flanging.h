#ifndef _FLANGING_
#define _FLANGING_


#include "math.h"
#include "Module.h"


class Flanging : public Module
{
protected:
	
	double Wideness;


	bool On;
	int Countdown;
public:
	
	FreeDelayModule *DynamicDelay;
	FreeDelayModule *StaticDelay;


	double Phase;



	bool Playing;

	double Input;
	double Output;

	Flanging(double Wideness) : Module()
	{
		this->Wideness = Wideness;
		
		double Phase = -Wideness;
		double Speed = 1.0;
	
		StaticDelay = new FreeDelayModule(Wideness);
		DynamicDelay = new FreeDelayModule(Wideness*2.0);

		Input = 0.0;
		Output = 0.0;
	}

	void NoteOn()
	{ 
		DynamicDelay->NoteOn();
		StaticDelay->NoteOn();
		
		Playing = true;
		On = true;
		Output = 0.0;

	}

	bool Run()
	{
	
		Phase = Limit(Phase, -Wideness, Wideness);

		DynamicDelay->Input(Input, 0.0);
		StaticDelay->Input(Input, 0.0);

		Playing = DynamicDelay->Run();
		StaticDelay->Run();

		double WavePos = (Phase-Wideness)/PlayStep;
		double CWavePos = ceil(WavePos);

		Output = 0.0;
		Output = Output + 0.5 * (DynamicDelay->Output(WavePos)*(1.0-(CWavePos-WavePos)) + DynamicDelay->Output(CWavePos)*(CWavePos-WavePos));
		Output = Output + 0.5 * (StaticDelay->Output((-Wideness)/PlayStep));
		/*
		std::cout << "    flanging input: " << Input;
		std::cout << "    flanging output: " << Output << std::endl;
		*/
		



		if(!On)
		{
			Countdown = Countdown - 1;
			if (Countdown <= 0)
			{
			
				Playing = false;
				Input = 0.0;
			}
		}


		return Playing;
	}
	
	void NoteOff()
	{
		On = false;
		Countdown = DynamicDelay->getSize();

		DynamicDelay->NoteOff();
		StaticDelay->NoteOff();
	}


};

#endif