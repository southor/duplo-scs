#ifndef _MODULE_
#define _MODULE_

class Module
{
public:

	bool Playing;



	virtual void NoteOn()
	{
		Playing = true;
	}
	virtual bool Run() = 0;
	virtual void NoteOff()
	{
		Playing = false;
	}
	
	virtual void End()
	{
	}

};

#endif