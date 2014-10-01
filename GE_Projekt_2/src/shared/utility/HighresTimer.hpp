#ifndef SRC_SHARED_UTILITY_TIMER_HPP
#define SRC_SHARED_UTILITY_TIMER_HPP

#include <chrono>

#ifdef _WIN32
#define WCLOCK
#endif


class HighresTimer
{
#ifndef WCLOCK
	std::chrono::high_resolution_clock::time_point m_start, m_end;
	std::chrono::high_resolution_clock::time_point m_totalStart;
#else
private:

	long long m_frequency;        // ticks per second
	long long m_totalStart;
	long long m_start, m_end;           // ticks
#endif
	long long m_totalTime;
public:
	HighresTimer();
	/*!
	Starts measuring time
	*/
	void Start();

	/*!
	Stops measuring time
	*/
	void Stop();

	/*!
	Gets the delta time
	\return Returns time in seconds passed between calling Start and Stop
	*/
	double GetDelta();

	/*!
	Gets the total time
	\return Returns the total time passed since the application started
	*/
	long long GetTotal();

};
#endif