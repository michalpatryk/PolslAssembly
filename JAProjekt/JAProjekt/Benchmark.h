#pragma once
#include "BMPEditor.h"
#include <vector>
//debug class - used to run benchamarks on the app
class Benchmark
{
	int statisticsRuns = 10;	//changing this variable changes the amount of benchmark runs on each core (default 10)
	BMPEditor* bmpEditorPointer;
	std::vector<std::vector<unsigned long long>> cppWholeTimes;
	std::vector<std::vector<unsigned long long>> cppThreadTimes;
	std::vector<std::vector<unsigned long long>> asmWholeTimes;
	std::vector<std::vector<unsigned long long>> asmThreadTimes;
public:
	Benchmark(BMPEditor* bmpEditorPointer) : bmpEditorPointer(bmpEditorPointer) {};
	void run();
};

