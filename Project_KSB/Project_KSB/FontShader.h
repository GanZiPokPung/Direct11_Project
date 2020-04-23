#pragma once
class FontShader
{
private:
	struct MatrixBufferType
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX proj;
	};

	struct PixelBufferType
	{
		XMFLOAT4 pixelColor;
	};

public:
	FontShader();
	FontShader(const FontShader& other);
	~FontShader(); 

public:
	bool Initialize(ID3D11Device* device, HWND hWnd);
	void Shutdown();
	bool Render(ID3D11DeviceContext* deviceContext, int indexCount,
		XMMATRIX world, XMMATRIX view, XMMATRIX proj,
		ID3D11ShaderResourceView* texture, XMFLOAT4 color);
	
private:
	bool InitializeShader(ID3D11Device* device, HWND hwnd, WCHAR* vsPath, WCHAR* psPath);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob* blob, HWND hwnd, WCHAR* shaderPath);

	bool SetShaderParameters(ID3D11DeviceContext* deviceContext, XMMATRIX world, XMMATRIX view,
		XMMATRIX proj, ID3D11ShaderResourceView* texture, XMFLOAT4 color);
	void RenderShader(ID3D11DeviceContext* deviceContext, int indexCount);

private:
	ID3D11VertexShader* m_VertexShader = nullptr;
	ID3D11PixelShader* m_PixelShader = nullptr;
	ID3D11InputLayout* m_Layout = nullptr;
	ID3D11Buffer* m_MatrixBuffer = nullptr;
	ID3D11SamplerState* m_SampleState = nullptr;
	ID3D11Buffer* m_PixelBuffer = nullptr;

};

