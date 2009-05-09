#ifndef _FEED_BACK_1_
#define _FEED_BACK_1_

#include "EnvFunc2.h"
#include "../Modules/DelayModule.h"

class FBack1 : public EnvFunc2
{
public:

	DelayModule Delay; // Maxdelay set to 1.0
	double DelayAmount;

	double FeedbackAmount;

	FBack1() : EnvFunc2(), Delay(1.0)
	{
		FeedbackAmount = 0.125;
		Delay.Delay = 0.025;
		DelayAmount = 0.025;
	}

	void NoteOn()
	{
		EnvFunc2::NoteOn();
		Delay.NoteOn();
	}

	bool Run()
	{
		Delay.Delay = DelayAmount * 40.0 / Fre;

		if (Env1->Playing) {
			if ( ! Env1->Run()) Delay.NoteOff();
		}

		

		Env2->Run();
		
		Delay.In = Amp;
		Playing = Delay.Run();

		

		
		Pos = Pos + TimeStep*Fre;
		
		double EPos = Pos + FeedbackAmount * Delay.Out /* + (1.0 + Sine(Pos / 70.0)) / 64.0 */;

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
		

		Amp = TriSawSineSqr(EPos, ESaw, ESqr, ETri) * EnvAmp1 * Vol;


		return Playing;
	}

	void NoteOff()
	{
		EnvFunc2::NoteOff();
		//Delay.NoteOff();
	}

	void End()
	{
		EnvFunc2::End();
		Delay.End();
	}
};

#endif