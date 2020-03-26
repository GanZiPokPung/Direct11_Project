#pragma once

class Texture;
class ModelType;
class Model3D : public AlignedAllocationPolicy<16>
{
private:
	struct VertexType
	{
		XMFLOAT3 pos;
		XMFLOAT2 tex;
		XMFLOAT3 nor;
	};

	struct ModelType
	{
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
	};

public:
	Model3D();
	Model3D(const Model3D& other);
	~Model3D();

	bool Initialize(ID3D11Device* device, char* modelPath, WCHAR* texturePath);
	void Shutdown();
	void Render(ID3D11DeviceContext* deviceContext);

	int GetIndexCount();
	ID3D11ShaderResourceView* GetTexture();

private:
	bool InitializeBuffers(ID3D11Device* device);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext* deviceContext);

	bool LoadTexture(ID3D11Device* device, WCHAR* texturePath);
	void ReleaseTexture();
	
	bool LoadModel(char* modelPath);
	void ReleaseModel();

private:
	ID3D11Buffer* m_VertexBuffer = nullptr;
	ID3D11Buffer* m_IndexBuffer = nullptr;
	int m_vertexCount = 0;
	int m_indexCount = 0;
	Texture* m_Texture = nullptr;
	ModelType* m_Model = nullptr;
};

