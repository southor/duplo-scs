
#ifndef _SCS_PLAYER_
#define _SCS_PLAYER_

#include "NotePlayer.h"
#include "Dup_Util.h"
#include "Dup_Base.h"

// SCS includes
#include "SoundCollector.h"



#define PRINT_CONTROLLER_WITH_NO_EFFECT 0


//static const double BASE_FRE = 440.0; // should be inside NotePlayer

struct SCSMidiController
{
	dup_uint8 midiController;
	double central;
	double span;	// span in each direction
	double defVal;
	double val;
	double *SCSController;
};

class SCSPlayer : public Dup::Dup_Module
{
	

private:

	dup_val lOutput;
	dup_val rOutput;

	dup_val vol;
	//dup_val localVol;
	dup_val expr;	
	dup_val amp;
	dup_val fre;
	dup_val pan;
	dup_val wheel;
	dup_uint16 wheelSens;
	dup_val mod;
	dup_val modDepth;
	dup_val modFre;
	dup_val trem;
	dup_val compress;
	dup_val compressMod;
	
	


	dup_uint16 modeData;

	dup_uint16 nControllers;
	SCSMidiController *controllers;

	SoundCollector *soundCollector; // SCS sound collector


	dup_val storedBeatPos; // used for momdulation calculations
	dup_val storedModPos; // used for momdulation calculations
	

public:	

	// modes bit-pos
	static const dup_uint16 IGNORE_FRE = 0x8000; //ignores note-on fre


	static const dup_uint32 PATHID = 110;


	static const int NOTE_OFFSET = -69;

	static const dup_uint16 N_INPUTS = 0;
	static const dup_uint16 N_OUTPUTS = 2;



	static const dup_uint16 L_OUTPUT = 0;
	static const dup_uint16 R_OUTPUT = 1;

	
	SCSPlayer(Dup::ModuleHandler *moduleH, dup_uint32 id, Dup::MessageParser msgParser) :	
	Dup_Module(moduleH,
		id,
		N_INPUTS,
		N_OUTPUTS)
	{	
		soundCollector = NULL;

		lOutput = 0.0;
		rOutput = 0.0;
		vol = 1.0;
		//localVol = 1.0;
		expr = 1.0;
		//amp = 1.0;
		amp = 0.4;
		pan = 0.0;

		modeData = 0;
		fre = 1.0;
		wheel = 0.0;
		wheelSens = 1.0;
		mod = 0.0;		
		modFre = 1.73;
		modDepth = 2.0;
		trem = 0.0;

		compress = 0.0;
		compressMod = 1.0;

		storedBeatPos = 0.0;
		storedModPos = 0.0;
		
		setOutputSourcePointer(L_OUTPUT, &lOutput);
		setOutputSourcePointer(R_OUTPUT, &rOutput);

		nControllers = 0;
		controllers = NULL;
	}

	void setSoundCollector(SoundCollector *soundCollector)
	{
		this->soundCollector = soundCollector;
		setFre(fre);
		setPan(pan);
	}

	SoundCollector *getSoundCollector()
	{
		return soundCollector;
	}

	SCSMidiController *getController(dup_uint16 i)
	{
		if(i < nControllers) return (controllers+i);
		else return NULL;
	}

	bool setController(SCSMidiController controller, dup_uint16 i)
	{
		if(i < nControllers)
		{			
			*(controllers+i) = controller;
			//(controllers+i)->val = controller.defVal;
			return true;
		}
		return false;
	}

	void newControllers(dup_uint16 nControllers)
	{		
		if (this->nControllers != nControllers)
		{			
			delete[] controllers;					
			if (nControllers == 0) controllers = NULL;
			else controllers = new SCSMidiController[nControllers];			
			this->nControllers = nControllers;
		}
		
		for(int i=0;i<nControllers;i++)
		{
			(controllers+i)->midiController = 0;
			(controllers+i)->SCSController = NULL;
			(controllers+i)->central = 0.0;
			(controllers+i)->defVal = 0.0;
			(controllers+i)->span = 1.0;
			(controllers+i)->val = 0.0;
		}
		
	}

	void changeController(dup_uint8 midiController, double newVal)
	{		
		SCSMidiController *controller = findController(midiController);
		if (controller != NULL)
		{
			controller->val = newVal;
			if (controller->SCSController != NULL)
			{
				*(controller->SCSController) = controller->central + newVal*controller->span;
			}
		}
		else
		{
			dup_uint8 leftNumber = midiController / 16;
			dup_uint8 rightNumber = midiController % 16;
			char str[3] = {'0' + leftNumber, '0' + rightNumber, '\0'};
			if (leftNumber >= 10) str[0] =  'A' + leftNumber - 10;
			if (rightNumber >= 10) str[1] =  'A' + rightNumber - 10;
			
			if (PRINT_CONTROLLER_WITH_NO_EFFECT) MessageBox(NULL, str,"controller no effect",MB_OK);
		}
	}

	SCSMidiController *findController(dup_uint8 midiController)
	{
		int i=0;
		while(i<nControllers)
		{			
			if((controllers+i)->midiController == midiController)
			{
				return controllers+i;
			}
			i++;
		}
		return NULL;
	}

	double getControllerValue(dup_uint8 midiController)
	{
		SCSMidiController *controller = findController(midiController);
		if (controller != NULL)
		{
			return controller->val;
		}
		else return 0.0;		
	}

	dup_val getVolMod()
	{
		return vol;
	}

	void setVolMod(dup_val vol)
	{
		this->vol = vol;
	}

	dup_val getPan()
	{
		return pan;
	}

	void setPan(dup_val pan)
	{
		//this->pan = pan;
		//if(soundCollector != NULL) //soundCollector->Pan = pan;
		//	soundCollector->setPan(pan);
		if (soundCollector != NULL)
		{			
			if (soundCollector->getCurrentInstrumentNumber() != 0)
			{
				soundCollector->setInstrumentPan(pan);
				return;
			}			
		}
		
		this->pan = pan;
	}

	//dup_val getAmp()
	//{
	//	return amp;
	//}

	void setAmp(dup_val amp)
	{
		//(this->amp = amp;
		if (soundCollector != NULL)
		{			
			if (soundCollector->getCurrentInstrumentNumber() != 0)
			{
				soundCollector->setInstrumentVol(amp);
				return;
			}			
		}
		this->amp = amp;
	}

	//dup_val getExpr()
	//{
	//	return expr;
	//}

	void setExpr(dup_val expr)
	{
		//this->expr = expr;
		if (soundCollector != NULL)
		{			
			if (soundCollector->getCurrentInstrumentNumber() != 0)
			{
				soundCollector->setInstrumentExpr(expr);
				return;
			}			
		}
		this->expr = expr;
	}

	dup_val calcCurrentModPos()
	{
		return storedModPos + (Dup::beatPos-storedBeatPos)*modFre;
	}

	dup_val calcFre()
	{	
		//modPos = modPos + (Dup::beatPos-storedBeatPos)*modFre*modFre;
		//storedBeatPos = Dup::beatPos;


		dup_val actualModPos = calcCurrentModPos();
		if (actualModPos > 1.0)
		{
			// reset mod calcualtion member variables
			storedModPos = actualModPos - ((int)actualModPos);
			storedBeatPos = Dup::beatPos;			
			actualModPos = storedModPos;
		}
		return Dup::frequency(fre, 0, wheel*wheelSens - mod*(Dup::sine(actualModPos)*modDepth));
	}

	dup_val getFre()
	{
		return fre;
	}

	void setFre(dup_val fre)
	{
		this->fre = fre;
		if (soundCollector != NULL) soundCollector->setFre(calcFre());
	}

	void setWheel(dup_val wheel)
	{
		this->wheel = wheel;
		if (soundCollector != NULL) soundCollector->setFre(calcFre());
	}

	dup_val getWheel()
	{
		return wheel;
	}

	void setWheelSensitivity(dup_uint16 wheelSens)
	{
		this->wheelSens = wheelSens;
	}

	void setModulation(dup_val mod)
	{
		this->mod = mod;
		if (soundCollector != NULL) soundCollector->setFre(calcFre());
	}

	dup_val getModulation()
	{
		return mod;
	}

	void setModulationDepth(dup_val modDepth)
	{
		this->modDepth = modDepth;
	}

	dup_val getModulationDepth()
	{
		return modDepth;
	}

	void setModulationFre(dup_val newModFre)
	{
		// reset mod calcualtion member variables
		storedModPos = calcCurrentModPos();
		storedBeatPos = Dup::beatPos;
		this->modFre = newModFre;
	}

	dup_val getModulationFre()
	{
		return modFre;
	}

	void setTremolo(dup_val trem)
	{
		this->trem = trem;		
	}

	dup_val getTremolo()
	{
		return trem;
	}


	void setCompress(dup_val compress)	{ this->compress = compress; }

	dup_val getCompress()	{ return compress; }

	void setCompressMod(dup_val compressMod)	{ this->compressMod = compressMod; }

	dup_val getCompressMod()	{ return compressMod; }

	void setIgnoreFre(bool on)
	{
		if (on) modeData = modeData | IGNORE_FRE;
		else modeData = modeData & (~IGNORE_FRE);
	}

	virtual bool receiveMessage(dup_message *msg, int size, dup_uint8 format)
	{
				
		if ((size > 0) && (soundCollector != NULL))
		{
			Dup::MessageParser parser(msg, size, format);			
			dup_uint8 type;
			parser.getData(&type, sizeof(type));
			
			dup_uint8 note;
			dup_val vel;
			
			dup_val fre;
			dup_val pan;

			switch(type)
			{
				case 0x40: // set mode

					modeData = parser.getData32();
				break;
				case 0x61: // set fre				
					
					parser.getData(&fre, sizeof(fre)); // fre
					setFre(fre);
				break;
				case 0x62: // set pan				
					parser.getData(&pan, sizeof(pan)); // pan
					setPan(pan);
				break;
				case 0x80: // note on (with note)
					
					// parsing
					parser.getData(&note, sizeof(note)); // note					
					parser.getData(&vel, sizeof(vel)); // attack-vel

					// set frequency
					fre = Dup::frequency(baseFre, 0, note + NOTE_OFFSET);
					

					if ((modeData | IGNORE_FRE) != modeData)
					{
						//soundCollector->Fre = fre;
						setFre(fre);
					}
					soundCollector->Vel = vol*vel*3.0;
					soundCollector->AfterTouchLevel = 1.0;
					soundCollector->NoteOn();
				break;
				case 0x81: // note on (with fre)
					
					// parsing
					parser.getData(&fre, sizeof(fre)); // fre				
					parser.getData(&vel, sizeof(vel)); // attack-vel

					if ((modeData | IGNORE_FRE) != modeData)
					{
						//soundCollector->Fre = fre;
						setFre(fre);
					}
					soundCollector->Vel = vol*vel*3.0;
					soundCollector->AfterTouchLevel = 1.0;
					soundCollector->NoteOn();
				break;
				case 0x82: // note off

					soundCollector->NoteOff();
				break;
				case 0x83: // after touch

					

					parser.getData(&note, sizeof(note)); // note					
					parser.getData(&vel, sizeof(vel)); // aftertouch-vel

					soundCollector->AfterTouchLevel = vel;
					soundCollector->AfterTouch();

				break;
				default:
					return false;
				break;
			}
			return true;
		}
		return false;
	}

	dup_val calcCompression(dup_val y)
	{

		static const dup_val COMPRESS_LOUDNESS = 0.75;
		dup_val loudness = COMPRESS_LOUDNESS;

		bool p = y < 0;
		if (p) y = -y; // only work with positive values
		
		if (amp == 0.0) return 0.0;
		else
		{
			y = (1.0 - compress)*y  // linear component
					+
					compress*(1.0 - 1.0 / (y*compressMod*compressMod*16.0/amp+1.0)) * loudness / (compressMod + loudness); // compressed component
			if (p) y = -y;
			return y;
		}

		
	}

	virtual void run()
	{		
		if (soundCollector == NULL)
		{
			lOutput = 0.0;
			rOutput = 0.0;
		}
		else
		{
			if(mod != 0) soundCollector->setFre(calcFre());
			if (soundCollector->Playing) soundCollector->Run();

			lOutput = (soundCollector->LAmp)*amp*expr;
			rOutput = (soundCollector->RAmp)*amp*expr;

			if(trem != 0)
			{
				lOutput = lOutput + lOutput*trem*Dup::sine(Dup::beatPos*3.0)*0.9;
				rOutput = rOutput + lOutput*trem*Dup::sine(Dup::beatPos*3.0+0.2)*0.9;
			}
			if(compress != 0)
			{
				lOutput = calcCompression(lOutput);
				rOutput = calcCompression(rOutput);
			}
		}			
	}



};


#endif