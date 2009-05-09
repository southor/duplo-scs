
#ifndef _DUP_CURVES_
#define _DUP_CURVES_

#include "Dup_Base.h"
#include "Dup_Func.h"

namespace Dup
{

	double sine(double pos)
	{
		//pos = loop(pos);
		return sin(pos*PI*2.0);
	}

	double saw(double pos)
	{		
		return (loop(pos) * 2.0)-1.0;
	}

	double trisaw(double pos, double sym)
	{
		double y;
		pos = loop(pos);
		sym = limit(sym,-1.0,1.0)/2.0+0.5;
		if (pos > 0.5+sym/2.0) y = (pos-(0.5+sym/2.0))/(0.5-sym/2.0)-1.0;
		else y = (pos-(0.5-sym/2.0))*(-2.0/sym)+1.0;
		if (pos < 0.5-sym/2.0) y = pos/(0.5-sym/2.0);
		return y;
	}

	double decay(double pos)
	{		
		return (pos<0 ? 1.0 : 1.0/(pos+1.0));
	}

};


#endif