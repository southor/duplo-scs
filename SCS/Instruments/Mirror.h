#ifndef _MIRROR_
#define _MIRROR_

#include "Standards.h"
#include "Curves.h"

class Mirror : public Instrument
{

protected:
	

public:
	

	double Amp;
	double  Pos;

	double (*AmpFunc)(double);

	Mirror() : Instrument()
	{
		this->AmpFunc = NULL;
		Pos = 0;
	}

	void NoteOn()
	{
		Amp = 0;
	
		Pos = 0;
		
		Playing = true;
	}

	bool Run()
	{
		
		
		Pos = Pos + TimeStep*Fre;

		if (AmpFunc != NULL) Amp = AmpFunc(Pos);
		else Amp = 0;

		
		//cout << Amp << endl;

		return Playing;
	}

	void NoteOff()
	{
		Playing = false;
	}

	void End()
	{
	}
};

#endif