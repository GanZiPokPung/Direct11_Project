#include "pch.h"
#include "Direct3D.h"

Direct3D::Direct3D()
{
}

Direct3D::Direct3D(const Direct3D& other)
{
}

Direct3D::~Direct3D()
{
}

ID3D11Device* Direct3D::GetDevice()
{
	return m_Device;
}

ID3D11DeviceContext* Direct3D::GetDeviceContext()
{
	return m_DeviceContext;
}

void Direct3D::GetProjectionMatrix(XMMATRIX& proj)
{
	proj = m_projection;
}

void Direct3D::GetWorldMatrix(XMMATRIX& world)
{
	world = m_world;
}

void Direct3D::GetOrthoMatrix(XMMATRIX& ortho)
{
	ortho = m_ortho;
}

void Direct3D::SaveVideoCardInfo()
{
	ofstream file;

	file.open("Graphic card info.txt", ios::out);

	if (file.is_open())
	{
		file << "Video Card Name : " <<  m_videoCardDescription << endl;
		file << "Vidio Memrory : " << m_videoCardMemory << endl;
		file.close();
	}
	
}

bool Direct3D::Initialize(int width, int height, bool vsync, HWND hwnd, bool fullscreen, float Far, float Near)
{
	m_vsyncCheck = vsync;

	// 어뎁터 정보, 해상도 정보 등
	IDXGIFactory* factory = nullptr;
	if (FAILED(CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)& factory)))
	{
		MSG_ERROR(hwnd, L"Create DXGIFactory failed!");
		return false;
	}

	IDXGIAdapter* adapter = nullptr;
	if (FAILED(factory->EnumAdapters(0, &adapter)))
	{
		MSG_ERROR(hwnd, L"Set graphic device failed!");
		return false;
	}

	IDXGIOutput* adapterOutput = nullptr;
	if (FAILED(adapter->EnumOutputs(0, &adapterOutput)))
	{
		MSG_ERROR(hwnd, L"Set display device failed!");
		return false;
	}

	unsigned int numModes = 0;
	if (FAILED(adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM,
		DXGI_ENUM_MODES_INTERLACED, &numModes, NULL)))
	{
		MSG_ERROR(hwnd, L"Get display modes count failed!");
		return false;
	}

	DXGI_MODE_DESC* displayModeList = new DXGI_MODE_DESC[numModes];

	if (FAILED(adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM,
		DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList)))
	{
		MSG_ERROR(hwnd, L"Get display modes failed!");
		return false;
	}

	unsigned int numerator = 0;
	unsigned int denominator = 0;
	for (unsigned int i = 0; i < numModes; ++i)
	{
		if ((unsigned int)width == displayModeList[i].Width)
		{
			if ((unsigned int)height == displayModeList[i].Height)
			{
				numerator = displayModeList[i].RefreshRate.Numerator;
				denominator = displayModeList[i].RefreshRate.Denominator;
			}
		}
	}
	SAFE_DELETE_ARRAY(displayModeList);

	DXGI_ADAPTER_DESC adapterDesc;
	if (FAILED(adapter->GetDesc(&adapterDesc)))
	{
		return false;
	}

	m_videoCardMemory = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);

	size_t stringLength = 0;
	if (0 != wcstombs_s(&stringLength, m_videoCardDescription, 128, adapterDesc.Description, 128))
	{
		MSG_ERROR(hwnd, L"Get graphic device name failed!");
		return false;
	}

	SaveVideoCardInfo();
	SAFE_RELEASE(adapterOutput);
	SAFE_RELEASE(adapter);
	SAFE_RELEASE(factory);

	// 스왑체인
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	Z_Memory(swapChainDesc);

	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Width = width;
	swapChainDesc.BufferDesc.Height = height;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	
	if (m_vsyncCheck)
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = numerator;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = denominator;
	}
	else
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	}

	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = hwnd;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;

	if (fullscreen)
		swapChainDesc.Windowed = false;
	else
		swapChainDesc.Windowed = true;

	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags = 0;

	D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;

	if (FAILED(D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &featureLevel,
		1, D3D11_SDK_VERSION, &swapChainDesc, &m_SwapChain, &m_Device, NULL, &m_DeviceContext)))
	{
		MSG_ERROR(hwnd, L"Create device and swapChain failed!");
		return false;
	}

	// 백버퍼
	ID3D11Texture2D* backBuffer = nullptr;
	if (FAILED(m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)& backBuffer)))
	{
		MSG_ERROR(hwnd, L"Get backbuffer failed!");
		return false;
	}

	if (FAILED(m_Device->CreateRenderTargetView(backBuffer, NULL, &m_RenderTargetView)))
	{
		MSG_ERROR(hwnd, L"Create render target view failed!");
		return false;
	}

	SAFE_RELEASE(backBuffer);

	// 깊이 버퍼
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	Z_Memory(depthBufferDesc);

	depthBufferDesc.Width = width;
	depthBufferDesc.Height = height;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	if (FAILED(m_Device->CreateTexture2D(&depthBufferDesc, NULL, &m_DepthStencilBuffer)))
	{
		MSG_ERROR(hwnd, L"Create depth buffer failed!");
		return false;
	}

	// 스텐실 상태
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	Z_Memory(depthStencilDesc);

	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	if (FAILED(m_Device->CreateDepthStencilState(&depthStencilDesc, &m_DepthStencilState)))
	{
		MSG_ERROR(hwnd, L"Create depth stencil state failed!");
		return false;
	}

	m_DeviceContext->OMSetDepthStencilState(m_DepthStencilState, 1);

	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	Z_Memory(depthStencilViewDesc);

	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	if (FAILED(m_Device->CreateDepthStencilView(m_DepthStencilBuffer, &depthStencilViewDesc,
		&m_DepthStencilView)))
	{
		MSG_ERROR(hwnd, L"Create depth stencil view failed!");
		return false;
	}

	m_DeviceContext->OMSetRenderTargets(1, &m_RenderTargetView, m_DepthStencilView);

	D3D11_RASTERIZER_DESC rasterDesc;
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.f;

	if (FAILED(m_Device->CreateRasterizerState(&rasterDesc, &m_RasterState)))
	{
		MSG_ERROR(hwnd, L"Create rasterizer state failed!");
		return false;
	}

	m_DeviceContext->RSSetState(m_RasterState);

	D3D11_VIEWPORT viewport;
	viewport.Width = (float)width;
	viewport.Height = (float)height;
	viewport.MinDepth = 0.f;
	viewport.MaxDepth = 1.f;
	viewport.TopLeftX = 0.f;
	viewport.TopLeftY = 0.f;

	m_DeviceContext->RSSetViewports(1, &viewport);

	float FOV = XM_PI / 4.f;
	float aspect = (float)width / (float)height;

	m_projection = XMMatrixPerspectiveFovLH(FOV, aspect, Near, Far);
	m_world = XMMatrixIdentity();
	m_ortho = XMMatrixOrthographicLH((float)width, (float)height, Near, Far);

	// 2D 렌더링을 위해 Z 버퍼를 끄는 스텐실 상태
	D3D11_DEPTH_STENCIL_DESC depthDisabledStencilDesc;
	Z_Memory(depthDisabledStencilDesc);

	depthDisabledStencilDesc.DepthEnable = false;
	depthDisabledStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthDisabledStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthDisabledStencilDesc.StencilEnable = true;
	depthDisabledStencilDesc.StencilReadMask = 0xFF;
	depthDisabledStencilDesc.StencilWriteMask = 0xFF;

	depthDisabledStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthDisabledStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	depthDisabledStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthDisabledStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	if (FAILED(m_Device->CreateDepthStencilState(&depthDisabledStencilDesc, &m_DepthDisabledStencilState)))
	{
		MSG_ERROR(hwnd, L"Create Z depth stencil state failed!");
		return false;
	}

	return true;
}

void Direct3D::Shutdown()
{
	// 종료시 윈도우 모드로 설정하지 않으면 스왑체인 해제 시 예외 발생
	if (nullptr != m_SwapChain)
	{
		m_SwapChain->SetFullscreenState(false, NULL);
	}

	SAFE_RELEASE(m_RasterState);
	SAFE_RELEASE(m_DepthStencilView);
	SAFE_RELEASE(m_DepthStencilState);
	SAFE_RELEASE(m_DepthDisabledStencilState);
	SAFE_RELEASE(m_DepthStencilBuffer);
	SAFE_RELEASE(m_RenderTargetView);
	SAFE_RELEASE(m_DeviceContext);
	SAFE_RELEASE(m_Device);
	SAFE_RELEASE(m_SwapChain);
}

void Direct3D::BeginScene(float r, float g, float b, float a)
{
	float color[4] = { r, g, b, a };

	m_DeviceContext->ClearRenderTargetView(m_RenderTargetView, color);
	m_DeviceContext->ClearDepthStencilView(m_DepthStencilView, D3D11_CLEAR_DEPTH, 1.f, 0);
}

void Direct3D::EndScene()
{
	if (m_vsyncCheck)
		m_SwapChain->Present(1, 0);
	else
		m_SwapChain->Present(0, 0);
}

void Direct3D::TurnZBufferOn()
{
	m_DeviceContext->OMSetDepthStencilState(m_DepthStencilState, 1);
}

void Direct3D::TurnZBufferOff()
{
	m_DeviceContext->OMSetDepthStencilState(m_DepthDisabledStencilState, 1);
}
