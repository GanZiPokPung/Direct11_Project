#include "pch.h"
#include "ModelTexture.h"
#include "Texture.h"

ModelTexture::ModelTexture()
{
}

ModelTexture::ModelTexture(const ModelTexture& other)
{
}

ModelTexture::~ModelTexture()
{
}

bool ModelTexture::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext,
	char* filePath)
{
	if (!InitializeBuffers(device))
		return false;

	return LoadTexture(device, deviceContext, filePath);
}

void ModelTexture::Shutdown()
{
	ReleaseTexture();
	ShutdownBuffers();
}

void ModelTexture::Render(ID3D11DeviceContext* deviceContext)
{
	RenderBuffers(deviceContext);
}

int ModelTexture::GetIndexCount()
{
	return m_indexCount;
}

ID3D11ShaderResourceView* ModelTexture::GetTexture()
{
	return m_Texture->GetTexture();
}

bool ModelTexture::InitializeBuffers(ID3D11Device* device)
{
	m_vertexCount = 3;
	m_indexCount = 3;

	VertexType* vertices = new VertexType[m_vertexCount];
	ULONG* indices = new ULONG[m_indexCount];

	vertices[0].pos = XMFLOAT3(-1.f, -1.f, 0.f);
	vertices[0].tex = XMFLOAT2(0.f, 1.f);

	vertices[1].pos = XMFLOAT3(0.f, 1.f, 0.f);
	vertices[1].tex = XMFLOAT2(0.5f, 0.f);

	vertices[2].pos = XMFLOAT3(1.f, -1.f, 0.f);
	vertices[2].tex = XMFLOAT2(1.f, 1.f);

	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;

	D3D11_BUFFER_DESC vertexBufferDesc;
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
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

void ModelTexture::ShutdownBuffers()
{
	SAFE_RELEASE(m_IndexBuffer);
	SAFE_RELEASE(m_VertexBuffer);
}

void ModelTexture::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	UINT stride = sizeof(VertexType);
	UINT offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);
	deviceContext->IASetIndexBuffer(m_IndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

bool ModelTexture::LoadTexture(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* filePath)
{
	m_Texture = new Texture;

	return m_Texture->Initialize(device, deviceContext, filePath);
}

void ModelTexture::ReleaseTexture()
{
	SAFE_SHUTDOWN(m_Texture);
}
