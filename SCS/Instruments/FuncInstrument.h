#ifndef _FUNCINSTRUMENT_
#define _FUNCINSTRUMENT_

#include "Standards.h"
#include "Curves.h"

template <class FuncT>
class FuncInstrument : public Instrument
{

protected:
	

public:
	

	double Amp;
	EnvelopeT *Env;
	double  Pos;

	FuncT AmpFunc;
	//double (*AmpFunc)(double);

	double Vol;

	FuncInstrument() : Instrument()
	{
		Env = new EnvelopeT();
		Pos = 0;
		Vol = 1.0;

		

	}

	void NoteOn()
	{
		Amp = 0;
	
		Pos = 0;
		Env->NoteOn();
		
		Playing = true;
	}

	bool Run()
	{
		

		Playing = Env->Run();
		
		Pos = Pos + TimeStep*Fre;

		//if (AmpFunc != NULL) Amp = AmpFunc(Pos) * Env->Amp * Vol;
		//else Amp = 0;

		Amp = AmpFunc(Pos) * Env->Amp * Vol;

		return Playing;
	}

	void NoteOff()
	{
		Env->NoteOff();
	}

	void End()
	{
	}
};

#endif