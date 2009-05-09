
#ifndef _NOTE_DRUM_
#define _NOTE_DRUM_

#include "Dup_Module.h"
#include "Dup_Util.h"
#include "Dup_Base.h"


class NoteDrum : public Dup::Dup_Module
{
	

private:

	dup_moduleid *IDArray;
	dup_val *freArray;
	dup_val *panArray;
	const static int ARRAY_SIZE = 256;

	const static dup_val DEFAULT_FRE;

	

public:	

	static const dup_uint32 PATHID = 112;

	static const dup_uint16 N_INPUTS = 0;
	static const dup_uint16 N_OUTPUTS = 0;
	
	NoteDrum(Dup::ModuleHandler *moduleH, dup_uint32 id, Dup::MessageParser msgParser);

	void setData(dup_uint8 i, dup_moduleid module, dup_val fre, dup_val pan);

	dup_val getVol();

	void setVol(dup_val vol);	

	virtual bool receiveMessage(dup_message *msg, int size, dup_uint8 format);

};

#endif

#ifndef _NOTE_DRUM_DEC_
#define _NOTE_DRUM_DEC_

	const dup_val NoteDrum::DEFAULT_FRE = 300.0;

	NoteDrum::NoteDrum(Dup::ModuleHandler *moduleH, dup_uint32 id, Dup::MessageParser msgParser) :	
	Dup_Module(moduleH,
		id,
		N_INPUTS,
		N_OUTPUTS)
	{

		IDArray = new dup_moduleid[ARRAY_SIZE];
		Dup::clearMemory(IDArray, sizeof(dup_moduleid)*ARRAY_SIZE);
		panArray = new dup_val[ARRAY_SIZE];
		freArray = new dup_val[ARRAY_SIZE];
		for(int i=0;i<ARRAY_SIZE;i++)
		{
			*(panArray+i) = 0.0;
			*(freArray+i) = DEFAULT_FRE;
		}

	}

	void NoteDrum::setData(dup_uint8 i, dup_moduleid module, dup_val fre, dup_val pan)
	{
		*(IDArray+i) = module;
		*(freArray+i) = fre;
		*(panArray+i) = pan;
	}

	bool NoteDrum::receiveMessage(dup_message *msg, int size, dup_uint8 format)
	{		
		if (size > 0)
		{
			Dup::MessageParser parser(msg, size, format);			
			dup_uint8 type;
			dup_uint8 data;
			dup_val vel;
			
			parser.getData(&type, sizeof(type));
			
			
			dup_val fre;

			switch(type)
			{
				case 0x80: // note on (with note)
				{
					// construct note on with fre
					Dup::MessageConstructor msgC(sizeof(type)+sizeof(fre)+sizeof(vel));
					
					parser.getData(&data, sizeof(data));
					parser.getData(&vel, sizeof(vel));

					fre = *(freArray+data);
					
					msgC.addData8(0x81); 
					msgC.addData(&fre, sizeof(fre));
					msgC.addData(&vel, sizeof(vel));
					
					// construct set pan message to set pan first
					Dup::MessageConstructor panMsgC(sizeof(type)+sizeof(dup_val));
					panMsgC.addData8(0x62);
					panMsgC.addDataVal(*(panArray+data));					
					postMessage(0.0, *(IDArray+data), panMsgC.getSize(), panMsgC.getMessage(), panMsgC.getFormat());

					postMessage(0.0, *(IDArray+data), msgC.getSize(), msgC.getMessage(), msgC.getFormat());
				}
				break;
				case 0x82: // note off
				{
					// construct note off
					Dup::MessageConstructor msgC(sizeof(type)+sizeof(dup_uint8)+sizeof(vel));
					
					parser.getData(&data, sizeof(data));
					parser.getData(&vel, sizeof(vel));
					
					msgC.addData8(0x82);
					msgC.addData(&data, sizeof(data));
					msgC.addData(&vel, sizeof(vel));

					postMessage(0.0, *(IDArray+data), msgC.getSize(), msgC.getMessage(), msgC.getFormat());
				}
				break;
				default:
					return false;
				break;
			}
			return true;
		}
		return false;
	}


#endif