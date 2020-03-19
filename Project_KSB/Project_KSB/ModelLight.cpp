#include "pch.h"
#include "ModelLight.h"
#include "Texture.h"

ModelLight::ModelLight()
{
}

ModelLight::ModelLight(const ModelLight& other)
{
}

ModelLight::~ModelLight()
{
}

bool ModelLight::Initialize(ID3D11Device* device, WCHAR* filePath)
{
	if (!InitializeBuffers(device))
		return false;

	return LoadTexture(device, filePath);
}

void ModelLight::Shutdown()
{
	ReleaseTexture();
	ShutdownBuffers();
}

void ModelLight::Render(ID3D11DeviceContext* deviceContext)
{
	RenderBuffers(deviceContext);
}

int ModelLight::GetIndexCount()
{
	return m_indexCount;
}

ID3D11ShaderResourceView* ModelLight::GetTexture()
{
	return m_Texture->GetTexture();
}

bool ModelLight::InitializeBuffers(ID3D11Device* device)
{
	m_vertexCount = 6;
	m_indexCount = 6;

	VertexType* vertices = new VertexType[m_vertexCount];
	ULONG* indices = new ULONG[m_indexCount];

	vertices[0].pos = XMFLOAT3(-1.f, 1.f, 0.f);
	vertices[0].tex = XMFLOAT2(0.f, 0.f);

	vertices[1].pos = XMFLOAT3(1.f, 1.f, 0.f);
	vertices[1].tex = XMFLOAT2(1.f, 0.f);

	vertices[2].pos = XMFLOAT3(-1.f, -1.f, 0.f);
	vertices[2].tex = XMFLOAT2(0.f, 1.f);

	vertices[3].pos = XMFLOAT3(1.f, 1.f, 0.f);
	vertices[3].tex = XMFLOAT2(1.f, 0.f);

	vertices[4].pos = XMFLOAT3(1.f, -1.f, 0.f);
	vertices[4].tex = XMFLOAT2(1.f, 1.f);

	vertices[5].pos = XMFLOAT3(-1.f, -1.f, 0.f);
	vertices[5].tex = XMFLOAT2(0.f, 1.f);

	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;
	indices[3] = 3;
	indices[4] = 4;
	indices[5] = 5;

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

void ModelLight::ShutdownBuffers()
{
	SAFE_RELEASE(m_IndexBuffer);
	SAFE_RELEASE(m_VertexBuffer);
}

void ModelLight::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	UINT stride = sizeof(VertexType);
	UINT offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);
	deviceContext->IASetIndexBuffer(m_IndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

bool ModelLight::LoadTexture(ID3D11Device* device, WCHAR* filePath)
{
	m_Texture = new Texture;

	return m_Texture->Initialize(device, filePath);
}

void ModelLight::ReleaseTexture()
{
	SAFE_SHUTDOWN(m_Texture);
}
