#include "Benchmark.h"
#include <fstream>
#include <sstream>
void Benchmark::run()
{

	cppWholeTimes = std::vector<std::vector<unsigned long long>>(64, std::vector<unsigned long long>(statisticsRuns, 0));
	cppThreadTimes = std::vector<std::vector<unsigned long long>>(64, std::vector<unsigned long long>(statisticsRuns, 0));
	asmWholeTimes = std::vector<std::vector<unsigned long long>>(64, std::vector<unsigned long long>(statisticsRuns, 0));
	asmThreadTimes = std::vector<std::vector<unsigned long long>>(64, std::vector<unsigned long long>(statisticsRuns, 0));
	std::ofstream outStreamCppTotal("benchmarkCppTotal.txt");
	std::ofstream outStreamCppThreads("benchmarkCppThreads.txt");
    std::ofstream outStreamAsmTotal("benchmarkAsmTotal.txt");
	std::ofstream outStreamAsmThreads("benchmarkAsmThreads.txt");

	std::ostringstream os;
	for (int i = 0; i < 64; i++)	//cores
	{
		
		for (int j = 0; j < statisticsRuns; j++)	//statistics
		{
			bmpEditorPointer->runAlgorithm(AlgorithmType::cppAlgorithm, i+1);
			cppWholeTimes[i][j] = bmpEditorPointer->wholeTime;
			//cppWholeTimes[i][j] = 1;
			cppThreadTimes[i][j] = bmpEditorPointer->threadsTime;
			//cppThreadTimes[i][j] = 1;
		}
		for (int j = 0; j < statisticsRuns; j++)	//statistics
		{
			bmpEditorPointer->runAlgorithm(AlgorithmType::asmAlgorithm, i+1);
			asmWholeTimes[i][j] = bmpEditorPointer->wholeTime;
			asmThreadTimes[i][j] = bmpEditorPointer->threadsTime;
		}
		os << i;
		OutputDebugStringA(os.str().c_str());
		

	}
	
	for (int i = 0; i < 64; i++)	//cores
	{
		for (int j = 0; j < statisticsRuns - 1; j++)	//statistics
		{
			outStreamCppTotal << cppWholeTimes[i][j] << ",";
			outStreamCppThreads << cppThreadTimes[i][j] << ",";
			outStreamAsmTotal << asmWholeTimes[i][j] << ",";
			outStreamAsmThreads << asmThreadTimes[i][j] << ",";
		}
		outStreamCppTotal << cppWholeTimes[i][statisticsRuns - 1] << std::endl;;
		outStreamCppThreads << cppThreadTimes[i][statisticsRuns - 1] << std::endl;;
		outStreamAsmTotal << asmWholeTimes[i][statisticsRuns - 1] << std::endl;;
		outStreamAsmThreads << asmThreadTimes[i][statisticsRuns - 1] << std::endl;;
	}

	outStreamCppTotal.close();
	outStreamCppThreads.close();
	outStreamAsmTotal.close();
	outStreamAsmThreads.close();
	
	//std::string toBeParsed = bmpEditor.runAlgorithm(AlgorithmType::asmAlgorithm, ui.coreHorizontalSlider->value());
}
