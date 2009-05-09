
#ifndef _MESSAGE_SENDER_
#define _MESSAGE_SENDER_

#include "Dup_Module.h"
#include "Dup_Util.h"
#include "Dup_Base.h"


class MessageSender : public Dup::Dup_Module
{
	

private:

	dup_moduleid *IDArray;
	const static int ARRAY_SIZE = 256;

public:	

	static const dup_uint32 PATHID = 111;

	static const dup_uint16 N_INPUTS = 0;
	static const dup_uint16 N_OUTPUTS = 0;
	
	MessageSender(Dup::ModuleHandler *moduleH, dup_uint32 id, Dup::MessageParser msgParser) :	
	Dup_Module(moduleH,
		id,
		N_INPUTS,
		N_OUTPUTS)
	{

		IDArray = new dup_moduleid[ARRAY_SIZE];
		Dup::clearMemory(IDArray, sizeof(IDArray)*ARRAY_SIZE);

	}

	void setAddress(dup_uint8 i, dup_moduleid module)
	{
		*(IDArray+i) = module;
	}

	virtual bool receiveMessage(dup_message *msg, int size, dup_uint8 format)
	{		
		if (size > 0)
		{
			Dup::MessageParser parser(msg, size, format);			
			dup_uint8 i;
			parser.getData(&i, sizeof(i));
			
			postMessage(0.0, *(IDArray+i), size, msg, format);

			return true;
		}
		return false;
	}

};


#endif