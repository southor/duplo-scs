#include "allinstruments.h"
#include "allmodules.h"

static const double NORMAL_SCSP_VOL = 0.4;

static const bool SHOW_ADDITIVE = false;

Additive* addAddetive(SCSPlayer *SCSP, SoundCollector *soundCollector, char *fileName)
{
	SCSMidiController controller;

	Additive *SCSi1 = new Additive(fileName);
	soundCollector->setInstrument(SCSi1);
	soundCollector->addAP(&(SCSi1->Amp1), &(SCSi1->Pan1), soundCollector->getInstrumentUse());
	soundCollector->addAP(&(SCSi1->Amp2), &(SCSi1->Pan2), soundCollector->getInstrumentUse());
	soundCollector->addAP(&(SCSi1->Amp3), &(SCSi1->Pan3), soundCollector->getInstrumentUse());

	SCSP->newControllers(3);

	controller.central = 0.5;
			controller.span = 0.49; // should not reach zero
			controller.defVal = 1.0;
			controller.midiController = 46; // brightness1
			controller.SCSController = &(SCSi1->HighAmplify);
			SCSP->setController(controller, 0);

	controller.central = 0.0;
			controller.span = 3.0; // warning! cannot change this without modifing class Additive
			controller.defVal = 0.0;
			controller.midiController = 40; // mod1
			controller.SCSController = &(SCSi1->HarmonicScaling);
			SCSP->setController(controller, 1);

	controller.central = 0.5;
			controller.span = 0.5;
			controller.defVal = -1.0;
			controller.midiController = 41; // mod2
			controller.SCSController = &(SCSi1->Apply1Bool);
			SCSP->setController(controller, 2);

	
	return SCSi1;
}


void setupEnvController(SCSPlayer *SCSP,						
						EnvelopeT *env,
						int startNumber,
						SCSMidiController attackController,
						SCSMidiController holdController,
						SCSMidiController decayController,
						SCSMidiController sustainController,
						SCSMidiController releaseController)
{
	attackController.SCSController = &(env->Attack);
	holdController.SCSController = &(env->Hold);
	decayController.SCSController = &(env->Decay);
	sustainController.SCSController = &(env->Sustain);
	releaseController.SCSController = &(env->Release);

	//attackController.defVal = 0.01;
	//holdController.defVal = -1.0;
	//decayController.defVal = -0.8;
	//sustainController.defVal = 0.0;
	//releaseController.defVal = -0.98;
	
	SCSP->setController(attackController, startNumber+0);
	SCSP->setController(holdController, startNumber+1);
	SCSP->setController(decayController, startNumber+2);
	SCSP->setController(sustainController, startNumber+3);
	SCSP->setController(releaseController, startNumber+4);
}

class Func0
{
public:
	double (*FP)(double);
	Func0() : FP(NULL)			{}
	double operator () (double Pos)		{ return FP(Pos); }
};

class Func1
{
public:
	double (*FP)(double, double);
	double Mod1;
	Func1() : FP(NULL), Mod1(0.0)	{}
	double operator ()(double Pos)		{ return FP(Pos, Mod1); }
};

class Func2
{
public:
	double (*FP)(double, double, double);
	double Mod1;
	double Mod2;
	Func2() : FP(NULL), Mod1(0.0), Mod2(0.0)	{}
	double operator ()(double Pos)		{ return FP(Pos, Mod1, Mod2); }
};

class Func3
{
public:
	double (*FP)(double, double, double, double);
	double Mod1;
	double Mod2;
	double Mod3;
	Func3() : FP(NULL), Mod1(0.0), Mod2(0.0), Mod3(0.0)		{}
	double operator ()(double Pos)		{ return FP(Pos, Mod1, Mod2, Mod3); }
};

class Func4
{
public:
	double (*FP)(double, double, double, double, double);
	double Mod1;
	double Mod2;
	double Mod3;
	double Mod4;
	Func4() : FP(NULL), Mod1(0.0), Mod2(0.0), Mod3(0.0), Mod4(0.0)		{}
	double operator ()(double Pos)		{ return FP(Pos, Mod1, Mod2, Mod3, Mod4); }
};



void SCSCreateInstrument(SCSPlayer *SCSP, SoundCollector *soundCollector, dup_uint8 patch)
{
	
	SCSMidiController controller;
	SCSMidiController attackController, holdController, decayController, sustainController, releaseController;
	//soundCollector->deleteAllInstruments();

	attackController.central = 0.25;
	attackController.span = 0.25;
	attackController.defVal = -0.96;
	attackController.midiController = 35;

	holdController.central = 0.125;
	holdController.span = 0.125;
	holdController.defVal = -1.0;
	holdController.midiController = 36;

	decayController.central = 0.5;
	decayController.span = 0.5;
	decayController.defVal = -0.9;
	decayController.midiController = 37;

	sustainController.central = 0.5;
	sustainController.span = 0.5;
	sustainController.defVal = 1.0;
	sustainController.midiController = 38;

	releaseController.central = 0.5;
	releaseController.span = 0.5;
	releaseController.defVal = -0.9;
	releaseController.midiController = 33;


	switch(patch)
	{
		case 1:
		{
			BasDrum1 *SCSi1 = new BasDrum1();
			SCSP->setVolMod(1.1*NORMAL_SCSP_VOL);
			soundCollector->setInstrument(SCSi1);
			soundCollector->addAP(&(SCSi1->Amp), NULL, soundCollector->getInstrumentUse());

		}
		break;
		case 2:
		{
			BasDrum2 *SCSi1 = new BasDrum2();
			SCSP->setVolMod(0.8*NORMAL_SCSP_VOL);
			soundCollector->setInstrument(SCSi1);
			soundCollector->addAP(&(SCSi1->Amp), NULL, soundCollector->getInstrumentUse());
		}
		break;
		case 3:
		{
			BatoBass *SCSi1 = new BatoBass();			
			SCSP->setVolMod(0.5*NORMAL_SCSP_VOL);
			soundCollector->setInstrument(SCSi1);
			soundCollector->addAP(&(SCSi1->Amp), NULL, soundCollector->getInstrumentUse());

			SCSP->newControllers(7);

			setupEnvController(SCSP,
								SCSi1->Envelope,
								0,
								attackController,
								holdController,
								decayController,
								sustainController,
								releaseController);
			
			
			controller.central = 0.5;
			controller.span = 0.5;
			controller.defVal = 0.0;
			controller.midiController = 40; // mod1
			controller.SCSController = &(SCSi1->MaxDist);
			SCSP->setController(controller, 5);

			controller.central = 5.0;
			controller.span = 5.0;
			controller.defVal = 0.0;
			controller.midiController = 41; // mod2
			controller.SCSController = &(SCSi1->DistSpeed);
			SCSP->setController(controller, 6);
		}
		break;
		case 4:
		{
			Blip1 *SCSi1 = new Blip1();
			
			
			
			
			SCSP->setVolMod(0.7*NORMAL_SCSP_VOL);
			soundCollector->setInstrument(SCSi1);
			soundCollector->addAP(&(SCSi1->Amp1), &(SCSi1->Pan1), soundCollector->getInstrumentUse());
			soundCollector->addAP(&(SCSi1->Amp2), &(SCSi1->Pan2), soundCollector->getInstrumentUse());


			SCSP->newControllers(8);
			
			attackController.SCSController = &(SCSi1->Env1->Attack);
			holdController.SCSController = &(SCSi1->Env1->Hold);
			decayController.SCSController = &(SCSi1->Env1->Decay);
			sustainController.SCSController = &(SCSi1->Env1->Sustain);
			releaseController.SCSController = &(SCSi1->Env1->Release);

			//attackController.defVal = 0.01;
			//holdController.defVal = -1.0;
			//decayController.defVal = -0.8;
			//sustainController.defVal = 0.0;
			//releaseController.defVal = -0.98;
			
			SCSP->setController(attackController, 0);
			SCSP->setController(holdController, 1);
			SCSP->setController(decayController, 2);
			SCSP->setController(sustainController, 3);
			SCSP->setController(releaseController, 4);
			
			controller.central = 0.8;
			controller.span = 0.8;
			controller.defVal = 0.0;
			controller.midiController = 50; // reverb
			controller.SCSController = &(SCSi1->EchoQuantity);
			SCSP->setController(controller, 5);

			controller.central = 0.5;
			controller.span = 0.5;
			controller.defVal = 0.0;
			controller.midiController = 40; // mod1
			controller.SCSController = &(SCSi1->MaxDist);
			SCSP->setController(controller, 6);

			controller.central = 3.0;
			controller.span = 3.0;
			controller.defVal = 0.0;
			controller.midiController = 41; // mod2
			controller.SCSController = &(SCSi1->DistSpeed);
			SCSP->setController(controller, 7);

		}
		break;
		case 5:
		{
			Brass1 *SCSi1 = new Brass1();
			soundCollector->setInstrument(SCSi1);
			soundCollector->addAP(&(SCSi1->Amp), NULL, soundCollector->getInstrumentUse());
			
			SCSP->newControllers(8);
			
			controller.central = 0.5;
			controller.span = 0.5;
			controller.defVal = -0.65;
			controller.midiController = 50; // reverb
			controller.SCSController = &(SCSi1->EchoAmount);
			SCSP->setController(controller, 0);

			controller.central = 1.5;
			controller.span = 1.5;
			controller.defVal = -0.8;
			controller.midiController = 40; // mod1
			controller.SCSController = &(SCSi1->Mod1);
			SCSP->setController(controller, 1);

			controller.central = 0.5;
			controller.span = 0.5;
			controller.defVal = -0.8;
			controller.midiController = 41; // mod2
			controller.SCSController = &(SCSi1->ThinMod);
			SCSP->setController(controller, 2);

			setupEnvController(SCSP,
								SCSi1->Env1,
								3,
								attackController,
								holdController,
								decayController,
								sustainController,
								releaseController);
		}
		break;
		case 6:
		{
			Cymbal1 *SCSi1 = new Cymbal1();
			SCSi1->ignoreNoteOff = true;
			
			soundCollector->setInstrument(SCSi1);
			soundCollector->addAP(&(SCSi1->Amp), NULL, soundCollector->getInstrumentUse());

			

			SCSP->newControllers(1);
			
			controller.central = 1.05;
			controller.span = 1.0;
			controller.defVal = -0.2;
			controller.midiController = 49; // decay
			controller.SCSController = &(SCSi1->Decay);
			SCSP->setController(controller, 0);

			

		}
		break;
		case 7:
		{
			CPRaw *SCSi1 = new CPRaw();
			SCSP->setVolMod(0.7*NORMAL_SCSP_VOL);
			soundCollector->setInstrument(SCSi1);
			soundCollector->addAP(&(SCSi1->Amp), NULL, soundCollector->getInstrumentUse());
			
			SCSP->newControllers(7);

			controller.central = 0.6;
			controller.span = 2.0;
			controller.defVal = 0.0;
			controller.midiController = 40; // mod1
			controller.SCSController = &(SCSi1->VoblAmount);
			SCSP->setController(controller, 0);

			controller.central = 0.05;
			controller.span = 0.05;
			controller.defVal = 0.0;
			controller.midiController = 41; // mod2
			controller.SCSController = &(SCSi1->VoblFre);
			SCSP->setController(controller, 1);

			

			setupEnvController(SCSP,
								SCSi1->Env1,
								2,
								attackController,
								holdController,
								decayController,
								sustainController,
								releaseController);
		}
		break;
		case 8:
		{
			Drum1 *SCSi1 = new Drum1();
			SCSP->setVolMod(1.5*NORMAL_SCSP_VOL);
			soundCollector->setInstrument(SCSi1);
			soundCollector->addAP(&(SCSi1->Amp), NULL, soundCollector->getInstrumentUse());
		}
		break;
		case 9:
		{
			Drum2 *SCSi1 = new Drum2();
			SCSP->setVolMod(1.5*NORMAL_SCSP_VOL);
			soundCollector->setInstrument(SCSi1);
			soundCollector->addAP(&(SCSi1->Amp), NULL, soundCollector->getInstrumentUse());
		}
		break;
		case 10:
		{
			Fir *SCSi1 = new Fir();
			SCSP->setVolMod(1.5*NORMAL_SCSP_VOL);
			SCSi1->Env1->Attack = 0.05;
			soundCollector->setInstrument(SCSi1);
			soundCollector->addAP(&(SCSi1->Amp), NULL, soundCollector->getInstrumentUse());

			SCSP->newControllers(5);

			setupEnvController(SCSP,
								SCSi1->Env1,
								0,
								attackController,
								holdController,
								decayController,
								sustainController,
								releaseController);
		}
		break;
		case 11:
		{
			Flute *SCSi1 = new Flute();
			soundCollector->setInstrument(SCSi1);
			soundCollector->addAP(&(SCSi1->Amp1), &(SCSi1->Pan1), soundCollector->getInstrumentUse());
			soundCollector->addAP(&(SCSi1->Amp2), &(SCSi1->Pan2), soundCollector->getInstrumentUse());

			SCSP->newControllers(9);
			
			attackController.SCSController = &(SCSi1->Env->Attack);
			holdController.SCSController = &(SCSi1->Env->Hold);
			decayController.SCSController = &(SCSi1->Env->Decay);
			sustainController.SCSController = &(SCSi1->Env->Sustain);
			releaseController.SCSController = &(SCSi1->Env->Release);

			//attackController.defVal = 0.01;
			//holdController.defVal = -1.0;
			//decayController.defVal = -0.8;
			//sustainController.defVal = 0.0;
			//releaseController.defVal = -0.98;
			
			SCSP->setController(attackController, 0);
			SCSP->setController(holdController, 1);
			SCSP->setController(decayController, 2);
			SCSP->setController(sustainController, 3);
			SCSP->setController(releaseController, 4);


			controller.central = 1.0;
			controller.span = 1.0;
			controller.defVal = -1.0;
			controller.midiController = 40; // Mod1
			controller.SCSController = &(SCSi1->Mod1);
			SCSP->setController(controller, 5);

			controller.central = 0.5;
			controller.span = 0.5;
			controller.defVal = 0.0;
			controller.midiController = 41; // Mod2 (changing will only give affect after another note on)
			controller.SCSController = &(SCSi1->Mod2);
			SCSP->setController(controller, 6);

			controller.central = 0.5;
			controller.span = 0.5;
			controller.defVal = 0.0;
			controller.midiController = 42; // Mod3
			controller.SCSController = &(SCSi1->LeftEcho);
			SCSP->setController(controller, 7);

			controller.central = 0.5;
			controller.span = 0.5;
			controller.defVal = 0.0;
			controller.midiController = 43; // Mod4
			controller.SCSController = &(SCSi1->RightEcho);
			SCSP->setController(controller, 8);

		}
		break;
		case 12:
		{
			Gong1 *SCSi1 = new Gong1();
			soundCollector->setInstrument(SCSi1);
			soundCollector->addAP(&(SCSi1->Amp), NULL, soundCollector->getInstrumentUse());	

		}
		break;
		case 13:
		{
			Harmic *SCSi1 = new Harmic();
			soundCollector->setInstrument(SCSi1);
			soundCollector->addAP(&(SCSi1->Amp), NULL, soundCollector->getInstrumentUse());

			SCSP->newControllers(6);
			
			attackController.SCSController = &(SCSi1->Env1->Attack);
			holdController.SCSController = &(SCSi1->Env1->Hold);
			decayController.SCSController = &(SCSi1->Env1->Decay);
			sustainController.SCSController = &(SCSi1->Env1->Sustain);
			releaseController.SCSController = &(SCSi1->Env1->Release);

			//attackController.defVal = 0.01;
			//holdController.defVal = -1.0;
			//decayController.defVal = -0.8;
			//sustainController.defVal = 0.0;
			//releaseController.defVal = -0.98;
			
			SCSP->setController(attackController, 0);
			SCSP->setController(holdController, 1);
			SCSP->setController(decayController, 2);
			SCSP->setController(sustainController, 3);
			SCSP->setController(releaseController, 4);

			// add the real decay controller for this instrument (inverted)
			controller.central = 32.0;
			controller.span = 32.0;
			controller.defVal = -0.7;
			controller.midiController = 34; // real decay (inverted)
			controller.SCSController = &(SCSi1->Decay);
			SCSP->setController(controller, 5);


		}
		break;
		case 14:
		{
			Harp *SCSi1 = new Harp();
			SCSP->setVolMod(1.1*NORMAL_SCSP_VOL);
			SCSi1->useRingBuff = false;
			soundCollector->setInstrument(SCSi1);
			soundCollector->addAP(&(SCSi1->Amp1), NULL, soundCollector->getInstrumentUse());
			soundCollector->addAP(&(SCSi1->Amp2), NULL, soundCollector->getInstrumentUse());

			SCSP->newControllers(6);

			controller.central = 0.5;
			controller.span = 0.5;
			controller.defVal = 0.0;
			controller.midiController = 50; // extra reverb
			controller.SCSController = &(SCSi1->EchoAmount);
			SCSP->setController(controller, 0);

			setupEnvController(SCSP,
								SCSi1->Env1,
								1,
								attackController,
								holdController,
								decayController,
								sustainController,
								releaseController);
		}
		break;
		case 15:
		{
			Helicopter *SCSi1 = new Helicopter(1.0);
			SCSP->setVolMod(2.0*NORMAL_SCSP_VOL);
			soundCollector->setInstrument(SCSi1);
			soundCollector->addAP(&(SCSi1->Amp), NULL, soundCollector->getInstrumentUse());

		}
		break;
		case 16:
		{
			HiHat *SCSi1 = new HiHat();
			soundCollector->setInstrument(SCSi1);
			soundCollector->addAP(&(SCSi1->Amp), NULL, soundCollector->getInstrumentUse());
		}
		break;
		case 17:
		{
			KrankyNoise *SCSi1 = new KrankyNoise();
			SCSP->setVolMod(0.5*NORMAL_SCSP_VOL);
			soundCollector->setInstrument(SCSi1);
			soundCollector->addAP(&(SCSi1->Amp), NULL, soundCollector->getInstrumentUse());

			
		}
		break;
		case 18:
		{
			FuncInstrument<Func0> *SCSi1 = new FuncInstrument<Func0>();			
			SCSi1->AmpFunc.FP = Tri;
			soundCollector->setInstrument(SCSi1);
			soundCollector->addAP(&(SCSi1->Amp), NULL, soundCollector->getInstrumentUse());

			SCSP->newControllers(5);

			setupEnvController(SCSP,
								SCSi1->Env,
								0,
								attackController,
								holdController,
								decayController,
								sustainController,
								releaseController);
		}
		break;
		case 19:
		{
			FuncInstrument<Func0> *SCSi1 = new FuncInstrument<Func0>();
			SCSi1->AmpFunc.FP = Sine;
			soundCollector->setInstrument(SCSi1);
			soundCollector->addAP(&(SCSi1->Amp), NULL, soundCollector->getInstrumentUse());

			SCSP->newControllers(5);

			setupEnvController(SCSP,
								SCSi1->Env,
								0,
								attackController,
								holdController,
								decayController,
								sustainController,
								releaseController);
		}
		break;
		case 20:
		{
			FuncInstrument<Func0> *SCSi1 = new FuncInstrument<Func0>();
			SCSi1->AmpFunc.FP = SoftSqr;
			soundCollector->setInstrument(SCSi1);
			soundCollector->addAP(&(SCSi1->Amp), NULL, soundCollector->getInstrumentUse());

			SCSP->newControllers(5);

			setupEnvController(SCSP,
								SCSi1->Env,
								0,
								attackController,
								holdController,
								decayController,
								sustainController,
								releaseController);

		}
		break;
		case 21:
		{
			FuncInstrument<Func0> *SCSi1 = new FuncInstrument<Func0>();
			SCSi1->AmpFunc.FP = Sqr;			
			soundCollector->setInstrument(SCSi1);
			soundCollector->addAP(&(SCSi1->Amp), NULL, soundCollector->getInstrumentUse());

			SCSP->newControllers(5);

			setupEnvController(SCSP,
								SCSi1->Env,
								0,
								attackController,
								holdController,
								decayController,
								sustainController,
								releaseController);
		}
		break;
		case 22:
		{
			FuncInstrument<Func0> *SCSi1 = new FuncInstrument<Func0>();
			SCSi1->AmpFunc.FP = Saw;			
			soundCollector->setInstrument(SCSi1);
			soundCollector->addAP(&(SCSi1->Amp), NULL, soundCollector->getInstrumentUse());

			SCSP->newControllers(5);

			setupEnvController(SCSP,
								SCSi1->Env,
								0,
								attackController,
								holdController,
								decayController,
								sustainController,
								releaseController);
		}
		break;
		case 23:
		{
			MixLead1 *SCSi1 = new MixLead1();		
			soundCollector->setInstrument(SCSi1);
			soundCollector->addAP(&(SCSi1->Amp1), &(SCSi1->Pan1), soundCollector->getInstrumentUse());
			soundCollector->addAP(&(SCSi1->Amp2), &(SCSi1->Pan2), soundCollector->getInstrumentUse());
			soundCollector->addAP(&(SCSi1->Amp3), &(SCSi1->Pan3), soundCollector->getInstrumentUse());
			soundCollector->addAP(&(SCSi1->Amp4), &(SCSi1->Pan4), soundCollector->getInstrumentUse());

			SCSP->newControllers(10);

			controller.central = 0.5;
			controller.span = 0.5;
			controller.defVal = 1.0;
			controller.midiController = 46; // brightness
			controller.SCSController = &(SCSi1->Brightness);
			SCSP->setController(controller, 0);

			controller.central = 1.0;
			controller.span = 1.0;
			controller.defVal = 0.0;
			controller.midiController = 40; // mod1
			controller.SCSController = &(SCSi1->Whine);
			SCSP->setController(controller, 1);

			controller.central = 1.6;
			controller.span = 1.6;
			controller.defVal = 0.0;
			controller.midiController = 41; // mod2
			controller.SCSController = &(SCSi1->WhineSpeed);
			SCSP->setController(controller, 2);

			controller.central = 0.2;
			controller.span = 0.2;
			controller.defVal = 0.5;
			controller.midiController = 50; // reverb
			controller.SCSController = &(SCSi1->EchoQuantity);
			SCSP->setController(controller, 3);

			controller.central = 0.5;
			controller.span = 0.5;
			controller.defVal = 1.0;
			controller.midiController = 22; // Switch1
			controller.SCSController = &(SCSi1->WhineBehavour);
			SCSP->setController(controller, 4);

			setupEnvController(SCSP,
								SCSi1->Env1,
								5,
								attackController,
								holdController,
								decayController,
								sustainController,
								releaseController);




		}
		break;
		case 24:
		{
			MixLead2 *SCSi1 = new MixLead2();		
			soundCollector->setInstrument(SCSi1);
			soundCollector->addAP(&(SCSi1->Amp1), &(SCSi1->Pan1), soundCollector->getInstrumentUse());
			soundCollector->addAP(&(SCSi1->Amp2), &(SCSi1->Pan2), soundCollector->getInstrumentUse());
			soundCollector->addAP(&(SCSi1->Amp3), &(SCSi1->Pan3), soundCollector->getInstrumentUse());
			soundCollector->addAP(&(SCSi1->Amp4), &(SCSi1->Pan4), soundCollector->getInstrumentUse());

			SCSP->newControllers(5);

			setupEnvController(SCSP,
								SCSi1->Env1,
								0,
								attackController,
								holdController,
								decayController,
								sustainController,
								releaseController);
		}
		break;
		case 25:
		{
			MString *SCSi1 = new MString();		
			SCSP->setVolMod(2.0*NORMAL_SCSP_VOL);
			soundCollector->setInstrument(SCSi1);
			soundCollector->addAP(&(SCSi1->Amp), NULL, soundCollector->getInstrumentUse());

			SCSP->newControllers(7);

			// add the decay controller for this instrument (inverted)
			controller.central = 32.0;
			controller.span = 32.0;
			controller.defVal = -0.7;
			controller.midiController = 34; // decay (inverted)
			controller.SCSController = &(SCSi1->Decay);
			SCSP->setController(controller, 0);
			
			controller.central = 0.5;
			controller.span = 0.5;
			controller.defVal = -0.65;
			controller.midiController = 50; // reverb
			controller.SCSController = &(SCSi1->EchoAmount);
			SCSP->setController(controller, 1);

			setupEnvController(SCSP,
								SCSi1->Env1,
								2,
								attackController,
								holdController,
								decayController,
								sustainController,
								releaseController);
		}
		break;
		case 26:
		{
			New1 *SCSi1 = new New1();
			SCSP->setVolMod(0.5*NORMAL_SCSP_VOL);
			soundCollector->setInstrument(SCSi1);
			soundCollector->addAP(&(SCSi1->Amp), NULL, soundCollector->getInstrumentUse());


			SCSP->newControllers(6);
			
			attackController.SCSController = &(SCSi1->Env1->Attack);
			holdController.SCSController = &(SCSi1->Env1->Hold);
			decayController.SCSController = &(SCSi1->Env1->Decay);
			sustainController.SCSController = &(SCSi1->Env1->Sustain);
			releaseController.SCSController = &(SCSi1->Env1->Release);

			//attackController.defVal = 0.01;
			//holdController.defVal = -1.0;
			//decayController.defVal = -0.8;
			//sustainController.defVal = 0.0;
			//releaseController.defVal = -0.98;
			
			SCSP->setController(attackController, 0);
			SCSP->setController(holdController, 1);
			SCSP->setController(decayController, 2);
			SCSP->setController(sustainController, 3);
			SCSP->setController(releaseController, 4);


			controller.central = 3.0;
			controller.span = 3.0;
			controller.defVal = 0.0;
			controller.midiController = 40; // mod1
			controller.SCSController = &(SCSi1->Mod1);
			SCSP->setController(controller, 05);



		}
		break;
		case 27:
		{
			Noise *SCSi1 = new Noise();		
			soundCollector->setInstrument(SCSi1);
			soundCollector->addAP(&(SCSi1->E), NULL, soundCollector->getInstrumentUse());

		}
		break;
		case 28:
		{
			NoiseFlanging *SCSi1 = new NoiseFlanging(1.0);		
			soundCollector->setInstrument(SCSi1);
			soundCollector->addAP(&(SCSi1->Flang->Output), NULL, soundCollector->getInstrumentUse());
		}
		break;
		case 29:
		{
			OrganBass *SCSi1 = new OrganBass();		
			soundCollector->setInstrument(SCSi1);
			soundCollector->addAP(&(SCSi1->Amp1), &(SCSi1->Pan1), soundCollector->getInstrumentUse());
			soundCollector->addAP(&(SCSi1->Amp2), &(SCSi1->Pan2), soundCollector->getInstrumentUse());

			SCSP->newControllers(1);

			controller.central = 0.0;
			controller.span = 1.0;
			controller.defVal = 0.1;
			controller.midiController = 40; // mod1
			controller.SCSController = &(SCSi1->Mod);
			SCSP->setController(controller, 0);
		}
		break;
		case 30:
		{
			Piano *SCSi1 = new Piano();		
			SCSP->setVolMod(2.5*NORMAL_SCSP_VOL);
			soundCollector->setInstrument(SCSi1);
			soundCollector->addAP(&(SCSi1->Amp), NULL, soundCollector->getInstrumentUse());

			SCSP->newControllers(6);

			controller.central = 150.0;
			controller.span = 150.0;
			controller.defVal = 0.1;
			controller.midiController = 40; // mod1
			controller.SCSController = &(SCSi1->Mod1);
			SCSP->setController(controller, 0);

			setupEnvController(SCSP,
								SCSi1->Env1,
								1,
								attackController,
								holdController,
								decayController,
								sustainController,
								releaseController);
		}
		break;
		case 31:
		{
			Piano2 *SCSi1 = new Piano2();		
			SCSP->setVolMod(2.7*NORMAL_SCSP_VOL);
			soundCollector->setInstrument(SCSi1);
			soundCollector->addAP(&(SCSi1->Amp), NULL, soundCollector->getInstrumentUse());

			SCSP->newControllers(13);

			controller.central = 150.0;
			controller.span = 150.0;
			controller.defVal = 0.1;
			controller.midiController = 40; // mod1
			controller.SCSController = &(SCSi1->Mod1Low);
			SCSP->setController(controller, 0);

			controller.central = 150.0;
			controller.span = 150.0;
			controller.defVal = 0.1;
			controller.midiController = 41; // mod2
			controller.SCSController = &(SCSi1->Mod1High);
			SCSP->setController(controller, 1);

			controller.central = 0.5;
			controller.span = 0.5;
			controller.defVal = 0.1;
			controller.midiController = 50; // extra reverb
			controller.SCSController = &(SCSi1->EchoAmount);
			SCSP->setController(controller, 2);

			setupEnvController(SCSP,
								SCSi1->Env1,
								3,
								attackController,
								holdController,
								decayController,
								sustainController,
								releaseController);

			// ---------- set second enveloper -----------

			attackController.midiController = 28;
			holdController.midiController = 29;
			decayController.midiController = 30;
			sustainController.midiController = 31;
			releaseController.midiController = 32;

			setupEnvController(SCSP,
								SCSi1->Env2,
								8,
								attackController,
								holdController,
								decayController,
								sustainController,
								releaseController);
		}
		break;
		case 32:
		{
			Pluto *SCSi1 = new Pluto();		
			soundCollector->setInstrument(SCSi1);
			soundCollector->addAP(&(SCSi1->Amp1), &(SCSi1->Pan1), soundCollector->getInstrumentUse());
			soundCollector->addAP(&(SCSi1->Amp2), &(SCSi1->Pan2), soundCollector->getInstrumentUse());

			SCSP->newControllers(10);


			attackController.SCSController = &(SCSi1->Envelope->Attack);
			holdController.SCSController = &(SCSi1->Envelope->Hold);
			decayController.SCSController = &(SCSi1->Envelope->Decay);
			sustainController.SCSController = &(SCSi1->Envelope->Sustain);
			releaseController.SCSController = &(SCSi1->Envelope->Release);

			//attackController.defVal = 0.01;
			//holdController.defVal = -1.0;
			//decayController.defVal = -0.8;
			//sustainController.defVal = 0.0;
			//releaseController.defVal = -0.98;
			
			SCSP->setController(attackController, 0);
			SCSP->setController(holdController, 1);
			SCSP->setController(decayController, 2);
			SCSP->setController(sustainController, 3);
			SCSP->setController(releaseController, 4);



			controller.central = 0.0;
			controller.span = 2.0;
			controller.defVal = -0.8;
			controller.midiController = 40; // mod1
			controller.SCSController = &(SCSi1->Mod1);
			SCSP->setController(controller, 5);

			controller.central = 0.0;
			controller.span = 2.0;
			controller.defVal = -0.8;
			controller.midiController = 41; // mod2
			controller.SCSController = &(SCSi1->ModMin);
			SCSP->setController(controller, 6);

			controller.central = 0.0;
			controller.span = 2.0;
			controller.defVal = -0.8;
			controller.midiController = 42; // mod3
			controller.SCSController = &(SCSi1->ModMax);
			SCSP->setController(controller, 7);

			controller.central = 10.0;
			controller.span = 10.0;
			controller.defVal = 0.3;
			controller.midiController = 43; // mod4
			controller.SCSController = &(SCSi1->ModSpeed);
			SCSP->setController(controller, 8);

			controller.central = 10.0;
			controller.span = 10.0;
			controller.defVal = 0.3;
			controller.midiController = 44; // mod5
			controller.SCSController = &(SCSi1->DropSpeed);
			SCSP->setController(controller, 9);
		}
		break;
		case 33:
		{
			SoftBass *SCSi1 = new SoftBass();		
			soundCollector->setInstrument(SCSi1);
			soundCollector->addAP(&(SCSi1->Amp), NULL, soundCollector->getInstrumentUse());

			SCSP->newControllers(6);

			setupEnvController(SCSP,
								SCSi1->Env1,
								0,
								attackController,
								holdController,
								decayController,
								sustainController,
								releaseController);

			controller.central = 0.5;
			controller.span = 0.5;
			controller.defVal = -0.8;
			controller.midiController = 50; // extra reverb
			controller.SCSController = &(SCSi1->EchoQuantity);
			SCSP->setController(controller, 5);

			 ;
		}
		break;
		case 34:
		{
			Saw1 *SCSi1 = new Saw1();			
			SCSP->setVolMod(0.7*NORMAL_SCSP_VOL);
			soundCollector->setInstrument(SCSi1);
			soundCollector->addAP(&(SCSi1->Amp1), &(SCSi1->Pan1), soundCollector->getInstrumentUse());
			soundCollector->addAP(&(SCSi1->Amp2), &(SCSi1->Pan2), soundCollector->getInstrumentUse());

			SCSP->newControllers(7);

			controller.central = 0.5;
			controller.span = 0.5;
			controller.defVal = 0.95;
			controller.midiController = 46; // brightness1
			controller.SCSController = &(SCSi1->Brightness1);
			SCSP->setController(controller, 0);

			controller.central = 0.5;
			controller.span = 0.5;
			controller.defVal = 0.93;
			controller.midiController = 45; // brightness2
			controller.SCSController = &(SCSi1->Brightness2);
			SCSP->setController(controller, 1);


			setupEnvController(SCSP,
								SCSi1->Env1,
								2,
								attackController,
								holdController,
								decayController,
								sustainController,
								releaseController);

			
		}
		break;
		case 35:
		{
			String1 *SCSi1 = new String1();		
			SCSP->setVolMod(1.6*NORMAL_SCSP_VOL);
			soundCollector->setInstrument(SCSi1);
			soundCollector->addAP(&(SCSi1->Amp), NULL, soundCollector->getInstrumentUse());

			SCSP->newControllers(6);

			setupEnvController(SCSP,
								SCSi1->Env1,
								0,
								attackController,
								holdController,
								decayController,
								sustainController,
								releaseController);

			controller.central = 0.5;
			controller.span = 0.5;
			controller.defVal = 0.8;
			controller.midiController = 50; // extra reverb
			//controller.SCSController = &(SCSi1->MellowBrightness);
			controller.SCSController = &(SCSi1->ExtraReverb);
			SCSP->setController(controller, 5);
		}
		break;
		case 36:
		{
			String2 *SCSi1 = new String2();		
			SCSP->setVolMod(0.9*NORMAL_SCSP_VOL);
			soundCollector->setInstrument(SCSi1);
			soundCollector->addAP(&(SCSi1->Amp), NULL, soundCollector->getInstrumentUse());

			SCSP->newControllers(11);

			controller.central = 0.5;
			controller.span = 0.5;
			controller.defVal = 0.8;
			controller.midiController = 40; // mod1
			//controller.SCSController = &(SCSi1->MellowBrightness);
			controller.SCSController = &(SCSi1->Osc1Brightness);
			SCSP->setController(controller, 0);

			controller.central = 1.0;
			controller.span = 1.0;
			controller.defVal = 0.0;
			controller.midiController = 41; // mod2
			//controller.SCSController = &(SCSi1->MellowBrightness);
			controller.SCSController = &(SCSi1->Osc2Brightness);
			SCSP->setController(controller, 1);

			controller.central = 0.5;
			controller.span = 0.5;
			controller.defVal = 0.8;
			controller.midiController = 42; // mod3
			controller.SCSController = &(SCSi1->MellowAmount);
			SCSP->setController(controller, 2);

			controller.central = 0.5;
			controller.span = 0.5;
			controller.defVal = 0.15;
			controller.midiController = 43; // mod4
			controller.SCSController = &(SCSi1->WhineAmount);
			SCSP->setController(controller, 3);

			controller.central = 1.0;
			controller.span = 1.0;
			controller.defVal = 0.15;
			controller.midiController = 44; // mod5
			controller.SCSController = &(SCSi1->WhineSpeed);
			SCSP->setController(controller, 4);

			controller.central = 0.5;
			controller.span = 0.5;
			controller.defVal = -0.7;
			controller.midiController = 50; // extra reverb
			controller.SCSController = &(SCSi1->EchoAmount);
			SCSP->setController(controller, 5);
			
			setupEnvController(SCSP,
								SCSi1->Env1,
								6,
								attackController,
								holdController,
								decayController,
								sustainController,
								releaseController);

		}
		break;
		case 37:
		{
			SynthBass1 *SCSi1 = new SynthBass1();		
			soundCollector->setInstrument(SCSi1);
			SCSi1->Mod1 = 1.0;
			soundCollector->addAP(&(SCSi1->Amp), NULL, soundCollector->getInstrumentUse());

			SCSP->newControllers(7);

			controller.central = 3.0;
			controller.span = 3.0;
			controller.defVal = 0.0;
			controller.midiController = 40; // mod1
			controller.SCSController = &(SCSi1->Mod1);
			SCSP->setController(controller, 0);

			controller.central = 0.5;
			controller.span = 0.5;
			controller.defVal = 0.0;
			controller.midiController = 41; // mod2
			controller.SCSController = &(SCSi1->ThinMod);
			SCSP->setController(controller, 1);

			setupEnvController(SCSP,
								SCSi1->Envelope,
								2,
								attackController,
								holdController,
								decayController,
								sustainController,
								releaseController);

		}
		break;
		case 38:
		{
			Tamb *SCSi1 = new Tamb();		
			soundCollector->setInstrument(SCSi1);
			soundCollector->addAP(&(SCSi1->Amp), NULL, soundCollector->getInstrumentUse());


		}
		break;
		case 39:
		{
			Violin *SCSi1 = new Violin();		
			soundCollector->setInstrument(SCSi1);
			soundCollector->addAP(&(SCSi1->Amp), NULL, soundCollector->getInstrumentUse());

			SCSP->newControllers(5);
				
			setupEnvController(SCSP,
								SCSi1->Env1,
								0,
								attackController,
								holdController,
								decayController,
								sustainController,
								releaseController);

		}
		break;
		case 40:
		{
			Voice1 *SCSi1 = new Voice1();		
			soundCollector->setInstrument(SCSi1);
			soundCollector->addAP(&(SCSi1->Amp), NULL, soundCollector->getInstrumentUse());

			SCSP->newControllers(6);

			controller.central = 0.5;
			controller.span = 0.5;
			controller.defVal = 0.0;
			controller.midiController = 46; // brightness
			controller.SCSController = &(SCSi1->Brightness);
			SCSP->setController(controller, 0);
				
			setupEnvController(SCSP,
								SCSi1->Env1,
								1,
								attackController,
								holdController,
								decayController,
								sustainController,
								releaseController);
		}
		break;
		case 41:
		{
			WindKnock *SCSi1 = new WindKnock();		
			soundCollector->setInstrument(SCSi1);
			soundCollector->addAP(&(SCSi1->Amp), NULL, soundCollector->getInstrumentUse());
		}

		break;
		case 42:
		{
			FMString1 *SCSi1 = new FMString1();
			soundCollector->setInstrument(SCSi1);
			soundCollector->addAP(&(SCSi1->Amp), NULL, soundCollector->getInstrumentUse());

			SCSP->newControllers(5);
			
			attackController.SCSController = &(SCSi1->Env1->Attack);
			holdController.SCSController = &(SCSi1->Env1->Hold);
			decayController.SCSController = &(SCSi1->Env1->Decay);
			sustainController.SCSController = &(SCSi1->Env1->Sustain);
			releaseController.SCSController = &(SCSi1->Env1->Release);

			//attackController.defVal = 0.01;
			//holdController.defVal = -1.0;
			//decayController.defVal = -0.8;
			//sustainController.defVal = 0.0;
			//releaseController.defVal = -0.98;
			
			SCSP->setController(attackController, 0);
			SCSP->setController(holdController, 1);
			SCSP->setController(decayController, 2);
			SCSP->setController(sustainController, 3);
			SCSP->setController(releaseController, 4);


		}

		break;
		case 43:
		{
			Additive *instrument = addAddetive(SCSP, soundCollector, "addetive1.bmp");
			instrument->Vol *= 1.5;

			if (SHOW_ADDITIVE) MessageBox(NULL,"instrument 1 choosen","ok",MB_OK);
		}
		break;
		case 44:
		{
			addAddetive(SCSP, soundCollector, "addetive2.bmp");

			if (SHOW_ADDITIVE) MessageBox(NULL,"instrument 2 choosen","ok",MB_OK);
		}
		break;
		case 45:
		{
			addAddetive(SCSP, soundCollector, "addetive3.bmp");

			if (SHOW_ADDITIVE) MessageBox(NULL,"instrument 3 choosen","ok",MB_OK);
		}
		break;
		case 46:
		{
			addAddetive(SCSP, soundCollector, "addetive4.bmp");

			if (SHOW_ADDITIVE) MessageBox(NULL,"instrument 4 choosen","ok",MB_OK);
		}
		break;
		case 47:
		{
			addAddetive(SCSP, soundCollector, "addetive5.bmp");

			if (SHOW_ADDITIVE) MessageBox(NULL,"instrument 5 choosen","ok",MB_OK);
		}
		break;
		case 48:
		{
			Additive *instrument = addAddetive(SCSP, soundCollector, "addetive6.bmp");
			//instrument->Vol *= 1.5;

			if (SHOW_ADDITIVE) MessageBox(NULL,"instrument 6 choosen","ok",MB_OK);
		}
		break;
		case 49:
		{
			addAddetive(SCSP, soundCollector, "addetive7.bmp");

			if (SHOW_ADDITIVE) MessageBox(NULL,"instrument 7 choosen","ok",MB_OK);
		}
		break;
		case 50:
		{
			addAddetive(SCSP, soundCollector, "addetive8.bmp");

			if (SHOW_ADDITIVE) MessageBox(NULL,"instrument 8 choosen","ok",MB_OK);
		}
		break;
		case 51:
		{
			addAddetive(SCSP, soundCollector, "addetive9.bmp");

			if (SHOW_ADDITIVE) MessageBox(NULL,"instrument 9 choosen","ok",MB_OK);
		}
		break;
		case 52:
		{
			Additive *instrument = addAddetive(SCSP, soundCollector, "addetive10.bmp");
			instrument->Resolution = 1000;
			
			if (SHOW_ADDITIVE) MessageBox(NULL,"instrument 10 choosen","ok",MB_OK);
		}
		case 53:
		{
			Additive *instrument = addAddetive(SCSP, soundCollector, "addetive11.bmp");
			instrument->Resolution = 1000;
			
			if (SHOW_ADDITIVE) MessageBox(NULL,"instrument 11 choosen","ok",MB_OK);
		}
		break;
		case 73:
		{
			// Megafunc
			FuncInstrument<Func3> *SCSi1 = new FuncInstrument<Func3>();			
			SCSi1->AmpFunc.FP = TriSawSineSqr;
			soundCollector->setInstrument(SCSi1);
			soundCollector->addAP(&(SCSi1->Amp), NULL, soundCollector->getInstrumentUse());

			SCSP->newControllers(8);

			setupEnvController(SCSP,
								SCSi1->Env,
								0,
								attackController,
								holdController,
								decayController,
								sustainController,
								releaseController);

			controller.central = 0.5;
			controller.span = 0.5;
			controller.defVal = 0.0;
			
			controller.midiController = 40; // mod1
			controller.SCSController = &(SCSi1->AmpFunc.Mod1);
			SCSP->setController(controller, 5);

			controller.midiController = 41; // mod2
			controller.SCSController = &(SCSi1->AmpFunc.Mod2);
			SCSP->setController(controller, 6);

			controller.midiController = 42; // mod3
			controller.SCSController = &(SCSi1->AmpFunc.Mod3);
			SCSP->setController(controller, 7);

			//MessageBox(NULL,"instrument megafunc choosen","ok",MB_OK);
		}
		break;
		case 72: // handle both 72 and
		case 74: // 74 at the same time
		{
			
			EnvFunc1 *SCSi1;

			if (patch == 74) SCSi1 = new EnvFunc1();
			else if (patch == 72) SCSi1 = new MulEnvFunc1();
			else assert(false); //error

			soundCollector->setInstrument(SCSi1);
				soundCollector->addAP(&(SCSi1->Amp), NULL, soundCollector->getInstrumentUse());

			SCSP->newControllers(11);

			setupEnvController(SCSP,
								SCSi1->Env,
								0,
								attackController,
								holdController,
								decayController,
								sustainController,
								releaseController);

			controller.central = 0.5;
			controller.span = 0.5;
			
			controller.defVal = 0.95; // top modulators
			//controller.val = 0.95;
			controller.midiController = 40; // mod1
			controller.SCSController = &(SCSi1->TSaw);
			SCSP->setController(controller, 5);

			controller.midiController = 41; // mod2
			controller.SCSController = &(SCSi1->TSqre);
			SCSP->setController(controller, 6);

			controller.midiController = 42; // mod3
			controller.SCSController = &(SCSi1->TTri);
			SCSP->setController(controller, 7);
			
			controller.defVal = -1.0; // bottom modulators
			//controller.val = -1.0;
			controller.midiController = 43; // mod4
			controller.SCSController = &(SCSi1->BSaw);
			SCSP->setController(controller, 8);

			controller.midiController = 44; // mod5
			controller.SCSController = &(SCSi1->BSqre);
			SCSP->setController(controller, 9);

			controller.midiController = 45; // mod6
			controller.SCSController = &(SCSi1->BTri);
			SCSP->setController(controller, 10);

			//MessageBox(NULL,"instrument megafunc choosen","ok",MB_OK);
		}
		break;
		case 75:
		case 76:
		{
			
			EnvFunc2 *SCSi1;

			int nControllers;
			double *mod7 = NULL; //delay
			double *mod8 = NULL; //feedback amount
			if (patch == 75)
			{
				SCSi1 = new EnvFunc2();
				nControllers = 16;
			}
			else if (patch == 76)
			{
				FBack1 *fBack1 = new FBack1();
				SCSi1 = fBack1;
				nControllers = 18;
				//mod7 = &(fBack1->Delay.Delay);
				mod7 = &(fBack1->DelayAmount);
				mod8 = &(fBack1->FeedbackAmount);
			}
			else
			{
				assert(false);
			}

			soundCollector->setInstrument(SCSi1);
			soundCollector->addAP(&(SCSi1->Amp), NULL, soundCollector->getInstrumentUse());

			SCSP->newControllers(nControllers);

			setupEnvController(SCSP,
								SCSi1->Env1,
								0,
								attackController,
								holdController,
								decayController,
								sustainController,
								releaseController);			


			controller.central = 0.5;
			controller.span = 0.5;
			
			controller.defVal = 0.95; // top modulators
			//controller.val = 0.95;
			controller.midiController = 40; // mod1
			controller.SCSController = &(SCSi1->TSaw);
			SCSP->setController(controller, 5);

			controller.midiController = 41; // mod2
			controller.SCSController = &(SCSi1->TSqre);
			SCSP->setController(controller, 6);

			controller.midiController = 42; // mod3
			controller.SCSController = &(SCSi1->TTri);
			SCSP->setController(controller, 7);
			
			controller.defVal = -1.0; // bottom modulators
			//controller.val = -1.0;
			controller.midiController = 43; // mod4
			controller.SCSController = &(SCSi1->BSaw);
			SCSP->setController(controller, 8);

			controller.midiController = 44; // mod5
			controller.SCSController = &(SCSi1->BSqre);
			SCSP->setController(controller, 9);

			controller.midiController = 45; // mod6
			controller.SCSController = &(SCSi1->BTri);
			SCSP->setController(controller, 10);

			//MessageBox(NULL,"instrument megafunc choosen","ok",MB_OK);



			// ---------- set second enveloper -----------

			attackController.midiController = 28;
			holdController.midiController = 29;
			decayController.midiController = 30;
			sustainController.midiController = 31;
			releaseController.midiController = 32;

			setupEnvController(SCSP,
								SCSi1->Env2,
								11,
								attackController,
								holdController,
								decayController,
								sustainController,
								releaseController);



			if (patch == 76)
			{
				assert(mod7 && mod8);
				
				controller.central = 0.125;
				controller.span = 0.125;
				
				controller.defVal = -0.9;
				controller.midiController = 47; // mod7
				controller.SCSController = mod7; // delay time amount
				SCSP->setController(controller, 16);

				controller.central = 0.25;
				controller.span = 0.25;
				
				controller.defVal = -0.5;
				controller.midiController = 48; // mod8
				controller.SCSController = mod8; // delay feeback amount 
				SCSP->setController(controller, 17);
			}
		}
		break;
		case 78:
		{
			
			Tobe1 *SCSi1;

			SCSi1 = new Tobe1();

			soundCollector->setInstrument(SCSi1);
			soundCollector->addAP(&(SCSi1->Amp), NULL, soundCollector->getInstrumentUse());

			SCSP->newControllers(8);

			setupEnvController(SCSP,
								SCSi1->Env,
								0,
								attackController,
								holdController,
								decayController,
								sustainController,
								releaseController);

			controller.central = 0.5;
			controller.span = 0.5;

			//MessageBox(NULL,"instrument megafunc choosen","ok",MB_OK);

			controller.defVal = -0.765625;
			controller.midiController = 40; // mod1
			controller.SCSController = &(SCSi1->TobeSpeed);
			SCSP->setController(controller, 5);

			controller.central = 1.5;
			controller.span = 1.5;
			controller.defVal = -0.3333;
			controller.midiController = 41; // mod2
			controller.SCSController = &(SCSi1->TobeAmount);
			SCSP->setController(controller, 6);

			controller.central = 0.5;
			controller.span = 0.5;
			controller.defVal = 0.0;
			controller.midiController = 46; // brightness
			controller.SCSController = &(SCSi1->Brightness);
			SCSP->setController(controller, 7);

		}
		break;
		case 79:
		{
			
			Brass2 *SCSi1;

			SCSi1 = new Brass2();

			soundCollector->setInstrument(SCSi1);
			soundCollector->addAP(&(SCSi1->Amp), NULL, soundCollector->getInstrumentUse());

			SCSP->newControllers(12);

			controller.central = 0.5;
			controller.span = 0.5;
			controller.defVal = 0.5;
			controller.midiController = 40; // mod1
			controller.SCSController = &(SCSi1->Mod1);
			SCSP->setController(controller, 0);

			controller.central = 0.5;
			controller.span = 0.5;
			controller.defVal = 0.3;
			controller.midiController = 50; // extra reverb
			controller.SCSController = &(SCSi1->EchoAmount);
			SCSP->setController(controller, 1);

			setupEnvController(SCSP,
								SCSi1->Env1,
								2,
								attackController,
								holdController,
								decayController,
								sustainController,
								releaseController);

			setupEnvController(SCSP,
								SCSi1->Env2,
								7,
								attackController,
								holdController,
								decayController,
								sustainController,
								releaseController);

			// ---------- set second enveloper -----------

			attackController.midiController = 28;
			holdController.midiController = 29;
			decayController.midiController = 30;
			sustainController.midiController = 31;
			releaseController.midiController = 32;

			setupEnvController(SCSP,
								SCSi1->Env2,
								7,
								attackController,
								holdController,
								decayController,
								sustainController,
								releaseController);

			
		}
		break;
		case 80:
		{

			Percussion1 *SCSi1 = new Percussion1();
			
			soundCollector->setInstrument(SCSi1);
			soundCollector->addAP(&(SCSi1->Amp), NULL, soundCollector->getInstrumentUse());

			

			SCSP->newControllers(22);
			
			controller.central = 0.5;
			controller.span = 0.5;
			controller.defVal = -0.2;
			controller.midiController = 49; // decay
			controller.SCSController = &(SCSi1->AmpDecay);
			SCSP->setController(controller, 0);

			controller.central = 0.5;
			controller.span = 0.5;
			controller.defVal = -0.2;
			controller.midiController = 40; // Mod1
			controller.SCSController = &(SCSi1->Mod1);
			SCSP->setController(controller, 1);

			controller.central = 8.05;
			controller.span = 8.0;
			controller.defVal = -0.2;
			controller.midiController = 41; // Mod2
			controller.SCSController = &(SCSi1->Mod1Drop);
			SCSP->setController(controller, 2);

			controller.central = 0.0;
			controller.span = 2.7;
			controller.defVal = 0.0;
			controller.midiController = 42; // Mod3
			controller.SCSController = &(SCSi1->LinearFreChange);
			SCSP->setController(controller, 3);

			controller.central = 2.3;
			controller.span = 2.3;
			controller.defVal = -0.2;
			controller.midiController = 43; // Mod4
			controller.SCSController = &(SCSi1->LogFreChange);
			SCSP->setController(controller, 4);

			controller.central = 0.5;
			controller.span = 0.5;
			controller.defVal = 1.0;
			controller.midiController = 44; // Mod5
			controller.SCSController = &(SCSi1->FMDepth);
			SCSP->setController(controller, 5);

			controller.central = 1.0;
			controller.span = 1.0;
			controller.defVal = 1.0;
			controller.midiController = 45; // Mod6
			controller.SCSController = &(SCSi1->FMDepthDrop);
			SCSP->setController(controller, 6);

			controller.central = 1.0;
			controller.span = 1.0;
			controller.defVal = 1.0;
			controller.midiController = 47; // Mod7
			controller.SCSController = &(SCSi1->FMFreMod);
			SCSP->setController(controller, 7);

			controller.central = 0.0;
			controller.span = 1.0;
			controller.defVal = 0.0;
			controller.midiController = 48; // Mod8
			controller.SCSController = &(SCSi1->FMFreDrop);
			SCSP->setController(controller, 8);

			controller.central = 0.5;
			controller.span = 0.5;
			controller.defVal = 1.0;
			controller.midiController = 22; // Switch1
			controller.SCSController = &(SCSi1->Waveform);
			SCSP->setController(controller, 9);

			controller.central = 0.5;
			controller.span = 0.5;
			controller.defVal = 1.0;
			controller.midiController = 24; // Switch3
			controller.SCSController = &(SCSi1->SwitchMod1DropPolarity);
			SCSP->setController(controller, 10);

			controller.central = 0.5;
			controller.span = 0.5;
			controller.defVal = 1.0;
			controller.midiController = 25; // Switch4
			controller.SCSController = &(SCSi1->UseStaticFMFre);
			SCSP->setController(controller, 11);

			controller.central = 0.5;
			controller.span = 0.5;
			controller.defVal = 0.0;
			controller.midiController = 12; // Linear Decay
			controller.SCSController = &(SCSi1->LengthMod);
			SCSP->setController(controller, 12);

			// temp controllers (do not use env controllers in the future)

			controller.central = 0.5;
			controller.span = 0.5;
			controller.defVal = 0.0;
			controller.midiController = 28; // env2 attack
			controller.SCSController = &(SCSi1->NoiseMod);
			SCSP->setController(controller, 13);

			controller.central = 0.5;
			controller.span = 0.5;
			controller.defVal = 0.0;
			controller.midiController = 30; // env2 decay
			controller.SCSController = &(SCSi1->NoiseFreMod);
			SCSP->setController(controller, 14);

			controller.central = 0.5;
			controller.span = 0.5;
			controller.defVal = 0.0;
			controller.midiController = 31; // env2 sustain
			controller.SCSController = &(SCSi1->NoiseDecay);
			SCSP->setController(controller, 15);

			controller.central = 0.5;
			controller.span = 0.5;
			controller.defVal = 0.0;
			controller.midiController = 32; // env2 release
			controller.SCSController = &(SCSi1->NoiseLengthMod);
			SCSP->setController(controller, 16);

			controller.central = 0.5;
			controller.span = 0.5;
			controller.defVal = 0.0;
			controller.midiController = 35; // env1 attack
			controller.SCSController = &(SCSi1->TrashNoise);
			SCSP->setController(controller, 17);

			controller.central = 0.5;
			controller.span = 0.5;
			controller.defVal = 0.0;
			controller.midiController = 37; // env1 decay
			controller.SCSController = &(SCSi1->TrashResponseTime);
			SCSP->setController(controller, 18);

			controller.central = 0.5;
			controller.span = 0.5;
			controller.defVal = 0.0;
			controller.midiController = 38; // env1 sustain
			controller.SCSController = &(SCSi1->NoiseStartValue);
			SCSP->setController(controller, 19);

			controller.central = 0.5;
			controller.span = 0.5;
			controller.defVal = 0.0;
			controller.midiController = 33; // env1 release
			controller.SCSController = &(SCSi1->NoiseAttackTime);
			SCSP->setController(controller, 20);

			controller.central = 0.5;
			controller.span = 0.5;
			controller.defVal = 0.0;
			controller.midiController = 61; // rev (xx)
			controller.SCSController = &(SCSi1->NoiseVaryMod);
			SCSP->setController(controller, 21);

		}
		break;
		case 81:
		{

			Vobler1 *SCSi1 = new Vobler1();
			
			soundCollector->setInstrument(SCSi1);
			soundCollector->addAP(&(SCSi1->Amp), NULL, soundCollector->getInstrumentUse());

			

			SCSP->newControllers(12);

			setupEnvController(SCSP,
								SCSi1->Env,
								0,
								attackController,
								holdController,
								decayController,
								sustainController,
								releaseController);

			controller.central = 0.5;
			controller.span = 0.5;
			controller.defVal = -0.2;
			controller.midiController = 40; // Mod1
			controller.SCSController = &(SCSi1->SawMod);
			SCSP->setController(controller, 5);

			controller.central = 0.5;
			controller.span = 0.5;
			controller.defVal = -0.2;
			controller.midiController = 41; // Mod2
			controller.SCSController = &(SCSi1->SqrMod);
			SCSP->setController(controller, 6);

			controller.central = 0.5;
			controller.span = 0.5;
			controller.defVal = -0.2;
			controller.midiController = 42; // Mod3
			controller.SCSController = &(SCSi1->ThinMod);
			SCSP->setController(controller, 7);

			controller.central = 0.5;
			controller.span = 0.5;
			controller.defVal = -0.2;
			controller.midiController = 43; // Mod4
			controller.SCSController = &(SCSi1->SawVoblMod);
			SCSP->setController(controller, 8);

			controller.central = 0.5;
			controller.span = 0.5;
			controller.defVal = -0.2;
			controller.midiController = 44; // Mod5
			controller.SCSController = &(SCSi1->SqrVoblMod);
			SCSP->setController(controller, 9);

			controller.central = 0.5;
			controller.span = 0.5;
			controller.defVal = -0.2;
			controller.midiController = 45; // Mod6
			controller.SCSController = &(SCSi1->SineVoblMod);
			SCSP->setController(controller, 10);

			controller.central = 4.0;
			controller.span = 4.0;
			controller.defVal = -0.2;
			controller.midiController = 47; // Mod7
			controller.SCSController = &(SCSi1->VoblSpeed);
			SCSP->setController(controller, 11);

		}
		break;
		case 82:
		{

			Nakos *SCSi1 = new Nakos();
			
			soundCollector->setInstrument(SCSi1);
			soundCollector->addAP(&(SCSi1->Amp), NULL, soundCollector->getInstrumentUse());

			

			SCSP->newControllers(10);

			setupEnvController(SCSP,
								SCSi1->Env,
								0,
								attackController,
								holdController,
								decayController,
								sustainController,
								releaseController);

			controller.central = 0.5;
			controller.span = 0.5;
			controller.defVal = -0.2;
			controller.midiController = 40; // Mod1
			controller.SCSController = &(SCSi1->Mod);
			SCSP->setController(controller, 5);

			controller.central = 0.5;
			controller.span = 0.5;
			controller.defVal = -0.2;
			controller.midiController = 41; // Mod2
			controller.SCSController = &(SCSi1->ThinMod);
			SCSP->setController(controller, 6);

			controller.central = 1.0;
			controller.span = 1.0;
			controller.defVal = -0.2;
			controller.midiController = 42; // Mod3
			controller.SCSController = &(SCSi1->VoblSpeed);
			SCSP->setController(controller, 7);

			controller.central = 0.5;
			controller.span = 0.5;
			controller.defVal = -0.2;
			controller.midiController = 43; // Mod4
			controller.SCSController = &(SCSi1->VoblType);
			SCSP->setController(controller, 8);

			controller.central = 0.5;
			controller.span = 0.5;
			controller.defVal = -0.2;
			controller.midiController = 46; // Brightness
			controller.SCSController = &(SCSi1->Brightness);
			SCSP->setController(controller, 9);


		}
		break;
		case 83:
		{
			Brambo *SCSi1 = new Brambo();
			SCSP->setVolMod(1.5*NORMAL_SCSP_VOL);
			soundCollector->setInstrument(SCSi1);
			soundCollector->addAP(&(SCSi1->Amp), NULL, soundCollector->getInstrumentUse());

			SCSP->newControllers(5);

			controller.central = 0.5;
			controller.span = 0.5;
			controller.defVal = -0.2;
			controller.midiController = 40; // Mod1
			controller.SCSController = &(SCSi1->Harm1);
			SCSP->setController(controller, 0);

			controller.central = 3.0;
			controller.span = 3.0;
			controller.defVal = -0.2;
			controller.midiController = 41; // Mod2
			controller.SCSController = &(SCSi1->Harm1Multiple);
			SCSP->setController(controller, 1);

			controller.central = 0.1;
			controller.span = 0.1;
			controller.defVal = -0.2;
			controller.midiController = 42; // Mod3
			controller.SCSController = &(SCSi1->InHarm);
			SCSP->setController(controller, 2);

			controller.central = 0.5;
			controller.span = 0.5;
			controller.defVal = -0.2;
			controller.midiController = 43; // Mod1
			controller.SCSController = &(SCSi1->Harm2);
			SCSP->setController(controller, 3);

			controller.central = 3.0;
			controller.span = 3.0;
			controller.defVal = -0.2;
			controller.midiController = 44; // Mod2
			controller.SCSController = &(SCSi1->Harm2Multiple);
			SCSP->setController(controller, 4);
		}
		break;
		case 84:
		{
			Brass3 *SCSi1;

			SCSi1 = new Brass3();

			soundCollector->setInstrument(SCSi1);
			soundCollector->addAP(&(SCSi1->Amp), NULL, soundCollector->getInstrumentUse());

			SCSP->newControllers(11);

			controller.central = 4.0;
			controller.span = 4.0;
			controller.defVal = 0.5;
			controller.midiController = 40; // mod1
			controller.SCSController = &(SCSi1->ThinMod);
			SCSP->setController(controller, 0);

			//controller.central = 0.5;
			//controller.span = 0.5;
			//controller.defVal = 0.3;
			//controller.midiController = 50; // extra reverb
			//controller.SCSController = &(SCSi1->EchoAmount);
			//SCSP->setController(controller, 1);

			setupEnvController(SCSP,
								SCSi1->Env1,
								1,
								attackController,
								holdController,
								decayController,
								sustainController,
								releaseController);

			setupEnvController(SCSP,
								SCSi1->Env2,
								6,
								attackController,
								holdController,
								decayController,
								sustainController,
								releaseController);

			
		}
		//case 83:
		//{
		//	Snare1 *SCSi1 = new Snare1();
		//	SCSP->setVolMod(1.5*NORMAL_SCSP_VOL);
		//	soundCollector->setInstrument(SCSi1);
		//	soundCollector->addAP(&(SCSi1->Amp), NULL, soundCollector->getInstrumentUse());
		//}
		//break;
		
		default:						
			
		break;
	}

	SCSP->setSoundCollector(soundCollector);
	
}