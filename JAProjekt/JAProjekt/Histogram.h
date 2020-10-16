#pragma once
#define NOMINMAX
#include <string>
#include <Windows.h>
#include <fstream>
class Histogram
{
	std::string sourceFilename;
	std::string destinationFilename;
	LONG biWidth;
	LONG  biHeight;
	DWORD bfOffBits;
	unsigned long long R[255];
	unsigned long long G[255];
	unsigned long long B[255];
public:
	Histogram(std::string destinationFilename, std::string sourceFilename, LONG biWidth, LONG biHeight, DWORD bfOffBits);
	void run(std::string endAppend, DWORDLONG maxProgramMemUse, unsigned int threadCount);
};

