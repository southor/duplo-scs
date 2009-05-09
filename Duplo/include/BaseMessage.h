
#ifndef _BASE_MESSAGE_
#define _BASE_MESSAGE_

#include "Dup_Declares.h"

namespace Dup
{

	class BaseMessage
	{

	protected:

		dup_uint16 size;

		dup_message *msg;

		dup_uint8 format;

	public:

		// format (bit set)
		static const dup_uint8 REVERSE_BYTE_ORDER;
		static const dup_uint8 BEAT_MODE;


		BaseMessage(dup_message *msg, dup_uint16 size, dup_uint8 format);
		
		dup_message *getMessage();

		dup_uint16 getSize();

		dup_uint8 getFormat();

		bool reverseByteOrder();
		
		bool beatMode();

	};
};

#endif

#ifndef _BASE_MESSAGE_DEC_
#define _BASE_MESSAGE_DEC_

namespace Dup
{

	const dup_uint8 BaseMessage::REVERSE_BYTE_ORDER = 0x80;
	
	const dup_uint8 BaseMessage::BEAT_MODE = 0x40;
	
	 

	BaseMessage::BaseMessage(dup_message *msg, dup_uint16 size, dup_uint8 format)
	{
		
		this->format = format;
		this->size = size;
		this->msg = msg;


	}

	dup_message *BaseMessage::getMessage()
	{
		return msg;
	}

	dup_uint16 BaseMessage::getSize()
	{
		return size;
	}

	bool BaseMessage::reverseByteOrder()
	{
		
		return ((format | BaseMessage::REVERSE_BYTE_ORDER) == format);
	}

	bool BaseMessage::beatMode()
	{
		return ((format | BaseMessage::BEAT_MODE) == format);
	}


	dup_uint8 BaseMessage::getFormat()
	{
		return format;
	}

};

#endif