#pragma once
#define NOMINMAX
#include "PerformanceTimer.h"
#include "Histogram.h"
#include <fstream>
#include <optional>
#include <string>
#include <thread>
#include <vector>

#include <Windows.h>
#include <wingdi.h>
enum class AlgorithmType
{
	cppAlgorithm, asmAlgorithm
};
enum class HeaderType
{
	v1, v5
};


class BMPEditor
{
	float treshold = 0.3;
	std::string sourceFilename;
	std::string destinationFilename;
	std::string criticalEscape;
	HeaderType headerType;
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
	
	Histogram* preHistogram = nullptr;
	Histogram* postHistogram = nullptr;

	typedef void(CALLBACK* BINARIZATIONPROC)(char* begin, char* end, long biWidth, float treshold);
	HINSTANCE hDLL;
	BINARIZATIONPROC binprocPtr;

	

	
	std::optional<std::string> headerParser(std::ifstream& fileStream);
	void headerRewriter(std::ifstream& fileStream, std::ofstream& outStream);
	void getMemoryStatus();
	void algorithmParallelRunner(DWORDLONG maxProgramMemUse, std::ifstream& fileStream, std::ofstream& outStream,
		unsigned int threadCount, AlgorithmType algType);
	void BMPEditor::algorithmForLoop(unsigned int threadCount, AlgorithmType algType, char* arrToSplit, long rowsPerThread,
		long rowSize, long extra, std::vector<std::thread>& threadVector);
	bool isEnoughDiskSpace();

	void wipEditor(char* begin, char* end, long biWidth, float treshold);
	
public:
	unsigned long long wholeTime;
	unsigned long long threadsTime;
	BMPEditor() {};
	void setSourceFilename(std::string file) { sourceFilename = file; };
	void setDestinationFilename(std::string file) { destinationFilename = file; };
	std::string runAlgorithm(AlgorithmType algType, unsigned int threadCount);
	void setTreshold(float treshold) { this->treshold = treshold; }
	unsigned long long* getPreHistogramR() { return preHistogram->getR(); }
	unsigned long long* getPreHistogramG() { return preHistogram->getG(); }
	unsigned long long* getPreHistogramB() { return preHistogram->getB(); }
	unsigned long long* getPostHistogramR() { return postHistogram->getR(); }
	unsigned long long* getPostHistogramG() { return postHistogram->getG(); }
	unsigned long long* getPostHistogramB() { return postHistogram->getB(); }
};

