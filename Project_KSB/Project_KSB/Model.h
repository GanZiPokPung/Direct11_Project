#pragma once
class Model : public AlignedAllocationPolicy<16>
{
private:
	struct VertexType
	{
		XMFLOAT3 pos;
		XMFLOAT4 color;
	};

public:
	Model();
	Model(const Model& other);
	~Model();

	bool Initialize(ID3D11Device* device);
	void Shutdown();
	void Render(ID3D11DeviceContext* deviceContext);

	int GetIndexCount();

private:
	bool InitializeBuffers(ID3D11Device* device);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext* deviceContext);

private:
	ID3D11Buffer* m_VertexBuffer = nullptr;
	ID3D11Buffer* m_IndexBuffer = nullptr;
	int m_vertexCount = 0;
	int m_indexCount = 0;
};

