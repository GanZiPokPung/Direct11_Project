#include "pch.h"
#include "Timer.h"

Timer::Timer()
{
}

Timer::Timer(const Timer& other)
{
}

Timer::~Timer()
{
}

bool Timer::Initialize()
{
	QueryPerformanceFrequency((LARGE_INTEGER*)& m_frequency);
	if (0 == m_frequency.QuadPart)
		return false;

	m_ticksPerMs = (float)(m_frequency.QuadPart / SECOND);
	QueryPerformanceCounter((LARGE_INTEGER*)& m_startTime);
	return true;
}

void Timer::Frame()
{
	LARGE_INTEGER currentTime;
	QueryPerformanceCounter((LARGE_INTEGER*)& currentTime);
	float timeDifference = (float)(currentTime.QuadPart - m_startTime.QuadPart);
	m_frameTime = timeDifference / m_ticksPerMs;
	m_startTime = currentTime;
}
