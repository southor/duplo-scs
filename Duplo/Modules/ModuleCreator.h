
#ifndef _MODULE_CREATOR_
#define _MODULE_CREATOR_

class ModuleHandler;

namespace Dup
{

	void createModule(ModuleHandler *mH, dup_uint32 pathNr, dup_uint32 id, MessageParser argsMsg);

};


#endif



#ifndef _MODULE_CREATOR_DEC_
#define _MODULE_CREATOR_DEC_

#include "Dup_AllModules.h"
#include "ModuleHandler.h"
#include "MessageParser.h"

namespace Dup
{

	void createModule(ModuleHandler *mH, dup_uint32 pathNr, dup_uint32 id, MessageParser argsMsg)
	{
	/*
		switch(pathNr)
		{
			case NotePlayer::PATHID
				mH->addModule(new NotePlayer(mH, id, argsMsg));
			break;
			case StereoWaveBuffer::PATHID			
				mH->addModule(new StereoWaveBuffer(mH, id, argsMsg));
			break;
			case SineOscillator::PATHID	
				mH->addModule(new SineOscillator(mH, id, argsMsg));
			break;
			case TimeEnv1::PATHID	
				mH->addModule(new TimeEnv1(mH, id, argsMsg));
			break;		
		}
	*/
	}
};

#endif