#ifndef _INSTRUMENT_
#define _INSTRUMENT_

#include "Module.h"

#include <fstream>

class Instrument : public Module
{
public:

	Instrument()
	{
		Duration = 0;
		Fre = 0;
	}


	double Duration;
	double Fre;

	void printToFile(const std::string &name)
	{
		const char *n = name.c_str();
		std::fstream outFile(n, std::fstream::out);
		if (outFile.good())
		{
			printToStream(outFile);
		}
		outFile.close();
	}

	virtual void printToStream(std::ostream &s)
	{
	}

};

#endif