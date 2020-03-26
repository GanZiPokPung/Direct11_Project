#include "pch.h"
#include "Model3D.h"
#include "Texture.h"

Model3D::Model3D()
{
}

Model3D::Model3D(const Model3D& other)
{
}

Model3D::~Model3D()
{
}

bool Model3D::Initialize(ID3D11Device* device, char* modelPath, WCHAR* texturePath)
{
	if (!LoadModel(modelPath))
		return false;

	if (!InitializeBuffers(device))
		return false;

	return LoadTexture(device, texturePath);
}

void Model3D::Shutdown()
{
	ReleaseTexture();
	ReleaseModel();
	ShutdownBuffers();
}

void Model3D::Render(ID3D11DeviceContext* deviceContext)
{
	RenderBuffers(deviceContext);
}

int Model3D::GetIndexCount()
{
	return m_indexCount;
}

ID3D11ShaderResourceView* Model3D::GetTexture()
{
	return m_Texture->GetTexture();
}

bool Model3D::InitializeBuffers(ID3D11Device* device)
{
	VertexType* vertices = new VertexType[m_vertexCount];
	ULONG* indices = new unsigned long[m_indexCount];

	for (int i = 0; i < m_vertexCount; ++i)
	{
		vertices[i].pos = XMFLOAT3(m_Model[i].x, m_Model[i].y, m_Model[i].z);
		vertices[i].tex = XMFLOAT2(m_Model[i].tu, m_Model[i].tv);
		vertices[i].nor = XMFLOAT3(m_Model[i].nx, m_Model[i].ny, m_Model[i].nz);

		indices[i] = i;
	}

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

void Model3D::ShutdownBuffers()
{
	SAFE_RELEASE(m_VertexBuffer);
	SAFE_RELEASE(m_IndexBuffer);
}

void Model3D::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	UINT stride = sizeof(VertexType);
	UINT offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);
	deviceContext->IASetIndexBuffer(m_IndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

bool Model3D::LoadTexture(ID3D11Device* device, WCHAR* texturePath)
{
	m_Texture = new Texture;
	return m_Texture->Initialize(device, texturePath);
}

void Model3D::ReleaseTexture()
{
	SAFE_SHUTDOWN(m_Texture);
}

bool Model3D::LoadModel(char* modelPath)
{
	ifstream fin;
	fin.open(modelPath);

	if (fin.fail())
		return false;

	char input = 0;
	fin.get(input);
	while (':' != input)
	{
		fin.get(input);
	}

	fin >> m_vertexCount;
	m_indexCount = m_vertexCount;
	m_Model = new ModelType[m_vertexCount];

	fin.get(input);
	while (':' != input)
	{
		fin.get(input);
	}
	fin.get(input);
	fin.get(input);

	for (int i = 0; i < m_vertexCount; ++i)
	{
		fin >> m_Model[i].x >> m_Model[i].y >> m_Model[i].z;
		fin >> m_Model[i].tu >> m_Model[i].tv;
		fin >> m_Model[i].nx >> m_Model[i].ny >> m_Model[i].nz;
	}

	fin.close();

	return true;
}

void Model3D::ReleaseModel()
{
	SAFE_DELETE_ARRAY(m_Model);
}
