#include "pch.h"
#include "Text.h"
#include "Font.h"
#include "FontShader.h"

Text::Text()
{
}

Text::Text(const Text& other)
{
}

Text::~Text()
{
}

bool Text::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, HWND hwnd, int screenWidth, int screenHeight, XMMATRIX view)
{
	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;

	m_view = view;

	m_Font = new Font;
	if (!m_Font->Initialize(device, "../Project_KSB/data/fontdata.txt",
		L"../Project_KSB/data/font.dds"))
	{
		MSG_ERROR(hwnd, L"Could not initialize the font object");
		return false;
	}

	m_FontShader = new FontShader;
	if (!m_FontShader->Initialize(device, hwnd))
		return false;

	if (!InitializeSentence(&m_Sentence1, 16, device))
		return false;
	if (!UpdateSentence(m_Sentence1, "Hello", 100, 100, 1.f, 1.f, 1.f, deviceContext))
		return false;

	if (!InitializeSentence(&m_Sentence2, 16, device))
		return false;
	if (!UpdateSentence(m_Sentence2, "Goodbye", 100, 200, 1.f, 1.f, 0.f, deviceContext))
		return false;

	if (!InitializeSentence(&m_Sentence3, 16, device))
		return false;
	if (!UpdateSentence(m_Sentence3, "Hello World!", 100, 300, 1.f, 0.f, 0.f, deviceContext))
		return false;

	return true;
}

void Text::Shutdown()
{
	ReleaseSentence(&m_Sentence1);
	ReleaseSentence(&m_Sentence2);
	ReleaseSentence(&m_Sentence3);

	SAFE_SHUTDOWN(m_FontShader);
	SAFE_SHUTDOWN(m_Font);
}

bool Text::Render(ID3D11DeviceContext* deviceContext, XMMATRIX world, XMMATRIX ortho)
{
	if (!RenderSentence(deviceContext, m_Sentence1, world, ortho))
		return false;

	if (!RenderSentence(deviceContext, m_Sentence2, world, ortho))
		return false;

	return true;
}

bool Text::SetMousePosition(int x, int y, ID3D11DeviceContext* deviceContext)
{
	char tempString[16] = { 0, };
	_itoa_s(x, tempString, 10);

	char mouseString[16] = { 0, };
	strcpy_s(mouseString, "Mouse X: ");
	strcat_s(mouseString, tempString);

	if (!UpdateSentence(m_Sentence1, mouseString, 20, 20, 1.f, 1.f, 1.f, deviceContext))
		return false;

	_itoa_s(y, tempString, 10);

	strcpy_s(mouseString, "Mouse Y: ");
	strcat_s(mouseString, tempString);

	if (!UpdateSentence(m_Sentence2, mouseString, 20, 40, 1.f, 1.f, 1.f, deviceContext))
		return false;

	return true;
}

bool Text::SetKeyState(char key, ID3D11DeviceContext* deviceContext)
{
	return true;
}

bool Text::InitializeSentence(SentenceType** sentence, int maxLength, ID3D11Device* device)
{
	*sentence = new SentenceType;

	(*sentence)->vertexBuffer = nullptr;
	(*sentence)->indexBuffer = nullptr;

	(*sentence)->maxLength = maxLength;
	(*sentence)->vertexCount = 6 * maxLength;
	
	(*sentence)->indexCount = (*sentence)->vertexCount;

	VertexType* vertices = new VertexType[(*sentence)->vertexCount];
	ULONG* indices = new ULONG[(*sentence)->indexCount];

	memset(vertices, 0, (sizeof(VertexType) * (*sentence)->vertexCount));

	for (int i = 0; i < (*sentence)->indexCount; ++i)
	{
		indices[i] = i;
	}

	D3D11_BUFFER_DESC vertexBufferDesc;
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * (*sentence)->vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vertexData;
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;
	
	if (FAILED(device->CreateBuffer(&vertexBufferDesc, &vertexData, &(*sentence)->vertexBuffer)))
		return false;

	D3D11_BUFFER_DESC indexBufferDesc;
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(ULONG) * (*sentence)->indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA indexData;
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	if (FAILED(device->CreateBuffer(&indexBufferDesc, &indexData, &(*sentence)->indexBuffer)))
		return false;
	
	SAFE_DELETE_ARRAY(vertices);
	SAFE_DELETE_ARRAY(indices);

	return true;
}

bool Text::UpdateSentence(SentenceType* sentence, char* text, int posX, int posY, float r, float g, float b, ID3D11DeviceContext* deviceContext)
{
	sentence->r = r;
	sentence->g = g;
	sentence->b = b;

	int numLetters = (int)strlen(text);

	if (numLetters > sentence->maxLength)
		return false;

	VertexType* vertices = new VertexType[sentence->vertexCount];
	memset(vertices, 0, (sizeof(VertexType) * sentence->vertexCount));

	// uv
	float drawX = (float)(((m_screenWidth / 2) * -1) + posX);
	float drawY = (float)((m_screenHeight / 2) - posY);

	m_Font->BuildVertexArray((void*)vertices, text, drawX, drawY);

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	if (FAILED(deviceContext->Map(sentence->vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource)))
		return false;
	
	VertexType* verticesPtr = (VertexType*)mappedResource.pData;

	memcpy(verticesPtr, (void*)vertices, sizeof(VertexType) * sentence->vertexCount);

	deviceContext->Unmap(sentence->vertexBuffer, 0);

	SAFE_DELETE_ARRAY(vertices);

	return true;
}

void Text::ReleaseSentence(SentenceType** sentence)
{
	if (*sentence)
	{
		SAFE_RELEASE((*sentence)->vertexBuffer);
		SAFE_RELEASE((*sentence)->indexBuffer);
		SAFE_DELETE((*sentence));
	}
}

bool Text::RenderSentence(ID3D11DeviceContext* deviceContext, SentenceType* sentence, XMMATRIX world, XMMATRIX ortho)
{
	UINT stride = sizeof(VertexType);
	UINT offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, &sentence->vertexBuffer, &stride, &offset);
	deviceContext->IASetIndexBuffer(sentence->indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	
	XMFLOAT4 pixelColor = XMFLOAT4(sentence->r, sentence->g, sentence->b, 1.f);

	if (!m_FontShader->Render(deviceContext, sentence->indexCount, world, m_view, ortho,
		m_Font->GetTexture(), pixelColor))
		return false;

	return true;
}
