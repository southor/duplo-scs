
#ifndef _DUP_STRING_LOADER_
#define _DUP_STRING_LOADER_

#include "Dup_Declares.h"
#include "fstream"

namespace Dup
{

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
		Length = smallest(limit(Length, 0, INT_MAX), filelength(FileName));

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

			//MessageBox(NULL, FileName, "fuck", MB_ICONEXCLAMATION | MB_OK);
			
		}
		
		toffile.close();
		if(incorrectfile) return NULL;
		else return NewString;
		
	}

};

#endif