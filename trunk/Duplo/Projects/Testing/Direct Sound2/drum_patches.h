
static const int N_DRUMS = 16;

static const double DRUM_SCSP_VOL = 0.6;

#include "Dup.h"

#include "percussion_setups.h"

// SCS
#include "allinstruments.h"



// @return false if N_DRUMS was exceeded, otherwise true.
bool setupGMDrums(Dup::ModuleHandler *mH,dup_moduleid startId, NoteDrum *noteDrum)
{
	SoundCollector *soundCollector;
	SCSPlayer *SCSP;
	
	dup_moduleid id = startId;
	SCSP = (SCSPlayer *)(mH->getModule(id));
	soundCollector = SCSP->getSoundCollector();
	{
		SCSP->setVolMod(1.21*DRUM_SCSP_VOL);
		BasDrum1 *SCSi1 = new BasDrum1();
		soundCollector->setInstrument(SCSi1);
		soundCollector->addAP(&(SCSi1->Amp), NULL, soundCollector->getInstrumentUse());
		
		// Acoustic Bass Drum
		noteDrum->setData(35, id, 70, 0.0);
	}
	id++;
	SCSP = (SCSPlayer *)(mH->getModule(id));
	soundCollector = SCSP->getSoundCollector();
	{
		SCSP->setVolMod(4.5*DRUM_SCSP_VOL);
		Percussion1 *SCSi1 = new Percussion1();
		soundCollector->setInstrument(SCSi1);
		soundCollector->addAP(&(SCSi1->Amp), NULL, soundCollector->getInstrumentUse());	
		DrumPatches::setupTomToms(SCSi1);

		// tom-toms
		noteDrum->setData(41, id, 90, -0.4);
		noteDrum->setData(43, id, 100, -0.5);
		noteDrum->setData(45, id, 111, -0.2);
		noteDrum->setData(47, id, 123, -0.0);
		noteDrum->setData(48, id, 136, -0.25);
		noteDrum->setData(50, id, 151, 0.5);
	}
	id++;
	SCSP = (SCSPlayer *)(mH->getModule(id));			
	soundCollector = SCSP->getSoundCollector();
	{
		SCSP->setVolMod(0.48*DRUM_SCSP_VOL);
		BasDrum2 *SCSi1 = new BasDrum2();
		soundCollector->setInstrument(SCSi1);
		soundCollector->addAP(&(SCSi1->Amp), NULL, soundCollector->getInstrumentUse());				

		// Bass Drum 1
		noteDrum->setData(36, id, 100, 0.0); 
	}
	
	id++;
	SCSP = (SCSPlayer *)(mH->getModule(id));
	soundCollector = SCSP->getSoundCollector();
	{
		SCSP->setVolMod(0.92*DRUM_SCSP_VOL);
		Percussion1 *SCSi1 = new Percussion1();
		soundCollector->setInstrument(SCSi1);
		soundCollector->addAP(&(SCSi1->Amp), NULL, soundCollector->getInstrumentUse());
		DrumPatches::setupCrashCymbal(SCSi1);
		
		// Crash Cymbals
		noteDrum->setData(49, id, 4000, -0.45); // Crash Cymbal 1
		noteDrum->setData(57, id, 5500, 0.45); // Crash Cymbal 2
	}
	id++;
	SCSP = (SCSPlayer *)(mH->getModule(id));
	soundCollector = SCSP->getSoundCollector();
	{
		SCSP->setVolMod(0.46*DRUM_SCSP_VOL);
		Percussion1 *SCSi1 = new Percussion1();
		soundCollector->setInstrument(SCSi1);
		soundCollector->addAP(&(SCSi1->Amp), NULL, soundCollector->getInstrumentUse());
		DrumPatches::setupRideCymbal(SCSi1);
		
		// Ride Cymbals
		noteDrum->setData(51, id, 5000, -0.35); // Ride Cymbal 1
		noteDrum->setData(59, id, 7200, -0.35); // Ride Cymbal 2
	}
	id++;
	SCSP = (SCSPlayer *)(mH->getModule(id));			
	soundCollector = SCSP->getSoundCollector();
	{
		SCSP->setVolMod(0.25*DRUM_SCSP_VOL);
		Cymbal1 *SCSi1 = new Cymbal1();
		soundCollector->setInstrument(SCSi1);
		soundCollector->addAP(&(SCSi1->Amp), NULL, soundCollector->getInstrumentUse());				
		
		// Chinese Cymbal
		noteDrum->setData(52, id, 1500, -0.2); 
	}
	id++;
	SCSP = (SCSPlayer *)(mH->getModule(id));
	soundCollector = SCSP->getSoundCollector();
	{
		SCSP->setVolMod(0.83*DRUM_SCSP_VOL);
		Percussion1 *SCSi1 = new Percussion1();
		soundCollector->setInstrument(SCSi1);
		soundCollector->addAP(&(SCSi1->Amp), NULL, soundCollector->getInstrumentUse());
		DrumPatches::setupRideBell(SCSi1);

		// Ride Bell
		noteDrum->setData(53, id, 2000, 0.2);
	}
	id++;
	SCSP = (SCSPlayer *)(mH->getModule(id));
	soundCollector = SCSP->getSoundCollector();
	{
		SCSP->setVolMod(0.83*DRUM_SCSP_VOL);
		Percussion1 *SCSi1 = new Percussion1();
		soundCollector->setInstrument(SCSi1);
		soundCollector->addAP(&(SCSi1->Amp), NULL, soundCollector->getInstrumentUse());
		DrumPatches::setupSplashCymbal(SCSi1);

		// Splash Cymbal
		noteDrum->setData(55, id, 7000, 0.1);
	}
	id++;
	SCSP = (SCSPlayer *)(mH->getModule(id));			
	soundCollector = SCSP->getSoundCollector();
	{
		SCSP->setVolMod(1.8*DRUM_SCSP_VOL);
		Drum1 *SCSi1 = new Drum1();
		soundCollector->setInstrument(SCSi1);				
		soundCollector->addAP(&(SCSi1->Amp), NULL, soundCollector->getInstrumentUse());				
		
		// Electric Snare
		noteDrum->setData(40, id, 500, -0.2);

	}
	id++;
	SCSP = (SCSPlayer *)(mH->getModule(id));			
	soundCollector = SCSP->getSoundCollector();
	{
		SCSP->setVolMod(2.8*DRUM_SCSP_VOL);
		Drum2 *SCSi1 = new Drum2();
		soundCollector->setCurrentInstrumentNumber(1);
		soundCollector->setInstrument(SCSi1);
		soundCollector->setInstrumentOct(4.0);
		soundCollector->setInstrumentVol(0.35);
		soundCollector->addAP(&(SCSi1->Amp), NULL, soundCollector->getInstrumentUse());
		
		Percussion1 *SCSi2 = new Percussion1();
		soundCollector->setCurrentInstrumentNumber(2);
		soundCollector->setInstrument(SCSi2);
		soundCollector->setInstrumentOct(0.5);
		soundCollector->setInstrumentVol(0.6);
		soundCollector->addAP(&(SCSi2->Amp), NULL, soundCollector->getInstrumentUse());
		DrumPatches::setupSnare1(SCSi2);


		// Acoustic Snare
		noteDrum->setData(38, id, 300, -0.3);
	}
	id++;
	SCSP = (SCSPlayer *)(mH->getModule(id));			
	soundCollector = SCSP->getSoundCollector();
	{
		SCSP->setVolMod(1.1*DRUM_SCSP_VOL);
		HiHat *SCSi1 = new HiHat();
		soundCollector->setInstrument(SCSi1);
		soundCollector->addAP(&(SCSi1->Amp), NULL, soundCollector->getInstrumentUse());
		
		// hi-hats
		noteDrum->setData(42, id, 800, -0.3); // closed hi-hat
		noteDrum->setData(44, id, 900, -0.3); // pedal hi-hat
		//noteDrum->setData(46, id, 1000, -0.3);
	}
	id++;
	SCSP = (SCSPlayer *)(mH->getModule(id));
	soundCollector = SCSP->getSoundCollector();
	{
		SCSP->setVolMod(0.5*DRUM_SCSP_VOL);
		Percussion1 *SCSi1 = new Percussion1();
		soundCollector->setInstrument(SCSi1);
		soundCollector->addAP(&(SCSi1->Amp), NULL, soundCollector->getInstrumentUse());
		DrumPatches::setupOpenHiHat(SCSi1);

		// open hi-hat
		noteDrum->setData(46, id, 1000, -0.3);
	}
	id++;
	SCSP = (SCSPlayer *)(mH->getModule(id));
	soundCollector = SCSP->getSoundCollector();
	{
		SCSP->setVolMod(0.4*DRUM_SCSP_VOL);
		Percussion1 *SCSi1 = new Percussion1();
		soundCollector->setInstrument(SCSi1);
		soundCollector->addAP(&(SCSi1->Amp), NULL, soundCollector->getInstrumentUse());
		DrumPatches::setupAgogo(SCSi1);

		// Agogo
		noteDrum->setData(67, id, 349.5, 0.15); // High Agogo
		noteDrum->setData(68, id, 261.0, 0.3); // Low Agogo
	}
	id++;
	SCSP = (SCSPlayer *)(mH->getModule(id));
	soundCollector = SCSP->getSoundCollector();
	{
		SCSP->setVolMod(0.7*DRUM_SCSP_VOL);
		Percussion1 *SCSi1 = new Percussion1();
		soundCollector->setInstrument(SCSi1);
		soundCollector->addAP(&(SCSi1->Amp), NULL, soundCollector->getInstrumentUse());
		DrumPatches::setupCabasa(SCSi1);

		// Cabasa
		noteDrum->setData(69, id, 3000, -0.7);
	}
	id++;
	SCSP = (SCSPlayer *)(mH->getModule(id));
	soundCollector = SCSP->getSoundCollector();
	{
		SCSP->setVolMod(1.0*DRUM_SCSP_VOL);
		Percussion1 *SCSi1 = new Percussion1();
		soundCollector->setInstrument(SCSi1);
		soundCollector->addAP(&(SCSi1->Amp), NULL, soundCollector->getInstrumentUse());
		DrumPatches::setupMaracas(SCSi1);

		// Maracas
		noteDrum->setData(70, id, 4000, -0.3);
	}
	id++;
	SCSP = (SCSPlayer *)(mH->getModule(id));			
	soundCollector = SCSP->getSoundCollector();
	{
		SCSP->setVolMod(0.11*DRUM_SCSP_VOL);
		Tamb *SCSi1 = new Tamb();
		soundCollector->setInstrument(SCSi1);
		soundCollector->addAP(&(SCSi1->Amp), NULL, soundCollector->getInstrumentUse());

		// Tambourine
		noteDrum->setData(54, id, 4500, -0.0);
	}
	
	return (id - startId) < N_DRUMS;
}
