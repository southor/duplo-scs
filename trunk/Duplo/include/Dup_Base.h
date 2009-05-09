
#ifndef _DUP_BASE_H
#define _DUP_BASE_H

#include "Dup_Declares.h"
#include "math.h"

namespace Dup
{

	const double PI = 3.141592654;

	dup_pos timePos;

	dup_pos beatPos;



	//double frequency(double Fre,int Oct,int Note);
	double frequency(double fre, int oct, double note);
	dup_pos loop(dup_pos A, dup_pos Min, dup_pos Max);
	dup_pos loop(dup_pos A);




	double frequency(double fre, int oct, double note)
	{
		return fre*pow(2.0, note/12.0 + oct);
	}

	/*
	double frequency(double Fre,int Oct,int Note)
	{

		
		while(Note <= -7 || Note >= 7)
		{
			if (Note > 6) {
				Oct = Oct+1;
				Note = Note-12;
			}
			if (Note < -6) {
				Oct = Oct-1;
				Note = Note+12;
			}
		}
	
	
		// Oct = Oct+int((double)(Note+5)/12.0);
		// Note = Note-int((double)Note/7.0)*12;



		Fre = Fre * pow(2.0,(double)Oct);

		switch(Note)
		{
			case 1:
				Fre = Fre*1.059463094;
			break;
			case 2:
				Fre = Fre*1.122462048;
			break;
			case 3:
				Fre = Fre*1.189207115;
			break;
			case 4:
				Fre = Fre*1.25992105;
			break;
			case 5:
				Fre = Fre*1.334839854;
			break;
			case 6:
				Fre = Fre*1.414213562;
			break;
			case -1:
				Fre = Fre/1.059463094;
			break;
			case -2:
				Fre = Fre/1.122462048;
			break;
			case -3:
				Fre = Fre/1.189207115;
			break;
			case -4:
				Fre = Fre/1.25992105;
			break;
			case -5:
				Fre = Fre/1.334839854;
			break;
			case -6:
				Fre = Fre/1.414213562;
			break;
		}
		
		return Fre;
		
		
	}
	*/

	dup_pos loop(dup_pos A, dup_pos Min, dup_pos Max)
	{
		dup_pos Value, B, C;
		Value = A-Min;
		Max = Max-Min;
		if (Value/Max >= 0.0) {
			B = int(Value/Max);
		}
		else {
			C = int((-1.0)*Value/Max);
			B = int(Value/Max+C+3.0)-(C+3.0);
		}
		Value = Value-B*Max;

		return Value+Min;
	}

	dup_pos loop(dup_pos A)
	{
		dup_pos Value, B, C;
		Value = A;
		if (Value/1.0 >= 0.0) {
			B = int(Value);
		}
		else {
			C = int((-1.0)*Value);
			B = int(Value+C+3.0)-(C+3.0);
		}
		
		return Value-B;
	}

};


#endif