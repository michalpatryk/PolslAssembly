#include "Benchmark.h"
#include <fstream>
void Benchmark::run()
{

	cppWholeTimes = std::vector<std::vector<unsigned long long>>(64, std::vector<unsigned long long>(statisticsRuns, 0));
	cppThreadTimes = std::vector<std::vector<unsigned long long>>(64, std::vector<unsigned long long>(statisticsRuns, 0));
	asmWholeTimes = std::vector<std::vector<unsigned long long>>(64, std::vector<unsigned long long>(statisticsRuns, 0));
	asmThreadTimes = std::vector<std::vector<unsigned long long>>(64, std::vector<unsigned long long>(statisticsRuns, 0));
	std::ofstream outStreamCppTotal("benchmarkCppTotal.txt");
	std::ofstream outStreamCppThreads("benchmarkCppTotalThreads.txt");
    std::ofstream outStreamAsmTotal("benchmarkAsmTotal.txt");
	std::ofstream outStreamAsmThreads("benchmarkAsmThreads.txt");
	for (int i = 1; i <= 64; i++)	//cores
	{
		
		for (int j = 0; j < statisticsRuns; j++)	//statistics
		{
			bmpEditorPointer->runAlgorithm(AlgorithmType::cppAlgorithm, i);
			cppWholeTimes[i][j] = bmpEditorPointer->wholeTime;
			cppThreadTimes[i][j] = bmpEditorPointer->threadsTime;
		}
		for (int j = 0; j < statisticsRuns; j++)	//statistics
		{
			bmpEditorPointer->runAlgorithm(AlgorithmType::asmAlgorithm, i);
			cppWholeTimes[i][j] = bmpEditorPointer->wholeTime;
			cppThreadTimes[i][j] = bmpEditorPointer->threadsTime;
		}
		
		
	}
	
	for (int i = 1; i <= 64; i++)	//cores
	{
		for (int j = 0; j < statisticsRuns; j++)	//statistics
		{
			outStreamCppTotal << cppWholeTimes[i][j] << ",";
			outStreamCppThreads << cppThreadTimes[i][j] << ",";
			outStreamAsmTotal << asmWholeTimes[i][j] << ",";
			outStreamAsmThreads << asmThreadTimes[i][j] << ",";
		}
		outStreamCppTotal << ";" << std::endl;
		outStreamCppThreads << ";" << std::endl;
		outStreamAsmTotal << ";" << std::endl;
		outStreamAsmThreads << ";" << std::endl;
	}
	//outStreamAsm.
	outStreamCppTotal.close();
	outStreamCppThreads.close();
	outStreamAsmTotal.close();
	outStreamAsmThreads.close();
	
	//std::string toBeParsed = bmpEditor.runAlgorithm(AlgorithmType::asmAlgorithm, ui.coreHorizontalSlider->value());
}
