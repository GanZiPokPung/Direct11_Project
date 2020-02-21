#pragma once

class Input;
class Graphic;
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

	Input*		m_Input = nullptr;
	Graphic*	m_Graphic = nullptr;
};

static LRESULT CALLBACK WndProc(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam);
static System* ApplicationHandle = nullptr;

