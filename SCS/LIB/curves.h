#include "Base.h"

#ifndef _CURVES_
#define _CURVES_

double Rise1(double Pos);
double Rise2(double Pos);
double Fall1(double Pos);
double Top(double Pos);
double Dep(double Pos);
double Hill1(double Pos);
double Vall1(double Pos);
double SS(double Cov,double Length,double Begin,double End);
double Bounce(double Pos, double Min, double Max);
double Line(double Pos, double PosStart, double PosEnd, double AmpStart, double AmpEnd);

double Sqr(double Pos);
double SqrSaw(double Pos, double FlatLength, double Rise1Length , double Fall1Length);
double Saw(double Pos);
double Saw2(double Pos);
double Tri(double Pos);
double TriSaw(double Pos,double Mod1);
double TriSqr(double Pos, double Mod);
double Sine(double Pos);
double Rec(double Pos, double Mod);
double BSine(double Pos, double Mod);
//double SineSqr(double Pos);


/**
 * a=1, b=0, c=1	->	"saw"
 * a=0, b=1, c=0	->	"sqr"
 * a=0, b=0, c=1	->	"tri"
 * a=0, b=0, c=0	->	"sine"
 */
double TriSawSineSqr(double Pos, double a, double b, double c);


// ____________________________________________________________________________________________________________________
// ____________________________________________Basic Modification Functions____________________________________________
// ____________________________________________________________________________________________________________________

inline double ExactSin(double Pos)
{
	return sin(Pos*PI*2.0);
}

inline double rSin(double Pos)
{

#ifdef FAST_SINE

	static int TablePos;
	TablePos = (int)(SineTable.Size*Loop(Pos) + 0.5);

	return *(SineTable.Table + TablePos);
#endif

	return ExactSin(Pos);
}



inline double FreSin(double Pos)
{
	double Tmp = rSin(Loop(Pos));
	if (Tmp < 0.0) return 1.0-Tmp/(-2.0);
	else return 1.0+Tmp;
}

inline double Inv(double Value)
{
	return Value*(-1.0);
}



inline double Rise1(double Pos)
{
	if (Pos < 0) return 0;
	if (Pos > 1) return 1;
	else return Pos;
}

inline double Rise2(double Pos)
{
	if (Pos > 0) return (-1)/(Pos+1)+1;
	else return ((-1)/(1-Pos)+1)*(-1);		
}

inline double Rise2A(double Pos)
{
	if (Pos > 0) return (-1)/(Pos+1)+1;
	else return 0;		
}

inline double Rise2S(double Pos, double Mod)
{
	if (Pos < 0) return 0;
	if (Pos > 1) return 1;
	else return (-1)/(Pos*Mod+1) + 1 + Pos/(Mod+1);
}

inline double Rise3(double Pos)
{
	if (Pos < 0) return 0;
	else if (Pos > 1) return 1;
	else return rSin(Pos/2-0.25)/2+0.5;
}

inline double Rise4(double Pos)
{
	if (Pos < -1) return -1;
	else if (Pos > 1) return 1;
	else return rSin(Pos/4.0);
}

inline double Rise4A(double Pos)
{
	if (Pos < 0) return 0;
	else if (Pos > 1) return 1;
	else return rSin(Pos/4.0);
}

inline double Fall1(double Pos) {
	double Amp = Limit(Pos*(-1)+1.0, 0.0, 1.0);
return Amp;
}

inline double Fall2(double Pos)
{
	if (Pos < 0) return 1;
	else return 1/(Pos+1);
}

inline double Fall2S(double Pos, double Mod)
{
	//cout << Pos << "   ";
	if (Pos < 0) return 1;
	if (Pos > 1) return 0;
	else return 1/(Pos*Mod+1)-Pos/(Mod+1);
}

inline double Fall3(double Pos)
{
	if (Pos < 0) return 1;
	else if (Pos > 1) return 0;
	else return rSin(Pos/2-0.25)/(-2)+0.5;
}

inline double Fall5(double Pos)
{
	return Rise2S(1-Pos, 4.0);
}

inline double Fall5S(double Pos, double Mod)
{
	return Rise2S(1-Pos, Mod);
}

double Top(double Pos) {
	double Amp;
	//Pos = Loop(Pos,0,1);
	if (Inside(Pos))
	{
		if (Pos < 0.5) Amp = Pos*2;
		else Amp = Pos*(-2.0)+2.0;
	}
	else Amp = 0;
	return Amp;
}

double Dep(double Pos) {
	double Amp;
	//Pos = Loop(Pos,0,1);
	if (Inside(Pos))
	{
		if (Pos < 0.5) Amp = Pos*(-2.0)+1;
		else Amp = Pos*2.0-1.0;
	}
	else Amp = 1;
	return Amp;
}

inline double Hill1(double Pos)
{
	if (Inside(Pos)) return sin(Pi(Pos+0.25)*2)/(-2)+0.5;
	else return 0;
}

inline double Vall1(double Pos) {
	//	Pos = Loop(Pos,0,1);
	if (Inside(Pos)) return sin(Pi(Pos+0.25)*2)/2+0.5;
	else return 1;
}

inline double Hill2(double Pos) {
	if (Inside(Pos)) return rSin(Pos/2);
	else return 0;
}

inline double Vall2(double Pos) {
	//	Pos = Loop(Pos,0,1);
	if (Inside(Pos)) return rSin(Pos/2+0.5)+1;
	else return 1;
}

double Shot1(double Pos)
{
	if (Pos < 0) return 0;
	else return Hill2(Rise2(Pos));
}

double Shot2(double Pos) {
	Pos = Pos/1.9;
	Pos = Rise2S(Pos, 5.0)*0.3 + Pos*0.7;
	return Hill1(Rise2S(Pos, 50.0)*0.3 + Pos*0.7);
}



double Steps(double Val, double Steps)
{
	return floor(Val*Steps+0.5)/floor(Steps+0.5);
}

inline double Reverse(double Pos, double Start) {
	
	return Start-Pos;
}

// ____________________________________________________________________________________________________________________
// _________________________________________Advanced Modification Functions____________________________________________
// ____________________________________________________________________________________________________________________



double SS(double Cov,double Length,double BeginLength,double EndLength)
{
	double Amp = 1;
	Cov = Loop(Cov,0,Length);
	if (Cov < BeginLength) Amp = Hill1(Cov*0.5/BeginLength);
	if (Length-Cov < EndLength) Amp = Hill1((Length-Cov)*0.5/EndLength);

	return Amp;
}



double Bounce(double Pos, double Min, double Max)
{
	// fix displacement
	Pos = Pos-Min;
	Max = Max-Min;

	if (((int)(Pos/Max))%2 == 0)
	{
		return Min + Loop(Pos,0,Max);
	}
	else
	{
		return Min + Max - Loop(Pos,0,Max);
	}
}

double Line(double Pos, double PosStart, double PosEnd, double AmpStart, double AmpEnd)
{
	double IPos = Pos - PosStart;
	double dy = AmpEnd-AmpStart;
	double dx = PosEnd-PosStart;

	double k = ((dx == 0.0) ? 0.0 : (dy / dx));

	return AmpStart + IPos * k;
}

double Merge(double Pos, double Value1, double Value2, double MergeStart, double MergeLength, double (*MergeFunc)(double))
{
	if (Pos < MergeStart) return Value1;
	if (Pos > MergeStart+MergeLength) return Value2;
	double MergePos = (Pos-MergeStart)/MergeLength;
	return MergeFunc(MergePos)*Value2 + (MergeFunc(MergePos)*(-1)+1)*Value1;
}

double Merge(double Pos, double Value1, double Value2, double MergeStart, double MergeLength)
{
	return Merge(Pos, Value1, Value2, MergeStart, MergeLength, Rise1);
}

double Merge(double Pos, double Value1, double Value2, double (*MergeFunc)(double))
{
	return Merge(Pos, Value1, Value2, 0, 1, MergeFunc);
}

double Merge(double Pos, double Value1, double Value2)
{
	return Merge(Pos, Value1, Value2, 0, 1, Rise1);
}

double Vobl1(double Pos)
{
	return Rise2(Tri(Pos*52.56)*TriSaw(Pos*150,-0.8)*Tri(Pos*137.8)*Sine(Pos*95.43)*Sine(Pos*113.6784)*Tri(Pos*116.9)*8)*0.5+0.5;
}

double Soft(double Pos)
{
	Pos = Loop(Pos,0,1);
	double Amp = sin(Pi(Pos-0.5)*2);
	return Amp;
}

double Vobl2(double Pos)
{	
	double CPos = Pos;
	return 1.0 + 5.0*Soft(Rise2(Soft(CPos*52.56)*Soft(CPos*150)*Soft(CPos*137.8)*Soft(CPos*95.43)*Soft(CPos*113.6784)*Saw2(CPos*116.9)*8));
}

inline double Crossfade(double A, double B, double Mod)
{
	return A*(Mod+1)/2+B*(Mod-1)/(-2);
}

double SweepSteps1(double Val, double Steps)
{
	double FSteps = floor(Steps);
	double CSteps = ceil(Steps);
	double V = Steps-floor(Steps);

	return floor(Val*FSteps+0.5)*(1.0-V)/FSteps + floor(Val*CSteps+0.5)*V/CSteps;

}

// not working properly
double SweepSteps2(double Pos, double Val, double Steps)
{
	double FSteps = floor(Steps);
	double CSteps = ceil(Steps);
	double V = Steps-FSteps;

	return Crossfade(floor(Val*FSteps+0.5)*(1.0-V)/FSteps, floor(Val*CSteps+0.5)*V/CSteps, Sine(Pos)); 

}

// Gradually goes from a linear slope from 0.0 to 1.0 to a vertical slope from 0.0 to 1.0 at Pos = 0.5
double SoftPrecipice(double Pos, double Mod)
{
	Pos = Loop(Pos);
	bool Section2 = Pos > 0.5;

	// Fix so that Pos is between 
	if (Section2)
	{
		Pos = (Pos - 0.5) * 2.0;
	}
	else 
	{		
		Pos = 1.0 - Pos * 2.0;
	}

	double Value = Fall2S(Pos, Mod) * 0.5;

	if (Section2) Value = 1.0 - Value;	

	return Value;
}


// ____________________________________________________________________________________________________________________
// _______________________________________________Basic Sound Functions________________________________________________
// ____________________________________________________________________________________________________________________


inline double Sqr(double Pos)
{
	double Amp;
	Pos = Loop(Pos,0,1);
	if (Pos < 0.5) Amp = 1;
	else Amp = -1;
return Amp;
}

#define BSqr Rec;
#define Pulse Rec;

inline double Rec(double Pos, double Mod) // Mod 0.0 to 1.0
{
	Pos = Loop(Pos,0,1);

	if (Pos > Mod) return 0;
	if (Pos < Mod/2) return 1;
	return -1;
}

inline double Rec2(double Pos, double Mod)
{
	Pos = Loop(Pos);
	Mod = Mod/2;
	if (Pos < Mod) return 1;
	if (Pos < 0.5) return 0;
	if (Pos < 0.5+Mod) return -1;
	return 0;
}

inline double Saw(double Pos)
{	
	Pos = Loop(Pos,0,1);
	double Amp = Pos*2-1;
	return Amp;
}

double Saw2(double Pos) {
	double Amp;
	Pos = Loop(Pos,0,1);
	if (Pos > 0.75) Amp = (Pos-0.75)*4-1;
	else Amp = (Pos-0.25)*(-4)+1;
	if (Pos < 0.25) Amp = Pos*4.0;
	return Amp;
}

double BSaw(double Pos, double B)
{
	Pos = Loop(Pos);
	if (Pos > B) return 0;
	else Pos = Pos/B;

	double Amp = Pos*2-1;
	return Amp;
}


inline double Tri(double Pos)
{
	double Amp;
	Pos = Loop(Pos,0,1);
	if (Pos > 0.75) Amp = (Pos-0.75)*4-1;
	else Amp = (Pos-0.25)*(-4)+1;
	if (Pos < 0.25) Amp = Pos*4.0;
	return Amp;
}


inline double BTri(double Pos, double B)
{
	Pos = Loop(Pos);
	if (Pos > B) return 0;
	else Pos = Pos/B;
	double Amp;
	
	if (Pos > 0.75) Amp = (Pos-0.75)*4-1;
	else Amp = (Pos-0.25)*(-4)+1;
	if (Pos < 0.25) Amp = Pos*4.0;
	return Amp;
}

/*
double DynSaw(double Pos,double Mod1) {
	double Amp;
	Pos = Loop(Pos,0,1);
	Mod1 = Limit(Mod1,0.0001,0.9999);
	if (Pos > 0.5+Mod1/2) Amp = (Pos-(0.5+Mod1/2))/(0.5-Mod1/2)-1;
	else Amp = (Pos-(0.5-Mod1/2))*(-2/Mod1)+1;
	if (Pos < 0.5-Mod1/2) Amp = Pos/(0.5-Mod1/2.0);
return Amp;
}
*/

#define SawTri TriSaw;

inline double TriSaw(double Pos,double Mod1)
{
	double Amp;
	Pos = Loop(Pos,0,1);
	Mod1 = Limit(Mod1,-0.9999,0.9999)/2.0+0.5;
	if (Pos > 0.5+Mod1/2) Amp = (Pos-(0.5+Mod1/2))/(0.5-Mod1/2)-1;
	else Amp = (Pos-(0.5-Mod1/2))*(-2/Mod1)+1;
	if (Pos < 0.5-Mod1/2) Amp = Pos/(0.5-Mod1/2.0);
	return Amp;
}


#define BSawTri BTriSaw;

double BTriSaw(double Pos,double Mod1, double B)
{	
	Pos = Loop(Pos);
	if (Pos > B) return 0;
	else Pos = Pos/B;
	double Amp;
	Mod1 = Limit(Mod1,-1.0,1.0)/2.0+0.5;
	if (Pos > 0.5+Mod1/2) Amp = (Pos-(0.5+Mod1/2))/(0.5-Mod1/2)-1;
	else Amp = (Pos-(0.5-Mod1/2))*(-2/Mod1)+1;
	if (Pos < 0.5-Mod1/2) Amp = Pos/(0.5-Mod1/2.0);
	return Amp;
}

#define SqrTri TriSqr;	

double TriSqr(double Pos, double Mod)
{
	Pos = Loop(Pos,0.0,1.0);
	Mod = Limit(Mod,0.0,0.9999);
	double CMod = Mod/4;
	if (Pos <= CMod) return -1;
	if (Pos < 0.5-CMod) return Limit(Pos/(0.25-CMod)-(Mod/(1-Mod)+1),-1.0,1.0);
	if (Pos <= 0.5+CMod) return 1;
	if (Pos < 1-CMod) return Limit(1+(2+Mod)/(1-Mod)-Pos/(0.25-CMod),-1.0,1.0);
	return -1;
}

double BTriSqr(double Pos, double Mod, double B)
{
	Pos = Loop(Pos,0.0,1.0);
	if (Pos > B) Pos = 0.0;
	else Pos = Pos / B;
	return TriSqr(Pos, Mod);
}

inline double Sine(double Pos)
{
	Pos = Loop(Pos,0,1);
	//double Amp = sin(Pi(Pos-0.5)*2);
	
	return -rSin(Pos-0.5);
}


inline double NegSine(double Pos)
{
	Pos = Loop(Pos,0,1);
	return rSin(Pos-0.5);
}


inline double BSine(double Pos, double Mod) // Mod 0.0 to 1.0
{
	
	if (Pos < Mod)
	{
		return rSin(Pos/Mod);
	}
	else if (Pos <= 1.0)
	{
		return 0;
	}
	else
	{
		Pos = Loop(Pos);
		return BSine(Pos,Mod);
	}	
}

#define SineSqr SqrSine

double SqrSine(double Pos, double Mod)
{
	Pos = Loop(Pos);
	double OMod = 1.0-Mod;

	if (Mod <= 0.0) return NegSine(Pos);
	if (Mod >= 1.0) return Sqr(Pos);

	// first rise
	double Line = OMod/4.0;
	if (Pos < Line) return NegSine(Pos/OMod);
	
	// top
	Pos = Pos-Line;
	Line = Mod/2.0;
	if (Pos < Line) return -1.0;
	
	// fall
	Pos = Pos-Line;
	Line = OMod/2.0;
	if (Pos < Line) return NegSine(Pos/OMod+0.25);
	
	// bottom
	Pos = Pos-Line;
	Line = Mod/2.0;
	if (Pos < Line) return 1.0;

	// rise 2
	Pos = Pos-Line;
	return NegSine(Pos/OMod+0.75);
}


inline double SoftSqr(double Pos)
{
	Pos = Loop(Pos,0,1);
	double Amp = NegSine(Pos+Tri(Pos*2)/8);
	return Amp;
}

#define SawSqr SqrSaw;

// Mod = 0.0 is Square, 1.0 is Saw
inline double SqrSaw(double Pos, double Mod)
{
	if (Mod == 0) return Sqr(Pos);
	Pos = Loop(Pos);
	if (Pos <= 0.5-Mod/2.0) return 1.0;
	Pos = Pos - (0.5 - Mod/2.0);
	if (Pos >= Mod) return -1.0;
	
	return 1.0-(Pos*2/Mod);
}

#define SawSqr2 SqrSaw2;

// Mod = 0.0 is Square, 1.0 is Saw
inline double SqrSaw2(double Pos, double Mod)
{
	Pos = Loop(Pos);
	if (Mod <= 0.0) return Sqr(Pos);
	if (Mod >= 1.0) return Saw(Pos);
	if (Pos < 0.5) return 1.0 + Pos*Mod*2.0 - Mod;
	return (Pos - 0.5)*Mod*2.0 - 1.0;
}

#define SineSaw SawSine;

inline double SawSine(double Pos, double Mod)
{
	Pos = Loop(Pos);
	double Point1 = 0.5-Mod/2.0;
	if (Pos < Point1) return Sine(Pos*0.25/Point1);
	double Point2 = 0.5+Mod/2.0;
	if (Pos < Point2) return Sine(0.5*(Pos-Point1)/Mod+0.25);
	return Sine(0.25*(Pos-Point2)/Point1+0.75);	
}


#define TriSawSqr SqrTriSaw;
#define TriSqrSaw SqrTriSaw;
#define SawTriSqr SqrTriSaw;
#define SawSqrTri SqrTriSaw;
#define SqrSawTri SqrTriSaw;

/*
  Mod1 = -1 : saw
  Mod1 = 0 : Tri
  Mod1 = 1 : saw (other side)

  Mod2 = 0 : No square (completly following Mod1)
  Mod2 = 1 : Clean square (no matter what Mod1 is)
*/
double SqrTriSaw(double Pos, double Mod1, double Mod2)
{
	Pos = Loop(Pos);
	double Amp = 1.0;
	if (Pos > 0.5) 
	{
		Pos = 1.0-Pos;
		Amp = -1.0;
	}
	Pos = Pos*2;

	double ST = Mod1*0.5 + 0.5;
	double P1 = ST-ST*Mod2;
	double P2 = ST+(1.0-ST)*Mod2;

	if (Pos < P1) return Amp*Pos/P1;
	if (Pos < P2) return Amp*1.0;
	return Amp*(1.0-(Pos-P2)/(1.0-P2));

}



// ____________________________________________________________________________________________________________________
// ______________________________________________Advanced Sound Functions______________________________________________
// ____________________________________________________________________________________________________________________



double MultiCurve(double Pos, double TFirst, double EFirst, double T2, double E2, double TLast, double ELast)
{
	Pos = Loop(Pos, 0, 1);
	if (Pos < TFirst)
	{
		double T0 = TLast-1;
		return (EFirst-ELast)*(Pos-T0)/(TFirst-T0)+ELast;
	}
	if (Pos < T2)
	{
		return (E2-EFirst)*(Pos-TFirst)/(T2-TFirst)+EFirst;
	}
	if (Pos < TLast)
	{
		return (ELast-E2)*(Pos-T2)/(TLast-T2)+E2;
	}
	else
	{
		double TL = TFirst+1;
		return (EFirst-ELast)*(Pos-TLast)/(TL-TLast)+ELast;
	}
}





double SqrSawAdv(double Pos, double FlatLength, double Rise1Length , double Fall1Length)
{
	//double Amp;
	double Place;
	Pos = Loop(Pos);
	
	Place = FlatLength;
	if (Pos < Place) return -1;
	Place = Place+Rise1Length;
	if (Pos < Place) return Rise1((Pos-FlatLength)/Rise1Length)*2-1;
	if (Pos < 1-Fall1Length) return 1;
	return Fall1((Pos+Fall1Length-1)/Fall1Length)*2-1;

}


double TriSawSineSqr(double Pos, double a, double b, double c)
{	
	Pos = Loop(Pos);
	if (a < 0.0) a = 0.0;
	if (b < 0.0) b = 0.0;
	if (c < 0.0) c = 0.0;

	double TPoint;

	// introduce mirroring at 0.5 (then only work with [0, 0.5])
	bool Mirror = (Pos > 0.5);
	if (Mirror) Pos = 1.0-Pos;	

	// apply sawtooth properties "a"
	TPoint = (1.0 - a) / 4.0;
	if (Pos < TPoint) Pos = Line(Pos, 0.0, TPoint, 0.0, 0.25);
	else Pos = Line(Pos, TPoint, 0.5, 0.25, 0.5);

	// apply square properties "b"
	TPoint = (1.0 - b) / 4.0;
	if (Pos < TPoint) Pos = Line(Pos, 0.0, TPoint, 0.0, 0.25);
	else if (Pos < (0.5-TPoint)) Pos = 0.25;
	else Pos = Line(Pos, (0.5-TPoint), 0.5, 0.25, 0.5);

	// mix trianlge with sine "c"
	double Amp = (1.0-c) * Sine(Pos) + c * Tri(Pos);

	// return mirrored amp if mirroring
	return Mirror ? (-Amp) : Amp;

	//return Pos;
}

double BTriSawSineSqr(double Pos, double a, double b, double c, double thin)
{
	if (Pos < thin) return TriSawSineSqr(Pos/thin, a, b, c);
	else if (Pos <= 1.0) return 0;
	else
	{
		Pos = Loop(Pos);
		return BTriSawSineSqr(Pos, a, b, c, thin);
	}
}

#endif