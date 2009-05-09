
#ifndef _DUP_QUEUE_
#define _DUP_QUEUE_

#include "Dup_Declares.h"



namespace Dup
{

	
	template<class element, element NULL_ELEMENT> class Queue
	{
	
	
	private:
		//static const element NULL_ELEMENT = 0;
		
		dup_uint nObjects;
		dup_uint size;
		
		element *buffer;

		dup_uint front;
		dup_uint end;
		
	public:		
		
		Queue(dup_uint size)
		{
			this->size = size;

			buffer = new element[size];

			nObjects = 0;
			front = 0;
			end = 0;
		}


		~Queue()
		{
			delete[] buffer;
		}

		bool push(element obj)
		{
			if (nObjects < size)
			{
				*(buffer+end) = obj;
				nObjects++;
				end++;
				if (end >= size) end = 0;
				
				return true;
			}
			return false;
		}

		element pop()
		{
			if (nObjects > 0)
			{
				element returnObject = *(buffer+front);
				nObjects--;
				front++;
				if (front >= size) front = 0;
				return returnObject;
			}
			return NULL_ELEMENT;
		}

		element peek()
		{
			if (nObjects > 0)
			{			
				return *(buffer+front);
			}
			return NULL_ELEMENT;
		}

		bool empty()
		{
			return (nObjects == 0);
		}
		
		// currently number of objects in queue
		dup_uint getNObjects()
		{
			return nObjects;
		}
		
		// returns size of array
		dup_uint getSize()
		{
			return size;
		}
		

	};
};

#endif