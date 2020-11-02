#include "Histogram.h"



#include <algorithm>
#include <iostream>
#include <thread>
#include <vector>
//extern "C" int _stdcall cppHistogram1(char* begin, char* end, long biWidth, unsigned long long* R, unsigned long long* G, unsigned long long* B);
Histogram::Histogram(std::string destinationFilename, std::string sourceFilename, LONG biWidth, LONG biHeight, DWORD bfOffBits)
{
	this->destinationFilename = destinationFilename;
	this->sourceFilename = sourceFilename;
	this->biWidth = biWidth;
	this->biHeight = biHeight;
	this->bfOffBits = bfOffBits;
}

void Histogram::cppHistogram1(char* begin, char* end, long biWidth, unsigned long long* R, unsigned long long* G, unsigned long long* B)
{
	char* currPos = begin;
	long currByteLoc = 0;
	while (currPos < end)
	{
		B[(BYTE)(*currPos)] += 1;
		G[(BYTE) * (currPos + 1)] += 1;
		R[(BYTE) * (currPos + 2)] += 1;
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


void Histogram::run(std::string endAppend, DWORDLONG maxProgramMemUse, unsigned int threadCount)
{
	std::ifstream inFileStream(sourceFilename, std::ios::binary);
	destinationFilename.append(endAppend);
	std::ofstream outFileStream(destinationFilename);
	BITMAPFILEHEADER fileHeader = { 19778, 14, 0, 0, 100 };
	BITMAPCOREHEADER infoHeader = { 12,256,256,0,24 };

	outFileStream.write((char*)&fileHeader, 14);
	outFileStream.write((char*)&infoHeader, 12);

	inFileStream.seekg(bfOffBits, std::ios::beg);

	maxProgramMemUse = maxProgramMemUse - (maxProgramMemUse % threadCount);
	long rowSize = std::ceil((float)(24 * biWidth) / 32.0) * 4;
	DWORD remainingFileSize = biHeight * rowSize;




	DWORDLONG splitValue = remainingFileSize / threadCount;
	std::vector<std::thread> threadVector;
	long rowsPerThread = std::floor(remainingFileSize / (rowSize * threadCount));

	while (maxProgramMemUse < remainingFileSize)
	{
		char* arrToSplit = new char[maxProgramMemUse];
		inFileStream.read(arrToSplit, maxProgramMemUse);
		std::vector<std::thread> threadVector;

		long rowsPerThread = std::floor(maxProgramMemUse / (rowSize * threadCount));
		long processedSize = rowsPerThread * threadCount;

		for (unsigned int i = 0; i < threadCount; i++)
		{
			if (i + 1 == threadCount)
			{
				//HERE insert color calculations
				/*std::thread t1(cppBinarization1,
					(arrToSplit + (i * rowsPerThread * rowSize)),
					(arrToSplit + ((i + 1) * rowsPerThread * rowSize) + extra),
					rowSize,
					0.2
				);
				threadVector.push_back(std::move(t1));*/

			}
			else
			{
				//std::thread t1(cppBinarization1,
				//	(arrToSplit + (i * rowsPerThread * rowSize)),
				//	(arrToSplit + ((i + 1) * rowsPerThread * rowSize)),
				//	rowSize,
				//	0.2
				//);
				//threadVector.push_back(std::move(t1));
			}

		}
		for (std::thread& th : threadVector)
		{
			th.join();
		}
		delete[] arrToSplit;
		remainingFileSize -= processedSize;
	}

	char* arrToSplit = new char[remainingFileSize];
	inFileStream.read(arrToSplit, remainingFileSize);

	long extra = remainingFileSize - (rowsPerThread * threadCount * rowSize);
	for (long i = 0; i < threadCount; i++)
	{
		if (i + 1 == threadCount)
		{
			//HERE insert color calculations
			/*std::thread t1(cppBinarization1,
				(arrToSplit + (i * rowsPerThread * rowSize)),
				(arrToSplit + ((i + 1) * rowsPerThread * rowSize) + extra),
				rowSize,
				0.2
			);
			threadVector.push_back(std::move(t1));*/
			//cppHistogram1(
			//	arrToSplit + (i * rowsPerThread * rowSize),
			//	arrToSplit + ((i + 1) * rowsPerThread * rowSize) + extra,
			//	rowSize,
			//	R,	G,	B
			//);

		}
		else
		{
			//std::thread t1(cppBinarization1,
			//	(arrToSplit + (i * rowsPerThread * rowSize)),
			//	(arrToSplit + ((i + 1) * rowsPerThread * rowSize)),
			//	rowSize,
			//	0.2
			//);
			//threadVector.push_back(std::move(t1));

			cppHistogram1(
				arrToSplit + (i * rowsPerThread * rowSize),
				arrToSplit + ((i + 1) * rowsPerThread * rowSize),
				rowSize,
				R, G, B
			);
		}

	}

	for (std::thread& th : threadVector)
	{
		th.join();
	}
	delete[] arrToSplit;


	unsigned long long* histogramMaxArray = new unsigned long long[3];
	histogramMaxArray[0] = *std::max_element(R, R + 255);
	histogramMaxArray[1] = *std::max_element(G, G + 255);
	histogramMaxArray[2] = *std::max_element(B, B + 255);
	unsigned long long histogramMax = *std::max_element(histogramMaxArray, histogramMaxArray + 2);

	delete[] histogramMaxArray;

	const int arrToWriteWidth = 256;
	const int arrToWriteHeight = 256;
	unsigned long long inBetweenValue = (histogramMax / arrToWriteHeight) * 1.05;
	unsigned char arrToWrite[arrToWriteHeight][arrToWriteWidth * 3];
	for (int i = 0; i < arrToWriteHeight; i++)
	{
		for (int j = 0; j < arrToWriteWidth; j++)
		{
			if (R[j] > i * inBetweenValue && R[j] <= (i + 1) * inBetweenValue)
			{
				arrToWrite[i][j * 3 + 2] = 255;		//Red
			}
			else arrToWrite[i][j * 3 + 2] = 0;
			if (G[j] > i * inBetweenValue && G[j] <= (i + 1) * inBetweenValue)
			{
				arrToWrite[i][j * 3 + 1] = 255;		//Green
			}
			else arrToWrite[i][j * 3 + 1] = 0;
			if (B[j] > i * inBetweenValue && B[j] <= (i + 1) * inBetweenValue)
			{
				arrToWrite[i][j * 3 + 0] = 255;		//Blue
			}
			else arrToWrite[i][j * 3 + 0] = 0;
			
		}
	}
	outFileStream.seekp(100, std::ios::beg);
	for(int i = 0; i < arrToWriteHeight; i++)
	{
		outFileStream.write((char*)arrToWrite[i], arrToWriteWidth * 3);
	}
	
	inFileStream.close();
	outFileStream.close();
}

Histogram::~Histogram()
{
	delete[] R;
	delete[] G;
	delete[] B;
}
