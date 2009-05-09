#ifndef _PERCUSSION1_
#define _PERCUSSION1_

#include "Instrument.h"
#include "Curves.h"

class Percussion1 : public Instrument
{
private:

	//double CalcNoise(double ActualTrashNoise)
	//{
	//	return NegSine(Rise2(NegSine(NPos*72.56)*NegSine(NPos*147.555)*NegSine(NPos*127.8)*NegSine(NPos*95.43)*NegSine(NPos*113.6784)*Tri(NPos*116.9) * ActualTrashNoise /* * Rise2(ActualNoiseFre/200.0) */) / Rise2(ActualTrashNoise));
	//}

	//double CalcNoise()
	//{
	//	return rSin(NPos);
	//}

public:
	
	double Pos;
	double FMPos;
	double NPos;
	double TimePos;
	double Amp;

	double VaryingFre; //new
	
	
	
	// Modifications

	double Mod1;
	double Mod1Drop;
	double LinearFreChange;
	double LogFreChange;
	double AmpDecay;
	double LengthMod;
	double FMDepth;
	double FMDepthDrop; // Dropping Actual FMDepth with Decay curve
	double FMFreMod; // Is multiplied with either StaticFMBaseFre or Fre depending on StaticFMFre
	double FMFreDrop;
	double NoiseMod; // More noise means less of the other component
	double NoiseFreMod;
	double TrashNoise;
	double TrashResponseTime;
	double NoiseDecay;
	double NoiseLengthMod;
	double NoiseStartValue;
	double NoiseAttackTime;

	double NoiseVaryMod; //new
	

	double Waveform; // 3-switcher
	double SwitchMod1DropPolarity; // 2-switcher
	double UseStaticFMFre; // 2 switcher

	// Constants
		
	double MaxLength;
	double StaticFMBaseFre;	
	
	

	Percussion1() : Instrument()
	{
		Pos = 0.0;
		FMPos = 0.0;
		NPos = 0.0;
		TimePos = 0.0;
		VaryingFre = Fre;

		// Modifications

		AmpDecay = 1.0;
		LengthMod = 0.6;
		LinearFreChange = 1.5;
		LogFreChange = 1.0;
		Mod1 = 1.0;
		Mod1Drop = 0.0;
		FMDepth = 0.0;
		FMDepthDrop = 0.0;
		FMFreMod = 1.4142135623730950488016887242097;
		FMFreDrop = 0.0;		
		NoiseMod = 0.7;
		NoiseFreMod = 0.3;
		TrashNoise = 0.5;
		TrashResponseTime = 1.0;
		NoiseDecay = 0.4;
		NoiseLengthMod = 0.6;
		NoiseStartValue = 1.0;
		NoiseAttackTime = 0.0;

		NoiseVaryMod = 0.0;
		
		srand(38774);
		
		
		// switches
		
		Waveform = 1.0;
		SwitchMod1DropPolarity = 0.0;
		UseStaticFMFre = 1.0;
	
		Amp = 0;

		// constants

		StaticFMBaseFre = 176.0;
		MaxLength = 16.0;
	}

	void NoteOn()
	{
		TimePos = 0;
		Pos = 0.0;
		FMPos = 0.0;
		NPos = 0.0;
		Playing = true;
	}

	bool Run()
	{
		TimePos = TimePos+TimeStep;
		Amp = 0.0;

		bool boolUseStaticFMFre = (UseStaticFMFre > 0.5);

		double ActualFMFreMod = FMFreMod * FMFreMod * FMFreMod * 3.0;

		// calc FMFre

		double FMFre = ActualFMFreMod * (boolUseStaticFMFre ? StaticFMBaseFre : Fre);

		// calc FMFre with Drop or rise

		if (FMFreDrop >= 0.0) FMFre = FMFre * Fall2(TimePos*FMFreDrop*64.0); // Log Down curve
		else FMFre = FMFre + TimePos*FMFre*FMFreDrop*FMFreDrop*12.0; // Linear up curve
		
		if (FMFre < 0.01) FMFre = 0.01;

		// Inc FMPos
		FMPos = FMPos + TimeStep*FMFre;

		// calc ActualFMDepth

		double ActualFMDepth = FMDepth*FMDepth*Fall2(TimePos*FMDepthDrop*FMDepthDrop*256.0)*128.0;

		
		//// calc vary Fre (new code)
		//if (Pos >= 1.0)
		//{
		//	double VaryValue = Fre * NoiseVaryMod;
		//	VaryingFre = Random(Fre - VaryValue, Fre + VaryValue);
		//	Pos -= 1.0;
		//}
		VaryingFre = Fre;

		
		
		
		// calc ActualFre
		
		double FreChangeSign = (LinearFreChange >= 0.0) ? 1.0 : -8.0;	
		
		//double ActualFre;
		//if ( (! boolUseStaticFMFre) && ActualFMFreMod > 1.0) ActualFre = Fre * (ActualFMFreMod - 1.0);
		//else ActualFre = Fre;

		double ActualFre = (VaryingFre - TimePos*VaryingFre*LinearFreChange*LinearFreChange*FreChangeSign) * Fall2(TimePos*LogFreChange*LogFreChange);
		ActualFre = ContinousFrequency(ActualFre, 0, Sine(FMPos)*ActualFMDepth);	// applying FM
		if (ActualFre <= 1.0) ActualFre = 1.0;
	
		// Inc Pos
		Pos = Pos + TimeStep*ActualFre;

		// Inc Noise position
		double ActualNoiseFre = NoiseFreMod*NoiseFreMod*NoiseFreMod*NoiseFreMod*NoiseFreMod*NoiseFreMod*NoiseFreMod*Fre*2.0;
		
		NPos += TimeStep*ActualNoiseFre;
		


	
		double ActualLength;
		if (NoiseMod < 1.0) // Add first component
		{
			
			// Calc ActualMod1 (Might Switch polarity of Mod1Drop (parameter))

			if (Mod1 < 0.0) Mod1 = 0.0;
			else if (Mod1 > 1.0) Mod1 = 1.0;
			
			double ActualMod1 = (SwitchMod1DropPolarity > 0.5)
								? (Mod1 - Mod1 * Fall2(TimePos*Mod1Drop))
								: (Mod1 * Fall2(TimePos*Mod1Drop));

			if (ActualMod1 < 0.0) ActualMod1 = 0.0;
			else if (ActualMod1 > 1.0) ActualMod1 = 1.0;

			// Genereate, depending on waveform parameter

			if (Waveform < (1.0/3.0)) Amp = Amp + BSine(Pos, 1.0 - ActualMod1);
			else if (Waveform < (2.0/3.0)) Amp = Amp + SawSine(Pos, 0.5 - ActualMod1 * 0.5);
			else Amp = Amp + SqrSine(Pos, ActualMod1);

			// Apply Amp Decay
			
			double ActualAmpDecay = AmpDecay * AmpDecay;
			ActualLength = LengthMod * LengthMod * MaxLength;

			double AmpMod = Fall2(TimePos/ActualAmpDecay) * (1.0 - TimePos/ActualLength);
			if (AmpMod < 0.0) AmpMod = 0.0;

			Amp *= AmpMod * (1.0-NoiseMod); // if only noise no of this component
		}
		
		double ActualNoiseLength;
		if (NoiseMod > 0.0) // Add Noise component
		{

			// Calculate current TrashNoise Amount
			double ActualTrashResponseTime = TrashResponseTime*TrashResponseTime;
			//double TrashResponse = 1.0 - 1.0 / ((TimePos / TrashResponseTime) + 1.0);
			double TrashResponse = (TrashResponseTime <= 0.0) ?
										1.0 :
										(1.0 - ActualTrashResponseTime / (TimePos + ActualTrashResponseTime));
			double ActualTrashNoise = TrashNoise * 12.0 * TrashResponse;

			// Generate Noise
			if (ActualTrashNoise < 0.05) ActualTrashNoise = 0.05;
			//double NoiseAmp = CalcNoise(ActualTrashNoise);
			double NoiseAmp = NegSine(Rise2(NegSine(NPos*72.56)*NegSine(NPos*147.555)*NegSine(NPos*127.8)*NegSine(NPos*95.43)*NegSine(NPos*113.6784)*Tri(NPos*116.9) * ActualTrashNoise /* * Rise2(ActualNoiseFre/200.0) */) / Rise2(ActualTrashNoise));
			//double NoiseAmp = CalcNoise();

			// Calculate Noise decay
			ActualNoiseLength = NoiseLengthMod * NoiseLengthMod * MaxLength;
			double DecayNoiseAmpMod = Fall2(TimePos/(NoiseDecay*NoiseDecay));
			
			// Apply Noise Envelope
			//double NoiseAmpMod = 0.0;
			//if ((TimePos >= NoiseAttackTime) || (NoiseAttackTime <= 0.0) || false)
			//{
			//	// Apply Decay Noise Curve
			//	NoiseAmpMod = DecayNoiseAmpMod;
			//}
			//else
			//{
			//	//// Calculate Noise Attack
			//	//double AttackPos = TimePos/NoiseAttackTime;
			//	//double AttackNoiseAmpMod = (NoiseStartValue + AttackPos*(1.0 - NoiseStartValue));										
			//	//// Apply Both Noise Curves
			//	//NoiseAmpMod += AttackNoiseAmpMod * (1.0 - AttackPos);
			//	//NoiseAmpMod += DecayNoiseAmpMod * AttackPos;
			//}

			// Calculate Attack + Decay = Envelope
			double AttackPos = 0.25 * TimePos / (NoiseAttackTime*NoiseAttackTime*NoiseAttackTime);			
			double EnvelopeNoiseAmpMod = NoiseStartValue + (DecayNoiseAmpMod - NoiseStartValue)
											* (1.0 - 1.0/(AttackPos + 1.0));

			// Apply Envelope
			double NoiseAmpMod = EnvelopeNoiseAmpMod * (1.0 - TimePos/ActualNoiseLength) * NoiseMod;
			
			
			// Apply Noise
			
			if (NoiseAmpMod < 0.0) NoiseAmpMod = 0.0;
			else if (NoiseAmpMod > 1.0) NoiseAmpMod = 1.0;

			Amp += NoiseAmp * NoiseAmpMod;
		}

		Amp = Amp * 0.5;

		//if ((ActualFre < 5.0) && (Amp < 0.001) && (Amp > -0.001)) Playing = false;
		//if (TimePos/ActualLength > 1 && Amp == 0.0) Playing = false;
		if ((TimePos > ActualLength) && (TimePos > ActualNoiseLength)) Playing = false;

		return Playing;
	}

	void NoteOff()
	{
	}

	void End()
	{
	}

	virtual void printToStream(std::ostream &s)
	{
		s << "AmpDecay = " << AmpDecay << ";					// Decay" << std::endl;
		s << "Mod1 = " << Mod1 << ";					// Mod1" << std::endl;
		s << "Mod1Drop = " << Mod1Drop << ";					// Mod2" << std::endl;
		s << "LinearFreChange = " << LinearFreChange << ";				// Mod3" << std::endl;
		s << "LogFreChange = " << LogFreChange << ";				// Mod4" << std::endl;
		s << "FMDepth = " << FMDepth << ";					// Mod5" << std::endl;
		s << "FMDepthDrop = " << FMDepthDrop << ";				// Mod6" << std::endl;
		s << "FMFreMod = " << FMFreMod << ";				// Mod7" << std::endl;
		s << "FMFreDrop = " << FMFreDrop << ";					// Mod8" << std::endl;
		s << "LengthMod = " << LengthMod << ";				// linear decay" << std::endl;
		s << "SwitchMod1DropPolarity = " << SwitchMod1DropPolarity << ";			// switch3" << std::endl;
		s << "NoiseMod = " << NoiseMod << ";					// (env2 attack)" << std::endl;
		s << "NoiseFreMod = " << NoiseFreMod << ";				// (env2 decay)" << std::endl;
		s << "NoiseDecay = " << NoiseDecay << ";				// (env2 sustain)" << std::endl;
		s << "NoiseLengthMod = " << NoiseLengthMod << ";				// (env2 release)" << std::endl;		
		s << "TrashNoise = " << TrashNoise << ";				// (env2 attack)" << std::endl;
		s << "TrashResponseTime = " << TrashResponseTime << ";				// (env1 decay)" << std::endl;
		s << "NoiseStartValue = " << NoiseStartValue << ";				// (env1 sustain)" << std::endl;
		s << "NoiseAttackTime = " << NoiseAttackTime << ";				// (env1 release)" << std::endl;
	}

};

#endif