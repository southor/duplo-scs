
#ifndef _DUP_FUNC_
#define _DUP_FUNC_

namespace Dup
{

	// length in number of bytes
	void clearMemory(void* target, const int& length);
	void copyData(void* source, void* target,const int& length); 
	bool between(int value, int lim1, int lim2);
	int smallest(int a,int b);
	int limit(int value,int min,int max);
	double limit(double value,double min,double max);
	void swapBytes(void *from, void *to, int l);

	void clearMemory(void* target, const int& length) // length in number of bytes
	{
		for(int i=0;i<length;i++)
		{
			*((char*)target+i) = 0;
		}
	}

	void copyData(void* source, void* target,const int& length) // length in number of bytes
	{
		for(int i=0;i<length;i++)
		{
			*((char*)target+i) = *((char*)source+i);
		}
	}


	bool between(int value, int lim1, int lim2)
	{
		return ((value >= lim1 || value >= lim2) && (value <= lim1 || value <= lim2));
	}

	int smallest(int a,int b)
	{
		if(a < b) return a;
		else return b;
	}

	int limit(int value,int min,int max)
	{
		if (value > max) value = max;
		if (value < min) value = min;
		return value;
	}

	double limit(double value,double min,double max)
	{
		if (value > max) value = max;
		if (value < min) value = min;
		return value;
	}


	void swapBytes(void *from, void *to, int l)
	{
		dup_uint8 tmp;	
		int m = l/2;

		for(int i=0;i<m;i++)
		{
			tmp = *(((dup_uint8*)from)+i);
			*(((dup_uint8*)to)+i) = *(((dup_uint8*)from)+l-(i+1));
			*(((dup_uint8*)to)+l-(i+1)) = tmp;
		}

		if (l%2 != 0)
		{
			*(((dup_uint8*)to)+l) = *(((dup_uint8*)from)+l);
		}
	}

	/*
	void equal(void *a, void *b, int l)
	{
		for(int i=0;i<l;i++)
		{
		}
	}
	*/

};

#endif