
#ifndef _CHANNEL_PLAYER_
#define _CHANNEL_PLAYER_

#include "Dup_Module.h"
#include "Dup_Util.h"
#include "Dup_Base.h"

struct PlayInfo
{
	dup_uint8 note;
	dup_moduleid module;
};

class ChannelPlayer : public Dup::Dup_Module
{

private:

	

	Dup::Queue<dup_moduleid, 0> freePlayers;

	Dup::Dup_List<PlayInfo *, NULL> playing;
	
	dup_uint16 nPlayers;

public:	

	static const dup_uint32 PATHID = 109;

	static const dup_uint16 N_INPUTS = 0;
	static const dup_uint16 N_OUTPUTS = 0;

	
	ChannelPlayer(Dup::ModuleHandler *moduleH, dup_uint32 id, Dup::MessageParser msgParser) :	
	Dup_Module(moduleH,
		id,
		N_INPUTS,
		N_OUTPUTS) , freePlayers(msgParser.getData16()) , playing()
	{	

		nPlayers = freePlayers.getSize();

		for(int i=0;i<nPlayers;i++)
		{
			freePlayers.push(msgParser.getData32());
		}
	}

	virtual bool receiveMessage(dup_message *msg, int size, dup_uint8 format)
	{
		
		
		if (size > 0)
		{
			Dup::MessageParser parser(msg, size, format);
			dup_uint8 type;
			parser.getData(&type, sizeof(type));
			
			dup_uint8 note;
			dup_val vel;
			
			PlayInfo *play;

			switch(type)
			{
				case 0x80: // note on	
				{
					// parsing
					parser.getData(&note, sizeof(note)); // note

					
					dup_moduleid module = freePlayers.pop();
					if (module == 0) // no free players
					{
						// first playing module should start playing this note instead
						if (!playing.empty())
						{
							play = playing.getFirst();
													
							playing.remove(play);
							play->note = note;
							playing.addLast(play);
							postMessage(0.0, play->module, size, msg, format);
						}
					}
					else
					{						
						play = new PlayInfo;
						play->note = note;
						play->module = module;
						
						playing.addLast(play);
						postMessage(0.0, module, size, msg, format);
					}
				}
				break;
				case 0x82: // note off
				{
					// parsing
					parser.getData(&note, sizeof(note)); // note

					bool playerFound = false;
					playing.resetIterator();
					while(!(playing.end() || playerFound))
					{
						play = playing.next();
						
						if (play->note == note)
						{
							postMessage(0.0, play->module, size, msg, format);
							playerFound = true;
							playing.remove(play);
							freePlayers.push(play->module);
						}
					}
				}
				break;
				default:
					return false;
				break;
			}
			return true;
		}
		return false;
	}

	


};


#endif