#ifndef _MULTITONE_
#define _MULTITONE_


struct ToneData
{
	short int Tone;
	double Start;
	double Length;
};



class MultiTone
{
protected:
	int nTone;
	ToneData *ToneArray;
	int iarray;
public:
	MultiTone(int nTone)
	{
		iarray = 0;
		this->nTone = nTone;
		if (nTone > 0)
		{
			ToneArray = new ToneData[nTone];
			for (int i=0;i<nTone;i++)
			{
				(ToneArray+i)->Start = 0;
				(ToneArray+i)->Length = 0;
				(ToneArray+i)->Tone = PAUSE;
			}
		}
		else
		{
			//cout << "ERROR: MultiTone needs at least 1 tone" << endl;
		}
	}

	MultiTone(MultiTone *Melody, int TransposeValue = 0)
	{
		*this = *Melody;
		ToneArray = new ToneData[nTone];
		for (int i=0;i<nTone;i++)
		{
			(ToneArray+i)->Start = Melody->GetStart(i);
			(ToneArray+i)->Length = Melody->GetLength(i);
			(ToneArray+i)->Tone = Melody->GetTone(i);
		}
		Transpose(TransposeValue);
	}


	~MultiTone()
	{
		//delete ToneArray;
	}
	
	void Transpose(int Value)
	{
		for (int i=0;i<nTone;i++)
		{
			if ((ToneArray+i)->Tone != PAUSE) (ToneArray+i)->Tone = (ToneArray+i)->Tone + Value;
		}
	}

	short int GetTone(int i)
	{

		if (Inside(i,0,nTone-1) && nTone > 1)
		{
			
			return (ToneArray+i)->Tone;
		}
		return PAUSE;
	}
	
	double GetLength(int i)
	{
		if (Inside(i,0,nTone-1) && nTone > 1)
		{
			
			
			return (ToneArray+i)->Length;
		}
		else return 0;
	}
	
	double GetStart(int i)
	{
		if (Inside(i,0,nTone-1) && nTone > 1)
		{
			return (ToneArray+i)->Start;
		}
		return 0;
	}
	
	void SetTone(double Length, double Space, short int Tone)
	{
		(ToneArray+iarray)->Length = Length;
		(ToneArray+iarray)->Tone = Tone;

		for (int i = iarray+1;i<nTone;i++)
		{
			(ToneArray+i)->Start = (ToneArray+i)->Start + Space;
		}
		iarray++;	
	}
	
	int Size()
	{
		return nTone;
	}
	
	int GetI()
	{
		return iarray;
	}

	void SetI(int i)
	{
		iarray = i;
	}

};



#endif