#ifndef _STRING2_
#define _STRING2_

#include "Standards.h"
#include "Curves.h"

class String2 : public STInstrument
{
public:

	// private
	double Pos;
	double WhinePos; // the whinings current position

	// modules
	EnvelopeT *Env1;
	EchoModule *Echo;
	
	// modulations
	double WhineSpeed; // How fast frequency should go up and down
	double WhineAmount; // How much frequensy should go up and down
	double FreAffect; // How frequensy affect whineSpeed and whineAmount

	double MellowAmount; // amount of the mellow part of the sound	
	//double MellowBrightness; // How bright the mellow part of the sound sound should be
	double Osc2Brightness;
	double Osc1Brightness;

	double EchoAmount;

	String2() : STInstrument()
	{
		Env1 = new EnvelopeT(0.025, 0.03, 0.05, 1.0, 0.1);

		Pos = 0;
	
		Echo = new EchoModule(0.005, 4);
		Echo->Dim = 1.5;

		WhinePos = 0;
		WhineSpeed = 1.2;
		MellowAmount = 0.5;
		//MellowBrightness = 0.8;
		Osc1Brightness = 1.0;
		Osc2Brightness = 0.8;
		WhineAmount = 0.15;
		FreAffect = 0.035;

		EchoAmount = 0.27;
	}

	void NoteOn()
	{
		Env1->NoteOn();
		Echo->NoteOn();

		STInstrument::NoteOn();
		Pos = 0;
		WhinePos = Random(0, 10.0);
	}

	#define SCS_BR Osc1Brightness
	#define SCS_MB Osc2Brightness	
	bool Run()
	{
		STInstrument::Run();
		
		
		

		Amp = 0;

		if (Env1->Playing == true)
		{
			if(Env1->Run() == false)
			{
				Echo->NoteOff();
			}

			double SqrRootFre = SqrRoot(Fre+20)+1/FreAffect;
			WhinePos = WhinePos + WhineSpeed*TimeStep*(SqrRootFre/80);
			double WhineValue = NegSine(WhinePos*4.01+0.733)*0.3+NegSine(WhinePos*2.34+0.2)*0.23+BSine(WhinePos*3.8, 0.55)*0.25+NegSine(WhinePos*9.31+0.95)*1.0;
			//double WhineAmount = 0.002;
			double ActualWhineAmount = WhineAmount/SqrRootFre;
			double WhineSpeed = (1/(TimePos+0.2))/64+0.05;
			Pos = Pos + TimeStep*Fre*(WhineValue*ActualWhineAmount + 1.0);
			

			//Amp = Amp + TriSaw(Pos, Brightness-Fre*FreBrightness); // old right code
			
			double Dividing = Rise2(MellowAmount);
			Dividing = MellowAmount;

			// new test code
			Amp = Amp + BSine(Pos+Tri(Pos+0.7)*0.2*SCS_BR+rSin(Pos*2+0.8)*0.05, 0.18*SCS_BR)*0.45 * (1.0-Dividing);
			Amp = Amp + BSine(Pos+Tri(Pos+0.7)*0.2*SCS_BR+BSine(Pos*2+0.8, 0.5)*0.05 + 0.48, 0.25*SCS_BR)*0.5 * (1.0-Dividing);
			Amp = Amp + rSin(Pos*1.000001+Tri(Pos*1.000001+0.7)*0.1+BSine(Pos*1.000001*2+0.8, 0.17)*0.05 + 0.6)*0.15 * (1.0-Dividing);

			
			Amp = Amp + Dividing*(TriSaw(Pos + 0.3,0.4*SCS_MB)*0.1 + TriSaw(Pos + 0.5,-1.0*SCS_MB)*0.1 + TriSaw(Pos + 0.8,0.97*SCS_MB)*0.6);
			Amp = Amp - Dividing*(TriSaw(Pos + 0.4,0.88*SCS_MB)*0.15 + TriSaw(Pos + 0.6,-0.54*SCS_MB)*0.1);

			
			Amp = Amp * Env1->Amp;
			
		}
		
		
	
		//if (Echo->Out)

		// Echo
		Echo->In = Amp;
		Playing = Echo->Run();
		
		//if (TimePos < 0.5) cout << Echo->Out << endl;
		Amp = (Amp * (1.0 - EchoAmount)) + Echo->Out * EchoAmount;
	
		return Playing;
	}
	
	void NoteOff()
	{
		Env1->NoteOff();
		
		STInstrument::NoteOff();
	}

	void End()
	{
		Echo->End();
	}

};

#endif