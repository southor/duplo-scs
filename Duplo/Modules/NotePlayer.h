
#ifndef _NOTE_PLAYER_
#define _NOTE_PLAYER_

#include "Dup_Module.h"
#include "Dup_Util.h"
#include "Dup_Base.h"

static const double DEFAULT_BASE_FRE = 440.0;

double baseFre = DEFAULT_BASE_FRE;


class NotePlayer : public Dup::Dup_Module
{
	

private:

	dup_val fre;

	dup_moduleid *envelopers;
	dup_uint16 envArraySize;

	dup_uint16 modeData;

	void transmitNoteOn(dup_val vel)
	{
		Dup::MessageConstructor msgConstr(1+sizeof(vel));
		msgConstr.addData8(0x80); // add noteOn
		msgConstr.addData(&vel, sizeof(vel)); // add vel
				
		toAllEnvelopers(&msgConstr);
	}

	void transmitNoteOff(dup_val vel)
	{
		Dup::MessageConstructor msgConstr(1+sizeof(vel));
		msgConstr.addData8(0x82); // add noteOff
		msgConstr.addData(&vel, sizeof(vel)); // add vel
				
		toAllEnvelopers(&msgConstr);
	}

public:	

	// modes bit-pos
	static const dup_uint16 IGNORE_FRE = 0x8000; //ignores note-on fre


	static const dup_uint32 PATHID = 100;


	static const int NOTE_OFFSET = -69;

	static const dup_uint16 N_INPUTS = 0;
	static const dup_uint16 N_OUTPUTS = 1;

	/*
	 * constants for determining at
	 *  which position a certain output
	 *  is placed
	 */
	static const dup_uint16 FRE_OUTPUT = 0;

	
	NotePlayer(Dup::ModuleHandler *moduleH, dup_uint32 id, Dup::MessageParser msgParser) :	
	Dup_Module(moduleH,
		id,
		N_INPUTS,
		N_OUTPUTS)
	{		
		// set fre output
		setOutputSourcePointer(FRE_OUTPUT, &fre);

		fre = 1.0;

		envArraySize = 0;
		envelopers = NULL;

		modeData = 0;
	}

	bool addEnveloper(dup_moduleid env)
	{
		
		dup_uint16 emptySpace = 0;
		bool foundEmptySpace = false;
		
		bool foundEnv = false;
		int i=0;
		while((i<envArraySize) && (!foundEnv))
		{
			if (*(envelopers+i) == MODULE_ID_NULL)
			{
				emptySpace = i;
				foundEmptySpace = true;
			}			
			i++;
			foundEnv = (*(envelopers+i) == env);
		}
		if (foundEnv)
		{
			return false;
		}
		else
		{
			if (foundEmptySpace)
			{
				*(envelopers+emptySpace) = env;
			}
			else
			{
				dup_moduleid *newEnvelopers = new dup_moduleid[envArraySize+1];
				if (envelopers != NULL)
				{
					memcpy(newEnvelopers, envelopers, envArraySize*sizeof(dup_moduleid));
				}
				*(newEnvelopers+envArraySize) = env;
				envArraySize++;
				delete[] envelopers;
				envelopers = newEnvelopers;
			}
			return true;
		}
		
	}

	bool removeEnveloper(dup_moduleid env)
	{
		int i=0;
		while(i<envArraySize)
		{
			if (*(envelopers+i) == env)
			{
				*(envelopers+i) = MODULE_ID_NULL;
				return true;
			}
			i++;
		}
		return false;
	}

	dup_val getFre()
	{
		return fre;
	}

	void setFre(dup_val fre)
	{
		this->fre = fre;
	}

	void setIgnoreFre(bool on)
	{
		if (on) modeData = modeData | IGNORE_FRE;
		else modeData = modeData & (~IGNORE_FRE);
	}

	virtual bool receiveMessage(dup_message *msg, int size, dup_uint8 format)
	{
		
		
		if (size > 0)
		{
			Dup::MessageParser parser(msg, size, format);			
			dup_uint8 type;
			parser.getData(&type, sizeof(type));
			
			dup_uint8 note;
			dup_val vel;

			switch(type)
			{
				case 0x40: // set mode
				
					modeData = parser.getData32();
				break;
				case 0x61: // set fre				
					parser.getData(&fre, sizeof(fre)); // fre
				break;
				case 0x80: // note on (with note)
										
					// parsing
					parser.getData(&note, sizeof(note)); // note					
					parser.getData(&vel, sizeof(vel)); // attack-vel

					// set frequency if ignoreFre is off
					if ((modeData | IGNORE_FRE) != modeData)
					{
						fre = Dup::frequency(baseFre, 0, note + NOTE_OFFSET);
					}

					// send new msg to envelopers				
					transmitNoteOn(vel);
				break;
				case 0x81: // note on (with fre)					
					
					// parsing
					if ((modeData | IGNORE_FRE) == modeData)
					{
						dup_val fre;
						parser.getData(&fre, sizeof(fre)); // ignores fre						
					}
					else
					{
						parser.getData(&fre, sizeof(fre)); // fre
					}
					parser.getData(&vel, sizeof(vel)); // attack-vel

					// send new msg to envelopers
					transmitNoteOn(vel);
				break;
				case 0x82: // note off					
					
					std::cout << "playing noteoff   ";

					// parsing				
					parser.getData(&note, sizeof(note)); // note (ignores)
					parser.getData(&vel, sizeof(vel)); // release-vel

					// send new msg to envelopers
					transmitNoteOff(vel);
				break;
				default:
					return false;
				break;
			}
			return true;
		}
		return false;
	}

	void toAllEnvelopers(Dup::MessageConstructor *msgConstr)
	{
		toAllEnvelopers(msgConstr->getMessage(), msgConstr->getSize(), msgConstr->getFormat());
	}

	void toAllEnvelopers(dup_message *msg, dup_uint16 size, dup_uint8 format)
	{
		//std::cout << "to all envleopers: " << envArraySize << std::endl;
		
		dup_moduleid currentEnv;
		for(int i=0;i<envArraySize;i++)
		{
			currentEnv = *(envelopers+i);
			if (currentEnv != MODULE_ID_NULL)
			{
				postMessage(0.0, currentEnv, size, msg, format);
			}
		}
	
	}


};


#endif