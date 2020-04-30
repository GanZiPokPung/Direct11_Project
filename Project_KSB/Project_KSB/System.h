#pragma once

//#define SOUND_MODE

class Graphic;
class Fps;
class Cpu;
class Timer;
class InputDX;
class Sound;
class System
{
public:
	System();
	System(const System& other);
	~System();

	bool Initialize();
	void Shutdown();
	void Run();

	LRESULT CALLBACK MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam);

private:
	bool Frame();
	void InitializeWindows(int& width, int& height);
	void ShutdownWindows();

private:
	LPCWSTR		m_applicationName;
	HINSTANCE	m_hInstance;
	HWND		m_hWnd;

	InputDX* m_InputDX = nullptr;
	Graphic*	m_Graphic = nullptr;
	Fps*        m_Fps = nullptr;
	Cpu*        m_Cpu = nullptr;
	Timer*      m_Timer = nullptr;
#ifdef SOUND_MODE
	Sound*		m_Sound = nullptr;
#endif
};

static LRESULT CALLBACK WndProc(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam);
static System* ApplicationHandle = nullptr;

