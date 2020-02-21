#pragma once

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