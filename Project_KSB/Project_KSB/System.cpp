#include "pch.h"
#include "System.h"
#include "Graphic.h"
#include "Input.h"
#include "Fps.h"
#include "Cpu.h"
#include "Timer.h"
#include "InputDX.h"
#include "Sound.h"

System::System()
{
}

System::System(const System& other)
{
}

System::~System()
{
}

bool System::Initialize()
{
	int screenWidth = 0;
	int screenHeight = 0;

	InitializeWindows(screenWidth, screenHeight);

	m_InputDX = new InputDX;
	if (!m_InputDX->Initialize(m_hInstance, m_hWnd, screenWidth, screenHeight))
		return false;

	m_Graphic = new Graphic;
	if (!m_Graphic->Initialize(screenWidth, screenHeight, m_hWnd))
		return false;

	m_Sound = new Sound;
	if (!m_Sound->Initialize(m_hWnd))
		return false;

	m_Fps = new Fps;
	m_Fps->Initialize();

	m_Cpu = new Cpu;
	m_Cpu->Initialize();

	m_Timer = new Timer;
	if (!m_Timer->Initialize())
		return false;

	return true;
}

void System::Shutdown()
{
	SAFE_SHUTDOWN(m_Graphic);
	SAFE_SHUTDOWN(m_InputDX);
	SAFE_SHUTDOWN(m_Sound);
	SAFE_SHUTDOWN(m_Cpu);
	SAFE_DELETE(m_Timer);
	SAFE_DELETE(m_Fps);


	ShutdownWindows();
}

void System::Run()
{
	MSG msg;
	Z_Memory(msg);

	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (WM_QUIT == msg.message)
				break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			if (!Frame())
				break;
		}

		if (m_InputDX->IsEscapePressed())
			break;
	}
}

LRESULT System::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	return DefWindowProc(hwnd, umsg, wparam, lparam);
}

bool System::Frame()
{
	int mouseX = 0, mouseY = 0;

	if (!m_InputDX->Frame())
		return false;

	m_Timer->Frame();
	m_Fps->Frame();
	m_Cpu->Frame();

	m_InputDX->GetMouseLocation(mouseX, mouseY);
	//m_InputDX->GetKeyState();

	if (!m_Graphic->Frame(mouseX, mouseY))
		return false;

	return m_Graphic->Render();
}

void System::InitializeWindows(int& width, int& height)
{
	// �ܺ� ������
	ApplicationHandle = this;
	// �ν��Ͻ�
	m_hInstance = GetModuleHandle(NULL);
	// �̸�
	m_applicationName = L"Project_KBS";

	// ������ Ŭ���� ����
	WNDCLASSEX wc;
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = m_applicationName;
	wc.cbSize = sizeof(WNDCLASSEX);

	// ������ Ŭ���� ���
	RegisterClassEx(&wc);

	// �ػ�
	/*width = GetSystemMetrics(SM_CXSCREEN);
	height = GetSystemMetrics(SM_CYSCREEN);*/

	width = WINDOW_WIDTH;
	height = WINDOW_HEIGHT;

	int posX = 0;
	int posY = 0;

	if (FULL_SCREEN)
	{
		// Ǯ��ũ�� ���� �����ߴٸ� ȭ�� �ػ󵵸� ����ũ�� �ػ󵵷� �����ϰ� 
		// ������ 32bit�� ����
		DEVMODE dmScreenSetting;
		memset(&dmScreenSetting, 0, sizeof(DEVMODE));
		dmScreenSetting.dmSize = sizeof(dmScreenSetting);
		dmScreenSetting.dmPelsWidth = (unsigned long)width;
		dmScreenSetting.dmPelsHeight = (unsigned long)height;
		dmScreenSetting.dmBitsPerPel = 32;
		dmScreenSetting.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		ChangeDisplaySettings(&dmScreenSetting, CDS_FULLSCREEN);
	}
	else
	{
		posX = (GetSystemMetrics(SM_CXSCREEN) - width) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - height) / 2;
	}

	// �����츦 �����ϰ� �ڵ��� ���Ѵ�.
	m_hWnd = CreateWindowEx(WS_EX_APPWINDOW, m_applicationName, m_applicationName,
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
		posX, posY, width, height, NULL, NULL, m_hInstance, NULL);

	ShowWindow(m_hWnd, SW_SHOW);
	SetForegroundWindow(m_hWnd);
	SetFocus(m_hWnd);
}

void System::ShutdownWindows()
{
	if (FULL_SCREEN)
	{
		// Ǯ��ũ�� ��� ���÷��� ���� �ʱ�ȭ
		ChangeDisplaySettings(NULL, 0);
	}

	DestroyWindow(m_hWnd);
	m_hWnd = NULL;

	UnregisterClass(m_applicationName, m_hInstance);
	m_hInstance = NULL;

	ApplicationHandle = NULL;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	switch (umsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;

	default:
		return ApplicationHandle->MessageHandler(hwnd, umsg, wparam, lparam);

	}
}
