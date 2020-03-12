#pragma once
class TextureShader : public AlignedAllocationPolicy<16>
{
private:
	struct MatrixBufferType
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX proj;
	};

public:
	TextureShader();
	TextureShader(const TextureShader& other);
	~TextureShader();

	bool Initialize(ID3D11Device* device, HWND hwnd);
	void Shutdown();
	bool Render(ID3D11DeviceContext* deviceContext, int indexCount, 
		XMMATRIX world, XMMATRIX view, XMMATRIX proj, ID3D11ShaderResourceView* texture);

private:
	bool InitializeShader(ID3D11Device* device, HWND hwnd, WCHAR* vsPath, WCHAR* psPath);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob* blob, HWND hwnd, WCHAR* shaderPath);

	bool SetShaderParameters(ID3D11DeviceContext* deviceContext, XMMATRIX world, XMMATRIX view, XMMATRIX proj,
		ID3D11ShaderResourceView* texture);
	void RenderShader(ID3D11DeviceContext* deviceContext, int indexCount);

private:
	ID3D11VertexShader* m_VertexShader = nullptr;
	ID3D11PixelShader* m_PixelShader = nullptr;
	ID3D11InputLayout* m_Layout = nullptr;
	ID3D11Buffer* m_MatrixBuffer = nullptr;

	ID3D11SamplerState* m_SampleState = nullptr;
};

