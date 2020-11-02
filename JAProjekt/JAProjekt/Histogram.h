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
	unsigned long long* R = new unsigned long long[256]();
	unsigned long long* G = new unsigned long long[256]();
	unsigned long long* B = new unsigned long long[256]();
	void cppHistogram1(char* begin, char* end, long biWidth, unsigned long long* R, unsigned long long* G, unsigned long long* B);
public:
	Histogram() = delete;
	Histogram(std::string destinationFilename, std::string sourceFilename, LONG biWidth, LONG biHeight, DWORD bfOffBits);
	void run(std::string endAppend, DWORDLONG maxProgramMemUse, unsigned int threadCount);

	
	void runNoOutFile(DWORDLONG maxProgramMemUse, unsigned int threadCount);
	unsigned long long* getR() { return R; };
	unsigned long long* getG() { return G; };
	unsigned long long* getB() { return B; };
	~Histogram();
};

