
#ifndef _DUP_WAVE_FILE_
#define _DUP_WAVE_FILE_

#include "Dup_Declares.h"
#include "iostream"
#include "fstream"

namespace Dup
{

	// savewave
	// int must be exactly 4 bytes (fix)
	int saveWave(char* fileName, dup_int32 waveFre, dup_int32 length, dup_int16 *lArray, dup_int16 *rArray);





	

	// savewave
	// int must be exactly 4 bytes (fix)
	int saveWave(char* fileName, dup_int32 waveFre, dup_int32 length, dup_int16 *lArray, dup_int16 *rArray)
	{
		dup_uint16 out;
	
		//ofstream WaveFile(fileName,ios::binary);

		std::ofstream waveFile;
	
		if(fileName != NULL) waveFile.open((const char*)fileName, std::ios::binary);

		if(waveFile.good())
		{

			out = 18770;
			waveFile.write((char *)&out,sizeof(dup_uint16));
			out = 17990;
			waveFile.write((char *)&out,sizeof(dup_uint16));

			length = 4*length+36;
			waveFile.write((char *)&length,sizeof(int));

			out = 16727;
			waveFile.write((char *)&out,sizeof(dup_uint16));
			out = 17750;
			waveFile.write((char *)&out,sizeof(dup_uint16));
			out = 28006;
			waveFile.write((char *)&out,sizeof(dup_uint16));
			out = 8308;
			waveFile.write((char *)&out,sizeof(dup_uint16));
			out = 16;
			waveFile.write((char *)&out,sizeof(dup_uint16));
			out = 0;
			waveFile.write((char *)&out,sizeof(dup_uint16));
			out = 1;
			waveFile.write((char *)&out,sizeof(dup_uint16));
			out = 2;
			waveFile.write((char *)&out,sizeof(dup_uint16));
			out = waveFre;
			waveFile.write((char *)&out,sizeof(dup_uint16));
			out = 0;
			waveFile.write((char *)&out,sizeof(dup_uint16));
			out = 45328;
			waveFile.write((char *)&out,sizeof(dup_uint16));
			out = 2;
			waveFile.write((char *)&out,sizeof(dup_uint16));
			out = 4;
			waveFile.write((char *)&out,sizeof(dup_uint16));
			out	= 16;
			waveFile.write((char *)&out,sizeof(dup_uint16));
			out = 24932;
			waveFile.write((char *)&out,sizeof(dup_uint16));
			out = 24948;
			waveFile.write((char *)&out,sizeof(dup_uint16));

			length = length-36;
			if (!(waveFile.write((char *)&length,sizeof(dup_int32))))
			{
				return 0;
			}
			length = length/4;

			dup_int16 lAmp, rAmp;
			

			for(long i=0;i<length;i++)
			{
				lAmp = (*(lArray+i));
				rAmp = (*(rArray+i));

				waveFile.write((char *)&lAmp, sizeof(short int));
				waveFile.write((char *)&rAmp, sizeof(short int));
			}
		}

		waveFile.close();
		return 1;
	}

};


#endif