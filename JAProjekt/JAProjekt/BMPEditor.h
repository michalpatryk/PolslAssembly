#pragma once
#define NOMINMAX
#include "PerformanceTimer.h"
#include <fstream>
#include <optional>
#include <string>

#include <Windows.h>
#include <wingdi.h>
enum class AlgorithmType
{
	cppAlgorithm, asmAlgorithm
};


class BMPEditor
{
	
	std::string sourceFilename;
	std::string destinationFilename;
	
	BITMAPFILEHEADER fileHeader;
		/*WORD  bfType;
		DWORD bfSize;
		WORD  bfReserved1;
		WORD  bfReserved2;
		DWORD bfOffBits;*/
	BITMAPINFOHEADER v1header;
	BITMAPV5HEADER v5Header;
	DWORD biSize;
	LONG biWidth;
	LONG biHeight;
	MEMORYSTATUSEX statex;
	
	PerformanceTimer totalTimer;
	PerformanceTimer algOnlyTimer;
	
	std::optional<std::string> headerParser(std::ifstream& fileStream);
	void headerRewriter(std::ifstream& fileStream, std::ofstream& outStream);
	void getMemoryStatus();
	void algorithmParallelRunner(DWORDLONG maxProgramMemUse, std::ifstream& fileStream, std::ofstream& outStream,
		unsigned int threadCount, AlgorithmType algType);
	bool isEnoughDiskSpace();

	void wipEditor(char* begin, char* end, long biWidth);
	
public:
	BMPEditor() {};
	void setSourceFilename(std::string file) { sourceFilename = file; };
	void setDestinationFilename(std::string file) { destinationFilename = file; };
	std::string runAlgorithm(AlgorithmType algType, unsigned int threadCount);
};

