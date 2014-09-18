#include "Timer.h"

bool Timer::init()
{
	if( !QueryPerformanceFrequency(&frequency))
		return false;

	return true;
}

void Timer::start()
{
	QueryPerformanceCounter(&startTime);
}

void Timer::end()
{
	LARGE_INTEGER t;
	
	QueryPerformanceCounter(&t);

	elapsedTime.QuadPart = t.QuadPart - startTime.QuadPart;

	elapsedTime.QuadPart *= 1000000;
	elapsedTime.QuadPart /= frequency.QuadPart;
}

double Timer::getTimeElapsed() const
{
	return (double)elapsedTime.QuadPart;
}