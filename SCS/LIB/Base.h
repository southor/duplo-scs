//#include <iostream.h>

#ifndef _BASE_
#define _BASE_

double Frequency(double Fre,int Oct,int Tone);
double Min(double value,double min);
double Man(double value,double max);
double Limit(double value,double min,double max);
int Limit(int value,int  min,int max);
inline bool Between(int value, int lim1, int lim2);
double Inside(double value,double min,double max);
double Inside(double value);
double Loop(double A,double Min,double Max);
double Loop(double A); // Min = 0, Max = 1
double PosFunc(double Tempo,double Cov,double Fre);
double TimeFunc(double Cov,double Tempo);
double WaveFunc(double Cov,double Tempo,int WaveFre);
double CovFunc(double Time,double Tempo);
double LVolFunc(double Vol, double Pan);
double RVolFunc(double Vol, double Pan);

// char* functions
//int stringlength(char* String);
//char* unitestring(char* String1, char* String2);



// ***********************************************************
// ********************  GLOBAL VARIABLES  *******************
// ***********************************************************


double FoundTone;
double PlayStep;
double TimeStep;
double const TIMESTEP44 = 0.000022675736961451247165532879818594;
double Tempo;

//short int* OutL;
//short int* OutR;

//double const TEMPOCONST = 2.398;
short int const PAUSE = 30000;

double const PI = 3.141592654;

double GlobalVol;

struct SineTableStruct
{
	int Size;
	double *Table;
} SineTable;




// *********************************************************
// ********************  FUNCTIONS  ************************
// *********************************************************


// ________________INIT_FUNCTIONS______________

void SetSineTable(unsigned int Size, double (*rSinFunc)(double))
{
	static bool FirstTime = true;
	SineTable.Size = Size;
	if (!FirstTime)
	{
		delete SineTable.Table;
		FirstTime = false;
	}
	SineTable.Table = new double[Size];

	for(int i=0;i<Size;++i)
	{
		*(SineTable.Table+i) = rSinFunc((double)i/(double)Size);
	}
}

// ________________CLEANUP_FUNCTIONS______________

// Don't call if not called SetSineTable
void cleanupSineTable()
{
	delete[] SineTable.Table;
}


// _________________Basic Tools Functions_________________

double Frequency(double Fre,int Oct,int Tone)
{

	if (Tone != PAUSE)
	{
		while(Tone <= -7 || Tone >= 7)
		{
			if (Tone > 6) {
				Oct = Oct+1;
				Tone = Tone-12;
			}
			if (Tone < -6) {
				Oct = Oct-1;
				Tone = Tone+12;
			}
		}
	
  
		// Oct = Oct+int((double)(Tone+5)/12.0);
		// Tone = Tone-int((double)Tone/7.0)*12;



		Fre = Fre * pow(2.0,(double)Oct);

		switch(Tone)
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
	else return 0;
	
}

// Used by frequency modulation. note/tone is a double instead of an int
double ContinousFrequency(double fre, int oct, double note)
{
	return fre*pow(2.0, note/12.0 + oct);
}

inline double Pi(double value) {
	return value*PI;
}

inline double Min(double value,double min)
{
	if (value < min) value = min;
	return value;
}
inline double Max(double value,double max)
{
	if (value > max) value = max;
	return value;
}
inline double Limit(double value,double min,double max)
{
	if (value > max) value = max;
	if (value < min) value = min;
return value;
}

inline int Limit(int value,int  min,int max)
{
	if (value > max) value = max;
	if (value < min) value = min;
return value;
}

inline bool Between(int value, int lim1, int lim2)
{
	return ((value >= lim1 || value >= lim2) && (value <= lim1 || value <= lim2));
}


inline double Inside(double value,double min,double max) {
	if (value > max) return false;
	if (value < min) return false;
	return true;
}
inline double Inside(double value) {
	if (value > 1) return false;
	if (value < 0) return false;
	return true;
}


inline double Loop(double A,double Min,double Max)
{
	double Value, B, C;
	Value = A-Min;
	Max = Max-Min;
	if (Value/Max >= 0) {
		B = int(Value/Max);
	}
	else {
		C = int((-1)*Value/Max);
		B = int(Value/Max+C+3)-(C+3);
	}
	Value = Value-B*Max;
return Value+Min;
}

inline double Loop(double A) {
	double Min = 0;
	double Max = 1;
	double Value, B, C;
	Value = A-Min;
	Max = Max-Min;
	if (Value/Max >= 0) {
		B = int(Value/Max);
	}
	else {
		C = int((-1)*Value/Max);
		B = int(Value/Max+C+3)-(C+3);
	}
	Value = Value-B*Max;
return Value+Min;
}

inline double PosFunc(double Tempo,double Cov,double Fre)
{
	double Sec = TimeFunc(Cov,Tempo);
	double Pos = 0;
	if (Fre != 0) { Pos = (Sec - int(Sec*Fre)/Fre)/(1/Fre); }
return Pos;
}

inline double TimeFunc(double Cov,double Tempo) {
	double Time = Cov*4*60/Tempo;
return Time;
}

inline double WaveFunc(double Cov,double Tempo,int WaveFre) {
	double Time = Cov*(double)WaveFre*4*60/Tempo;
return Time;
}


inline double CovFunc(double Time,double Tempo) {
	double Cov = (Time*Tempo)/(4*60);
return Cov;
}

inline double LVolFunc(double Vol, double Pan) {
	return Limit(Pan*(-1)+1,0.0,1.0)*Vol;
}

inline double RVolFunc(double Vol, double Pan) {
	return Limit(Pan+1,0.0,1.0)*Vol;
}




inline double Tot(bool Value, double R1, double R2)
{
	if (Value) return R1;
	else return R2;
}

inline double Abs(double Value)
{
	if (Value < 0) return Value*(-1);
	else return Value;
}

inline double Random(double Min, double Max)
{
	return (rand()/32768.0)*(Max-Min)+Min;
}

inline double SqrRoot(double x)
{
	return sqrt(x);
}

/*
// char* functions
int stringlength(char* String)
{
	short int i=0;
	while(*(String+i) != NULL)
	{
		i++;
	}
	return i;
}

char* unitestring(char* String1, char* String2)
{
	short int Length1 = stringlength(String1);
	short int Length2 = stringlength(String2);
	char* NewString = (char*)new char[Length1 + Length2 + 1];
	
	short int i=0;
	for(i=0;i<=Length1-1;i++)
	{
		*(NewString+i) = *(String1+i);
	}
	for(i=0;i<=Length2;i++)
	{
		*(NewString+i+Length1) = *(String2+i);
	}
	
	return NewString;
}
*/
#endif