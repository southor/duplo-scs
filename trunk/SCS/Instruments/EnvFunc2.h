#ifndef _ENV_FUNC_2_
#define _ENV_FUNC_2_

#include "Standards.h"
#include "Curves.h"

class EnvFunc2 : public Instrument
{
public:
	

	double Amp;
	EnvelopeT *Env1;
	EnvelopeT *Env2;
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

	EnvFunc2() : Instrument()
	{
		Env1 = new EnvelopeT();
		Env2 = new EnvelopeT();
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
		Env1->NoteOn();
		Env2->NoteOn();
		
		Playing = true;
	}

	bool Run()
	{
		

		Playing = Env1->Run();
		Env2->Run();
		
		Pos = Pos + TimeStep*Fre;

		//if (AmpFunc != NULL) Amp = AmpFunc(Pos) * Env->Amp * Vol;
		//else Amp = 0;

		double EnvAmp1 = Env1->Amp;
		double EnvAmp2 = Env2->Amp;

		//delta values
		double dSaw = TSaw - BSaw;
		double dSqr = TSqre - BSqre;
		double dTri = TTri - BTri;

		// effective values
		double ESaw = EnvAmp2 * dSaw + BSaw;
		double ESqr = EnvAmp2 * dSqr + BSqre;
		double ETri = EnvAmp2 * dTri + BTri;
		

		Amp = TriSawSineSqr(Pos, ESaw, ESqr, ETri) * EnvAmp1 * Vol;


		return Playing;
	}

	void NoteOff()
	{
		Env1->NoteOff();
		Env2->NoteOff();
	}

	void End()
	{
	}
};

#endif