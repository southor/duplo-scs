
#ifndef _PACKED_MESSAGE_
#define _PACKED_MESSAGE_

#include "BaseMessage.h"
#include "Dup_Declares.h"

namespace Dup
{

	class PackedMessage : public BaseMessage
	{
	private:
		
		dup_moduleid moduleId;
		
		dup_pos sendMoment;

	public:

		PackedMessage(dup_pos sendMoment, dup_moduleid moduleID, dup_uint16 size, dup_message *msg, dup_uint8 format = 0x00);

		dup_moduleid getModuleId();

		dup_pos getSendMoment();


	};
};

#endif

#ifndef _PACKED_MESSAGE_DEC_
#define _PACKED_MESSAGE_DEC_

namespace Dup
{

	PackedMessage::PackedMessage(dup_pos sendMoment,
								dup_moduleid moduleId,						
								dup_uint16 size,
								dup_message *msg,
								dup_uint8 format) : 
					BaseMessage(msg, size, format)
	{
		this->moduleId = moduleId;
		this->sendMoment = sendMoment;
	}

	dup_moduleid PackedMessage::getModuleId()
	{
		return moduleId;
	}

	dup_pos PackedMessage::getSendMoment()
	{
		return sendMoment;
	}

};




#endif