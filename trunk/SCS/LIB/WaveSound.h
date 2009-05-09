

#ifndef _WAVESOUND_
#define _WAVESOUND_

#include "Base.h"
#include "fstream"
#include "iostream"

// can handle all wavefrequanses but only stereo sounds (not quite true yet)
class WaveSound
{
protected:
	
	int Length;
	int WaveFre;
	double *LArray;
	double *RArray;



	bool LoadFile(const char *FileName)
	{
		unsigned short int In;
	
		//ifstream WaveFile((const char*)FileName,ios::binary);

		std::ifstream WaveFile;
	
		if(FileName != NULL) WaveFile.open((const char*)FileName, std::ios::binary);

		if(WaveFile.good())
		{

			WaveFile.read((char *)&In,sizeof(unsigned short int));
			WaveFile.read((char *)&In,sizeof(unsigned short int));
			WaveFile.read((char *)&Length,sizeof(int));
			WaveFile.read((char *)&In,sizeof(unsigned short int));
			WaveFile.read((char *)&In,sizeof(unsigned short int));
			WaveFile.read((char *)&In,sizeof(unsigned short int));
			WaveFile.read((char *)&In,sizeof(unsigned short int));
			WaveFile.read((char *)&In,sizeof(unsigned short int));
			WaveFile.read((char *)&In,sizeof(unsigned short int));
			WaveFile.read((char *)&In,sizeof(unsigned short int));
			
			WaveFile.read((char *)&In,sizeof(unsigned short int));
			WaveFile.read((char *)&WaveFre,sizeof(int));
			
			
			//WaveFile.read((char *)&In,sizeof(unsigned short int));
			WaveFile.read((char *)&In,sizeof(unsigned short int));
			WaveFile.read((char *)&In,sizeof(unsigned short int));
			WaveFile.read((char *)&In,sizeof(unsigned short int));
			WaveFile.read((char *)&In,sizeof(unsigned short int));
			WaveFile.read((char *)&In,sizeof(unsigned short int));
			WaveFile.read((char *)&In,sizeof(unsigned short int));
			std::cout << Length;
			WaveFile.read((char *)&Length,sizeof(int));
			Length = Length/4;	

			LArray = new double[Length];
			RArray = new double[Length];

			short int LAmp;
			short int RAmp;

			for(int i=0;i<Length;i++)
			{
				WaveFile.read((char *)&LAmp, sizeof(short int));
				WaveFile.read((char *)&RAmp, sizeof(short int));
				*(LArray+i) = ((double)LAmp)/32768.0;
				*(RArray+i) = ((double)RAmp)/32768.0;
			}
		
		
			WaveFile.close();
			return true;
		}
		else
		{
			WaveFile.close();
			return false;
		}
		

	}


public:


	WaveSound()
	{
		Length = 0;
		LArray = NULL;
		RArray = NULL;
	}
	
	WaveSound(double SongLength, int WaveFre)
	{
		this->Length = WaveFre*TimeFunc(SongLength,Tempo);
		this->WaveFre = WaveFre;
		LArray = new double[Length];
		RArray = new double[Length];
		CleanSound();
	}


	WaveSound(const char *FileName)
	{
		LoadFile(FileName);
	}
	
	~WaveSound()
	{
		if (LArray != NULL) delete LArray;
		if (RArray != NULL) delete RArray;
	}

	void CleanSound()
	{
		for (int i=0;i<Length;i++)
		{
			*(LArray+i) = 0.0;
			*(RArray+i) = 0.0;
		}
	}

	double GetLAmp(int i)
	{
		if (Inside(i,0,Length) && Length > 0)
		{
			return *(LArray+i);
		}
		return 0;
	}
	double GetRAmp(int i)
	{
		if (Inside(i,0,Length) && Length > 0)
		{
			return *(LArray+i);
		}
		return 0;
	}

	double GetAmp(double PlayPos, bool Left, bool Right)
	{
		double MixMod = 1.5;
		double WavePos = WaveFunc(PlayPos,Tempo,WaveFre);
		
		double BetweenPos = WavePos-(int)WavePos;

		double FrontAmp = 0;
		double BackAmp = 0;
		if (Left)
		{
			MixMod = MixMod-0.5;
			FrontAmp = FrontAmp+GetLAmp((int)(WavePos+0.5));
			BackAmp = BackAmp+GetLAmp((int)WavePos);
			//cout << WaveFre << " ";
		}
		if (Right)
		{
			MixMod = MixMod-0.5;
			FrontAmp = FrontAmp+GetRAmp((int)(WavePos+0.5));
			BackAmp = BackAmp+GetRAmp((int)WavePos);
		}


		return ((FrontAmp*BetweenPos+BackAmp*BetweenPos*(-1))*MixMod);
	}

	double GetLAmp(double PlayPos)
	{
		return GetAmp(PlayPos, true,false);
	}
	
	double GetRAmp(double PlayPos)
	{
		return GetAmp(PlayPos, false,true);
	}

	int SetLAmp(int i, double Amp)
	{
		if (Inside(i,0,Length) && Length > 0)
		{
			*(LArray+i) = Amp;
			return 1;
		}
		else return 0;
	}
	
	int SetRAmp(int i, double Amp)
	{
		if (Inside(i,0,Length) && Length > 0)
		{
			*(RArray+i) = Amp;
			return 1;
		}
		else return 0;
	}

	int MixLAmp(int i, double Amp)
	{
		if (Inside(i,0,Length) && Length > 0)
		{
			*(LArray+i) = Limit(*(LArray+i) + Amp, -1.0, 1.0);
			return 1;
		}
		else return 0;
	}

	int MixRAmp(int i, double Amp)
	{
		if (Inside(i,0,Length) && Length > 0)
		{
			*(RArray+i) = Limit(*(RArray+i) + Amp, -1.0, 1.0);
			return 1;
		}
		else return 0;
	}

	int GetLength()
	{
		return Length;
	}

	double GetTimeLength()
	{
		return (Length/WaveFre);
	}

	int GetWaveFre()
	{
		return WaveFre;
	}

	// savewave
	int SaveWave(char* FileName)
	{
		unsigned short int Out;
	
		//ofstream WaveFile(FileName,ios::binary);

		std::ofstream WaveFile;
	
		if(FileName != NULL) WaveFile.open((const char*)FileName, std::ios::binary);

		if(WaveFile.good())
		{

			Out = 18770;
			WaveFile.write((char *)&Out,sizeof(unsigned short int));
			Out = 17990;
			WaveFile.write((char *)&Out,sizeof(unsigned short int));

			Length = 4*Length+36;
			WaveFile.write((char *)&Length,sizeof(int));

			Out = 16727;
			WaveFile.write((char *)&Out,sizeof(unsigned short int));
			Out = 17750;
			WaveFile.write((char *)&Out,sizeof(unsigned short int));
			Out = 28006;
			WaveFile.write((char *)&Out,sizeof(unsigned short int));
			Out = 8308;
			WaveFile.write((char *)&Out,sizeof(unsigned short int));
			Out = 16;
			WaveFile.write((char *)&Out,sizeof(unsigned short int));
			Out = 0;
			WaveFile.write((char *)&Out,sizeof(unsigned short int));
			Out = 1;
			WaveFile.write((char *)&Out,sizeof(unsigned short int));
			Out = 2;
			WaveFile.write((char *)&Out,sizeof(unsigned short int));
			Out = WaveFre;
			WaveFile.write((char *)&Out,sizeof(unsigned short int));
			Out = 0;
			WaveFile.write((char *)&Out,sizeof(unsigned short int));
			Out = 45328;
			WaveFile.write((char *)&Out,sizeof(unsigned short int));
			Out = 2;
			WaveFile.write((char *)&Out,sizeof(unsigned short int));
			Out = 4;
			WaveFile.write((char *)&Out,sizeof(unsigned short int));
			Out	= 16;
			WaveFile.write((char *)&Out,sizeof(unsigned short int));
			Out = 24932;
			WaveFile.write((char *)&Out,sizeof(unsigned short int));
			Out = 24948;
			WaveFile.write((char *)&Out,sizeof(unsigned short int));

			Length = Length-36;
			if (!(WaveFile.write((char *)&Length,sizeof(int))))
			{
				return 0;
			}
			Length = Length/4;

			short int LAmp, RAmp;
			

			for(int i=0;i<Length;i++)
			{
				LAmp = (*(LArray+i)) * 32767.0;
				RAmp = (*(RArray+i)) * 32767.0;

				WaveFile.write((char *)&LAmp, sizeof(short int));
				WaveFile.write((char *)&RAmp, sizeof(short int));
			}
		}

		WaveFile.close();
			return 1;
	}

};

#endif