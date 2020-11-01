#include "BMPEditor.h"
//#pragma comment(lib, "JAProjektCppLibrary.dll")
//#include <JAProjektCppLibrary.dll>
#include <filesystem>
#include <iostream>
#include <thread>


typedef void(CALLBACK* BINARIZATIONPROC)(char* begin, char* end, long biWidth, float treshold);
std::optional <std::string> BMPEditor::headerParser(std::ifstream& fileStream)
{
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
		headerType = HeaderType::v1;
		break;
	}
	case(124):
	{		//bitmap v5 header
		fileStream.read((char*)(&v5Header.bV5Width), 120);
		biSize = v5Header.bV5Size;
		biWidth = v5Header.bV5Width;
		biHeight = v5Header.bV5Height;
		headerType = HeaderType::v5;
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
void BMPEditor::algorithmForLoop(unsigned int threadCount, AlgorithmType algType, char* arrToSplit, long rowsPerThread,
	long rowSize, long extra, std::vector<std::thread>& threadVector)
{
	float treshold = 0.3;
	HINSTANCE hDLL;
	BINARIZATIONPROC binprocPtr;
	
	if(algType == AlgorithmType::cppAlgorithm)
	{
		hDLL = LoadLibraryA("JAProjektCppLibrary");
	}
	else if (algType == AlgorithmType::asmAlgorithm)
	{
		hDLL = LoadLibraryA("JAProjektAsmLibrary");
	}
	if (hDLL != NULL)
	{
		if (algType == AlgorithmType::cppAlgorithm)
		{
			binprocPtr = (BINARIZATIONPROC)GetProcAddress(hDLL, "cppBinarization1");
		}
		else if (algType == AlgorithmType::asmAlgorithm)
		{
			binprocPtr = (BINARIZATIONPROC)GetProcAddress(hDLL, "asmBinarization1");
		}
		
		if (NULL != binprocPtr)
		{
			for (long i = 0; i < threadCount; i++)
			{
				if (i + 1 == threadCount)
				{
					std::thread t1(binprocPtr,
						(arrToSplit + (i * rowsPerThread * rowSize)),
						(arrToSplit + ((i + 1) * rowsPerThread * rowSize) + extra),
						rowSize,
						treshold
					);
					threadVector.push_back(std::move(t1));
				}
				else
				{
					std::thread t1(binprocPtr,
						(arrToSplit + (i * rowsPerThread * rowSize)),
						(arrToSplit + ((i + 1) * rowsPerThread * rowSize)),
						rowSize,
						treshold
					);
					threadVector.push_back(std::move(t1));
				}
			}
		}
		else throw "Error loading DLL!";
	}
	else throw "DLL file not found!";
	
	
}

void BMPEditor::algorithmParallelRunner(DWORDLONG maxProgramMemUse, std::ifstream& fileStream, std::ofstream& outStream,
	unsigned int threadCount, AlgorithmType algType)
{
	fileStream.seekg(fileHeader.bfOffBits, std::ios::beg);
	outStream.seekp(fileHeader.bfOffBits);
	maxProgramMemUse = maxProgramMemUse - (maxProgramMemUse % threadCount);
	long rowSize = std::ceil((float)(24 * biWidth) / 32.0) * 4;
	DWORD remainingFileSize = biHeight * rowSize;
	algOnlyTimer.start();
	algOnlyTimer.pause();

	//we are assuming, that we have enough memory to store rowSize*threadCount

	//Main alg loop
	while (maxProgramMemUse < remainingFileSize)
	{
		char* arrToSplit = new char[maxProgramMemUse];
		fileStream.read(arrToSplit, maxProgramMemUse);
		std::vector<std::thread> threadVector;

		long rowsPerThread = std::floor(maxProgramMemUse / (rowSize * threadCount));
		long processedSize = rowsPerThread * threadCount;

		algorithmForLoop(threadCount, algType, arrToSplit, rowsPerThread, rowSize, 0, threadVector);
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

	algorithmForLoop(threadCount, algType, arrToSplit, rowsPerThread, rowSize, extra, threadVector);
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

void BMPEditor::wipEditor(char* begin, char* end, long biWidth, float treshold)
{
	char* currPos = begin;
	long currByteLoc = 0;
	while (currPos < end)
	{
		float R = *(currPos);
		float G = *(currPos + 1);
		float B = *(currPos + 2);
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
	Histogram preEditHistogram(destinationFilename, sourceFilename, biWidth, biHeight, fileHeader.bfOffBits);
	preEditHistogram.run("_histPre.bmp", maxProgramMemUse, threadCount);
	algorithmParallelRunner(maxProgramMemUse, fileStream, outStream, threadCount, algType);
	Histogram postEditHistogram(destinationFilename, destinationFilename, biWidth, biHeight, fileHeader.bfOffBits);
	postEditHistogram.run("_histPost.bmp", maxProgramMemUse, threadCount);


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
