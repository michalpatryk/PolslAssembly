#include "BMPEditor.h"
#include <filesystem>
#include <iostream>
#include <thread>

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

void BMPEditor::getMemoryStatus()
{
	statex.dwLength = sizeof(statex);
	GlobalMemoryStatusEx(&statex);
}

std::string BMPEditor::runAlgorithm(AlgorithmType algType, unsigned int threadCount)
{
	//Stream initialization
	std::ifstream fileStream(sourceFilename, std::ios::binary);
	std::ofstream outStream(destinationFilename, std::ios::binary);
	if(!fileStream.is_open())
	{
		return std::string("File not found");
	}

	//Header parsing
	std::optional<std::string> headerParserError = headerParser(fileStream);
	if (headerParserError.has_value())
	{
		return headerParserError.value();
	}

	//Memory check	
	getMemoryStatus();
	DWORDLONG memsize = statex.ullTotalPhys;
	DWORDLONG maxProgramMemUse = memsize * 2 / 3;	//maximum 70% memory load

	//Thread vectorization
	std::vector<std::thread> threadVector;

	//TODO
	//Add full copy of anything between ios::begin and fileHeader.bfOffBits to output file
	//Add disk avaliable space test/warning
	//	--QueryPerformanceCounter-- START
	//All bellow in while ifstream+chunkSize > ios::end
	//	Add chunk read
	//	Add chunk division
	//	Add divided chunk process (by dll)
	//	Add processed chunk save
	//Add last chunk read
	//Add last divided chunk process (by dll)
	//Add last chunk save
	//	--QueryPerformanceCounter-- END
	//	Add extra timer - without file reads/writes
	
	fileStream.close();
	return std::string();
}
