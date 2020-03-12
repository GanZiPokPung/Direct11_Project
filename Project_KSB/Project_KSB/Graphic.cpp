#include "pch.h"
#include "Graphic.h"
#include "Direct3D.h"
#include "Camera.h"
#include "Model.h"
#include "ColorShader.h"
#include "ModelTexture.h"
#include "TextureShader.h"

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

#ifdef COLOR_MODE
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
#endif

#ifdef TEXTURE_MODE
	m_TextureShader = new TextureShader;
	if (!m_TextureShader->Initialize(m_Direct3D->GetDevice(), hwnd))
	{
		MSG_ERROR(hwnd, L"Could not initialize shader! : Texture");
		return false;
	}
	m_ModelTexture = new ModelTexture;
	if (!m_ModelTexture->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(),
		"../Project_KSB/data/stone01.tga"))
	{
		MSG_ERROR(hwnd, L"Could not initialize the model object.");
		return false;
	}
#endif

	m_Camera = new Camera;
	m_Camera->SetPosition(0.f, 0.f, -5.f);

	return true;
}

void Graphic::Shutdown()
{
	SAFE_SHUTDOWN(m_Direct3D);
#ifdef COLOR_MODE
	SAFE_SHUTDOWN(m_ColorShader);
	SAFE_SHUTDOWN(m_Model);
#endif
#ifdef TEXTURE_MODE
	SAFE_SHUTDOWN(m_TextureShader);
	SAFE_SHUTDOWN(m_ModelTexture);
#endif
	SAFE_DELETE(m_Camera);
}

bool Graphic::Frame()
{
	return Render();
}

bool Graphic::Render()
{
	m_Direct3D->BeginScene(0.f, 0.f, 0.f, 1.f);

	m_Camera->Render();

	XMMATRIX world, view, proj;
	m_Direct3D->GetWorldMatrix(world);
	m_Camera->GetViewMatrix(view);
	m_Direct3D->GetProjectionMatrix(proj);

#ifdef COLOR_MODE
	m_Model->Render(m_Direct3D->GetDeviceContext());
	m_ColorShader->Render(m_Direct3D->GetDeviceContext(), 
		m_Model->GetIndexCount(), world, view, proj);
#endif
#ifdef TEXTURE_MODE
	m_ModelTexture->Render(m_Direct3D->GetDeviceContext());
	m_TextureShader->Render(m_Direct3D->GetDeviceContext(), m_ModelTexture->GetIndexCount(),
		world, view, proj, m_ModelTexture->GetTexture());
#endif

	m_Direct3D->EndScene();

	return true;
}
