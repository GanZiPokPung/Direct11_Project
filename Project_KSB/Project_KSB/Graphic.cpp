#include "pch.h"
#include "Graphic.h"
#include "Direct3D.h"
#include "Camera.h"
#include "Model.h"
#include "ColorShader.h"

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
	m_Direct3D = new Direct3D;
	if (!m_Direct3D->Initialize(width, height, VSYNC_ENABLED, hwnd, FULL_SCREEN,
		SCREEN_FAR, SCREEN_NEAR))
	{
		MSG_ERROR(hwnd, L"Could not initialize Direct3D!");
		return false;
	}

	m_ColorShader = new ColorShader;
	if (!m_ColorShader->Initialize(m_Direct3D->GetDevice(), hwnd))
	{
		MSG_ERROR(hwnd, L"Could not initialize shader! : Color");
		return false;
	}

	m_Model = new Model;
	if (!m_Model->Initialize(m_Direct3D->GetDevice()))
	{
		MSG_ERROR(hwnd, L"Could not initialize Model!");
		return false;
	}

	m_Camera = new Camera;
	m_Camera->SetPosition(0.f, 0.f, -5.f);

	return true;
}

void Graphic::Shutdown()
{
	SAFE_SHUTDOWN(m_Direct3D);
	SAFE_SHUTDOWN(m_ColorShader);
	SAFE_SHUTDOWN(m_Model);
	SAFE_DELETE(m_Camera);
}

bool Graphic::Frame()
{
	return Render();
}

bool Graphic::Render()
{
	m_Direct3D->BeginScene(0.f, 0.f, 1.f, 1.f);

	m_Camera->Render();

	XMMATRIX world, view, proj;
	m_Direct3D->GetWorldMatrix(world);
	m_Camera->GetViewMatrix(view);
	m_Direct3D->GetProjectionMatrix(proj);

	m_Model->Render(m_Direct3D->GetDeviceContext());
	m_ColorShader->Render(m_Direct3D->GetDeviceContext(), 
		m_Model->GetIndexCount(), world, view, proj);

	m_Direct3D->EndScene();

	return true;
}
