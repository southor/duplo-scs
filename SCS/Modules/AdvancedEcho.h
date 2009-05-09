#ifndef _AdvancedEcho_
#define _AdvancedEcho_

#include "Module.h"
#include "FreeDelayModule.h"

class AdvancedEcho : public Module
{

public:

	//const double DEFAULT_REFLECT_SIZE = 0.3;

	bool On;
	int CountDown;

	double MaxDist;
	double MaxLength;

	int nInput;
	int nReflect;
	int nOutput;

	int nRecieving;

	FreeDelayModule **ReflectDelayLine;
	FreeDelayModule **OutputDelayLine;

	// Input/Outputs
	
	/* save all distances to lower number of square roots

	   from: input, reflection
       to:   reflection, output
	*/
	double Range;

	double *InputDistanceMap;
	double *ReflectDistanceMap;
	
	double *In;

	double *ReflectSize;

	double *Out;

	AdvancedEcho(int nInput, int nReflect, int nOutput, double MaxDist, double MaxLength) : Module()
	{

		//DEFAULT_REFLECT_SIZE = 0.3;
		Range = 2.0;

		this->MaxDist = MaxDist;
		this->MaxLength = MaxLength/TimeStep;

		this->nInput = nInput;
		In = new double[nInput];
		for (int i = 0;i<nInput;i++)
		{
			*(In+i) = 0;
		}


		this->nReflect = nReflect;
		ReflectSize = new double[nReflect];
		ReflectDelayLine = new FreeDelayModule*[nReflect];
		for (int i = 0;i<nReflect;i++)
		{
			*(ReflectSize+i) = 0.6;
			*(ReflectDelayLine+i) = new FreeDelayModule(MaxDist/340.0, false);
		}


		this->nOutput = nOutput;
		Out = new double[nOutput];
		OutputDelayLine = new FreeDelayModule*[nOutput];
		for (int i = 0;i<nOutput;i++)
		{
			*(Out+i) = 0;
			*(OutputDelayLine+i) = new FreeDelayModule(MaxDist/340.0, false);
		}


		
		//nSending = nInput+nReflect;
		nRecieving = nReflect+nOutput;

		//DistanceMapSize = nSending*(nSending*nRecieving);
		InputDistanceMap = new double[nInput*nRecieving];		
		for (int i = 0;i<(nInput*nRecieving);i++)
		{
			*(InputDistanceMap+i) = 0;
		}


		ReflectDistanceMap = new double[nReflect*nRecieving];
		for (int i = 0;i<(nReflect*nRecieving);i++)
		{
			*(ReflectDistanceMap+i) = 0;
		}

	}
	
	double GetMaxDist()
	{
		return MaxDist;
	}

	void RandomizeInDist(double Dist)
	{
		for (int i = 0;i<(nInput*nRecieving);i++)
		{
			*(InputDistanceMap+i) = Random(0.0, Dist);
		}
	}

	void RandomizeInDist()
	{
		RandomizeInDist(MaxDist);
	}

	void RandomizeReflectDist(double Dist)
	{
		for (int i = 0;i<(nInput*nRecieving);i++)
		{
			*(InputDistanceMap+i) = Random(0.0, Dist);
		}
	}

	void RandomizeReflectDist()
	{
		RandomizeReflectDist(MaxDist);
	}

	void RandomizeDist(double Dist)
	{
		RandomizeInDist(Dist);
		RandomizeReflectDist(Dist);
	}

	void RandomizeDist()
	{
		RandomizeInDist(MaxDist);
		RandomizeReflectDist(MaxDist);
	}


	void NoteOn()
	{
		int i;
		for (i = 0;i<nInput;i++)
		{
			*(In+i) = 0;
		}

		for (i = 0;i<nOutput;i++)
		{
			(*(OutputDelayLine+i))->NoteOn();
			*(Out+i) = 0;
		}
		
		for (i = 0;i<nReflect;i++)
		{
			(*(ReflectDelayLine+i))->NoteOn();
		}

		On = true;
		Playing = true;
	}

	bool Run()
	{
		
		

	
		double Dist;
		for (int i=0;i<nInput;i++)
		{
			for (int j=0;j<nReflect;j++)
			{
				Dist = *(InputDistanceMap+i*nRecieving+j);
				(*(ReflectDelayLine+j))->MixInput((*(In+i))/((Dist/Range)+1.0), (Dist/340.0)/TimeStep);
			}
			for (int j=0;j<nOutput;j++)
			{
				Dist = *(InputDistanceMap+i*nRecieving+nReflect+j);
				(*(OutputDelayLine+j))->MixInput((*(In+i))/((Dist/Range)+1.0), (Dist/340.0)/TimeStep);
			}
		}
		
		/*
		for (int i = 0;i<nReflect;i++)
		{
			
		}
		*/
		
		for (int i=0;i<nReflect;i++)
		{
			(*(ReflectDelayLine+i))->Run();
			
			for (int j=0;j<nReflect;j++)
			{
				Dist = *(ReflectDistanceMap+i*nRecieving+j);
				(*(ReflectDelayLine+j))->MixInput(((*(ReflectDelayLine+i))->Output(0))*(*(ReflectSize+i))/((Dist/Range)+1.0), (Dist/340.0)/TimeStep);
			}
			(*(ReflectDelayLine+i))->Input(0.0, 0);
			
			for (int j=0;j<nOutput;j++)
			{
				Dist = *(ReflectDistanceMap+(i+nReflect)*nRecieving+nReflect+j);
				(*(OutputDelayLine+j))->MixInput(((*(ReflectDelayLine+i))->Output(0))*(*(ReflectSize+i))/((Dist/Range)+1.0), (Dist/340.0)/TimeStep);
			}
			(*(ReflectDelayLine+i))->Input(0.0, 0);
		}
		
		/*
		for (int i = 0;i<nOutput;i++)
		{
				
		}
		*/
		for (int i=0;i<nOutput;i++)
		{
			(*(OutputDelayLine+i))->Run();
			*(Out+i) = (*(OutputDelayLine+i))->Output(0);
			(*(OutputDelayLine+i))->Input(0.0, 0);
		}

		if (On == false)
		{
			CountDown = CountDown - 1;
			if (CountDown <= 0) Playing = false;
		}

		return Playing;
	}

	void NoteOff()
	{
		On = false;
		CountDown = MaxLength;
	}

};

#endif