#ifndef _MUL_ENV_FUNC_1_
#define _MUL_ENV_FUNC_1_

#include "EnvFunc1.h"


class MulEnvFunc1 : public EnvFunc1
{
public:

	MulEnvFunc1() : EnvFunc1()
	{
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
		
		Amp = 0;
		Amp += TriSawSineSqr(Pos, ESaw - 0.3, ESqr, ETri) * EnvAmp * Vol;
		Amp += TriSawSineSqr(Pos + 0.3477, ESaw, ESqr - 0.3, ETri) * EnvAmp * Vol;
		Amp += TriSawSineSqr(Pos + 0.55, ESaw, ESqr, ETri - 0.3) * EnvAmp * Vol;


		return Playing;
	}

};

#endif