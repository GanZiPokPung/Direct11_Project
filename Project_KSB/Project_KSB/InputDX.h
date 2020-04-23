#pragma once

class InputDX
{
public:
	InputDX();
	~InputDX();

public:
	bool Initialize(HINSTANCE hInst, HWND hWnd, int screenWidth, int screenHeight);
	void Shutdown();
	bool Frame();

	bool IsEscapePressed();
	void GetMouseLocation(int& x, int& y);

private:
	bool ReadKeyboard();
	bool ReadMouse();
	void ProcessInput();

private:
	IDirectInput8* m_DirectInput = nullptr;
	IDirectInputDevice8* m_Keyboard = nullptr;
	IDirectInputDevice8* m_Mouse = nullptr;

	unsigned char m_keyboardState[256] = { 0, };
	DIMOUSESTATE m_mouseState;

	int m_screenWidth = 0;
	int m_screenHeight = 0;
	int m_mouseX = 0;
	int m_mouseY = 0;
};

