

#ifndef _STANDARDS_
#define _STANDARDS_

#include "Instrument.h"


class StandardModule : public Module
{

protected:
	

public:
	
	double Amp;
	double PlayPos;
	double TimePos;

	bool Playing;
	StandardModule() : Module()
	{
		Amp = 0;
		PlayPos = 0;
		TimePos = 0;
		Playing = false;
	}

	void NoteOn()
	{
		Amp = 0;
		PlayPos = 0;
		TimePos = 0;
	
		Playing = true;
	}
};


class StandardInstrument : public Instrument
{

protected:
	

public:
	
	double Amp;
	double PlayPos;
	double TimePos;

	StandardInstrument() : Instrument()
	{
		Amp = 0;
		PlayPos = 0;
		TimePos = 0;
	
	}

	void NoteOn()
	{
		Amp = 0;
		PlayPos = 0;
		TimePos = 0;
	
		Playing = true;
	}

	void NoteOff()
	{
		
		Playing = false;
	}

	void End()
	{
	}
};


class SPInstrument : public Instrument
{

protected:
	

public:
	
	double Amp;
	
	double PlayPos;

	double ReleasePos;
	

	bool On;
	

	SPInstrument() : Instrument()
	{
		Amp = 0;
		PlayPos = 0;
		
	
		Playing = false;
		On = false;
		
		ReleasePos = 0;
		
	}

	void NoteOn()
	{
		Amp = 0;
		PlayPos = 0;
		
	
		On = true;
		Playing = true;
	}

	bool Run()
	{
		PlayPos = PlayPos + PlayStep;
		
		if(On == false)
		{
			ReleasePos = ReleasePos + PlayStep;
	
		}

		return Playing;
	}

	void NoteOff()
	{
		On = false;
	}

	void End()
	{
	}
};

class STInstrument : public Instrument
{

protected:
	

public:
	
	double Amp;
	
	
	double TimePos;
	
	
	double ReleaseTime;

	bool On;
	

	STInstrument() : Instrument()
	{
		Amp = 0;
	
		TimePos = 0;
	
		Playing = false;
		On = false;
		
		
		ReleaseTime = 0;
	}

	void NoteOn()
	{
		Amp = 0;
		
		TimePos = 0;
		ReleaseTime = 0;

		On = true;
		Playing = true;
	}

	bool Run()
	{
		
		TimePos = TimePos + TimeStep;
		
		if(On == false)
		{
			ReleaseTime = ReleaseTime + TimeStep;

		}


		return Playing;
	}

	void NoteOff()
	{

		On = false;
	}

	void End()
	{
	}
};








class SPTInstrument : public Instrument
{

protected:
	

public:
	
	double Amp;
	
	double PlayPos;
	double TimePos;
	
	double ReleasePos;
	double ReleaseTime;

	bool On;
	

	SPTInstrument() : Instrument()
	{
		Amp = 0;
		PlayPos = 0;
		TimePos = 0;
	
		Playing = false;
		On = false;
		
		ReleasePos = 0;
		ReleaseTime = 0;
	}

	void NoteOn()
	{
		Amp = 0;
		PlayPos = 0;
		TimePos = 0;
	
		ReleasePos = 0;
		ReleaseTime = 0;

		On = true;
		Playing = true;
	}

	bool Run()
	{
		PlayPos = PlayPos + PlayStep;
		TimePos = TimePos + TimeStep;
		
		if(On == false)
		{
			ReleaseTime = ReleaseTime + TimeStep;
			ReleasePos = ReleasePos + PlayStep;
		}


		return Playing;
	}

	void NoteOff()
	{
		On = false;
	}

	void End()
	{
	}
};





#endif
