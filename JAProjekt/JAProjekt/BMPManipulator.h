#pragma once
#include <string>
#include "JAProjekt.h"
enum class algorithmType
{
	cpp,
	assembly
};
class BMPManipulator
{
	std::string filename{};
	
public:
	BMPManipulator() {};
	void setFilename(std::string filename_) { filename = filename_; };
	std::string runAlgorithm(algorithmType algSelector);

};

