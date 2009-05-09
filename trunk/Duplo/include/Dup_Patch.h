
#ifndef _DUP_PATCH_
#define _DUP_PATCH_

#include "Dup_Declares.h"
#include "Dup_StringLoader.h"
#include "String.h"
#include <iostream>

namespace Dup
{

	class Dup_Patch
	{
	private:

		dup_patchid id;
		
		dup_message *msgSequence;

		dup_uint32 length;

		dup_uint32 numOfMessages;

		dup_uint32 currentPos; // current position

		dup_uint32 currentMessageNum;

		dup_uint32 format;



		int getCurrentChunkLength()
		{
			return MESSAGE_OFFSET + getNextMessageSize();
		}

	public:

		static const dup_uint32 REVERSE_BYTE_ORDER = 0x80000000;

		static const dup_int32 FORMAT_OFFSET = 0;
		static const dup_int32 DELAY_OFFSET = sizeof(dup_uint8);
		static const dup_int32 ID_OFFSET = DELAY_OFFSET + sizeof(dup_pos);
		static const dup_int32 SIZE_OFFSET = ID_OFFSET + sizeof(dup_moduleid);
		static const dup_int32 MESSAGE_OFFSET = SIZE_OFFSET + sizeof(dup_uint16);




		Dup_Patch(dup_uint32 id, dup_message *msgSequence, dup_uint32 length, dup_uint32 numOfMessages)
		{
			this->currentPos = 0;
			this->currentMessageNum = 0;
			this->id = id;
			this->msgSequence = msgSequence;
			this->length = length;
			this->numOfMessages = numOfMessages;
		}

		Dup_Patch(char *fileName, dup_uint32 id)
		{
			this->currentPos = 0;
			this->currentMessageNum = 0;
			this->numOfMessages = 0;
			this->length = 0;
			this->msgSequence = NULL;
			this->id = id;
			
			if (fileName != NULL)
			{
				dup_uint32 fileLength = filelength(fileName);
				dup_uint32 MessagesSequenceBegin;

				if (fileLength >= 13+sizeof(MessagesSequenceBegin)+sizeof(numOfMessages)+sizeof(length))
				{
					std::cout << "loading patch" << std::endl;
					
					char * patchFile = loadstring(fileName, filelength(fileName));
					if (memcmp(patchFile, &"Dup_PatchFile", 13) == 0)
					{
						//char a[128];
						//std::cin >> a;
											
						memcpy(&format, patchFile+13, sizeof(format));
						memcpy(&MessagesSequenceBegin, patchFile+13+sizeof(format), sizeof(MessagesSequenceBegin));
						memcpy(&numOfMessages, patchFile+13+sizeof(format)+sizeof(MessagesSequenceBegin), sizeof(numOfMessages));
						memcpy(&length, patchFile+13+sizeof(format)+sizeof(MessagesSequenceBegin)+sizeof(numOfMessages), sizeof(length));
						

						if (SYSTEM_REVERSE_BYTE_ORDER) // swap format data if necessary
						{
							
							swapBytes(&format, &format, sizeof(format));
						}

						if (((format | Dup_Patch::REVERSE_BYTE_ORDER) == format) != SYSTEM_REVERSE_BYTE_ORDER)
						{						
							
							swapBytes(&MessagesSequenceBegin, &MessagesSequenceBegin, sizeof(MessagesSequenceBegin));
							swapBytes(&numOfMessages, &numOfMessages, sizeof(numOfMessages));
							swapBytes(&length, &length, sizeof(length));
						}
							
						

						msgSequence = new dup_byte[length];

						memcpy(msgSequence, patchFile+MessagesSequenceBegin, length);

						delete[] patchFile;

					}
				}
			}
		}

		dup_message *getSequence()
		{
			return msgSequence;
		}

		dup_patchid getId()
		{
			return id;
		}

		dup_uint32 getCurrentPos()
		{
			return currentPos;
		}

		bool setCurrentPos(dup_uint32 newPos, dup_uint32 newMsgNum)
		{
			if ((newPos < length) && (newMsgNum < numOfMessages))
			{
				currentPos = newPos;
				currentMessageNum = newMsgNum;
				return true;
			}
			else return false;
		}

		void resetCurrentPos()
		{
			currentPos = 0;
			currentMessageNum = 0;

		}

		dup_uint32 getCurrentMessageNum()
		{
			return currentMessageNum;
		}
		
		/*
		dup_uint32 getLength()
		{
			return length;
		}
		*/

		dup_uint32 getNumOfMessages()
		{
			return numOfMessages;
		}



		dup_uint8 getNextFormat()
		{	
			dup_uint8 tmp1 = (*((dup_uint8 *)(msgSequence + currentPos + FORMAT_OFFSET)));
			dup_uint8 tmp2 = ((format & 0x80000000) >> 3*8);
			dup_uint8 tmp3 = tmp1 | tmp2;
			
			return tmp3;
		}

		dup_pos getNextMessageDelay()
		{
			return *((dup_pos *)(msgSequence + currentPos + DELAY_OFFSET));
		}

		dup_moduleid getNextModuleId()
		{
			return *((dup_moduleid *)(msgSequence + currentPos + ID_OFFSET));
		}

		dup_uint16 getNextMessageSize()
		{
			dup_uint16 local = *((dup_uint16 *)(msgSequence + currentPos + SIZE_OFFSET));
			return local;
		}

		dup_message *getNextMessage()
		{
			return (dup_message *)(msgSequence + currentPos + MESSAGE_OFFSET);	
		}

		bool moreMessages()
		{
			//return ((length - getCurrentChunkLength()) > MESSAGE_OFFSET);

			// throw exception if currentPos is too big (compare with length)

			return currentMessageNum < numOfMessages;
		}

		bool forward()
		{		
			if(moreMessages())
			{
				//length = length - getCurrentChunkLength();
				currentPos = currentPos + getCurrentChunkLength();
				currentMessageNum++;
				
				return true;
			}
			return false;
		}

		Dup_Patch *duplicate()
		{
			return new Dup_Patch(id, msgSequence, length, numOfMessages);
		}

		dup_uint32 getPatchFormat()
		{
			return format;
		}

	};

};

#endif