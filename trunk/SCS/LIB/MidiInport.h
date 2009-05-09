

#ifndef _MIDIINPORT_
#define _MIDIINPORT_

#include "MusicGenerator.h"
#include "fstream"
#include "iostream"

struct VolGenerator
{
	MusicGenerator *gen;
	double specialVol;
};

struct DrumGenerator
{
	MusicGenerator *gen;
	int note;
	double specialVol;
};


// can handle all wavefrequanses but only stereo sounds (not quite true yet)
class MidiInport
{
protected:
	
	const static int OFFSET_NOTE = 60;	

	double totalStart;
	double totalEnd;

	VolGenerator musicGenerator[17];

	bool treatDrums;
	DrumGenerator drumGenerator[128];
	double drumVol;
	double drumPan;


public:


	MidiInport(double totalStart, double totalEnd, bool treatDrums)
	{
		this->treatDrums = treatDrums;
		this->totalStart = totalStart;
		this->totalEnd = totalEnd;
		
		for(int i=0;i<17;i++)
		{
			musicGenerator[i].gen = NULL;
			musicGenerator[i].specialVol = 1.0;
		}

		for(int i=0;i<128;i++)
		{
			drumGenerator[i].gen = NULL;
			drumGenerator[i].specialVol = 1.0;
			drumGenerator[i].note = 0;
		}
		drumVol = 100.0;
		drumPan = 0.0;
	}

	MusicGenerator *getGenerator(int i)
	{
		return musicGenerator[i-1].gen;
	}
	
	void setGenerator(int i, MusicGenerator *musicGenerator, double specialVol = 1.0)
	{
		this->musicGenerator[i-1].gen = musicGenerator;
		this->musicGenerator[i-1].specialVol = specialVol;
	}

	void setSpecialVol(int i, double specialVol)
	{
		this->musicGenerator[i-1].specialVol = specialVol;
	}


	// drums

	MusicGenerator *getDrumGenerator(int i)
	{
		return drumGenerator[i].gen;
	}
	
	void setDrumGenerator(int i, MusicGenerator *drumGenerator, int drumNote, double specialVol = 1.0)
	{
		this->drumGenerator[i].gen = drumGenerator;
		this->drumGenerator[i].specialVol = specialVol;
		this->drumGenerator[i].note = drumNote;
	}

	void setSpecialDrumVol(int i, double specialVol)
	{
		this->drumGenerator[i].specialVol = specialVol;
	}

	int getDrumNote(int i)
	{
		return this->drumGenerator[i].note;
	}

	void setDrumNote(int i, int note)
	{
		this->drumGenerator[i].note = note;
	}




	// midi functions

	__int8 getVol(__int8 channel)
	{
		if (channel == 9)
		{			
			return drumVol;
		}
		else
		{
			if (musicGenerator[channel].gen != NULL)
			{
				return musicGenerator[channel].gen->Vol;
			}
			else
			{
				return 0.0;
			}
		}
	}
	
	__int8 getPan(__int8 channel)
	{
		if (channel == 9)
		{			
			return drumPan;
		}
		else
		{
			if (musicGenerator[channel].gen != NULL)
			{
				return (musicGenerator[channel].gen->Pan+1.0)*64.0;
			}
			else
			{
				return 64.0;
			}
		}
		
	}
	
	void setVol(__int8 channel, __int8 vol)
	{
		//std::cout << "setting volume of channel " << ((int)channel)+1 << " to: " << (int)vol << std::endl;
		
		if (channel == 9 && treatDrums)
		{			
			drumVol = vol;
		}
		else
		{
			if (musicGenerator[channel].gen != NULL)
			{
				musicGenerator[channel].gen->Vol = vol;
			}
		}
	}

	void setPan(__int8 channel, __int8 pan)
	{
		//std::cout << "setting pan of channel " << ((int)channel)+1 << " to: " << (int)pan << std::endl;

		if (channel == 9 && treatDrums)
		{			
			drumPan = pan;
		}
		else
		{
			if ((musicGenerator[channel].gen) != NULL)
			{
				//std::cout << "setting pan of channel " << ((int)channel)+1 << " to: " << (pan/64.0)-1.0 << std::endl;
				musicGenerator[channel].gen->Pan = (pan/64.0)-1.0;
			}
		}
	}

	void playNote(__int8 channel, __int8 note, __int8 attackVelcoity, double startPos, double playLength)
	{
		
		if ((startPos > totalStart) && (startPos < totalEnd))
		{
			treatDrums = true;
			std::cout << treatDrums << std::endl;
			std::cout << "         " << (int)channel << std::endl;
			if ((channel == 9) && treatDrums)
			{
				std::cout << "trying to play drum: " << ((int)channel+1) << std::endl;
				std::cout << "note: " << (int)note << std::endl;


				

				if ((drumGenerator[note].gen) != NULL)
				{
					std::cout << "playing to channel: " << ((int)channel+1) << std::endl;
					std::cout << "       note: " << (int)note << std::endl;
					std::cout << "       attackVelcoity: " << (int)attackVelcoity << std::endl;
					std::cout << "       startPos: " << startPos << std::endl;
					std::cout << "       playLength: " << playLength << std::endl;
										
					double tmpVol = drumGenerator[note].gen->Vol;
					double volUsed = drumGenerator[note].specialVol*drumVol*(((double)attackVelcoity)/128.0);
					drumGenerator[note].gen->Vol = volUsed;
					drumGenerator[note].gen->PlayHole(startPos, playLength, drumGenerator[note].note);
					drumGenerator[note].gen->Vol = tmpVol;
				}
			}
			
			else if ((musicGenerator[channel].gen) != NULL)
			{
				std::cout << "playing to channel: " << ((int)channel+1) << std::endl;
				std::cout << "       note: " << (int)note << std::endl;
				std::cout << "       attackVelcoity: " << (int)attackVelcoity << std::endl;
				std::cout << "       startPos: " << startPos << std::endl;
				std::cout << "       playLength: " << playLength << std::endl;
				

				double tmpVol = musicGenerator[channel].gen->Vol;
				double volUsed = musicGenerator[channel].specialVol*tmpVol*(((double)attackVelcoity)/128.0);
				musicGenerator[channel].gen->Vol = volUsed;
				musicGenerator[channel].gen->PlayHole(startPos, playLength, note - OFFSET_NOTE);
				musicGenerator[channel].gen->Vol = tmpVol;
			}
			else
			{
				std::cout << "could not play to channel: " << ((int)channel+1) << std::endl;
			}
			
		}
	}
	



};

#endif