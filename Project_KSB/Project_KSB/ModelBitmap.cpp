#include "pch.h"
#include "ModelBitmap.h"
#include "Texture.h"

ModelBitmap::ModelBitmap()
{
}

ModelBitmap::ModelBitmap(const ModelBitmap& other)
{
}

ModelBitmap::~ModelBitmap()
{
}

bool ModelBitmap::Initialize(ID3D11Device* device, int screenWidth, int screenHeight, WCHAR* texturePath, int bitmapWidth, int bitmapHeight)
{
	// 화면 크기
	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;
	
	// 비트맵 픽셀 크기
	m_bitmapWidth = bitmapWidth;
	m_bitmapHeight = bitmapHeight;

	// 이전 렌더링 위치
	m_prevPosX = -1;
	m_prevPosY = -1;

	if (!InitializeBuffers(device))
		return false;

	return LoadTexture(device, texturePath);
}

void ModelBitmap::Shutdown()
{
	ReleaseTexture();
	ShutdownBuffers();
}

bool ModelBitmap::Render(ID3D11DeviceContext* deviceContext, int posX, int posY)
{
	if (!UpdateBuffers(deviceContext, posX, posY))
		return false;

	RenderBuffers(deviceContext);

	return true;
}

int ModelBitmap::GetIndexCount()
{
	return m_indexCount;
}

ID3D11ShaderResourceView* ModelBitmap::GetTexture()
{
	return m_Texture->GetTexture();
}

bool ModelBitmap::InitializeBuffers(ID3D11Device* device)
{
	m_indexCount = m_vertexCount = 6;

	VertexType* vertices = new VertexType[m_vertexCount];
	memset(vertices, 0, (sizeof(VertexType) * m_vertexCount));

	ULONG* indices = new ULONG[m_indexCount];
	
	for (int i = 0; i < m_indexCount; ++i)
	{
		indices[i] = i;
	}

	D3D11_BUFFER_DESC vertexBufferDesc;
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vertexData;
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	if (FAILED(device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_VertexBuffer)))
		return false;

	D3D11_BUFFER_DESC indexBufferDesc;
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(ULONG) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA indexData;
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	if (FAILED(device->CreateBuffer(&indexBufferDesc, &indexData, &m_IndexBuffer)))
		return false;

	SAFE_DELETE_ARRAY(vertices);
	SAFE_DELETE_ARRAY(indices);

	return true;
}

void ModelBitmap::ShutdownBuffers()
{
	SAFE_RELEASE(m_IndexBuffer);
	SAFE_RELEASE(m_VertexBuffer);
}

bool ModelBitmap::UpdateBuffers(ID3D11DeviceContext* deviceContext, int posX, int posY)
{
	float left, right, top, bottom;
	VertexType* vertices;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	VertexType* verticesPtr;

	if ((posX == m_prevPosX) && (posY == m_prevPosY))
		return true;

	m_prevPosX = posX;
	m_prevPosY = posY;

	left = (float)((m_screenWidth / 2) * -1) + (float)posX;
	right = left + (float)m_bitmapWidth;
	top = (float)(m_screenHeight / 2) - (float)posY;
	bottom = top - (float)m_bitmapHeight;

	vertices = new VertexType[m_vertexCount];
	
	// 1
	vertices[0].pos = XMFLOAT3(left, top, 0.f);
	vertices[0].tex = XMFLOAT2(0.f, 0.f);

	vertices[1].pos = XMFLOAT3(right, bottom, 0.f);
	vertices[1].tex = XMFLOAT2(1.f, 1.f);

	vertices[2].pos = XMFLOAT3(left, bottom, 0.f);
	vertices[2].tex = XMFLOAT2(0.f, 1.f);

	// 2
	vertices[3].pos = XMFLOAT3(left, top, 0.f);
	vertices[3].tex = XMFLOAT2(0.f, 0.f);

	vertices[4].pos = XMFLOAT3(right, top, 0.f);
	vertices[4].tex = XMFLOAT2(1.f, 0.f);

	vertices[5].pos = XMFLOAT3(right, bottom, 0.f);
	vertices[5].tex = XMFLOAT2(1.f, 1.f);

	if(FAILED(deviceContext->Map(m_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource)))
		return false;

	verticesPtr = (VertexType*)mappedResource.pData;

	memcpy(verticesPtr, (void*)vertices, (sizeof(VertexType) * m_vertexCount));

	deviceContext->Unmap(m_VertexBuffer, 0);

	SAFE_DELETE_ARRAY(vertices);

	return true;
}

void ModelBitmap::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	UINT stride = sizeof(VertexType);
	UINT offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);
	deviceContext->IASetIndexBuffer(m_IndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

bool ModelBitmap::LoadTexture(ID3D11Device* device, WCHAR* texturePath)
{
	m_Texture = new Texture;

	return m_Texture->Initialize(device, texturePath);
}

void ModelBitmap::ReleaseTexture()
{
	SAFE_SHUTDOWN(m_Texture);
}
