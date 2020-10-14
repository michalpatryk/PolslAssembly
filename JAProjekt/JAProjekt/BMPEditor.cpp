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

	//Main alg loop
	while (maxProgramMemUse < remainingFileSize)
	{
		char* arrToSplit = new char[maxProgramMemUse];
		fileStream.read(arrToSplit, maxProgramMemUse);

		DWORDLONG splitValue = maxProgramMemUse / threadCount;

		std::vector<std::thread> threadVector;
		for (unsigned int i = 0; i < threadCount; i++)
		{
			for (unsigned int j = 0; j * splitValue < maxProgramMemUse; j++)
			{
				if (algType == AlgorithmType::cppAlgorithm)
				{
					//std::thread t(RUNCPPALG(arrToSplit + j, arrToSplit + j+ splitValue));
				}
				//else { std::thread t(RUNASMALG(arrToSplit + j, arrToSplit + j+ splitValue)); };
			}
			//threadVector.push_back(move(t));

		}
		for (std::thread& th : threadVector)
		{
			th.join();
		}
		outStream.write(arrToSplit, maxProgramMemUse);
		delete[] arrToSplit;
		remainingFileSize -= maxProgramMemUse;
	}
	
	//Now we process the rest of the pixel array
	char* arrToSplit = new char[remainingFileSize];
	fileStream.read(arrToSplit, remainingFileSize);
	DWORDLONG extra = remainingFileSize % threadCount;

	DWORDLONG splitValue = remainingFileSize / threadCount;

	std::vector<std::thread> threadVector;
	for (unsigned int i = 0; i < threadCount; i++)
	{
		for (unsigned int j = 0; j * splitValue < maxProgramMemUse; j++)
		{
			int k = j * splitValue;
			if (algType == AlgorithmType::cppAlgorithm)
			{
				if (k > maxProgramMemUse)
				{
					//std::thread t(RUNCPPALG(arrToSplit + j, arrToSplit + j + splitValue + extra));
				}
				//std::thread t(RUNCPPALG(arrToSplit + j, arrToSplit + j + splitValue));

			}
			else
			{
				if (k > maxProgramMemUse)
				{
					//std::thread t(RUNASMALG(arrToSplit + j, arrToSplit + j + splitValue + extra));
				}
				//std::thread t(RUNASMALG(arrToSplit + j, arrToSplit + j + splitValue + extra));
			}
			//threadVector.push_back(move(t));
		}

	}
	for (std::thread& th : threadVector)
	{
		th.join();
	}
	//
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

std::string BMPEditor::runAlgorithm(AlgorithmType algType, unsigned int threadCount)
{
	//Stream initialization
	std::ifstream fileStream(sourceFilename, std::ios::binary);
	std::ofstream outStream(destinationFilename, std::ios::binary);
	
	LARGE_INTEGER li;
	QueryPerformanceFrequency(&li);	//gets frequency of the performance counter
	__int64 frequency = li.QuadPart;
	QueryPerformanceCounter(&li);	//gets current value of frequency timer
	__int64 CounterStart = li.QuadPart;

	
	if(!fileStream.is_open())
	{
		return std::string("File not found");
	}
	if(!isEnoughDiskSpace())
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
	//	--QueryPerformanceCounter-- START																low priority->		X
	//All bellow in while ifstream+chunkSize > ios::end																		X
	//	Add chunk read																										+
	//	Add chunk division																									+
	//	Add divided chunk process (by dll)														lowest priority->			X
	//	Add processed chunk save																							+
	//Add last chunk read																									+
	//Add last divided chunk process (by dll)																				+
	//Add last chunk save																									+
	//	--QueryPerformanceCounter-- END																	low priority->		X
	//	Add extra timer - without file reads/writes													very low priority->		X
	QueryPerformanceCounter(&li);
	__int64 CounterEnd = li.QuadPart;
	//TODO
	//outstream to the rest of file
	fileStream.close();
	outStream.close();
	return std::string();
}
