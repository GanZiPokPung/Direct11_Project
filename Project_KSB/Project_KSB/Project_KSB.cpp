// Project_KSB.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "pch.h"
#include "framework.h"
#include "Project_KSB.h"
#include "System.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	System *system = new System;
	
	if (true == system->Initialize())
		system->Run();

	SAFE_SHUTDOWN(system);

	return 0;
}