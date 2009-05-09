
#include "Base.h"
#include "Base.h"
#include "WaveSound.h"
#include "SoundRecorder.h"
#include "SoundCollector.h"
#include "MultiTone.h"


#ifndef _MUSICGENERATOR_
#define _MUSICGENERATOR_





class MusicGenerator : public SoundCollector
{	

protected:
	SoundRecorder* Recorder;

	double PlayPos;
	double EndPos;

	void Record()
	{
		//double VolMod = Vol*GlobalVol/10000.0; // original
		double VolMod = GlobalVol/10000.0; // changed
		Recorder->Record(LAmp*VolMod, RAmp*VolMod);
	}

	void StartTone(double Start,double Duration, int Tone)
	{
		if(StartRandom != 0.0)
		{
			Start = Start + Random(-1.0, 1.0)*StartRandom;
		}
		if(LengthRandom != 0.0)
		{
			Duration = Duration + Random(-1.0, 1.0)*LengthRandom;
		}
		Recorder->StartRecord(Start);
		this->PlayPos = Start;
		this->Duration = Duration;
		this->EndPos = Start+Duration;
		this->Fre = Frequency(FoundTone,Oct,Tone);
		NoteOn();
	}

public:

	bool Active;
	double StartRandom;
	double LengthRandom;

	MusicGenerator(WaveSound *Out) : SoundCollector()
	{
		Recorder = new SoundRecorder(Out);
		PlayPos = 0;
		Active = true;
		StartRandom = 0;
		LengthRandom = 0;
	}



	void StartPlay(double Start,double Duration, int Tone, double Length)
	{
		StartTone(Start, Duration, Tone);
		Play(Length);
	}

	void Play(double Length)
	{
		if (Active)
		{
			
			double End = PlayPos+Length;
			for(;PlayPos<End;PlayPos = PlayPos+PlayStep)
			{

				Run();
				
				Record();
			}
		}
	}

	void Play(int Tone, double Length)
	{
		Fre = Frequency(FoundTone,Oct,Tone);
		Play(Length);
	}

	void PlayRest(int Tone)
	{
		this->Fre = Frequency(FoundTone,Oct,Tone);
		PlayRest();
	}

	void PlayRest()
	{
		
		//cout << "playpos: " << PlayPos << endl;
		Play(EndPos-PlayPos);
		


		NoteOff();
		
		//cout << "playpos: " << PlayPos << endl;
		if (Active)
		{
		
			while(Playing)
			{
				PlayPos = PlayPos + PlayStep;
				Run();
				Record();
			}
		}
		
		//cout << "playpos: " << PlayPos << endl;
		
		End();
	}

	void PlayHole(double Start,double Duration, int Tone)
	{
		if (Tone != PAUSE)
		{
			StartTone(Start,Duration,Tone);
			//cout << "ok"<< endl;
			PlayRest();
			//cout << "hej" << endl;
		}
	}
		

	



	void PlayTones(double Start, double Duration, double Interval, int n, int Tone)
	{
		for (int i=0;i<n;i++)
		{
			PlayHole(Start+i*Interval,Duration,Tone);
		}
	}

	void PlayMulti(double Start, MultiTone *Tones, int Trans = 0)
	{
		for(int i=0;i<Tones->Size();i++)
		{
			if (Tones->GetTone(i) != PAUSE)
			{
				
				PlayHole(Start+Tones->GetStart(i),Tones->GetLength(i),Tones->GetTone(i)+Trans);
			}
		}
	}

	void PlayChord(double Start, double Duration, int Tone1 = PAUSE, int Tone2 = PAUSE, int Tone3 = PAUSE, int Tone4 = PAUSE, int Tone5 = PAUSE)
	{
		PlayHole(Start, Duration, Tone1);
		PlayHole(Start, Duration, Tone2);
		PlayHole(Start, Duration, Tone3);
		PlayHole(Start, Duration, Tone4);
		PlayHole(Start, Duration, Tone5);
	}

	void PlayChords(double Start, double Duration, double Interval, int n, int Tone1 = PAUSE, int Tone2 = PAUSE, int Tone3 = PAUSE, int Tone4 = PAUSE, int Tone5 = PAUSE)
	{
		for (int i=0;i<n;i++)
		{
			PlayChord(Start+i*Interval,Duration,Tone1, Tone2, Tone3, Tone4, Tone5);
		}
	}

	void OctUp(int n = 1)
	{
		Oct = Oct+n;
	}

	void OctDown(int n = 1)
	{
		Oct = Oct-n;
	}
	
};

#endif