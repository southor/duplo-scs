
#ifndef _DUP_MODULE_OUTPUT_
#define _DUP_MODULE_OUTPUT_

#include "Dup_Declares.h"
#include "Dup_List.h"
#include "Dup_ModuleInput.h"

namespace Dup
{

	//const static dup_val DUP_VAL_ZERO = 0.0; // how to fix this?

	class Dup_ModuleOutput
	{
	private:

		

		dup_val *sourceValue;

		Dup_List<void *, NULL> destinations;

	public:


		Dup_ModuleOutput(dup_val *sourceValue) : destinations()
		{
			this->sourceValue = sourceValue;
		}

		// warning, point sourceValue to a zero value
		Dup_ModuleOutput() : destinations()
		{
			
		}

		/*
		* returns true if soruce value pointer where NULL
		* before call
		*/	
		bool setSourcePointer(dup_val *sourceValue)
		{		
			if (sourceValue == NULL)
			{
				this->sourceValue = sourceValue;
				return true;
			}
			this->sourceValue = sourceValue;
			return false;
		}

		/*
		* returns the pointer to the sourcevalue
		* that this unit outputs.
		*/
		dup_val *getSourcePointer()
		{
			return sourceValue;
		}

		/*
		* returns the sourcevalue
		* that this unit outputs.
		*/
		dup_val getSourceValue()
		{
			return *sourceValue;
		}



		/*
		* adds a destinationUnit to this outputUnit
		*/
		void addDestinationUnit(Dup_ModuleInput *dest)
		{
			destinations.add(dest);
		}

		/*
		* get all destinationUnits
		*/
		Dup_List<void *, NULL> *getDestinationUnits()
		{
			return &destinations;
		}
		

		inline bool connect(Dup_ModuleInput *destUnit, bool check = true)
		{
			return destUnit->connect(this, check);
		}

		inline bool disconnect(Dup_ModuleInput *destUnit)
		{
			return destUnit->disconnect(this);
		}


		
	};

};

#endif