#include "pch.h"
#include "Cpu.h"

Cpu::Cpu()
{
}

Cpu::Cpu(const Cpu& other)
{
}

Cpu::~Cpu()
{
}

int Cpu::GetCpuPercentage()
{
	int usage = 0;

	if (m_canReadCpu)
		usage = (int)m_cpuUsage;

	return usage;
}

void Cpu::Initialize()
{
	// CPU 사용을 폴링하는 쿼리 개체
	PDH_STATUS status = PdhOpenQuery(NULL, 0, &m_queryHandle);
	if (ERROR_SUCCESS != status)
		m_canReadCpu = false;

	status = PdhAddCounter(m_queryHandle, TEXT("\\Processor(_Total)\\% processor time"), 0, &m_counterHandle);
	if (ERROR_SUCCESS != status)
		m_canReadCpu = false;

	m_lastSampleTime = GetTickCount();
	m_cpuUsage = 0;
}

void Cpu::Shutdown()
{
	if (m_canReadCpu)
		PdhCloseQuery(m_queryHandle);
}

void Cpu::Frame()
{
	PDH_FMT_COUNTERVALUE value;

	if (m_canReadCpu)
	{
		if ((m_lastSampleTime + SECOND) < GetTickCount())
		{
			m_lastSampleTime = GetTickCount();

			PdhCollectQueryData(m_queryHandle);

			PdhGetFormattedCounterValue(m_counterHandle, PDH_FMT_LONG, NULL, &value);

			m_cpuUsage = (long)value.largeValue;
		}
	}
}
