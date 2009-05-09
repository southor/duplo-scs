#ifndef _ENV_FUNC_1_
#define _ENV_FUNC_1_

#include "Standards.h"
#include "Curves.h"

class EnvFunc1 : public Instrument
{
public:
	

	double Amp;
	EnvelopeT *Env;
	double  Pos;
	
	//double (*AmpFunc)(double);

	double Vol;

	// top phase modulates
	double TSaw;
	double TSqre;
	double TTri;

	// bottom phase modulates
	double BSaw;
	double BSqre;
	double BTri;

	EnvFunc1() : Instrument()
	{
		Env = new EnvelopeT();
		Pos = 0;
		Vol = 1.0;

		double TSaw = 0.95;
		double TSqre = 0.95;
		double TTri = 0.95;

		double BSaw = 0.0;
		double BSqre = 0.0;
		double BTri = 0.0;
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

		double EnvAmp = Env->Amp;

		//delta values
		double dSaw = TSaw - BSaw;
		double dSqr = TSqre - BSqre;
		double dTri = TTri - BTri;

		// effective values
		double ESaw = EnvAmp * dSaw + BSaw;
		double ESqr = EnvAmp * dSqr + BSqre;
		double ETri = EnvAmp * dTri + BTri;
		

		Amp = TriSawSineSqr(Pos, ESaw, ESqr, ETri) * EnvAmp * Vol;


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