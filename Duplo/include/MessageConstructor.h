
#ifndef _MESSAGE_CONSTRUCTOR_
#define _MESSAGE_CONSTRUCTOR_

#include "Dup_Func.h"
#include "MessageController.h"

namespace Dup
{

	class MessageConstructor : public MessageController
	{
		

	public:

		MessageConstructor(int size, dup_uint8 format = 0x00) : MessageController(new dup_message(size), size, (SYSTEM_REVERSE_BYTE_ORDER ? (format | 0x80) : format))
		{
		}


		bool addData(void *data, int size)
		{
			if (rangeOK(size))
			{
				copyData(data, getCurrentPointer(), size);
				forward(size);
				return true;
			}
			return false;
		}

		bool addData8(dup_uint8 data)
		{
			return addData(&data, 1);
		}

		bool addData16(dup_uint16 data)
		{
			return addData(&data, 2);
		}

		bool addData32(dup_uint32 data)
		{
			return addData(&data, 4);
		}

		bool addDataVal(dup_val val)
		{
			return addData(&val, sizeof(val));
		}

		bool addDataPos(dup_pos pos)
		{
			return addData(&pos, sizeof(pos));
		}

	};
};

#endif