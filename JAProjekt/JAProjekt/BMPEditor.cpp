#include "BMPEditor.h"
//#pragma comment(lib, "JAProjektCppLibrary.dll")
//#include <JAProjektCppLibrary.dll>
#include <filesystem>
#include <iostream>
#include <thread>

#include "../JAProjektCppLibrary/JAProjektCppLibrary.h"

std::optional <std::string> BMPEditor::headerParser(std::ifstream& fileStream)
{
	int b = testFunction(5);
	fileStream.read((char*)(&fileHeader), 14);		//We are loading Bitmap file header
	DWORD headerSize;
	fileStream.read((char*)(&headerSize), 4);//gets the size of the header

	switch (headerSize)
	{
	case(40):
	{		//ibitmapinfoheader
		fileStream.read((char*)(&v1header.biWidth), 36);
		biSize = v1header.biSize;
		biWidth = v1header.biWidth;
		biHeight = v1header.biHeight;
		break;
	}
	case(124):
	{		//bitmap v5 header
		fileStream.read((char*)(&v5Header.bV5Width), 120);
		biSize = v5Header.bV5Size;
		biWidth = v5Header.bV5Width;
		biHeight = v5Header.bV5Height;
		break;
	}
	default:
	{
		return std::make_optional(std::string("Unsupported bitmap header"));
	}
	}

	fileStream.seekg(fileHeader.bfOffBits, std::ios::beg);		//set the read location to the beggining of the pixel array
	return std::nullopt;
}

void BMPEditor::headerRewriter(std::ifstream& fileStream, std::ofstream& outStream)
{
	fileStream.seekg(0, std::ios::beg);
	char* rewriter = new char[fileHeader.bfOffBits];
	fileStream.read(rewriter, fileHeader.bfOffBits);
	outStream.write(rewriter, fileHeader.bfOffBits);
}

void BMPEditor::getMemoryStatus()
{
	statex.dwLength = sizeof(statex);
	GlobalMemoryStatusEx(&statex);
}

//This function might look ugly, but it does it job well. Its a closed box that runs algorithms, nothing more.
//Yes, it takes a lot of arguments - but it wouldn't change a thing if these were made global by class.
//Yes, there is DRY - but its repeated only twice, and the second time it has some 'extras' - trying to make a special function
//just to avoid it would only lead to bugs and little gain.
void BMPEditor::algorithmParallelRunner(DWORDLONG maxProgramMemUse, std::ifstream& fileStream, std::ofstream& outStream,
	unsigned int threadCount, AlgorithmType algType)
{
	fileStream.seekg(fileHeader.bfOffBits, std::ios::beg);
	outStream.seekp(fileHeader.bfOffBits);
	maxProgramMemUse = maxProgramMemUse - (maxProgramMemUse % threadCount);
	DWORD remainingFileSize = fileHeader.bfSize;
	algOnlyTimer.start();
	algOnlyTimer.pause();

	//calculate bytes per row
	long rowSize = std::ceil((float)(24 * biWidth) / 32.0) * 4;
	//we are assuming, that we have enough memory to store rowSize*threadCount

	//Main alg loop
	while (maxProgramMemUse < remainingFileSize)
	{
		char* arrToSplit = new char[maxProgramMemUse];
		fileStream.read(arrToSplit, maxProgramMemUse);
		std::vector<std::thread> threadVector;

		long rowsPerThread = std::floor(maxProgramMemUse / (rowSize * threadCount));
		long processedSize = rowsPerThread * threadCount;

		for (unsigned int i = 0; i < threadCount; i++)
		{
			wipEditor(arrToSplit + (i * rowsPerThread * rowSize), arrToSplit + ((i + 1) * rowsPerThread * rowSize), rowSize);

		}

		algOnlyTimer.resume();
		for (std::thread& th : threadVector)
		{
			th.join();
		}
		algOnlyTimer.pause();
		outStream.write(arrToSplit, processedSize);
		delete[] arrToSplit;
		remainingFileSize -= processedSize;
	}

	//Now we process the rest of the pixel array
	char* arrToSplit = new char[remainingFileSize];
	fileStream.read(arrToSplit, remainingFileSize);
	DWORDLONG splitValue = remainingFileSize / threadCount;
	std::vector<std::thread> threadVector;

	long rowsPerThread = std::floor(remainingFileSize / (rowSize * threadCount));
	long extra = remainingFileSize - (rowsPerThread * threadCount * rowSize);
	for (long i = 0; i < threadCount; i++)
	{
		if (i + 1 > threadCount)
		{
			wipEditor(arrToSplit + (i * rowsPerThread * rowSize), arrToSplit + ((i + 1) * rowsPerThread * rowSize) + extra, rowSize);
		}
		else
		{
			wipEditor(arrToSplit + (i * rowsPerThread * rowSize), arrToSplit + ((i + 1) * rowsPerThread * rowSize), rowSize);
		}
	}
	algOnlyTimer.resume();

	for (std::thread& th : threadVector)
	{
		th.join();
	}
	algOnlyTimer.stop();
	outStream.write(arrToSplit, remainingFileSize);
	delete[] arrToSplit;
}

bool BMPEditor::isEnoughDiskSpace()
{
	//disk space
	BOOL  fResult;
	unsigned __int64 i64FreeBytesToCaller,
		i64TotalBytes,
		i64FreeBytes;
	fResult = GetDiskFreeSpaceEx(L"C:",
		(PULARGE_INTEGER)&i64FreeBytesToCaller,
		(PULARGE_INTEGER)&i64TotalBytes,
		(PULARGE_INTEGER)&i64FreeBytes);
	//string to wstring
	std::wstring stemp = std::wstring(sourceFilename.begin(), sourceFilename.end());
	LPCWSTR sw = stemp.c_str();
	//get file handle
	HANDLE hFile = CreateFile(sw, GENERIC_READ,
		FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
		return false; // error condition, could call GetLastError to find out more
	//get file size
	LARGE_INTEGER size;
	if (!GetFileSizeEx(hFile, &size))
	{
		CloseHandle(hFile);
		return false; // error condition, could call GetLastError to find out more
	}

	CloseHandle(hFile);
	if (size.QuadPart > i64FreeBytes)
	{
		return false;
	}
	else return true;
}

void BMPEditor::wipEditor(char* begin, char* end, long biWidth)
{
#if 0
	char* currPos = begin;
	while (currPos + 2 != end)
	{
		float R = *(currPos);
		float G = *(currPos + 1);
		float B = *(currPos + 2);
		uint8_t Res = R * 0.299 + G * 0.587 + B * 0.144;
		if (Res > 256 * 0.6)
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
		currPos += 3;
	}
#endif
	char* currPos = begin;
	long currByteLoc = 0;
	while (currPos < end)
	{
		float R = *(currPos);
		float G = *(currPos + 1);
		float B = *(currPos + 2);
		uint8_t Res = R * 0.299 + G * 0.587 + B * 0.144;
		if (Res > 256 * 0.6)
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
		if(currByteLoc + 3 > biWidth)
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

std::string BMPEditor::runAlgorithm(AlgorithmType algType, unsigned int threadCount)
{
	//Stream initialization
	std::ifstream fileStream(sourceFilename, std::ios::binary);
	std::ofstream outStream(destinationFilename, std::ios::binary);

	totalTimer.start();


	if (!fileStream.is_open())
	{
		return std::string("File not found");
	}
	if (!isEnoughDiskSpace())
	{
		return std::string("Not enough disk space on selected destination!");
	}
	//Header parsing
	std::optional<std::string> headerParserError = headerParser(fileStream);
	if (headerParserError.has_value())
	{
		return headerParserError.value();	//resets filestream to start
	}
	//Header writing in outfile
	headerRewriter(fileStream, outStream);

	//Memory check	
	getMemoryStatus();
	DWORDLONG memsize = statex.ullTotalPhys;
	DWORDLONG maxProgramMemUse = memsize * 2 / 3;	//maximum 70% memory load

	//Runs main algorithm
	algorithmParallelRunner(maxProgramMemUse, fileStream, outStream, threadCount, algType);


	//TODO
	//Add full copy of anything between ios::begin and fileHeader.bfOffBits to output file									+
	//Add disk avaliable space test/warning																					+
	//	--QueryPerformanceCounter-- START																					+
	//All bellow in while ifstream+chunkSize > ios::end																		X
	//	Add chunk read																										+
	//	Add chunk division																									+
	//	Add divided chunk process (by dll)														lowest priority->			X
	//	Add processed chunk save																							+
	//Add last chunk read																									+
	//Add last divided chunk process (by dll)																				+
	//Add last chunk save																									+
	//	--QueryPerformanceCounter-- END																						+
	//	Add extra timer - without file reads/writes																			+

	//TODO
	//outstream to the rest of file
	fileStream.close();
	outStream.close();

	totalTimer.stop();
	std::string totalTimerResult(std::to_string(totalTimer.getCounterTotalTicks()));
	std::string algOnlyTimeResult(std::to_string(algOnlyTimer.getCounterTotalTicks()));
	std::string returnString = totalTimerResult + "\n" + algOnlyTimeResult;
	return returnString;
}
