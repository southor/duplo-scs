#ifndef _ADDITIVE2_
#define _ADDITIVE2_

#include "Standards.h"
#include "Curves.h"
#include "Image.h"
#include "limits.h"

class Additive : public Instrument
{
public:
	
	// Functional
	double TimePos;
	double Pos;
	double ReleaseStart;

	// Image
	Image *Harmonic;

	// Modules		
	EnvelopeT *Env1;

	// Output
	double Amp1, Pan1;
	double Amp2, Pan2;
	double Amp3, Pan3;

	// Modifications
	double Vol;
	double HighMute;
	double Resolution;
	bool ConstantSpeed;
	//double Release;

	Additive(char *filename) : Instrument()
	{
		Harmonic = new Image(filename);
		Env1 = new EnvelopeT(0.0, 0.0, 0.5, 0.0, 0.01);
		

		Pos = 0;
		
		Amp1 = 0; Pan1 = -1;
		Amp2 = 0; Pan2 = 0;
		Amp3 = 0; Pan3 = 1;

		Resolution = 100;
		HighMute = 1;
		ConstantSpeed = true;

		Release = 0.03;
		ReleaseStart = 1000000000.0;
		Vol = 1.0;
	}

	void ChangeImage(char *filename)
	{
		delete Harmonic;
		Harmonic = new Image(filename);
	}

	void NoteOn()
	{
		TimePos = 0;
		Pos = 0;
		Playing = true;
		ReleaseStart = 1000000000.0;
	}

	bool Run()
	{
		TimePos = TimePos+TimeStep;
		//PlayPos = PlayPos+PlayStep;
		//double Env = Shot(PlayPos*30);
		//double Env = 1.0;
		Pos = Pos+TimeStep*Fre;	
		
		//Mod = Fall2(TimePos*8)*100+2;
		//Amp = Sine(Pos);

		//Amp = Sine(TriSaw(Pos,Sine(PlayPos*4)*0.1))*Sine(TriSqr(Pos*1,Sine(PlayPos*3)*0.1+0.2))*Sine(Pos+Sine(PlayPos*5)*0.15);
		//Amp = Tri((Tri(Pos*2)*Tri(Pos*7)*Tri(Pos*3)))+Tri(Pos*5)*Tri(Pos*11)*Tri(Pos*12);
		Amp1 = 0;
		Amp2 = 0;
		Amp3 = 0;

		if (Env1->Playing == true)
		{			
			Playing = Env1->Run();
		
			int x, y;
			double Amp;
			double Wave;
			double P = TimePos * Resolution / (double)Harmonic->Width();

			if (ConstantSpeed)
			{
					
				if (P > 2.0/3.0)
				{
					x = (Rise2(P-2.0/3.0)/3.0+2.0/3.0)*(double)Harmonic->Width();
				}
				else
				{
					x =  P * (double)Harmonic->Width();
				}
			}
			else
			{
				x = Rise2(P)*(double)Harmonic->Width();
			}


			
			for (int i=1;i<=Harmonic->Height();i++)
			{
				//cout << Harmonic->getValue(Rise2(TimePos*Harmonic->Width()),i) << endl;
				
				//Rise2(TimePos)*(double)Harmonic->Width()
							
				y = i-1;
				Amp = 0.15/((double)i*HighMute+(1-HighMute));
				Wave = rSin(Pos*i);
				
				Amp1 = Amp1 + Wave*Harmonic->getValue1(x,y)*Amp;
				Amp2 = Amp2 + Wave*Harmonic->getValue2(x,y)*Amp;
				Amp3 = Amp3 + Wave*Harmonic->getValue3(x,y)*Amp;
				//if (i% == 1) 
				//Amp = Amp + Fall2(i)*0.3*rSin(Pos*i)*(i%3);
			}	
		}


		
			
		//Amp = Fall2(Loop(Pos));


		
		
		return Playing;
	}

	void NoteOff()
	{
		Env1->NoteOff();
		Playing = true;
	}

	void End()
	{
	}

};

#endif