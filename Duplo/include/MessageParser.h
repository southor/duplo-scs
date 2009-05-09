
#ifndef _MESSAGE_PARSER_
#define _MESSAGE_PARSER_

#include "Dup_Func.h"
#include "string.h"

namespace Dup
{

	class MessageController;

	class MessageParser : public MessageController
	{

	public:

		MessageParser(dup_message *msg, int size, dup_uint8 format);
		

		void *getDataP(int size);
		

		bool getData(void *data, int size);

		dup_uint8 getData8();

		dup_uint16 getData16();

		dup_uint32 getData32();
		

	};
};

#endif

#ifndef _MESSAGE_PARSER_DEC_
#define _MESSAGE_PARSER_DEC_

#include "MessageController.h"

namespace Dup
{

	MessageParser::MessageParser(dup_message *msg, int size, dup_uint8 format) : MessageController(msg, size, format)
	{		
	}

	/*
	void *MessageParser::getDataP(int size)
	{
		if (rangeOK(size))
		{
			void *data = getCurrentPointer();
			forward(size);
			return data;			
		}
		return NULL;
	}
	*/

	bool MessageParser::getData(void *data, int size)
	{
		if (rangeOK(size))
		{
			
			if (reverseByteOrder() == SYSTEM_REVERSE_BYTE_ORDER)
			{				
				
				memcpy(data, getCurrentPointer(), size);				
			}
			else
			{
				
				swapBytes(getCurrentPointer(), data, size);
			}
			
			forward(size);
			
			return true;			
		}
		return false;
	}

	dup_uint8 MessageParser::getData8()
	{
		dup_uint8 returnVal;
		getData(&returnVal, 1);
		return returnVal;
	}

	dup_uint16 MessageParser::getData16()
	{
		dup_uint16 returnVal;
		getData(&returnVal, 2);
		return returnVal;
	}

	dup_uint32 MessageParser::getData32()
	{
		dup_uint32 returnVal;
		getData(&returnVal, 4);
		return returnVal;
	}

};


#endif