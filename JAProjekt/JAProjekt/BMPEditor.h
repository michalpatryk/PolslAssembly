#pragma once
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
	
	std::string filename;
	
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
	std::optional<std::string> headerParser(std::ifstream& fileStream);
	void getMemoryStatus();
public:
	BMPEditor() {};
	void setFilename(std::string file) { filename = file; };
	std::string runAlgorithm(AlgorithmType algType, unsigned int threadCount);
private:
	
};

