#include "pch.h"
#include "ColorShader.h"

ColorShader::ColorShader()
{
}

ColorShader::ColorShader(const ColorShader& other)
{
}

ColorShader::~ColorShader()
{
}

bool ColorShader::Initialize(ID3D11Device* device, HWND hwnd)
{
	return InitializeShader(device, hwnd,
		L"../Project_KSB/Color.vs", L"../Project_KSB/Color.ps");
}

void ColorShader::Shutdown()
{
	ShutdownShader();
}

bool ColorShader::Render(ID3D11DeviceContext* deviceContext, int indexCount, XMMATRIX world, XMMATRIX view, XMMATRIX proj)
{
	if (!SetShaderParameters(deviceContext, world, view, proj))
		return false;

	RenderShader(deviceContext, indexCount);

	return true;
}

bool ColorShader::InitializeShader(ID3D11Device* device, HWND hwnd, WCHAR* vsPath, WCHAR* psPath)
{
	// 쉐이더 파일 컴파일
	ID3D10Blob* errorMessage = nullptr;

	ID3D10Blob* vertexShaderBuffer = nullptr;
	if (FAILED(D3DCompileFromFile(vsPath, NULL, NULL, "ColorVertexShader", "vs_5_0",
		D3D10_SHADER_ENABLE_STRICTNESS, 0, &vertexShaderBuffer, &errorMessage)))
	{
		if (nullptr != errorMessage)
			OutputShaderErrorMessage(errorMessage, hwnd, vsPath);
		else
			MessageBox(hwnd, vsPath, L"Missing Shader File", MB_OK);

		return false;
	}
	
	ID3D10Blob* pixelShaderBuffer = nullptr;
	if (FAILED(D3DCompileFromFile(psPath, NULL, NULL, "ColorPixelShader", "ps_5_0",
		D3D10_SHADER_ENABLE_STRICTNESS, 0, &pixelShaderBuffer, &errorMessage)))
	{
		if (nullptr != errorMessage)
			OutputShaderErrorMessage(errorMessage, hwnd, psPath);
		else
			MessageBox(hwnd, psPath, L"Missing Shader File", MB_OK);

		return false;
	}

	// 쉐이더 생성
	if (FAILED(device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(),
		vertexShaderBuffer->GetBufferSize(), NULL, &m_VertexShader)))
		return false;

	if (FAILED(device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(),
		pixelShaderBuffer->GetBufferSize(), NULL, &m_PixelShader)))
		return false;

	// 레이아웃
	D3D11_INPUT_ELEMENT_DESC polygonLayout[2];
	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "COLOR";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	unsigned int numElement = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	if (FAILED(device->CreateInputLayout(polygonLayout, numElement,
		vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(),
		&m_Layout)))
		return false;

	SAFE_RELEASE(vertexShaderBuffer);
	SAFE_RELEASE(pixelShaderBuffer);

	D3D11_BUFFER_DESC matrixBufferDesc;
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	// 상수 버퍼 포인터를 만들어 이 클래스에서 정점 셰이더 상수 버퍼에 접근할 수 있게 한다.
	if (FAILED(device->CreateBuffer(&matrixBufferDesc, NULL, &m_MatrixBuffer)))
		return false;

	return true;
}

void ColorShader::ShutdownShader()
{
	SAFE_RELEASE(m_MatrixBuffer);
	SAFE_RELEASE(m_Layout);
	SAFE_RELEASE(m_PixelShader);
	SAFE_RELEASE(m_VertexShader);
}

void ColorShader::OutputShaderErrorMessage(ID3D10Blob* blob, HWND hwnd, WCHAR* shaderPath)
{
	OutputDebugStringA(reinterpret_cast<const char*>(blob->GetBufferPointer()));

	blob->Release();
	blob = nullptr;

	MessageBox(hwnd, L"Error compiling shader", shaderPath, MB_OK);
}

bool ColorShader::SetShaderParameters(ID3D11DeviceContext* deviceContext, XMMATRIX world, XMMATRIX view, XMMATRIX proj)
{
	world = XMMatrixTranspose(world);
	view = XMMatrixTranspose(view);
	proj = XMMatrixTranspose(proj);

	// 상수 버퍼의 내용을 쓸 수 있게 잠금
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	if (FAILED(deviceContext->Map(m_MatrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource)))
		return false;

	// 상수 버퍼의 데이터에 대한 포인터를 가져온다.
	MatrixBufferType* dataPtr = (MatrixBufferType*)mappedResource.pData;

	dataPtr->world = world;
	dataPtr->view = view;
	dataPtr->projection = proj;

	// 잠금을 푼다.
	deviceContext->Unmap(m_MatrixBuffer, 0);

	// 정점 셰이더의 상수 버퍼의 위치를 변경
	unsigned int bufferNumber = 0;

	// 정점 셰이더의 상수 버퍼를 바뀐 값으로 바꾼다.
	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_MatrixBuffer);

	return true;
}

void ColorShader::RenderShader(ID3D11DeviceContext* deviceContext, int indexCount)
{
	deviceContext->IASetInputLayout(m_Layout);

	deviceContext->VSSetShader(m_VertexShader, NULL, 0);
	deviceContext->PSSetShader(m_PixelShader, NULL, 0);
	
	deviceContext->DrawIndexed(indexCount, 0, 0);
}
