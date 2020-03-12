#pragma once

class Texture
{
private:
	struct TargaHeader
	{
		BYTE data1[12];
		USHORT width;
		USHORT height;
		BYTE bpp;
		BYTE data2;
	};

public:
	Texture();
	Texture(const Texture& other);
	~Texture();

public:
	ID3D11ShaderResourceView* GetTexture() { return m_TextureView; }

public:
	bool Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext,
		char* filePath);
	void Shutdown();

private:
	bool LoadTarga(char* filePath, int& height, int& width);

private:
	BYTE* m_TargaData = nullptr;
	ID3D11Texture2D* m_Texture = nullptr;
	ID3D11ShaderResourceView* m_TextureView = nullptr;
};

