
#ifndef _TIME_ENV_1_
#define _TIME_ENV_1_

#include "Dup_Module.h"
#include "Dup_WaveFile.h"
#include "Dup_Message.h"



class TimeEnv1 : public Dup::Dup_Module
{
		

private:

	dup_val in;
	
	dup_val out;
	
	dup_val vel;

	dup_val amp;

	dup_val foundAmp;

	// private-private

	dup_pos startTime;
	dup_pos endTime;

	int state;

public:

	// states
	static const int STATE_SILENCE = 0;
	static const int STATE_ATTACK = 1;
	static const int STATE_SUSTAIN = 2;
	static const int STATE_RELEASE = 3;




	static const dup_uint32 PATHID = 103;


	//static const dup_val VAL_SCALE = 1.0;

	static const dup_uint16 N_INPUTS = 1;
	static const dup_uint16 N_OUTPUTS = 1;

	/*
	 * constants for determining at
	 *  which position a certain output
	 *  is placed
	 */
	static const dup_uint16 Y_INPUT = 0;
	static const dup_uint16 Y_OUTPUT = 0;
	
	TimeEnv1(Dup::ModuleHandler *moduleH, dup_uint32 id, Dup::MessageParser msgParser) :
	Dup_Module(moduleH,
		id,
		N_INPUTS,
		N_OUTPUTS)
	{		
		in = 0.0;
		out = 0.0;
		vel = 0.0;

		state = STATE_SILENCE;

		endTime = Dup::MAX_POS;
		startTime = 0.0;

		amp = 0.0;
		foundAmp = 0.0;
		
		// set input and output
		setInputDestination(Y_INPUT, &in);
		setOutputSourcePointer(Y_OUTPUT, &out);

	}

	bool receiveMessage(dup_message *msg, int size, dup_uint8 format)
	{
		//std::cout << "--------- got msg env ----------:" << std::endl;
		
		if (Dup_Module::receiveMessage(msg, size, format))
		{
			return true;
		}
		else
		{
			if (size <= 0)
			{
				return false;
			}
			else
			{
			
				Dup::MessageParser parser(msg, size, format);			
				dup_uint8 type;
				parser.getData(&type, sizeof(type));
				
				switch(type)
				{				
					case 0x80: // noteOn
						dup_val newVel;
						std::cout << "parsing noteOn        time: " << Dup::timePos << std::endl;
						parser.getData(&newVel, sizeof(newVel)); // attack-vel
						//vel = 0.4;
						endTime = Dup::MAX_POS;
						

						switch(state)
						{
							case STATE_RELEASE:
							{
								foundAmp = amp*vel;
								vel = newVel;
								state = STATE_ATTACK;
								startTime = Dup::timePos;
							}
							break;
							case STATE_ATTACK:
							{
								foundAmp = amp*vel/newVel;
								vel = newVel;							
								startTime = Dup::timePos;
							}
							break;
							case STATE_SUSTAIN:
							{
								foundAmp = amp*vel/newVel;
								vel = newVel;
								startTime = Dup::timePos;
								state = STATE_ATTACK;
							}
							break;
							default:
								amp = 0.0;
								foundAmp = 0.0;
								state = STATE_ATTACK;
						}
					break;
					case 0x82: // noteOff
						std::cout << "parsing noteOff       time: " << Dup::timePos << std::endl;
						//vel = 0.0;
						//vel = 0.8;
						if (state == STATE_ATTACK)
						{
							vel = amp*vel;
							endTime = Dup::timePos;
						}
						else if (state == STATE_SUSTAIN)
						{
							endTime = Dup::timePos;
						}
						
						foundAmp = 0.0;
						state = STATE_RELEASE;
					break;
					default:
						return false;	
					break;
				}
				return true;
			}	
		}
	}

	virtual void run()
	{
		switch(state)
		{
			case STATE_RELEASE:
			{
				dup_pos timeVal = Dup::timePos-endTime;
				amp = foundAmp + Dup::decay(timeVal*8.0) - (timeVal)*0.25;
				if (amp < 0.0)
				{
					amp = 0.0;
					state = STATE_SILENCE;
				}
			}
			break;
			case STATE_ATTACK:
			{
				amp = foundAmp + (Dup::timePos-startTime)*100.0;
				if (amp > 1.0)
				{
					amp = 1.0;
					state = STATE_SUSTAIN;
				}
			}
			break;
			case STATE_SUSTAIN:
			{
				amp = 1.0;
			}
			break;
			default:
				amp = 0.0;
		}
		
		out = vel*in*amp;
		
		//out = vel*0.5;
	}

};


#endif