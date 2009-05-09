
#ifndef _DUP_LIST_
#define _DUP_LIST_

#include "Dup_Declares.h"
#include "assert.h"

namespace Dup
{

	template <class element> class ConsCell
	{
	protected:
	public:
		element obj;
		ConsCell<element>* next;
		
		ConsCell(element obj, ConsCell<element>* next)
		{

			this->obj = obj;
			this->next = next;
		}
	};


	template <class element, element NULL_ELEMENT> class Dup_List
	{
	private:
		ConsCell<element>* firstconscell;
		ConsCell<element>* lastconscell;
		ConsCell<element>* itr;
		//int numObjects;

		
	public:


		Dup_List()
		{
			firstconscell = NULL;
			lastconscell = NULL;
			resetIterator();
		}

		~Dup_List()
		{
			ConsCell<element>* cell = firstconscell;	
			ConsCell<element>* delcell;
			while(cell != NULL)
			{
				delcell = cell;
				cell = cell->next;
				delete delcell;
			}
		}

		void add(element obj)
		{
			firstconscell = new ConsCell<element>(obj, firstconscell);
			if (lastconscell == NULL) lastconscell = firstconscell;
		}

		void addLast(element obj)
		{
			if (lastconscell == NULL)
			{
				lastconscell = new ConsCell<element>(obj, NULL);
				firstconscell = lastconscell;
			}
			else
			{
				lastconscell->next = new ConsCell<element>(obj, NULL);
				lastconscell = lastconscell->next;
			}
		}

		element next()
		{
			if (itr != NULL)
			{
				element obj = itr->obj;
				itr = itr->next;
				return obj;
			
			}
			else
			{
				return NULL_ELEMENT;
			}
		}

		element get()
		{
			if (itr != NULL) return itr->obj;
			else return NULL_ELEMENT;
		}

		element getFirst()
		{
			if (firstconscell != NULL) return firstconscell->obj;
			else return NULL_ELEMENT;
		}
		void uniteLists(Dup_List<element, NULL_ELEMENT> *list)
		{
			if(lastconscell == NULL)
			{
				firstconscell = list->firstconscell;
				lastconscell = list->lastconscell;
				itr = list->firstconscell;
			}
			else
			{
				this->lastconscell->next = list->firstconscell;
				if(itr == NULL) itr = list->firstconscell;
				this->lastconscell = list->lastconscell;
			}
			list->firstconscell = NULL;
			list->lastconscell = NULL;
			list->itr = NULL;
			delete list;
		}

		void resetIterator()
		{
			itr = firstconscell;
		}

		bool end()
		{
			return (itr == NULL);
		}

		bool notEnd()
		{
			return (itr != NULL);
		}


		bool empty()
		{
			return (firstconscell == NULL);
		}

		bool remove(element obj)
		{
			ConsCell<element>* i = firstconscell;
			ConsCell<element>* pc = i;

			while (i != NULL)
			{
				if (i->obj == obj)
				{
					if (i == firstconscell)
					{
						firstconscell = i->next;
						if (i == lastconscell) lastconscell = NULL;
					}
					else
					{
						assert(pc != i);
						assert(firstconscell != lastconscell);

						if (i == lastconscell) lastconscell = pc;						
						pc->next = i->next;
						
					}
					
					if (itr == i) itr = i->next;
					delete i;
					return true;
				}
				else
				{
					pc = i;
					i = i->next;
				}
			}
			return false;
		}		


	};
};

#endif
