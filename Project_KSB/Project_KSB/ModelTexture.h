#pragma once

class Texture;
class ModelTexture : public AlignedAllocationPolicy<16>
{
private:
	struct VertexType
	{
		XMFLOAT3 pos;
		XMFLOAT2 tex;
	};

public:
	ModelTexture();
	ModelTexture(const ModelTexture& other);
	~ModelTexture();

	bool Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext,
		char* filePath);
	void Shutdown();
	void Render(ID3D11DeviceContext* deviceContext);

	int GetIndexCount();
	ID3D11ShaderResourceView* GetTexture();

private:
	bool InitializeBuffers(ID3D11Device* device);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext* deviceContext);

	bool LoadTexture(ID3D11Device* device, ID3D11DeviceContext* deviceContext,
		char* filePath);
	void ReleaseTexture();

private:
	ID3D11Buffer* m_VertexBuffer = nullptr;
	ID3D11Buffer* m_IndexBuffer = nullptr;
	int m_vertexCount = 0;
	int m_indexCount = 0;
	Texture* m_Texture = nullptr;
};

