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
#include "Model3D.h"
#include "Input.h"
#include "ModelBitmap.h"
#include "Text.h"
#include "Frustum.h"
#include "ModelList.h"


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
	/*m_ModelLight = new ModelLight;
	if (!m_ModelLight->Initialize(m_Direct3D->GetDevice(), L"../Project_KSB/data/seafloor.dds"))
	{
		MSG_ERROR(hwnd, L"Could not initialize the model object.");
		return false;
	}*/
	m_Model3D = new Model3D;
	if (!m_Model3D->Initialize(m_Direct3D->GetDevice(), "../Project_KSB/data/cube.txt", L"../Project_KSB/data/seafloor.dds"))
	{
		MSG_ERROR(hwnd, L"Could not initialize the model object.");
		return false;
	}
	m_Light = new Light;
	m_Light->SetAmbientColor(0.15f, 0.15f, 0.15f, 1.f);
	m_Light->SetDiffuseColor(1.f, 1.f, 1.f, 1.f);
	m_Light->SetDirection(0.f, 0.f, 1.f);
	m_Light->SetSpecularColor(0.5f, 0.5f, 0.5f, 1.f);
	m_Light->SetSpecularPower(32.f);
#endif

#ifdef TWO_D_MODE
	m_TextureShader = new TextureShader;
	if (!m_TextureShader->Initialize(m_Direct3D->GetDevice(), hwnd))
	{
		MSG_ERROR(hwnd, L"Could not initialize shader.");
		return false;
	}

	m_ModelBitmap = new ModelBitmap;
	if (!m_ModelBitmap->Initialize(m_Direct3D->GetDevice(), width, height, L"../Project_KSB/data/seafloor.dds", 256, 256))
	{
		MSG_ERROR(hwnd, L"Could not initialize the model object.");
		return false;
	}
#endif

	m_Camera = new Camera;
	m_Camera->SetPosition(0.f, 0.f, -1.f);
	m_Camera->Render();

	XMMATRIX view;
	m_Camera->GetViewMatrix(view);
	
#ifdef TEXT_MODE
	m_Text = new Text;
	if (!m_Text->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), hwnd, width, height, view))
	{
		MSG_ERROR(hwnd, L"Could not initialize the text object.");
		return false;
	}
#endif

#ifdef FRUSTUM_MODE
	m_Text = new Text;
	if (!m_Text->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), hwnd, width, height, view))
	{
		MSG_ERROR(hwnd, L"Could not initialize the text object.");
		return false;
	}

	m_Model = new Model3D;
	if (!m_Model->Initialize(m_Direct3D->GetDevice(), "../Project_KSB/data/cube.txt", L"../Project_KSB/data/seafloor.dds"))
	{
		MSG_ERROR(hwnd, L"Could not initialize the model object.");
		return false;
	}

	m_Light = new Light;
	m_Light->SetDirection(0.f, 0.f, 1.f);
	m_Light->SetAmbientColor(0.15f, 0.15f, 0.15f, 1.f);
	m_Light->SetDiffuseColor(1.f, 1.f, 1.f, 1.f);
	m_Light->SetDirection(0.f, 0.f, 1.f);
	m_Light->SetSpecularColor(0.5f, 0.5f, 0.5f, 1.f);
	m_Light->SetSpecularPower(32.f);

	m_LightShader = new LightShader;
	if (!m_LightShader->Initialize(m_Direct3D->GetDevice(), hwnd))
	{
		MSG_ERROR(hwnd, L"Could not initialize shader! : Light");
		return false;
	}

	m_ModelList = new ModelList;
	if (!m_ModelList->Initialize(25))
		return false;

	m_Frustum = new Frustum;
#endif

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
	//SAFE_SHUTDOWN(m_ModelLight);
	SAFE_SHUTDOWN(m_Model3D);
	SAFE_DELETE(m_Light);
#endif
#ifdef TWO_D_MODE
	SAFE_SHUTDOWN(m_TextureShader);
	SAFE_SHUTDOWN(m_ModelBitmap);
#endif
#ifdef TEXT_MODE
	SAFE_SHUTDOWN(m_Text);
#endif
#ifdef FRUSTUM_MODE
	SAFE_SHUTDOWN(m_LightShader);
	SAFE_SHUTDOWN(m_Model);
	SAFE_SHUTDOWN(m_ModelList);
	SAFE_SHUTDOWN(m_Text);
	SAFE_DELETE(m_Light);
#endif
	SAFE_DELETE(m_Camera);
}

bool Graphic::Frame(int fps, int cpu, float timeDelta, float rotationY)
{
	/*if (!m_Text->SetMousePosition(mouseX, mouseY, m_Direct3D->GetDeviceContext()))
		return false;*/

	if (!m_Text->SetFps(fps, m_Direct3D->GetDeviceContext()))
		return false;

	if (!m_Text->SetCpu(cpu, m_Direct3D->GetDeviceContext()))
		return false;

	m_Camera->SetPosition(0.f, 0.f, -10.f);
	m_Camera->SetRotation(0.f, rotationY, 0.f);
	return true;
}

bool Graphic::Render()
{
	m_Direct3D->BeginScene(0.f, 0.f, 1.f, 1.f);

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
	//m_ModelLight->Render(m_Direct3D->GetDeviceContext());
	m_Model3D->Render(m_Direct3D->GetDeviceContext());
	/*m_LightShader->Render(m_Direct3D->GetDeviceContext(), m_ModelLight->GetIndexCount(),
		world, view, proj, m_ModelLight->GetTexture(), m_Light->GetDirection(), m_Light->GetDiffuseColor());*/
	m_LightShader->Render(m_Direct3D->GetDeviceContext(), m_Model3D->GetIndexCount(),
		world, view, proj, m_Model3D->GetTexture(), m_Camera->GetPosition(),
		m_Light->GetDirection(), m_Light->GetDiffuseColor(), m_Light->GetAmbientColor(),
		m_Light->GetSpecularColor(), m_Light->GetSpecularPower());
#endif
#ifdef TWO_D_MODE
	XMMATRIX ortho;
	m_Direct3D->GetOrthoMatrix(ortho);

	m_Direct3D->TurnZBufferOff();

	if (!m_ModelBitmap->Render(m_Direct3D->GetDeviceContext(), 300, 300))
		return false;

	if (!m_TextureShader->Render(m_Direct3D->GetDeviceContext(), m_ModelBitmap->GetIndexCount(),
		world, view, ortho, m_ModelBitmap->GetTexture()))
		return false;

	m_Direct3D->TurnZBufferOn();

#endif
#ifdef TEXT_MODE
	XMMATRIX ortho;
	m_Direct3D->GetOrthoMatrix(ortho);
	m_Direct3D->TurnZBufferOff();
	m_Direct3D->TurnAlphaBlendingOn();

	if (!m_Text->Render(m_Direct3D->GetDeviceContext(), world, ortho))
		return false;

	m_Direct3D->TurnAlphaBlendingOff();
	m_Direct3D->TurnZBufferOn();
#endif

#ifdef FRUSTUM_MODE
	float posX = 0.f, posY = 0.f, posZ = 0.f;
	float radius = 1.f;
	XMFLOAT4 color;

	m_Frustum->ConstructFrustum(SCREEN_FAR, proj, view);

	int modelCount = m_ModelList->GetModelCount();
	int renderCount = 0;

	for (int i = 0; i < modelCount; ++i)
	{
		m_ModelList->GetData(i, posX, posY, posZ, color);

		if (m_Frustum->CheckCube(posX, posY, posZ, radius))
		{
			world = XMMatrixTranslation(posX, posY, posZ);
			m_Model->Render(m_Direct3D->GetDeviceContext());

			m_LightShader->Render(m_Direct3D->GetDeviceContext(), m_Model->GetIndexCount(), world, view, proj,
				m_Model->GetTexture(), m_Camera->GetPosition(), 
				m_Light->GetDirection(), color, m_Light->GetAmbientColor(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower());

			m_Direct3D->GetWorldMatrix(world);

			++renderCount;
		}
	}

	m_Text->SetRenderCount(renderCount, m_Direct3D->GetDeviceContext());

	m_Direct3D->TurnZBufferOff();
	m_Direct3D->TurnAlphaBlendingOn();

	XMMATRIX ortho;
	m_Direct3D->GetOrthoMatrix(ortho);
	m_Text->Render(m_Direct3D->GetDeviceContext(), world, ortho);
	m_Direct3D->TurnZBufferOn();
#endif

	m_Direct3D->EndScene();

	return true;
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