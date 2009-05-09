#ifndef _ROTATE_
#define _ROTATE_

// NOT done!

#include "Standards.h"
#include "Curves.h"
#include "Image.h"

class Rotate : public Instrument
{
public:
	
	// Functional
	double TimePos;
	double Pos;

	// Modules
	Image *Data;

	// Output
	double Amp;

	// Modifications

	Additive(char *filename) : Instrument()
	{
		Data = new Image(filename);
		

		Pos = 0;
		
		Amp = 0;

	}

	void NoteOn()
	{
		TimePos = 0;
		Pos = 0;
		Playing = true;
	}

	bool Run()
	{
		TimePos = TimePos+TimeStep;

		Pos = Pos+TimeStep*Fre;	

		Amp = 0;


		
		
		

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