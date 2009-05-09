#ifndef _NEW1_
#define _NEW1_

#include "Standards.h"
#include "Curves.h"

class New1 : public STInstrument
{
public:
	// private

	double Pos;
	double VoicePos;

	EnvelopeT *Env1;
	//EchoModule *Echo;
	
	// modulations
	double Mod1;

	New1() : STInstrument()
	{
		Env1 = new EnvelopeT(0.005, 0.1, 0.15, 0.9, 0.17);
		//Env2 = new EnvelopeT(0.005, 0.1, 0.1, 0.4, 0.17);
		Pos = 0;
		Mod1 = 0.2;
		VoicePos = Random(0.5, 5.5);
		//Echo = new EchoModule(0.01, 4);
	}

	void NoteOn()
	{
		Env1->NoteOn();

		STInstrument::NoteOn();
		Pos = 0;
	}

	bool Run()
	{
		STInstrument::Run();
		
		
		

		Amp = 0;

		Playing = Env1->Run();
		

		Pos = Pos + TimeStep*Fre;
		VoicePos = VoicePos+TimeStep;
	
	
		double Mod = Rise2(Bounce(VoicePos*16.73, 0, 1))*Mod1+0.7;
		Amp = Amp + TriSaw(Pos+TimePos, Mod) + TriSaw(Pos-TimePos, Mod); 
		Amp = Amp * Env1->Amp;
		
		
		
	
		//if (Echo->Out)

		// Echo
		//Echo->In = Amp;
		//Playing = Echo->Run();
		
		//if (TimePos < 0.5) cout << Echo->Out << endl;
		//Amp = Amp + Echo->Out * 0.7;
	
		return Playing;
	}
	
	void NoteOff()
	{
		Env1->NoteOff();
		
		STInstrument::NoteOff();
	}

	void End()
	{

	}

};

#endif