#include "pch.h"
#include "Graphic.h"
#include "Direct3D.h"
#include "Camera.h"
#include "Model.h"
#include "ColorShader.h"
#include "ModelTexture.h"
#include "TextureShader.h"
#include "Light.h"
#include "LightShader.h"
#include "ModelLight.h"
#include "Input.h"

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

#ifdef LIGHT_MODE
	m_LightShader = new LightShader;
	if (!m_LightShader->Initialize(m_Direct3D->GetDevice(), hwnd))
	{
		MSG_ERROR(hwnd, L"Could not initialize shader! : Light");
		return false;
	}
	m_ModelLight = new ModelLight;
	if (!m_ModelLight->Initialize(m_Direct3D->GetDevice(), L"../Project_KSB/data/seafloor.dds"))
	{
		MSG_ERROR(hwnd, L"Could not initialize the model object.");
		return false;
	}
	m_Light = new Light;
	m_Light->SetDiffuseColor(1.f, 1.f, 1.f, 1.f);
	m_Light->SetDirection(1.f, 0.f, 1.f);
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
#ifdef LIGHT_MODE
	SAFE_SHUTDOWN(m_LightShader);
	SAFE_SHUTDOWN(m_ModelLight);
	SAFE_DELETE(m_Light);
#endif
	SAFE_DELETE(m_Camera);
}

bool Graphic::Frame()
{
	static float rotation = 0.f;

	rotation += (float)XM_PI * 0.01f;
	if (rotation > 360.f)
		rotation -= 360.f;

	return Render(rotation);
}

bool Graphic::UpdateInput(Input* input)
{
	if (nullptr == input)
		return false;

	if (input->IsKeyDown('W'))
	{
		XMFLOAT3 camPos = m_Camera->GetPosition();
		m_Camera->SetPosition(camPos.x, camPos.y, camPos.z + 0.1f);
	}
	else if(input->IsKeyDown('S'))
	{
		XMFLOAT3 camPos = m_Camera->GetPosition();
		m_Camera->SetPosition(camPos.x, camPos.y, camPos.z - 0.1f);
	}

	return true;
}

bool Graphic::Render(float rotation)
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
#ifdef LIGHT_MODE
	world = XMMatrixRotationY(rotation);
	m_ModelLight->Render(m_Direct3D->GetDeviceContext());
	m_LightShader->Render(m_Direct3D->GetDeviceContext(), m_ModelLight->GetIndexCount(),
		world, view, proj, m_ModelLight->GetTexture(), m_Light->GetDirection(), m_Light->GetDiffuseColor());
#endif

	m_Direct3D->EndScene();

	return true;
}
