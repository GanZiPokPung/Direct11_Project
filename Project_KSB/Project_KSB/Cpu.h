#pragma once

#pragma comment(lib, "pdh.lib")
#include <pdh.h>

class Cpu
{
public:
	Cpu();
	Cpu(const Cpu& other);
	~Cpu();

public:
	int GetCpuPercentage();

public:
	void Initialize();
	void Shutdown();
	void Frame();
	
private:
	bool m_canReadCpu = true;
	HQUERY m_queryHandle;
	HCOUNTER m_counterHandle;
	ULONG m_lastSampleTime = 0;
	long m_cpuUsage = 0;
};

