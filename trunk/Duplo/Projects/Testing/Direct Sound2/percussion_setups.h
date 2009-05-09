
namespace DrumPatches {


	void setupSnare1(Percussion1 *SCSi1)
	{
		SCSi1->AmpDecay = 0.101563;				// Decay
		SCSi1->Mod1 = 0.421875;					// Mod1
		SCSi1->Mod1Drop = 0.05;					// Mod2
		SCSi1->LinearFreChange = 1.98281;			// Mod3
		SCSi1->LogFreChange = 2.62344;				// Mod4
		SCSi1->FMDepth = 0.875;					// Mod5
		SCSi1->FMDepthDrop = 0.46875;				// Mod6
		SCSi1->FMFreMod = 1.48438;					// Mod7
		SCSi1->FMFreDrop = 0.984375;				// Mod8
		//SCSi1->LengthMod = 0.554688;				// linear decay
		SCSi1->LengthMod = 0.354688;				// linear decay
		SCSi1->SwitchMod1DropPolarity = 0;			// switch3
		//SCSi1->NoiseMod = 0.21875;					// (env2 attack)
		SCSi1->NoiseMod = 0.41875;					// (env2 attack)
		//SCSi1->NoiseFreMod = 0.71875;				// (env2 decay)
		SCSi1->NoiseFreMod = 0.51875;				// (env2 decay)
		SCSi1->NoiseDecay = 0.164063;				// (env2 sustain)
		SCSi1->NoiseLengthMod = 0.171875;			// (env2 release)
		SCSi1->TrashNoise = 0.992188;				// (env2 attack)
		SCSi1->TrashResponseTime = 0;				// (env1 decay)
		SCSi1->NoiseStartValue = 0.992188;			// (env1 sustain)
		SCSi1->NoiseAttackTime = 0.171875;			// (env1 release)
	}

	void setupOpenHiHat(Percussion1 *SCSi1)
	{
		SCSi1->AmpDecay = 5.0 / 128.0;								// Decay
		SCSi1->Mod1 = 87.0 / 128.0;									// Mod1
		SCSi1->Mod1Drop = 8.0*2.0 * 64.0 / 128.0;					// Mod2
		SCSi1->LinearFreChange = 2.7*2.0 * (0.0 / 128.0 - 0.5);		// Mod3
		SCSi1->LogFreChange = 2.3*2.0 * 0.0 / 128.0;				// Mod4
		SCSi1->FMDepth = 127.0 / 128.0;								// Mod5
		SCSi1->FMDepthDrop = 2.0 * 0.0 / 128.0;						// Mod6
		SCSi1->FMFreMod = 2.0 * 127.0 / 128.0;						// Mod7
		SCSi1->FMFreDrop = 2.0 * (0.0 / 128.0 - 0.5);				// Mod8					
		SCSi1->LengthMod = 7.0 / 128.0;								// linear decay
		SCSi1->SwitchMod1DropPolarity = 0.0 / 128.0;				// switch3	
		SCSi1->NoiseMod = 127.0 / 128.0;							// (env2 attack)
		SCSi1->NoiseFreMod = 90.0 / 128.0;							// (env2 decay)
		SCSi1->NoiseDecay = 31.0 / 128.0;							// (env2 sustain)
		SCSi1->NoiseLengthMod = 41.0 / 128.0;						// (env2 release)
		SCSi1->TrashNoise = 127.0 / 128.0;							// (env1 attack)
		SCSi1->TrashResponseTime = 24.0 / 128.0;					// (env1 decay)
		SCSi1->NoiseStartValue = 30.0 / 128.0;						// (env1 sustain)
		SCSi1->NoiseAttackTime = 29.0 / 128.0;						// (env1 release)
	}

	void setupTomToms(Percussion1 *SCSi1)
	{
		SCSi1->AmpDecay = 36.0 / 128.0;								// Decay
		SCSi1->Mod1 = 87.0 / 128.0;									// Mod1
		SCSi1->Mod1Drop = 8.0*2.0 * 64.0 / 128.0;					// Mod2
		SCSi1->LinearFreChange = 2.7*2.0 * (41.0 / 128.0 - 0.5);	// Mod3
		SCSi1->LogFreChange = 2.3*2.0 * 83.0 / 128.0;				// Mod4
		SCSi1->FMDepth = 125.0 / 128.0;								// Mod5
		SCSi1->FMDepthDrop = 2.0 * 127.0 / 128.0;					// Mod6
		SCSi1->FMFreMod = 2.0 * 109.0 / 128.0;						// Mod7
		SCSi1->FMFreDrop = 2.0 * (68.0 / 128.0 - 0.5);				// Mod8					
		SCSi1->LengthMod = 52.0 / 128.0;							// linear decay
		SCSi1->SwitchMod1DropPolarity = 0 / 128.0;					// switch3					
		SCSi1->NoiseMod = 94.0 / 128.0;								// (env2 attack)
		SCSi1->NoiseFreMod = 68.0 / 128.0;							// (env2 decay)
		SCSi1->NoiseDecay = 2.0 / 128.0;							// (env2 sustain)
		SCSi1->NoiseLengthMod = 53.0 / 128.0;						// (env2 release)
		SCSi1->TrashNoise = 127.0 / 128.0;							// (env1 attack)
	}

	void setupCymbals(Percussion1 *SCSi1)
	{
		SCSi1->NoiseMod = 128.0 / 128.0;							// (env2 attack)
		SCSi1->NoiseFreMod = 54.0 / 128.0;							// (env2 decay)
		SCSi1->NoiseDecay = 11.5 / 128.0;							// (env2 sustain)
		SCSi1->NoiseLengthMod = 52.0 / 128.0;						// (env2 release)
		SCSi1->TrashNoise = 117.0 / 128.0;							// (env1 attack)
		SCSi1->TrashResponseTime = 0.8;								// (env1 decay)
		SCSi1->NoiseStartValue = 1.0;
		SCSi1->NoiseAttackTime = 0.27;
	}

	void setupCrashCymbal(Percussion1 *SCSi1)
	{
		setupCymbals(SCSi1);
	}

	void setupRideCymbal(Percussion1 *SCSi1)
	{
		setupCymbals(SCSi1);
		SCSi1->TrashNoise = 0.1;	
	}

	void setupSplashCymbal(Percussion1 *SCSi1)
	{
		setupCymbals(SCSi1);
		SCSi1->TrashResponseTime = 0.2;
		SCSi1->TrashNoise = 0.4;
		SCSi1->NoiseLengthMod = 0.28;
		SCSi1->NoiseDecay = 12.0 / 128.0;
	}

	void setupRideBell(Percussion1 *SCSi1)
	{
		SCSi1->AmpDecay = 20.0 / 128.0;								// Decay
		SCSi1->Mod1 = 87.0 / 128.0;									// Mod1
		SCSi1->Mod1Drop = 8.0*2.0 * 64.0 / 128.0;					// Mod2
		SCSi1->LinearFreChange = 2.7*2.0 * (0.0 / 128.0 - 0.5);		// Mod3
		SCSi1->LogFreChange = 2.3*2.0 * 0.0 / 128.0;				// Mod4
		SCSi1->FMDepth = 125.0 / 128.0;								// Mod5
		SCSi1->FMDepthDrop = 2.0 * 64.0 / 128.0;					// Mod6
		SCSi1->FMFreMod = 2.0 * 109.0 / 128.0;						// Mod7
		SCSi1->FMFreDrop = 2.0 * (68.0 / 128.0 - 0.5);				// Mod8					
		SCSi1->LengthMod = 52.0 / 128.0;							// linear decay
		SCSi1->SwitchMod1DropPolarity = 0 / 128.0;					// switch3	
		SCSi1->NoiseMod = 127.0 / 128.0;							// (env2 attack)
		SCSi1->NoiseFreMod = 74.0 / 128.0;							// (env2 decay)
		SCSi1->NoiseDecay = 7.0 / 128.0;							// (env2 sustain)
		SCSi1->NoiseLengthMod = 45.0 / 128.0;						// (env2 release)
		SCSi1->TrashNoise = 0.0 / 128.0;							// (env1 attack)
		SCSi1->TrashResponseTime = 0.5;								// (env1 decay)
		SCSi1->NoiseAttackTime = 0.22;
	}

	void setupMaracas(Percussion1 *SCSi1)
	{
		SCSi1->AmpDecay = 33.0 / 128.0;								// Decay
		SCSi1->Mod1 = 127.0 / 128.0;								// Mod1
		SCSi1->Mod1Drop = 8.0*2.0 * 0.0 / 128.0;					// Mod2
		SCSi1->LinearFreChange = 2.7*2.0 * (0.0 / 128.0 - 0.5);		// Mod3
		SCSi1->LogFreChange = 2.3*2.0 * 0.0 / 128.0;				// Mod4
		SCSi1->FMDepth = 127.0 / 128.0;								// Mod5
		SCSi1->FMDepthDrop = 2.0 * 0.0 / 128.0;						// Mod6
		SCSi1->FMFreMod = 2.0 * 127.0 / 128.0;						// Mod7
		SCSi1->FMFreDrop = 2.0 * (0.0 / 128.0 - 0.5);				// Mod8					
		SCSi1->LengthMod = 15.0 / 128.0;							// linear decay
		SCSi1->SwitchMod1DropPolarity = 0.0 / 128.0;				// switch3	
		SCSi1->NoiseMod = 127.0 / 128.0;							// (env2 attack)
		SCSi1->NoiseFreMod = 111.0 / 128.0;							// (env2 decay)
		SCSi1->NoiseDecay = 20.0 / 128.0;							// (env2 sustain)
		SCSi1->NoiseLengthMod = 10.0 / 128.0;						// (env2 release)
		SCSi1->TrashNoise = 127.0 / 128.0;							// (env1 attack)
		SCSi1->TrashResponseTime = 0.0 / 128.0;						// (env1 decay)
		SCSi1->NoiseStartValue = 10.0 / 128.0;						// (env1 sustain)
		SCSi1->NoiseAttackTime = 32.0 / 128.0;						// (env1 release)
	}

	void setupAgogo(Percussion1 *SCSi1)
	{
		SCSi1->AmpDecay = 19.0 / 128.0;								// Decay
		SCSi1->Mod1 = 87.0 / 128.0;									// Mod1
		SCSi1->Mod1Drop = 8.0*2.0 * 40.0 / 128.0 + 0.05;			// Mod2
		SCSi1->LinearFreChange = 2.7*2.0 * (45.0 / 128.0 - 0.5);	// Mod3
		SCSi1->LogFreChange = 2.3*2.0 * 62.0 / 128.0;				// Mod4
		SCSi1->FMDepth = 78.0 / 128.0;								// Mod5
		SCSi1->FMDepthDrop = 2.0 * 0.0 / 128.0;						// Mod6
		SCSi1->FMFreMod = 2.0 * 127.0 / 128.0;						// Mod7
		SCSi1->FMFreDrop = 2.0 * (63.0 / 128.0 - 0.5);				// Mod8					
		SCSi1->LengthMod = 20.0 / 128.0;							// linear decay
		SCSi1->SwitchMod1DropPolarity = 0.0 / 128.0;				// switch3	
		SCSi1->NoiseMod = 64.0 / 128.0;								// (env2 attack)
		SCSi1->NoiseFreMod = 86.0 / 128.0;							// (env2 decay)
		SCSi1->NoiseDecay = 15.0 / 128.0;							// (env2 sustain)
		SCSi1->NoiseLengthMod = 11.0 / 128.0;						// (env2 release)
		SCSi1->TrashNoise = 127.0 / 128.0;							// (env1 attack)
		SCSi1->TrashResponseTime = 127.0 / 128.0;					// (env1 decay)
		SCSi1->NoiseStartValue = 60.0 / 128.0;						// (env1 sustain)
		SCSi1->NoiseAttackTime = 0.0 / 128.0;						// (env1 release)
	}

	void setupCabasa(Percussion1 *SCSi1)
	{
		SCSi1->AmpDecay = 0.257813;					// Decay
		SCSi1->Mod1 = 0.992188;						// Mod1
		SCSi1->Mod1Drop = 0.05;						// Mod2
		SCSi1->LinearFreChange = -2.7;				// Mod3
		SCSi1->LogFreChange = 0;					// Mod4
		SCSi1->FMDepth = 0.992188;					// Mod5
		SCSi1->FMDepthDrop = 0;						// Mod6
		SCSi1->FMFreMod = 1.98438;					// Mod7
		SCSi1->FMFreDrop = -1;						// Mod8
		SCSi1->LengthMod = 0.117188;				// linear decay
		SCSi1->SwitchMod1DropPolarity = 0;			// switch3
		SCSi1->NoiseMod = 1.0;						// (env2 attack)
		SCSi1->NoiseFreMod = 0.523438;				// (env2 decay)
		SCSi1->NoiseDecay = 0.3125;					// (env2 sustain)
		SCSi1->NoiseLengthMod = 0.109375;			// (env2 release)
		SCSi1->TrashNoise = 0.992188;				// (env1 attack)
		SCSi1->TrashResponseTime = 0;				// (env1 decay)
		SCSi1->NoiseStartValue = 0;					// (env1 sustain)
		SCSi1->NoiseAttackTime = 0.1875;			// (env1 release)
	}

};