
#ifndef _MODULE_HANDLER_
#define _MODULE_HANDLER_

#include "Dup_Module.h"
#include "Dup_Util.h"
#include "Dup_Patch.h"
#include "PackedMessage.h"
#include <assert.h>

namespace Dup
{

	const dup_pos MAX_POS = 1000000000.0; // (very large number)

	class ModuleHandler : Dup_Module
	{
	private:

		Dup_List<Dup_Module *, NULL> modules;

		Dup_List<void *, NULL> patches;

		Dup_List<void *, NULL> incomingMessages;
		
		PackedMessage *nextMsg;



		// for time and beat
		dup_pos BPMSetTime; // last moment in time when BMP was set
		dup_pos BPMSetBeat; // last moment in beat when BMP was set

		dup_pos BPM; // beats per minute


		PackedMessage *findNextMessage();

	public:

		ModuleHandler(dup_uint32 id);

		bool addModule(Dup_Module *module);

		Dup_Module *getModule(dup_uint32 id);

		void runAllModules();

		bool modulesNoneNull();

		void transferAll();

		virtual bool receiveMessage(dup_message *msg, int size, dup_uint8 format);

		void postMessage(dup_pos delay, dup_moduleid id, dup_uint16 size, dup_message *msg, dup_uint8 format = 0x00, bool backMode = false);

		void dispatchMessage(dup_moduleid id, dup_message *msg, dup_uint16 size, dup_uint8 format);
		
		void dispatchMessages();

		void addPatch(char *fileName, dup_uint32 patchId);
		
		void addPatch(dup_uint32 id, dup_message *msg, dup_uint32 length, dup_uint32 numOfMessages);

		void addPatch(Dup_Patch *patch);

		void runPatch(dup_uint32 id);

		bool connectModules(dup_moduleid fromId, dup_uint32 fromOutput, dup_moduleid  toId, dup_uint32 toOutput);

		dup_pos beatToTime(dup_pos beat);

		dup_pos timeToBeat(dup_pos time);

		dup_pos updateBeat();

		void setBPM(dup_pos BPM);

		dup_pos getSendTime(PackedMessage *msg);

		void deleteAll();

	};
};

#endif

#ifndef _MODULE_HANDLER_DEC_
#define _MODULE_HANDLER_DEC_

#include "../Modules/ModuleCreator.h"



	Dup::ModuleHandler::ModuleHandler(dup_uint32 id) : Dup_Module(this, id, 0, 0), modules(), patches(), incomingMessages()
	{
		addModule(this);
		nextMsg = NULL;

		BPMSetTime = timePos;
		BPMSetBeat = beatPos;

		BPM = 120.0;
	}

	Dup::PackedMessage *Dup::ModuleHandler::findNextMessage()
	{
		// find next message to be dispatched
		PackedMessage *currentCandidate;
		PackedMessage *bestCandidate = NULL;

		dup_pos bestTime = MAX_POS;
		dup_pos sendTime;

		incomingMessages.resetIterator();
		while(incomingMessages.notEnd())
		{
			currentCandidate = (PackedMessage *)(incomingMessages.next());

			// compare sendTimes
			sendTime = getSendTime(currentCandidate);
			if (sendTime <= bestTime)
			{
				bestCandidate = currentCandidate;
				bestTime = sendTime;
			}
		}

		return bestCandidate;
	}

	bool Dup::ModuleHandler::addModule(Dup_Module *module)
	{		
		assert(module != NULL);
		Dup_Module *tmpModule;
		modules.resetIterator();
		while(modules.notEnd())
		{
			tmpModule = modules.next();
			if ((module == tmpModule) || (module->getID() == tmpModule->getID()))
			{
				return false;
			}
		}
		modules.add(module);
		return true;
	}

	Dup::Dup_Module *Dup::ModuleHandler::getModule(dup_uint32 id)
	{
		Dup_Module *module;
		modules.resetIterator();
		while(modules.notEnd())
		{
			module = (modules.next());
			if (id == module->getID())
			{					
				return module;
			}
		}
		return NULL; // no module was found
	}

	void Dup::ModuleHandler::runAllModules()
	{		
		Dup_Module *module;
		modules.resetIterator();
		while(modules.notEnd())
		{			
			module = (modules.next());
			assert(module != NULL);
			module->run();
		}

	}

	bool Dup::ModuleHandler::modulesNoneNull()
	{
		Dup_Module *module;
		modules.resetIterator();
		while(modules.notEnd())
		{			
			module = (modules.next());
			if (module == NULL) return false;
		}
		return true;
	}

	void Dup::ModuleHandler::transferAll()
	{
		modules.resetIterator();
		Dup::Dup_Module *module;
		while(modules.notEnd())
		{
			module = modules.next();
			assert(module != NULL);
			module->transferValues();
		}
	}

	bool Dup::ModuleHandler::receiveMessage(dup_message *msg, int size, dup_uint8 format)
	{
		if (Dup_Module::receiveMessage(msg, size, format))
		{
			return true;
		}
		else if (size > 0)
		{
			MessageParser parser(msg, size, format);			
			dup_uint8 type;
			parser.getData(&type, sizeof(type));

			switch(type)
			{
				case 0x9F: // set BPM
					dup_pos newBPM;
					
					// parsing	
					parser.getData(&newBPM, sizeof(newBPM)); // BPM
					
					setBPM(newBPM);

					// here we must update which message to be nextMessage
					// because those messages that are in BeatMode
					// could switch order with a message in TimeMode
					nextMsg = findNextMessage();
				break;
				case 0xA0: // module create
								
					dup_uint32 pathId;
					dup_uint32 id;

					// parsing	
					parser.getData(&pathId, sizeof(pathId)); // path id
					parser.getData(&id, sizeof(id)); // module id
					
					createModule(this, pathId, id, parser);
				break;
				case 0xA4: // connect
					dup_uint32 fromId;
					dup_uint32 toId;
					dup_uint16 fromPort;
					dup_uint16 toPort;
					
					// parsing
					parser.getData(&fromId, sizeof(fromId)); // from module id					
					parser.getData(&fromPort, sizeof(fromPort)); // from module output
					parser.getData(&toId, sizeof(toId)); // to module id
					parser.getData(&toPort, sizeof(toPort)); // to module output

					connectModules(fromId, fromPort, toId, toPort);
				break;
				dup_uint32 patchId;
				case 0xA5: // run patch
					
					
					// parsing
					parser.getData(&patchId, sizeof(patchId)); // patch id

					runPatch(patchId);
				break;
				case 0xA7: // add patch
					
					char *fileName;

					// parsing
					parser.getData(&patchId, sizeof(patchId)); // patch id
					fileName = new char[size-sizeof(patchId)];
					parser.getData(fileName, size-sizeof(patchId)); // filename

					addPatch(fileName, patchId);
				break;
				default:
					return false;
				break;
				
			}
			
			return true;
		}
		else
		{	
			return false;
		}
	}


	void Dup::ModuleHandler::postMessage(dup_pos delay, dup_moduleid id, dup_uint16 size, dup_message *msg, dup_uint8 format, bool backMode)
	{
		dup_pos sendTime;
		
		bool beatMode = ((format | PackedMessage::BEAT_MODE) == format);


		// if beatmode then translate
		if (beatMode) 
		{			
			//std::cout << "              found beatmode" << std::endl;
			sendTime = timePos + beatToTime(delay);
		}
		else
		{
			//std::cout << "      did not found beatmode" << std::endl;		
			sendTime = timePos + delay;
		}


		//std::cout << "get mess to: " << id << "  sendTime: " << sendTime << std::endl;
		PackedMessage *newMsg;
		if (beatMode)
		{
			newMsg = new PackedMessage(beatPos+delay, id, size, msg, format);
		}
		else
		{
			newMsg = new PackedMessage(sendTime, id, size, msg, format);
		}
		
		if (nextMsg == NULL)
		{
			nextMsg = newMsg;
		}
		else
		{
			dup_pos nextMsgSendTime;

			if (nextMsg->beatMode())
			{
				nextMsgSendTime = beatToTime(nextMsg->getSendMoment());
			}
			else
			{
				nextMsgSendTime = nextMsg->getSendMoment();
			}
			
			if (backMode)
			{
				if (sendTime <= nextMsgSendTime)
				{
					nextMsg = newMsg;
				}
			}
			else
			{
				if (sendTime < nextMsgSendTime)
				{
					nextMsg = newMsg;
				}
			}
				//std::cout << "overriding: " << id << "  sendTime: " << sendTime << std::endl;
		}
		if (backMode) incomingMessages.addLast(newMsg);
		else incomingMessages.add(newMsg);
	}

	void Dup::ModuleHandler::dispatchMessage(dup_moduleid id, dup_message *msg, dup_uint16 size, dup_uint8 format)
	{
		
		
		Dup_Module *module = getModule(id);
		if (module != NULL)
		{
			module->receiveMessage(msg, size, format);

			//std::cout << "dispatching to yes! " << id << std::endl;
		}
	}

	void Dup::ModuleHandler::dispatchMessages()
	{
		if (nextMsg == NULL)
		{
			return;
		}
		
		dup_pos sendTime = getSendTime(nextMsg);		
		
		while (sendTime <= timePos)
		{
			dispatchMessage(nextMsg->getModuleId(), nextMsg->getMessage(), nextMsg->getSize(), nextMsg->getFormat());
			incomingMessages.remove(nextMsg);
			
			nextMsg = NULL;
			if (incomingMessages.empty())
			{
				return;
			}
			else
			{
				nextMsg = findNextMessage();
				sendTime = getSendTime(nextMsg);
			}		
		}
	}

	void Dup::ModuleHandler::addPatch(char *fileName, dup_uint32 patchId)
	{
		addPatch(new Dup_Patch(fileName, patchId));
	}

	void Dup::ModuleHandler::addPatch(dup_uint32 id, dup_message *msg, dup_uint32 length, dup_uint32 numOfMessages)
	{
		addPatch(new Dup_Patch(id, msg, length, numOfMessages));
	}

	void Dup::ModuleHandler::addPatch(Dup_Patch *patch)
	{
		patches.add(patch);
	}

	void Dup::ModuleHandler::runPatch(dup_uint32 id)
	{
		/* patch must be run backwards in order to 
		 * make subPatches run in the right order
		 * this is compensated by setting backMode = true
		 * in the call to postMessage.
		 * To run patch backwards the patch is first parsed forward
		 * and the addresses to the mssages are saved in an array.
		 */
		
		Dup_Patch *p;
		bool foundPatch = false;
		patches.resetIterator();		
		while(patches.notEnd() && !foundPatch)
		{
			p = (Dup_Patch*)(patches.next());
			if(p->getId() == id) foundPatch = true;
		}		
		if (foundPatch)
		{
			p->resetCurrentPos();

			dup_uint32 numOfMessages = p->getNumOfMessages();
			dup_uint32 *pointers = new dup_uint32[p->getNumOfMessages()];			
			for(int i=0;i<numOfMessages;i++)
			{
				*(pointers+i) = p->getCurrentPos();
				p->forward();
			}

			for(int i=numOfMessages-1;i>=0;i--)
			{
				p->setCurrentPos(*(pointers+i), i);
				postMessage(p->getNextMessageDelay(),
							p->getNextModuleId(),
							p->getNextMessageSize(),
							p->getNextMessage(),
							p->getNextFormat(),
							true);
			}

			delete[] pointers;
		
		}

	}

	bool Dup::ModuleHandler::connectModules(dup_moduleid fromId, dup_uint32 fromOutput, dup_moduleid  toId, dup_uint32 toInput)
	{
		Dup_Module *fromModule = getModule(fromId);
		Dup_Module *toModule = getModule(toId);
		
		if ((fromModule != NULL) && (toModule != NULL))
		{
			if ((fromOutput < fromModule->getNOutputs()) && (toInput < toModule->getNInputs()))
			{
				fromModule->getOutput(fromOutput)->connect(toModule->getInput(toInput));
				
				return true;
			}
		}
		return false;
	}

	dup_pos Dup::ModuleHandler::beatToTime(dup_pos beat)
	{
		return (60.0*(beat-BPMSetBeat))/BPM + BPMSetTime;
	}

	dup_pos Dup::ModuleHandler::timeToBeat(dup_pos time)
	{		
		return (BPM*(time-BPMSetTime))/60.0 + BPMSetBeat;
	}

	dup_pos Dup::ModuleHandler::updateBeat()
	{
		beatPos = timeToBeat(timePos);
		return beatPos;
	}

	void Dup::ModuleHandler::setBPM(dup_pos BPM)
	{
		dup_pos newBPMSetTime = timePos;
		
		dup_pos newBPMSetBeat = timeToBeat(timePos);

		// ------------

		this->BPM = BPM;

		BPMSetTime = newBPMSetTime;
		BPMSetBeat = newBPMSetBeat;
	}

	dup_pos Dup::ModuleHandler::getSendTime(PackedMessage *msg)
	{
		if (msg->beatMode())
		{
			return beatToTime(msg->getSendMoment());
		}
		else return msg->getSendMoment();
	}


	void Dup::ModuleHandler::deleteAll()
	{
		Dup_Module *module;
		modules.resetIterator();
		while(modules.notEnd())
		{
			module = modules.next();
			if (module != this) delete module;
		}
	}



#endif