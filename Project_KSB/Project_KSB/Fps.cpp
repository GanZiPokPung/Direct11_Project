#include "pch.h"
#include <mmsystem.h>
#include "Fps.h"

Fps::Fps()
{
}

Fps::Fps(const Fps& other)
{
}

Fps::~Fps()
{
}

void Fps::Initialize()
{
	m_startTime = timeGetTime();
}

void Fps::Frame()
{
	++m_count;

	if (timeGetTime() - m_startTime >= SECOND)
	{
		m_fps = m_count;
		m_count = 0;

		m_startTime = timeGetTime();
	}
}
