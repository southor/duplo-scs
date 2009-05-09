#ifndef _TSS_STRFUNC_
#define _TSS_STRFUNC_

#include <limits.h>
#include <fstream>
#include "Func.h"

int stringlength(char* String);
int wordlength(char *String);
char *reversestring(char *String);
void reversestringin(char *String);
void copychararray(char* source, char* target, int length); // copies as long as string or as long as length
bool eqstring(char* String1, char* String2);
bool eqchararray(char* String1, char* String2, int chars);
char* copystring(char* String);
char* getstring(char* String, char Delimiter);
char* getstring(char* String, char Delimiter1, char Delimiter2);
char* getstring(char* String, char Delimiter1, char Delimiter2, char Delimiter3);
char* getrow(char* String);
int nextrow(char* String);
char* ignorechar(char* String, char Ignore);
char* ignorechars(char* String, char Ignore1, char Ignore2);
char* ignorechars(char* String, char Ignore1, char Ignore2, char Ignore3);
int getpos(char* String, char Delimiter);
int getpos(char* String, char Delimiter1, char Delimiter2);
int getpos(char* String, char Delimiter1, char Delimiter2, char Delimiter3);
char* takestring(char* String,short int From,short int To);
char* unitestrings(char* String1, char* String2);
char *unitestrings(char *String1, char *String2, char *String3, char *String4=NULL,
				    char *String5=NULL, char *String6=NULL, char *String7=NULL, char *String8=NULL,
					char *String9=NULL, char *String10=NULL, char *String11=NULL, char *String12=NULL,
					char *String13=NULL, char *String14=NULL, char *String15=NULL, char *String16=NULL);
void addstring(char* String1, char* String2);
int stringtoint(char* String);
bool stringnump(char* String);
char* inttostring(int Value);
bool comparestring(char* String1, char* String2, bool Rise);

bool sameextension(char* filename1, char*filename2); // the form .xxx  (3 letters)
char* loadstring(char* FileName, int Length = INT_MAX); 
int filelength(char* FileName); // not working correctly

#endif

#ifndef _TSS_STRFUNC_DEC_
#define _TSS_STRFUNC_DEC_

// String functions char*

// -----------------   String functions  -----------------------




int stringlength(char* String)
{
	
	int i=0;
	if(String != NULL)
	{
		while(*(String+i) != NULL)
		{
			i++;
			
		}
	}
	return i;
}

int wordlength(char *String)
{
	int i = 0;
	while((*(String+i) != 0) && (*(String+i) != 32))
	{
		i++;
	}
	return i;
}

char *reversestring(char *String)
{
	
	int length = stringlength(String);
	char *NewString = new char[length+1];
	//cout << length << endl;

	int oldpos = 0;
	int newpos = length-1;
	*(NewString+length) = 0;
	while(*(String+oldpos) != 0)
	{
		*(NewString+newpos) = *(String+oldpos);
		newpos--;
		oldpos++;
	}

	return NewString;
}

void reversestringin(char *String)
{
	char *TmpString = reversestring(String);
	
	int i = 0;
	while(*(TmpString+i) != 0)
	{
		*(String+i) = *(TmpString+i);
		i++;
	}
	delete TmpString;
}

void copychararray(char* source, char* target, int length)
{
	int i = 0;
	while(*(source+i) != 0 &&  i<length)
	{
		*(target+i) = *(source+i);
		i++;
	}
	if(*(source+i) == 0) *(target+i) = 0;
}

bool eqstring(char* String1, char* String2)
{
	bool Equals = true;
	short int i=0;
	while (Equals && (*(String1+i) != 0 || *(String2+i) != 0))
	{
		if (*(String1+i) != *(String2+i))
		{
			Equals = false;
		}
		else i++;
	}
	return Equals;
}

bool eqchararray(char* String1, char* String2, int chars)
{
	bool Equals = true;
	short int i=0;
	while (Equals && (i < chars))
	{
		if (*(String1+i) != *(String2+i))
		{
			Equals = false;
		}
		else i++;
	}
	return Equals;
}

bool eqsubstring(char* String1, char* String2, int chars)
{
	bool Equals = true;
	short int i=0;
	while (Equals && (*(String1+i) != 0 || *(String2+i) != 0))
	{
		if (*(String1+i) != *(String2+i))
		{
			Equals = false;
		}
		else i++;
	}
	return Equals;
}

char* copystring(char* String)
{
	short int Length = stringlength(String);
	char* NewString = (char*)new char[Length+1];
	for(short int i=0;i<=Length;i++)
	{
		*(NewString+i) = *(String+i);
	}	
	return NewString;
}

int getpos(char* String, char Delimiter)
{
	short int i=0;
	while((*(String+i) != Delimiter) && (*(String+i) != 0))
	{
		i++;
	}
	if((*(String+i) == 0) && (Delimiter != 0)) return 0;
	else return i;
}

int getpos(char* String, char Delimiter1, char Delimiter2)
{
	short int i=0;
	while((*(String+i) != Delimiter1) && (*(String+i) != 0) && (*(String+i) != Delimiter2))
	{
		i++;
	}
	if((*(String+i) == 0) && (Delimiter1 != 0) && (Delimiter2 != 0)) return 0;
	else return i;
}

int getpos(char* String, char Delimiter1, char Delimiter2, char Delimiter3)
{
	short int i=0;
	while((*(String+i) != Delimiter1) &&
		(*(String+i) != 0) &&
		(*(String+i) != Delimiter2) &&
		(*(String+i) != Delimiter3))
	{
		i++;
	}
	if((*(String+i) == 0) && (Delimiter2 != 0) && (Delimiter3 != 0)) return 0;
	else return i;
}

char* getstring(char* String, char Delimiter)
{
	return takestring(String, 0, getpos(String, Delimiter));
}

char* getstring(char* String, char Delimiter1, char Delimiter2)
{
	return takestring(String, 0, getpos(String, Delimiter1, Delimiter2));
}

char* getstring(char* String, char Delimiter1, char Delimiter2, char Delimiter3)
{
	return takestring(String, 0, getpos(String, Delimiter1, Delimiter2, Delimiter3));
}


char* ignorechar(char* String, char Ignore)
{
	short int Length=0;
	short int NewStringLength=0;
	while(*(String+Length) != 0)
	{
		
		Length++;
		if (*(String+Length) != Ignore) NewStringLength++;
	}
	
	char* NewString = new char[NewStringLength+1];
	short int i2=0;
	for(short int i=0;i<=Length;i++)
	{
		if (*(String+i) != Ignore)
		{
			*(NewString+i2) = *(String+i);
			i2++;
		}
	}
	*(NewString+i2) = 0;
	return NewString;	
}

char* ignorechars(char* String, char Ignore1, char Ignore2)
{
	short int Length=0;
	short int NewStringLength=0;
	while(*(String+Length) != 0)
	{
		
		Length++;
		if ((*(String+Length) != Ignore1) && (*(String+Length) != Ignore2)) NewStringLength++;
	}
	
	char* NewString = new char[NewStringLength+1];
	short int i2=0;
	for(short int i=0;i<=Length;i++)
	{
		if ((*(String+Length) != Ignore1) && (*(String+Length) != Ignore2))
		{
			*(NewString+i2) = *(String+i);
			i2++;
		}
	}
	*(NewString+i2) = 0;
	return NewString;	
}

char* ignorechars(char* String, char Ignore1, char Ignore2, char Ignore3)
{
	short int Length=0;
	short int NewStringLength=0;
	while(*(String+Length) != 0)
	{
		
		Length++;
		if ((*(String+Length) != Ignore1) && (*(String+Length) != Ignore2) && (*(String+Length) != Ignore3)) NewStringLength++;
	}
	
	char* NewString = new char[NewStringLength+1];
	short int i2=0;
	for(short int i=0;i<=Length;i++)
	{
		if ((*(String+Length) != Ignore1) && (*(String+Length) != Ignore2) && (*(String+Length) != Ignore3))
		{
			*(NewString+i2) = *(String+i);
			i2++;
		}
	}
	*(NewString+i2) = 0;
	return NewString;	
}

char* getrow(char* String)
{
	return ignorechar(takestring(String, 0, getpos(String, 10)), 13);
}

int nextrow(char* String)
{
	int pos = getpos(String, 10) + 1;
	if(*(String+pos) == 13) pos++;

	return pos;
}




char* takestring(char* String, short int From, short int To)
{
	char* NewString = new char[To+1-From];
	short int i;
	for(i=0;i<=To-From;i++)
	{
		*(NewString+i) = *(String+From+i);
	}
	*(NewString+i-1) = 0;
	return NewString;
}

char* unitestrings(char* String1, char* String2)
{
	if(String1 == NULL)
	{
		if(String2 == NULL) return NULL;
		else return String2;
	}
	else if(String2 == NULL) return String1;

	short int Length1 = stringlength(String1);
	short int Length2 = stringlength(String2);
	char* NewString = new char[Length1 + Length2 + 1];
	
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

char *unitestrings(char *String1, char *String2, char *String3, char *String4,
				    char *String5, char *String6, char *String7, char *String8,
					char *String9, char *String10, char *String11, char *String12,
					char *String13, char *String14, char *String15, char *String16)
{
	char *string[17] = {String1, String2, String3, String4,
						String5, String6, String7, String8,
						String9, String10, String11, String12,
						String13, String14, String15, String16, NULL};
	
	char *returnstring = copystring(String1);
	char *tmpstring;
	int i = 1;
	while(string[i] != NULL)
	{
		tmpstring = returnstring;
		returnstring = unitestrings(returnstring, string[i]);
		delete tmpstring;
		i++;
	}	
	return returnstring;
}

void addstring(char* String1, char* String2) // adds a string2 to string1 without concerning
{
	short int Length1 = stringlength(String1);
	short int Length2 = stringlength(String2);

	for(int i=0;i<=Length2;i++)
	{
		*(String1+i+Length1) = *(String2+i);
	}
}

int stringtoint(char* String)
{


	
	
	if(*String == 0) return 0;
	else
	{
		
		
		short int stringpos=0;
		// Find End of String
		while (*(String+stringpos) != 0)
		{
			stringpos++;
		}
	
		stringpos--;
	
		
		int p = 1;
		int Value = 0;
		//cout << "ok" << endl; // debugtest
		for(;stringpos>=0;stringpos--)
		{
		
			if (*(String+stringpos) == 45) // it is negative
			{
				
				Value = Value*(-1);
				break;
			}
			Value = Value + (*(String+stringpos)-48)*p;
			p = p*10;
			

		}
	
		return Value;
	}
}

bool stringnump(char* String)
{
	if (*String == 0) return true;
	else return (limitp(*String,48,57) && stringnump(String+1));
}

char* inttostring(int Value)
{
	if(Value == 0)
	{
		char *string = new char[2];
		*(string+0) = 48;
		*(string+1) = 0;
		return string;
	}
	
	static char BuildString[32];
	
	bool negative = false;
	if(Value < 0)
	{
		Value = Value*(-1);
		negative = true;
	}
	
	
	int rest = Value;
	int i = 0;
	while(rest > 0)
	{
		BuildString[i] = rest%10+48;
		rest = rest/10;
		i++;
	}
	if(negative)
	{
		BuildString[i] = 45;
		i++;
	}
	BuildString[i] = 0;

	char *NewString = reversestring(BuildString);
	
	return NewString;
}

bool comparestring(char* String1, char* String2, bool Rise)
{
	bool Equals = true;
	short int i=0;
	while (Equals && (*(String1+i) != 0 || *(String2+i) != 0))
	{
		if (*(String1+i) != *(String2+i))
		{
			Equals = false;
		}
		else i++;
	}
	if (Equals == true) return true;
	else
	{
		if (*(String1+i) < *(String2+i)) return Rise;
		else return !(Rise);
	}
}

bool sameextension(char* filename1, char*filename2)
{
	//MessageBox(NULL, "luda", "poooo", MB_ICONEXCLAMATION | MB_OK);
	if((filename1 == NULL) || (filename2 == NULL)) return false;
	//MessageBox(NULL, "buusa", "mmmmmmmmmmmmmm", MB_ICONEXCLAMATION | MB_OK);
	char *ext1 = filename1 + stringlength(filename1) - 4;
	
	//MessageBox(NULL, "test", "see", MB_ICONEXCLAMATION | MB_OK);
	char *ext2 = filename2 + stringlength(filename2) - 4;
	
	
	if(*(ext1+1) == *(ext2+1) && *(ext1+2) == *(ext2+2) && *(ext1+3) == *(ext2+3)) return true;
	else return false;
}


int filelength(char* FileName)
{
	
	int Length = 0;
	std::ifstream file((const char*)FileName, std::ios::binary);
	if (!file.good())
	{
		return 0;
	}
	else
	{
		char Tmp;
		while(file.read((&Tmp), sizeof(char)))
		{
			Length++;
		}

	}
	
	file.close();
	
	return Length;
}

char* loadstring(char* FileName, int Length)
{
	//MessageBox(NULL, "null-file", "no", MB_ICONEXCLAMATION | MB_OK);
	Length = Smallest(limit(Length, 0, INT_MAX), filelength(FileName));

	char *NewString = NULL;
	bool incorrectfile = false;

	std::ifstream toffile;
	//if(FileName != NULL) toffile.open((const char*)FileName, ios::binary, ios::nocreate);
	if(FileName != NULL) toffile.open((const char*)FileName, std::ios::binary);
	else return NULL;
	
	//toffile.setstate(std::filebuf::binary);

	//if(file.fail());
	//else
	if(toffile.good())
	{
		
		
		NewString = new char[Length+1];
		if((toffile.read(NewString, sizeof(char)*(Length))) == false) incorrectfile = true;
		*(NewString+Length) = 0;

		
		
	}
	
	toffile.close();
	if(incorrectfile) return NULL;
	else return NewString;
	
}


#endif
