#include "SoundRecorder.h"
#include "Standards.h"

#ifndef _LINERISEMOD_
#define _LINERISEMOD_

// ----------------------------------------------------------------------------------------------

class LineRiseMod : public SPTInstrument
{
protected:

	double Original;
public:

	double StartMod;
	double EndMod;
	double StartPos;
	double EndPos;

	double* Target;

	LineRiseMod(double* Target) : SPTInstrument()
	{
		this->Target = Target;
		
		StartMod = 1;
		EndMod = 1;
	
		StartPos = 0;
		EndPos = 1;
	}

	void NoteOn()
	{
		SPTInstrument::NoteOn();
		Original = *Target;
		*Target = Original*StartMod;
	}

	bool Run()
	{
		SPTInstrument::Run();
		if(PlayPos < EndPos)
		{
			*Target =  Original*(StartMod + (EndMod-StartMod)*(PlayPos-StartPos)/(EndPos-StartPos));
		}
		else
		{
			*Target = Original*EndMod;
			Playing = false;
		}
		
		return Playing;
	}
	
	

	void End()
	{
		SPTInstrument::End();
		*Target = Original;
	}

};

#endif