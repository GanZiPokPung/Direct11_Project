#pragma once

class Texture;

class Font
{
private:
	struct FontType
	{
		float left, right;
		int size;
	};
	struct VertexType
	{
		XMFLOAT3 pos;
		XMFLOAT2 tex;
	};

public:
	Font();
	Font(const Font& other);
	~Font();

public:
	ID3D11ShaderResourceView* GetTexture();

public:
	bool Initialize(ID3D11Device* device, char* fontPath, WCHAR* texturePath);
	void Shutdown();

	void BuildVertexArray(void* vertices, char* sentence, float drawX, float drawY);

private:
	bool LoadFontData(char* fontPath);
	void ReleaseFontData();
	bool LoadTexture(ID3D11Device* device, WCHAR* texturePath);
	void ReleaseTexture();

private:
	FontType* m_Font = nullptr;
	Texture* m_Texture = nullptr;
};

