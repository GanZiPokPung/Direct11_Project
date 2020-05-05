#include "pch.h"
#include "InputDX.h"


InputDX::InputDX()
{
}

InputDX::~InputDX()
{
}

bool InputDX::Initialize(HINSTANCE hInst, HWND hWnd, int screenWidth, int screenHeight)
{
	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;

	// Direct Input �������̽��� �ʱ�ȭ �Ѵ�.
	if (FAILED(DirectInput8Create(hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)& m_DirectInput, NULL)))
		return false;

	// Ű������ Direct Input �������̽��� ����
	if(FAILED(m_DirectInput->CreateDevice(GUID_SysKeyboard, &m_Keyboard, NULL)))
		return false;

	// ������ ������ �����Ѵ�. �� ��� Ű�����̹Ƿ� ���� ���� �� ������ ������ ����� �� �ִ�.
	if (FAILED(m_Keyboard->SetDataFormat(&c_dfDIKeyboard)))
		return false;

	// �ٸ� ���α׷��� �������� �ʵ��� Ű������ ���� ������ �����Ѵ�.
	if (FAILED(m_Keyboard->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE)))
		return false;

	// Ű���带 �Ҵ�޴´�.
	//if (FAILED(m_Keyboard->Acquire()))
		//return false;

	// // ���콺 Direct Input �������̽��� ����
	if (FAILED(m_DirectInput->CreateDevice(GUID_SysMouse, &m_Mouse, NULL)))
		return false;

	//�̸� ���� �� ���콺 ������ ������ ����Ͽ� ���콺�� ������ ������ ����
	if (FAILED(m_Mouse->SetDataFormat(&c_dfDIMouse)))
		return false;

	// �ٸ� ���α׷��� ���� �� �� �ֵ��� ���콺�� ���� ������ ����
	if (FAILED(m_Mouse->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE)))
		return false;

	// ���콺�� �Ҵ�޴´�.
	//if (FAILED(m_Mouse->Acquire()))
		//return false;

	return true;
}

void InputDX::Shutdown()
{
	if (nullptr != m_Mouse)
	{
		m_Mouse->Unacquire();
		SAFE_RELEASE(m_Mouse);
	}

	if (nullptr != m_Keyboard)
	{
		m_Keyboard->Unacquire();
		SAFE_RELEASE(m_Keyboard);
	}

	SAFE_RELEASE(m_DirectInput);
}

bool InputDX::Frame()
{
	// Ű������ ���� ���¸� �д´�.
	if (!ReadKeyboard())
		return false;
	if (!ReadMouse())
		return false;
	ProcessInput();

	return true;
}

bool InputDX::IsEscapePressed()
{
	if (m_keyboardState[DIK_ESCAPE] & 0x80)
		return true;

	return false;
}

bool InputDX::IsLeftArrowPressed()
{
	if (m_keyboardState[DIK_LEFTARROW] & 0x80)
		return true;

	return false;
}

bool InputDX::IsRightArrowPressed()
{
	if (m_keyboardState[DIK_RIGHTARROW] & 0x80)
		return true;

	return false;
}

void InputDX::GetMouseLocation(int& x, int& y)
{
	x = m_mouseX;
	y = m_mouseY;
}

bool InputDX::ReadKeyboard()
{
	HRESULT result = m_Keyboard->GetDeviceState(sizeof(m_keyboardState), (LPVOID)& m_keyboardState);
	if (FAILED(result))
	{
		// Ű���� ��Ŀ���� �Ҿ��ų� ȹ������ ���� ��� ��Ʈ���� �ٽ� �����´�.
		if (DIERR_INPUTLOST == result || DIERR_NOTACQUIRED == result)
			m_Keyboard->Acquire();
		else
			return false;
	}

	return true;
}

bool InputDX::ReadMouse()
{
	HRESULT result = m_Mouse->GetDeviceState(sizeof(m_mouseState), (LPVOID)& m_mouseState);
	if (FAILED(result))
	{
		// Ű���� ��Ŀ���� �Ҿ��ų� ȹ������ ���� ��� ��Ʈ���� �ٽ� �����´�.
		if (DIERR_INPUTLOST == result || DIERR_NOTACQUIRED == result)
			m_Mouse->Acquire();
		else
			return false;
	}

	return true;
}

void InputDX::ProcessInput()
{
	m_mouseX += m_mouseState.lX;
	m_mouseY += m_mouseState.lY;

	if (m_mouseX < 0) { m_mouseX = 0; }
	if (m_mouseY < 0) { m_mouseY = 0; }

	if (m_mouseX > m_screenWidth) { m_mouseX = m_screenWidth; }
	if (m_mouseY > m_screenHeight) { m_mouseY = m_screenHeight; }
}
