#ifndef _CYMBAL1_
#define _CYMBAL1_

#include "Standards.h"
#include "Curves.h"

class Cymbal1 : public StandardInstrument
{
protected:

public:
	double RPos[10][12];
	double RFre[10][12];
	double Pos;

	// Modifications
	int nMultiple;
	int nMixed;
	double Length;
	double Decay;
	
	bool ignoreNoteOff;
	
	



	Cymbal1() : StandardInstrument()
	{
		nMultiple = 6;
		nMixed = 6;
		Length = 1.5;
		Decay = 0.2;


		ignoreNoteOff = false;

		//TimePos = 2.0;
		for(int i=0;i<nMultiple;i++)
		{
			for(int j=0;j<nMixed;j++)
			{
				RPos[i][j] = rand()/15634.0;
				RFre[i][j] = rand()*rand()/(32768.0*32768.0)+0.2;
			}
		}
		Pos = 0;
	}


	bool Run()
	{
		TimePos = TimePos+TimeStep;
		//Pos = Pos+TimeStep*Fre;
		Pos = Pos+TimeStep*Fre*rand()/32768.0;
		
		//double E1;
		
		if (ignoreNoteOff)
		{
			if (TimePos < Length)
			{
				Playing = true;
			}
			else
			{
				Playing = false;
			}
		}
		


		Amp = 0;
		for(int j=0;j<nMixed;j++)
		{
		
			double E1 = 1;
			for(int i=0;i<nMultiple;i++)
			{
				
			
			
				RPos[i][j] = RPos[i][j]+TimeStep*Fre*RFre[i][j];
				//RPos[i] = RPos[i]+TimeStep*Fre;
				E1 = E1 * Sqr(RPos[i][j])/1.0;
			}
			
			//cout << E1 << endl;
			Amp = Amp + E1/5.0;
			
			
		}
		//cout << "amp " << Amp; 
		Amp = Rise2(Amp*4.0);
		//cout << "  after: " << Amp << endl; 
		//Amp = Sqr(RPos[1]);
		
		Amp = Amp*Fall2S(TimePos*Length,1.0/Decay);

	
		
		
		
		
		return Playing;

	
	
	
	}

};


#endif