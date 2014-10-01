#include "HighresTimer.hpp"

#ifdef _WIN32
#include <Windows.h>
#endif

HighresTimer::HighresTimer()
{
#ifndef WCLOCK
	m_totalStart = std::chrono::high_resolution_clock::now();
#else
	LARGE_INTEGER frequency;
	LARGE_INTEGER start;
	QueryPerformanceFrequency(&frequency);
	QueryPerformanceCounter(&start);
	m_frequency = frequency.QuadPart;
	m_start = start.QuadPart;
#endif
}

void HighresTimer::Start()
{
#ifndef WCLOCK
	m_start = std::chrono::high_resolution_clock::now();
#else
	LARGE_INTEGER start;
	QueryPerformanceCounter(&start);
	m_start = start.QuadPart;
#endif
}

void HighresTimer::Stop()
{
#ifndef WCLOCK
	m_end = std::chrono::high_resolution_clock::now();
#else
	LARGE_INTEGER end;
	QueryPerformanceCounter(&end);
	m_end = end.QuadPart;
#endif
}

double HighresTimer::GetDelta()
{
#ifndef WCLOCK
	return std::chrono::duration_cast<std::chrono::microseconds>(m_end - m_start).count() / 1000000.0;
#else
	return (m_end - m_start) / static_cast<double>(m_frequency);
#endif
}

long long HighresTimer::GetTotal()
{
#ifndef WCLOCK
	std::chrono::high_resolution_clock::time_point now = std::chrono::high_resolution_clock::now();
	return std::chrono::duration_cast<std::chrono::milliseconds>(now - m_totalStart).count();
#else
	LARGE_INTEGER now;
	QueryPerformanceCounter(&now);
	long long elapsed = 1000 * (now.QuadPart - m_totalStart) / m_frequency;
	return elapsed;
#endif
}