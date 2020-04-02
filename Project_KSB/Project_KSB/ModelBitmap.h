#pragma once
class Texture;
class ModelBitmap
{
private:
	struct VertexType
	{
		XMFLOAT3 pos;
		XMFLOAT2 tex;
	};

public:
	ModelBitmap();
	ModelBitmap(const ModelBitmap& other);
	~ModelBitmap();

	bool Initialize(ID3D11Device* device, int screenWidth, int screenHeight, WCHAR* texturePath, 
		int bitmapWidth, int bitmapHeight);
	void Shutdown();
	bool Render(ID3D11DeviceContext* deviceContext, int posX, int posY);

	int GetIndexCount();
	ID3D11ShaderResourceView* GetTexture();

private:
	bool InitializeBuffers(ID3D11Device* device);
	void ShutdownBuffers();
	bool UpdateBuffers(ID3D11DeviceContext* deviceContext, int posX, int posY);
	void RenderBuffers(ID3D11DeviceContext* deviceContext);

	bool LoadTexture(ID3D11Device* device, WCHAR* texturePath);
	void ReleaseTexture();

private:
	ID3D11Buffer* m_VertexBuffer = nullptr;
	ID3D11Buffer* m_IndexBuffer = nullptr;
	int m_vertexCount = 0;
	int m_indexCount = 0;
	Texture* m_Texture = nullptr;
	int m_screenWidth = 0;
	int m_screenHeight = 0;
	int m_bitmapWidth = 0;
	int m_bitmapHeight = 0;
	int m_prevPosX = 0;
	int m_prevPosY = 0;
};

