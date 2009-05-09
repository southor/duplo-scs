
#ifndef _DUP_MODULE_
#define _DUP_MODULE_

#include "Dup_Declares.h"
#include "Dup_Util.h"
#include "Dup_ModuleInput.h"
#include "Dup_ModuleOutput.h"
#include "MessageConstructor.h"
#include "Dup_Func.h"


namespace Dup
{

	
	class ModuleHandler;




	class Dup_Module
	{
	private:

		ModuleHandler *moduleH;

		dup_moduleid id;

	protected:

		Dup_ModuleInput *inputs;
		dup_uint32 nInputs;

		Dup_ModuleOutput *outputs;
		dup_uint32 nOutputs;
		
	public:

		Dup_Module(ModuleHandler *moduleH,
			dup_uint32 id,		
			dup_uint32 nInputs,
			dup_uint32 nOutputs);

		~Dup_Module();

		virtual bool receiveMessage(dup_message *msg, int size, dup_uint8 format);

		virtual void run();

		ModuleHandler *getModuleHandler();

		dup_moduleid getID();

		void transferValues();

		Dup_ModuleInput *getInputs();

		dup_uint32 getNInputs();

		Dup_ModuleInput *getInput(dup_uint i);

		Dup_ModuleOutput *getOutputs();

		dup_uint32 getNOutputs();

		bool setInputBaseVal(dup_uint32 i, dup_val base);

		Dup_ModuleOutput *getOutput(dup_uint i);

		bool setInputDestination(int i, dup_val *dest);

		bool setOutputSourcePointer(int i, dup_val *source);

		void postMessage(dup_pos delay, dup_moduleid id, dup_uint16 size, dup_message *msg, dup_uint8 format);

	};
}

#endif


#ifndef _DUP_MODULE_DEC_
#define _DUP_MODULE_DEC_

#include "ModuleHandler.h"


	Dup::Dup_Module::Dup_Module(ModuleHandler *moduleH,
		dup_uint32 id,		
		dup_uint32 nInputs,
		dup_uint32 nOutputs)
	{
		this->moduleH = moduleH;
		this->id = id;

		this->inputs = new Dup_ModuleInput[nInputs]();
		this->nInputs = nInputs;

		this->outputs = new Dup_ModuleOutput[nOutputs]();		
		this->nOutputs = nOutputs;

	}

	Dup::Dup_Module::~Dup_Module()
	{
		if (inputs != NULL) delete[] inputs;
		if (outputs != NULL) delete[] outputs;
	}


	bool Dup::Dup_Module::receiveMessage(dup_message *msg, int size, dup_uint8 format)
	{
		dup_uint8 type;
		dup_uint16 i;
		dup_val val;

		static const int TYPE_OFFSET = 0;
		static const int I_OFFSET = TYPE_OFFSET + sizeof(type);
		static const int VAL_OFFSET = I_OFFSET + sizeof(i);
		
		// error use msgparser.. or else not working with opposite byte order
		if (size >= (VAL_OFFSET + sizeof(val)))
		{
			type = *((dup_uint8 *)(msg+TYPE_OFFSET));
			if (type == 0xE0)
			{
				i = *((dup_uint32 *)(msg+I_OFFSET));
				val = *((dup_val *)(msg+VAL_OFFSET));

				setInputBaseVal(i, val);
				return true;
			}
		}
		return false;
	}

	void Dup::Dup_Module::run() {}

	Dup::ModuleHandler *Dup::Dup_Module::getModuleHandler()
	{
		return moduleH;
	}

	dup_moduleid Dup::Dup_Module::getID()
	{
		return id;
	}

	void Dup::Dup_Module::transferValues()
	{
		for(int i=0;i<nInputs;i++)
		{
			(inputs+i)->transferValues();
		}
	}

	Dup::Dup_ModuleInput *Dup::Dup_Module::getInputs()
	{
		return inputs;
	}

	dup_uint32 Dup::Dup_Module::getNInputs()
	{
		return nInputs;
	}

	Dup::Dup_ModuleInput *Dup::Dup_Module::getInput(dup_uint i)
	{
		if (i < nInputs) return (inputs+i);
		else return NULL;
	}

	Dup::Dup_ModuleOutput *Dup::Dup_Module::getOutputs()
	{
		return outputs;
	}

	dup_uint32 Dup::Dup_Module::getNOutputs()
	{
		return nOutputs;
	}

	Dup::Dup_ModuleOutput *Dup::Dup_Module::getOutput(dup_uint i)
	{
		if (i < nOutputs) return (outputs+i);
		else return NULL;
	}

	bool Dup::Dup_Module::setInputDestination(int i, dup_val *dest)
	{
		if (i < nInputs)
		{
			(inputs+i)->setDestination(dest);
			return true;
		}
		return false;
	}

	bool Dup::Dup_Module::setOutputSourcePointer(int i, dup_val *source)
	{
		if (i < nOutputs)
		{
			(outputs+i)->setSourcePointer(source);
			return true;
		}
		return false;
	}

	bool Dup::Dup_Module::setInputBaseVal(dup_uint32 i, dup_val base)
	{
		if (i < nInputs)
		{
			Dup_ModuleInput *modInp = getInput(i);
			if (modInp != NULL)
			{
				modInp->setBaseVal(base);
				return true;
			}			
		}
		return false;
	}

	void Dup::Dup_Module::postMessage(dup_pos delay, dup_moduleid id,
								dup_uint16 size,
								dup_message *msg,
								dup_uint8 format)
	{
		moduleH->postMessage(delay, id, size, msg, format);
	}


#endif