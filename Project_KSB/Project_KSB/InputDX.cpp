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

	// Direct Input 인터페이스를 초기화 한다.
	if (FAILED(DirectInput8Create(hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)& m_DirectInput, NULL)))
		return false;

	// 키보드의 Direct Input 인터페이스를 생성
	if(FAILED(m_DirectInput->CreateDevice(GUID_SysKeyboard, &m_Keyboard, NULL)))
		return false;

	// 데이터 형식을 설정한다. 이 경우 키보드이므로 사전 정의 된 데이터 형식을 사용할 수 있다.
	if (FAILED(m_Keyboard->SetDataFormat(&c_dfDIKeyboard)))
		return false;

	// 다른 프로그램과 공유하지 않도록 키보드의 협조 수준을 결정한다.
	if (FAILED(m_Keyboard->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE)))
		return false;

	// 키보드를 할당받는다.
	//if (FAILED(m_Keyboard->Acquire()))
		//return false;

	// // 마우스 Direct Input 인터페이스를 생성
	if (FAILED(m_DirectInput->CreateDevice(GUID_SysMouse, &m_Mouse, NULL)))
		return false;

	//미리 정의 된 마우스 데이터 형식을 사용하여 마우스의 데이터 형식을 설정
	if (FAILED(m_Mouse->SetDataFormat(&c_dfDIMouse)))
		return false;

	// 다른 프로그램과 공유 할 수 있도록 마우스의 협력 수준을 결정
	if (FAILED(m_Mouse->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE)))
		return false;

	// 마우스를 할당받는다.
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
	// 키보드의 현재 상태를 읽는다.
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
		// 키보드 포커스를 잃었거나 획득하지 않은 경우 컨트롤을 다시 가져온다.
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
		// 키보드 포커스를 잃었거나 획득하지 않은 경우 컨트롤을 다시 가져온다.
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
