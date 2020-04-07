#pragma once

class Timer
{
public:
	Timer();
	Timer(const Timer& other);
	~Timer();

public:
	float GetTime() { return m_frameTime; }

public:
	bool Initialize();
	void Frame();

private:
	LARGE_INTEGER m_frequency;
	float m_ticksPerMs = 0;
	LARGE_INTEGER m_startTime;
	float m_frameTime = 0;
};

