#include "Base.h"
#include "WaveSound.h"

#ifndef _SOUNDRECORDER_
#define _SOUNDRECORDER_


class SoundRecorder
{
private:
	WaveSound *Sound;
	int WavePos;
public:

	SoundRecorder(WaveSound *Sound)
	{
		this->Sound = Sound;
		WavePos = 0;
	}
	
	void StartRecord(double PlayPos)
	{
		WavePos = WaveFunc(PlayPos,Tempo,Sound->GetWaveFre());
	}

	void Record(double LAmp, double RAmp)
	{
		Sound->MixLAmp(WavePos, LAmp);
		Sound->MixRAmp(WavePos, RAmp);
		WavePos++;
	}
	
};

#endif