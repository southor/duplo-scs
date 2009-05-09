
#ifndef _TSS_FUNC_
#define _TSS_FUNC_

//#include "iostream"


void copyarray(char* source, char* target, const int& length);

inline bool between(int value, int lim1, int lim2);
inline int limit(int Value,int Min,int Max);
inline double dlimit(double Value,double Min,double Max);
inline bool limitp(int Value,int Min,int Max);
inline bool dlimitp(double Value,double Min,double Max);
inline int ifr(bool Value, int a, int b);

inline int Smallest(int a,int b);
inline char Smallest(char a,char b);
inline int Greatest(int a,int b);
inline float Greatest(float a,float b);
inline double Greatest(double a,double b);
inline char Greatest(char a,char b);

//int Power(unsigned int v, unsigned int n);



void zeromemory(void *mem, int size); // size in bytes


#endif


// ------------------------------- functions ----------------------------------
#ifndef _TSS_FUNC_DEC_
#define _TSS_FUNC_DEC_

void copyarray(char* source, char* target,const int& length) // length in number of bytes
{
	//std::cout << "copyarray length:" << length << std::endl;

	for(int i=0;i<length;i++)
	{
		*((char*)target+i) = *((char*)source+i);
	}
}
inline bool between(int value, int lim1, int lim2)
{
	return ((value >= lim1 || value >= lim2) && (value <= lim1 || value <= lim2));
}

inline int limit(int Value,int Min,int Max)
{
	if (Value > Max) Value = Max;
	if (Value < Min) Value = Min;
	return Value;
}

inline double dlimit(double Value,double Min,double Max)
{
	if (Value > Max) Value = Max;
	if (Value < Min) Value = Min;
	return Value;
}

inline bool limitp(int Value,int Min,int Max)
{
	bool P = true;
	if (Value > Max) P = false;
	else if (Value < Min) P = false;
	return P;
}

inline bool dlimitp(double Value,double Min,double Max)
{
	bool P = true;
	if (Value > Max) P = false;
	else if (Value < Min) P = false;
	return P;
}

inline int ifr(bool Value, int a, int b)
{
	if(Value) return a;
	else return b;
}

inline double dfr(bool Value, double a, double b)
{
	if(Value) return a;
	else return b;
}

inline int Smallest(int a,int b)
{
	if(a<b) return a;
	else return b;
}

inline char Smallest(char a,char b)
{
	if(a<b) return a;
	else return b;
}

inline int Greatest(int a,int b)
{
	if(a>b) return a;
	else return b;
}

inline double Greatest(double a,double b)
{
	if(a>b) return a;
	else return b;
}

inline float Greatest(float a,float b)
{
	if(a>b) return a;
	else return b;
}

inline char Greatest(char a,char b)
{
	if(a>b) return a;
	else return b;
}
/*
int Power(unsigned int v, unsigned int n)
{

	int r = 1;
	for(unsigned int i=0;i<n;i++)
	{
		r = r*v;
	}
	return r;

}
*/

inline void zeromemory(void *mem, int size)
{
	for(int i=0;i<size;i++)
	{
		*((char*)mem+i) = 0;
	}
}

#endif