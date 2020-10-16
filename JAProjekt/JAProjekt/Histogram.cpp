#include "Histogram.h"


#include <iostream>
#include <thread>
#include <vector>
extern "C" int _stdcall cppHistogram1(char* begin, char* end, long biWidth, unsigned long long* R, unsigned long long* G, unsigned long long* B);
Histogram::Histogram(std::string destinationFilename, std::string sourceFilename, LONG biWidth, LONG biHeight, DWORD bfOffBits)
{
	this->destinationFilename = destinationFilename;
	this->sourceFilename = sourceFilename;
	this->biWidth = biWidth;
	this->biHeight = biHeight;
	this->bfOffBits = bfOffBits;
}

void Histogram::run(std::string endAppend, DWORDLONG maxProgramMemUse, unsigned int threadCount)
{
	std::ifstream inFileStream(sourceFilename, std::ios::binary);
	destinationFilename.append(endAppend);
	std::ofstream outFileStream(destinationFilename);
	BITMAPFILEHEADER fileHeader = { (WORD)("BM"), 14, 0, 0, 100 };
	BITMAPCOREHEADER infoHeader = { 12,255,255,0,24 };

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
	inFileStream.close();
	outFileStream.close();
}

Histogram::~Histogram()
{
	delete[] R;
	delete[] G;
	delete[] B;
}
