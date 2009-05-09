#ifndef _Echo3D_
#define _Echo3D_

#include "AdvancedEcho.h"

struct Pos3D
{
	double X;
	double Y;
	double Z;
};

class Echo3D : public AdvancedEcho
{

public:


	Pos3D *InPos;
	
	Pos3D *ReflectPos;

	Pos3D *OutPos;


	Echo3D(int nInput, int nReflect, int nOutput, double MaxDist, double MaxLength) : AdvancedEcho(nInput, nReflect, nOutput, MaxDist, MaxLength)
	{

		InPos = new Pos3D[nInput];
		for (int i = 0;i<nInput;i++)
		{
			(InPos+i)->X = 0;
			(InPos+i)->Y = 0;
			(InPos+i)->Z = 0;
		}

		ReflectPos = new Pos3D[nReflect];
		for (int i = 0;i<nReflect;i++)
		{		
			(ReflectPos+i)->X = 0;
			(ReflectPos+i)->Y = 0;
			(ReflectPos+i)->Z = 0;;
		}

		OutPos = new Pos3D[nOutput];
		for (int i = 0;i<nOutput;i++)
		{
			(OutPos+i)->X = 0;
			(OutPos+i)->Y = 0;
			(OutPos+i)->Z = 0;;
		}
	}

	void SetInPos(int n, Pos3D Pos)
	{
		(InPos+n)->X = Pos.X;
		(InPos+n)->Y = Pos.Y;
		(InPos+n)->Z = Pos.Z;

		double XD;
		double YD;
		double ZD;

		for(int i=0;i<nReflect;i++)
		{
			XD = (ReflectPos+i)->X - (InPos+n)->X;
			YD = (ReflectPos+i)->Y - (InPos+n)->Y;
			ZD = (ReflectPos+i)->Z - (InPos+n)->Z;
			*(InputDistanceMap+n*nInput+i) = sqrt(XD*XD+YD*YD+ZD*ZD);	
		}
		for(int i=0;i<nOutput;i++)
		{
			XD = (OutPos+i)->X - (InPos+n)->X;
			YD = (OutPos+i)->Y - (InPos+n)->Y;
			ZD = (OutPos+i)->Z - (InPos+n)->Z;			
			*(InputDistanceMap+n*nInput+nReflect+i) = sqrt(XD*XD+YD*YD+ZD*ZD);
		}
		// haveto update distances too
	}

	void SetReflectPos(int n, Pos3D Pos)
	{
		(ReflectPos+n)->X = Pos.X;
		(ReflectPos+n)->Y = Pos.Y;
		(ReflectPos+n)->Z = Pos.Z;
	
		double XD;
		double YD;
		double ZD;

		for(int i=0;i<nInput;i++)
		{	
			XD = (ReflectPos+n)->X - (InPos+i)->X;
			YD = (ReflectPos+n)->Y - (InPos+i)->Y;
			ZD = (ReflectPos+n)->Z - (InPos+i)->Z;
			*(InputDistanceMap+i*nRecieving+n) = sqrt(XD*XD+YD*YD+ZD*ZD);
		}
		for(int i=0;i<nReflect;i++)
		{
			XD = (ReflectPos+n)->X - (ReflectPos+i)->X;
			YD = (ReflectPos+n)->Y - (ReflectPos+i)->Y;
			ZD = (ReflectPos+n)->Z - (ReflectPos+i)->Z;
			*(ReflectDistanceMap+(nInput+i)*nRecieving+n) = sqrt(XD*XD+YD*YD+ZD*ZD);
		}

		//----------
		
		for(int i=0;i<nReflect;i++)
		{
			XD = (ReflectPos+i)->X - (ReflectPos+n)->X;
			YD = (ReflectPos+i)->Y - (ReflectPos+n)->Y;
			ZD = (ReflectPos+i)->Z - (ReflectPos+n)->Z;
			*(ReflectDistanceMap+n*nRecieving+i) = sqrt(XD*XD+YD*YD+ZD*ZD);	
		}
		for(int i=0;i<nOutput;i++)
		{
			XD = (OutPos+i)->X - (ReflectPos+n)->X;
			YD = (OutPos+i)->Y - (ReflectPos+n)->Y;
			ZD = (OutPos+i)->Z - (ReflectPos+n)->Z;			
			*(ReflectDistanceMap+n*nRecieving+nReflect+i) = sqrt(XD*XD+YD*YD+ZD*ZD);
		}
		// haveto update distances too
	}

	void SetOutPos(int n, Pos3D Pos)
	{
		
		(OutPos+n)->X = Pos.X;
		(OutPos+n)->Y = Pos.Y;
		(OutPos+n)->Z = Pos.Z;
	
		double XD;
		double YD;
		double ZD;

		for(int i=0;i<nInput;i++)
		{	
			XD = (OutPos+n)->X - (InPos+i)->X;
			YD = (OutPos+n)->Y - (InPos+i)->Y;
			ZD = (OutPos+n)->Z - (InPos+i)->Z;
			*(InputDistanceMap+i*nRecieving+nReflect+n) = sqrt(XD*XD+YD*YD+ZD*ZD);
		}
		for(int i=0;i<nReflect;i++)
		{
			XD = (OutPos+n)->X - (ReflectPos+i)->X;
			YD = (OutPos+n)->Y - (ReflectPos+i)->Y;
			ZD = (OutPos+n)->Z - (ReflectPos+i)->Z;
			*(ReflectDistanceMap+(nInput+i)*nRecieving+nReflect+n) = sqrt(XD*XD+YD*YD+ZD*ZD);
		}

		// haveto update distances too
	}
};

#endif