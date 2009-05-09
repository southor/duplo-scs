
#ifndef _MESSAGE_CONTROLLER_
#define _MESSAGE_CONTROLLER_

#include "Dup_Declares.h"
#include "BaseMessage.h"

namespace Dup
{

	class MessageParser;


	class MessageController : public BaseMessage
	{
	private:


		int p;

	protected:

		bool rangeOK(int size);

		bool forward(int size);


	public:

		MessageController(dup_message *msg, dup_uint16 size, dup_uint8 format);

		dup_message *getCurrentPointer();

		MessageParser getParser();

	};
};

#endif

#ifndef _MESSAGE_CONTROLLER_DEC_
#define _MESSAGE_CONTROLLER_DEC_

#include "MessageParser.h"

namespace Dup
{

	bool MessageController::rangeOK(int size)
	{
		return (p+size <= this->size);
	}

	bool MessageController::forward(int size)
	{
		if (rangeOK(size))
		{
			p = p+size;
			return true;
		}
		else
		{
			return false;
		}
	}


	MessageController::MessageController(dup_message *msg, dup_uint16 size, dup_uint8 format) : BaseMessage(msg, size, format)
	{
		p = 0;		
	}

	dup_message *MessageController::getCurrentPointer()
	{
		return msg+p;
	}

	MessageParser MessageController::getParser()
	{
		
		MessageParser mP(msg, size, format);
		
		
		return mP;
	}

};

#endif