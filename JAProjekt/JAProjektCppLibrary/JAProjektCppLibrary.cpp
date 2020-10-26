#include "pch.h"
#include "JAProjektCppLibrary.h"

#include <cstdint>

int testFunction(unsigned long a)
{
	a *= 125;
	return a;
}

void cppBinarization1(char* begin, char* end, long biWidth, float treshold)
{
	char* currPos = begin;
	long currByteLoc = 0;
	while (currPos < end)
	{
		float R = (unsigned char)*(currPos + 2);
		float G = (unsigned char)*(currPos + 1);
		float B = (unsigned char)*(currPos + 0);
		uint8_t Res = R * 0.299 + G * 0.587 + B * 0.144;
		if (Res > 256 * treshold)
		{
			*(currPos) = 255;
			*(currPos + 1) = 255;
			*(currPos + 2) = 255;
		}
		else
		{
			*(currPos) = 0;
			*(currPos + 1) = 0;
			*(currPos + 2) = 0;
		}
		if (currByteLoc + 3 > biWidth)
		{
			currPos += (biWidth - currByteLoc);
			currByteLoc = 0;
		}
		else
		{
			currByteLoc += 3;
			currPos += 3;
		}
	}
}

JAPROJEKTCPPLIBRARY_API void cppHistogram1(char* begin, char* end, long biWidth, unsigned long long* R, unsigned long long* G, unsigned long long* B)
{
	char* currPos = begin;
	long currByteLoc = 0;
	while (currPos < end)
	{
		B[(BYTE)(*currPos)] += 1;
		G[(BYTE)*(currPos + 1)] += 1;
		R[(BYTE)*(currPos + 2)] += 1;
		if (currByteLoc + 3 > biWidth)
		{
			currPos += (biWidth - currByteLoc);
			currByteLoc = 0;
		}
		else
		{
			currByteLoc += 3;
			currPos += 3;
		}
	}
}
