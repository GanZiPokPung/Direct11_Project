#include "pch.h"
#include "Graphic.h"
#include "Direct3D.h"

Graphic::Graphic()
{
}

Graphic::Graphic(const Graphic& other)
{
}

Graphic::~Graphic()
{
}

bool Graphic::Initialize(int width, int height, HWND hwnd)
{
	m_Direct3D = (Direct3D*)_aligned_malloc(sizeof(Direct3D), 16);

	if (!m_Direct3D->Initialize(width, height, VSYNC_ENABLED, hwnd, FULL_SCREEN,
		SCREEN_FAR, SCREEN_NEAR))
	{
		MSG_ERROR(hwnd, L"Could not initialize Direct3D!");
		return false;
	}

	return true;
}

void Graphic::Shutdown()
{
	if (nullptr != m_Direct3D)
	{
		m_Direct3D->Shutdown();
		_aligned_free(m_Direct3D);
		m_Direct3D = nullptr;
	}
}

bool Graphic::Frame()
{
	return Render();
}

bool Graphic::Render()
{
	m_Direct3D->BeginScene(0.f, 0.f, 1.f, 1.f);
	m_Direct3D->EndScene();

	return true;
}
