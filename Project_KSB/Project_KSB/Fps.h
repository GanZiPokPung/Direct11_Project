#pragma once

#pragma comment(lib, "winmm.lib")
class Fps
{
public:
	Fps();
	Fps(const Fps& other);
	~Fps();

public:
	int GetFps() { return m_fps; }

public:
	void Initialize();
	void Frame();

private:
	int m_fps = 0;
	int m_count = 0;
	ULONG m_startTime = 0;
};

