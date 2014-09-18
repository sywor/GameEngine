#pragma once

#include <Windows.h>

class Timer
{
private:
	LARGE_INTEGER frequency, startTime, elapsedTime;
protected:
public:
	bool init();
	void start();
	void end();

	double getTimeElapsed() const;	
};