#include "pch.h"
#include "Font.h"
#include "Texture.h"

Font::Font()
{
}

Font::Font(const Font& other)
{
}

Font::~Font()
{
}

ID3D11ShaderResourceView* Font::GetTexture()
{
	return m_Texture->GetTexture();
}

bool Font::Initialize(ID3D11Device* device, char* fontPath, WCHAR* texturePath)
{
	if (!LoadFontData(fontPath))
		return false;

	return LoadTexture(device, texturePath);
}

void Font::Shutdown()
{
	ReleaseTexture();
	ReleaseFontData();
}

void Font::BuildVertexArray(void* vertices, char* sentence, float drawX, float drawY)
{
	VertexType* vertexPtr = (VertexType*)vertices;

	int numLetters = (int)strlen(sentence);

	int index = 0;
	int letter;

	for (int i = 0; i < numLetters; ++i)
	{
		letter = ((int)sentence[i]) - 32;

		if (0 == letter)
		{
			drawX = drawX + 3.f;
		}
		else
		{
			vertexPtr[index].pos = XMFLOAT3(drawX, drawY, 0.0f);  // Top left.
			vertexPtr[index].tex = XMFLOAT2(m_Font[letter].left, 0.0f);
			index++;

			vertexPtr[index].pos = XMFLOAT3((drawX + m_Font[letter].Size), (drawY - 16), 0.0f);  // Bottom right.
			vertexPtr[index].tex = XMFLOAT2(m_Font[letter].right, 1.0f);
			index++;

			vertexPtr[index].pos = XMFLOAT3(drawX, (drawY - 16), 0.0f);  // Bottom left.
			vertexPtr[index].tex = XMFLOAT2(m_Font[letter].left, 1.0f);
			index++;

			vertexPtr[index].pos = XMFLOAT3(drawX, drawY, 0.0f);  // Top left.
			vertexPtr[index].tex = XMFLOAT2(m_Font[letter].left, 0.0f);
			index++;

			vertexPtr[index].pos = XMFLOAT3(drawX + m_Font[letter].Size, drawY, 0.0f);  // Top right.
			vertexPtr[index].tex = XMFLOAT2(m_Font[letter].right, 0.0f);
			index++;

			vertexPtr[index].pos = XMFLOAT3((drawX + m_Font[letter].Size), (drawY - 16), 0.0f);  // Bottom right.
			vertexPtr[index].tex = XMFLOAT2(m_Font[letter].right, 1.0f);
			index++;

			drawX = drawX + m_Font[letter].Size + 1.0f;
		}
	}
}

bool Font::LoadFontData(char* fontPath)
{
	ifstream fin;
	char temp;

	m_Font = new FontType[95];
	
	fin.open(fontPath);
	if (fin.fail())
		return false;

	for (int i = 0; i < 95; ++i)
	{
		fin.get(temp);
		while (temp != ' ')
		{
			fin.get(temp);
		}
		fin.get(temp);
		while (temp != ' ')
		{
			fin.get(temp);
		}

		fin >> m_Font[i].left;
		fin >> m_Font[i].right;
	}

	return false;
}

void Font::ReleaseFontData()
{
	SAFE_DELETE_ARRAY(m_Font);
}

bool Font::LoadTexture(ID3D11Device* device, WCHAR* texturePath)
{
	m_Texture = new Texture;

	return m_Texture->Initialize(device, texturePath);
}

void Font::ReleaseTexture()
{
	SAFE_SHUTDOWN(m_Texture);
}
