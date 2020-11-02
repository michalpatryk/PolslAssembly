#pragma once
#include "BMPEditor.h"
#include <vector>

class Benchmark
{
	int statisticsRuns = 10;
	BMPEditor* bmpEditorPointer;
	std::vector<std::vector<unsigned long long>> cppWholeTimes;
	std::vector<std::vector<unsigned long long>> cppThreadTimes;
	std::vector<std::vector<unsigned long long>> asmWholeTimes;
	std::vector<std::vector<unsigned long long>> asmThreadTimes;
public:
	Benchmark(BMPEditor* bmpEditorPointer) : bmpEditorPointer(bmpEditorPointer) {};
	void run();
};

