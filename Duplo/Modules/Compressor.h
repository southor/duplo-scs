
#ifndef _DUPLO_COMPRESSOR_
#define _DUPLO_COMPRESSOR_

#include "Dup_Module.h"
#include "Dup_WaveFile.h"



class Compressor : public Dup::Dup_Module
{
	

private:
	
	// internal states

	dup_val currentMax;
	dup_val goalMax;

	// constant modifiers	

	dup_uint32 rate; // length of attack/release

	static const dup_val RATE_MOD; // length of attack/release

	static const dup_val LOUDNESS_MOD;
	static const dup_val BYPASS_LOUDNESS_MOD;
	
	// dynamic modifiers

	dup_val threshold;
	dup_val ratio;
	
	dup_val attack;		// 0.0 - 1.0
	dup_val release;	// 0.0 - 1.0

	dup_val outGain1;
	dup_val outGain2;

	bool bypass;
	
	// ports

	dup_val inputL;
	dup_val inputR;
	dup_val outputL;
	dup_val outputR;

	void setPortsZero()
	{
		inputL = 0.0;
		inputR = 0.0;
		outputL = 0.0;
		outputR = 0.0;
	}

	void update(dup_val y1, dup_val y2)
	{	
		if (y1 < 0.0) y1 = -y1;
		if (y2 < 0.0) y2 = -y2;

		//dup_val newMax = gain1 * gain2 * ((y1 > y2) ? y1 : y2);
		dup_val newMax = threshold * ((y1 > y2) ? y1 : y2);

		dup_val rateValue = static_cast<dup_val>(rate);
	
		if ((goalMax > newMax) && (release > 0.0))
		{
			dup_val diff = goalMax - newMax;
			goalMax -= diff / (release * release * rateValue * RATE_MOD);
		}
		else
		{
			goalMax = newMax;
		}

		if ((currentMax < goalMax) && (attack > 0.0))
		{
			dup_val diff = goalMax - currentMax;
			currentMax += diff / (attack * attack * rateValue * RATE_MOD);
		}
		else
		{
			currentMax = goalMax;
		}
	}

	inline dup_val calcCompressOutput(dup_val compress, dup_val y) const
	{
		dup_val y2 = (y < 0.0) ? -y : y;

		//return y / (compress + y2);
		return y / (compress + currentMax);

	}

public:

	//Dup::Dup_ModuleInput *getInput(dup_uint i);

	static const dup_uint32 PATHID = 118;


	static const dup_uint16 N_INPUTS = 2;
	static const dup_uint16 N_OUTPUTS = 2;

	/*
	 * constants for determining at
	 *  which position a certain output
	 *  is placed
	 */
	static const dup_uint16 Y_INPUT_L = 0;
	static const dup_uint16 Y_INPUT_R = 1;	
	static const dup_uint16 Y_OUTPUT_L = 0;
	static const dup_uint16 Y_OUTPUT_R = 1;

	//static const dup_val THRESHOLD_MIN;
	//static const dup_val THRESHOLD_MOD;
	static const dup_val COMPRESS_MOD;
	static const dup_val MIN_RATIO;
	
	// @msgParser pass rate in message (samplerate)
	Compressor(Dup::ModuleHandler *moduleH, dup_uint32 id, Dup::MessageParser msgParser) :
	Dup_Module(moduleH,
		id,
		N_INPUTS,
		N_OUTPUTS)
	{		

		currentMax = 0.0;
		goalMax = 0.0;

		setPortsZero();
		
		// set input and output
		setInputDestination(Y_INPUT_L, &inputL);
		setInputDestination(Y_INPUT_R, &inputR);
		setOutputSourcePointer(Y_OUTPUT_L, &outputL);
		setOutputSourcePointer(Y_OUTPUT_R, &outputR);

		// get data

		msgParser.getData(&rate, sizeof(rate));

		if (rate <= 0) rate = 1;

		threshold = 1.0;
		ratio = 0.0;
		attack = 0.0;
		release = 0.0;

		outGain1 = 1.0;
		outGain2 = 1.0;

		bypass = false;

	}

	void setThreshold(dup_val threshold)				{ this->threshold = threshold; }
	void setRatio(dup_val ratio)						{ this->ratio = ratio; }	
	void setAttack(dup_val attack)						{ this->attack = attack; }
	void setRelease(dup_val release)					{ this->release = release; }
	void setBypass(dup_val bypass)						{ this->bypass = (bypass > 0.5) ? true : false; }
	void setOutGain1(dup_val gain)						{ this->outGain1 = gain; }
	void setOutGain2(dup_val gain)						{ this->outGain2 = gain; }

	//void setGain1(dup_val gain1)
	//{
	//	this->gain1 = gain1;
	//}

	//void setGain2(dup_val gain2)
	//{
	//	this->gain2 = gain2;
	//}

	virtual void run()
	{
		update(inputL, inputR);

		//dup_val aThreshold = (threshold < THRESHOLD_MIN) ? THRESHOLD_MIN : threshold;
		//dup_val aLoudness = LOUDNESS_MOD * loudness;

		//dup_val compress = THRESHOLD_MOD * aThreshold * aThreshold * aThreshold;

		double valueL, valueR;
		if (bypass)
		{
			valueL = inputL * BYPASS_LOUDNESS_MOD;
			valueR = inputR * BYPASS_LOUDNESS_MOD;
		}
		else
		{

			dup_val compress1 = COMPRESS_MOD / (ratio + MIN_RATIO);
			dup_val compress2 = sqrt(compress1) * compress1;
			dup_val compress3 = compress1 * compress1;
			dup_val compress4 = compress1 * compress3;

			valueL = LOUDNESS_MOD * compress2 * calcCompressOutput(compress4, inputL);
			valueR = LOUDNESS_MOD * compress2 * calcCompressOutput(compress4, inputL);
		}

		outputL = outGain1 * outGain2 * valueL;
		outputR = outGain1 * outGain2 * valueR;
	}

};


const dup_val Compressor::RATE_MOD = 1.0;
const dup_val Compressor::LOUDNESS_MOD = 6.0;
const dup_val Compressor::BYPASS_LOUDNESS_MOD = 4.0;
const dup_val Compressor::MIN_RATIO = 0.2;

//const dup_val Compressor::RATIO_MIN = 0.0;
//const dup_val Compressor::THRESHOLD_MOD = 16.0;
const dup_val Compressor::COMPRESS_MOD = 0.3;



#endif