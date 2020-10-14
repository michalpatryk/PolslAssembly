#include "PerformanceTimer.h"

PerformanceTimer::PerformanceTimer()
{
	QueryPerformanceFrequency(&li);
	counterFrequency = li.QuadPart;
}

void PerformanceTimer::start()
{
	QueryPerformanceCounter(&li);
	//init start, last and total ticks
	counterStart = li.QuadPart;
	counterLast = li.QuadPart;
	counterTotalTicks = 0;
}

void PerformanceTimer::stop()
{
	QueryPerformanceCounter(&li);
	//increase total ticks, set end
	counterTotalTicks += li.QuadPart - counterLast;
	counterEnd = li.QuadPart;
	counterLast = li.QuadPart;
}

void PerformanceTimer::pause()
{
	QueryPerformanceCounter(&li);
	//increase total ticks, set counter last
	counterTotalTicks += li.QuadPart - counterLast;
	counterLast = li.QuadPart;
}

void PerformanceTimer::resume()
{
	QueryPerformanceCounter(&li);
	//set counterLast
	counterLast = li.QuadPart;
}
