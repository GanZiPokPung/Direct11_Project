#pragma once

inline void MSG_ERROR(HWND hwnd, const TCHAR* contents)
{
	MessageBox(hwnd, contents, L"Error", MB_ICONERROR);
}

template<typename T>
inline void Z_Memory(T& value)
{
	ZeroMemory(&value, sizeof(value));
}

template<typename T>
inline void SAFE_SHUTDOWN(T& value)
{
	if (nullptr != value)
	{
		value->Shutdown();
		delete value;
		value = nullptr;
	}
}

template<typename T>
inline void SAFE_RELEASE(T& value)
{
	if (nullptr != value)
	{
		value->Release();
		value = nullptr;
	}
}


template<typename T>
inline void SAFE_DELETE(T& value)
{
	if (nullptr != value)
	{
		delete value;
		value = nullptr;
	}
}

template<typename T>
inline void SAFE_DELETE_ARRAY(T& value)
{
	if (nullptr != value)
	{
		delete[] value;
		value = nullptr;
	}
}