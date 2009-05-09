
#ifndef _DUP_DECAY_
#define _DUP_DECAY_

#include "Dup_Module.h"
#include "Dup_Curves.h"
#include "Dup_Base.h"


	class Decay : public Dup::Dup_Module
	{


	private:

		dup_val shortness;
		
		dup_val out;

		dup_pos startTime;

	public:

		static const dup_int16 PATHID = 106;


		static const dup_uint16 N_INPUTS = 1;
		static const dup_uint16 N_OUTPUTS = 1;

		/*
		* constants for determining at
		*  which position a certain output
		*  is placed
		*/
		static const dup_uint16 SHORTNESS_INPUT = 0;
		static const dup_uint16 Y_OUTPUT = 0;

		
		Decay(Dup::ModuleHandler *moduleH, dup_uint32 id, Dup::MessageParser msgParser) :
		Dup_Module(moduleH,
			id,
			N_INPUTS,
			N_OUTPUTS)
		{		
			out = 0.0;
			shortness = 1.0;
			startTime = Dup::timePos;
			
			// set input and output
			setInputDestination(SHORTNESS_INPUT, &shortness);
			setInputBaseVal(SHORTNESS_INPUT, 0.0);
			setOutputSourcePointer(Y_OUTPUT, &out);

		}

		virtual bool receiveMessage(dup_message *msg, int size, dup_uint8 format)
		{
			//std::cout << "--------- got msg env ----------:" << std::endl;
			
			if (Dup_Module::receiveMessage(msg, size, format))
			{
				return true;
			}
			else
			{
				if (size > 0)
				{
					
					Dup::MessageParser parser(msg, size, format);			
					dup_uint8 type;
					parser.getData(&type, sizeof(type));
					
					switch(type)
					{				
						case 0x80: // noteOn
								startTime = Dup::timePos;
								//MessageBox(NULL, "decay noteon", "ok noteon", MB_ICONEXCLAMATION | MB_OK);					
							return true;
						break;
					}
					
				}
				return false;
			}
		}

		virtual void run()
		{
			//MessageBox(NULL, "decay run", "ok run", MB_ICONEXCLAMATION | MB_OK);		
			out = Dup::decay((Dup::timePos-startTime)*shortness);
		}
	};



#endif