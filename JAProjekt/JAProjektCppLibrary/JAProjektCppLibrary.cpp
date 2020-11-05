//Bitmap binarization, version 1.0
//Author: Michał Urbańczyk
//Subject: Jezyki Assemblerowe
//Year: 2020/2021
//Compiler: MSVC_x64
//File containing asmBinarization algorithms.
//The main idea of the algorith is: take every pixels RGB values, and perform an operation on provided array:
//result = R * 0.299 + G * 0.587 + B * 0.114
//if(result) > threshold [R,G,B] = [255, 255, 255]
//else [R, G, B] = [0, 0, 0]
//Naming convention is cppBinarization{version number}
#include "pch.h"
#include "JAProjektCppLibrary.h"

#include <cstdint>
// parameters are: beggining of bmp array, end of said array, width of the said array and treshold of binarization function
void cppBinarization1(char* begin, char* end, long biWidth, float treshold)
{
	char* currPos = begin;
	long currByteLoc = 0;
	while (currPos < end)
	{
		float R = (unsigned char)*(currPos + 2);
		float G = (unsigned char)*(currPos + 1);
		float B = (unsigned char)*(currPos + 0);
		uint16_t Res = R * 0.299 + G * 0.587 + B * 0.144;	//calculate grayscale
		if (Res >= 256 * treshold)	//change pixel to all white
		{
			*(currPos) = 255;
			*(currPos + 1) = 255;
			*(currPos + 2) = 255;
		}
		else	//change pixel to all black
		{
			*(currPos) = 0;
			*(currPos + 1) = 0;
			*(currPos + 2) = 0;
		}
		if (currByteLoc + 3 > biWidth)	//check for row end
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

