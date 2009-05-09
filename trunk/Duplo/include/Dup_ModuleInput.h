
#ifndef _DUP_MODULE_INPUT_
#define _DUP_MODULE_INPUT_

#include "Dup_Declares.h"
#include "Dup_List.h"

namespace Dup
{

	class Dup_ModuleOutput;

	class Dup_ModuleInput
	{
	private:

		dup_val *dest; // destination

		Dup_List<void *, NULL> sources;

		dup_val base;

	public:

		Dup_ModuleInput();
		
		bool connect(Dup_ModuleOutput *sourceUnit, bool check = true);
		bool disconnect(Dup_ModuleOutput *sourceUnit);
		dup_val *getDestinationPointer();
		dup_val transferValues();
		void setBaseVal(dup_val base);

		void setDestination(dup_val *dest);
		Dup_List<void *, NULL> *getSourceUnits();


		/*
		dup_value getValue()
		{
			return val;
		}
		*/

		/*
		* get all sourceUnits
		*/


		/*
			OLD CODE (single connected)


		bool connected()
		{
			return (sourceUnit == NULL);
		}

		
		bool disconnect()
		{
			if (sourceUnit == NULL)
			{
				return false;			
			}
			sourceUnit = NULL;
			return true;
		}

		
		dup_value *connect(Dup_ModuleOutput *sourceUnit)
		{
			this->sourceUnit = sourceUnit;
			sourceUnit->addDestination(this);
			return sourceUnit->getValuePointer();
		}

		dup_value *getValuePointer()
		{
			if (sourceUnit != NULL)
			{
				return NULL;
			}
			else
			{
				return sourceUnit->getSourcePointer();
			}
		}

		*/


		
	};
};

#endif

#ifndef _DUP_MODULE_INPUT_DEC_
#define _DUP_MODULE_INPUT_DEC_

#include "Dup_ModuleOutput.h"

namespace Dup
{

	Dup_ModuleInput::Dup_ModuleInput() : sources()
	{
		dest = NULL;
		base = 0.0;
	}

	bool Dup_ModuleInput::connect(Dup_ModuleOutput *sourceUnit, bool check)
	{
		if (check)
		{
			sources.resetIterator();
			while(sources.notEnd())
			{
				if (sourceUnit == sources.next())
				{					
					return false;
				}
			}
		}
		this->sources.add(sourceUnit);		
		sourceUnit->addDestinationUnit(this);
		return true;
	}

	bool Dup_ModuleInput::disconnect(Dup_ModuleOutput *sourceUnit)
	{
		sources.resetIterator();
		while(sources.notEnd())
		{
			if (sourceUnit == sources.next())
			{					
				return true;
			}
		}		
		return false;
	}


	dup_val *Dup_ModuleInput::getDestinationPointer()
	{
		return dest;
	}


	dup_val Dup_ModuleInput::transferValues()
	{
		if (dest == NULL)
		{
			return base;
		}
		else
		{
			*dest = base;
			sources.resetIterator();
			while(sources.notEnd())
			{
				*dest = *dest + ((Dup_ModuleOutput *)(sources.next()))->getSourceValue();			
			}
			return *dest;
		}
	}

	void Dup_ModuleInput::setBaseVal(dup_val base)
	{
		this->base = base;
	}

	void Dup_ModuleInput::setDestination(dup_val *dest)
	{
		this->dest = dest;
	}
	
	Dup_List<void *, NULL> *Dup_ModuleInput::getSourceUnits()
	{
		return &sources;
	}

};

#endif