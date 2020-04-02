#pragma once
class Direct3D : public AlignedAllocationPolicy<16>
{
public:
	Direct3D();
	Direct3D(const Direct3D& other);
	~Direct3D();

public:
	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetDeviceContext();

	void GetProjectionMatrix(XMMATRIX& proj);
	void GetWorldMatrix(XMMATRIX& world);
	void GetOrthoMatrix(XMMATRIX& ortho);
	void SaveVideoCardInfo();

public:
	bool Initialize(int width, int height, bool vsync, HWND hwnd,
					bool fullscreen, float Far, float Near);
	void Shutdown();
	
	void BeginScene(float r, float g, float b, float a);
	void EndScene();

	void TurnZBufferOn();
	void TurnZBufferOff();

private:
	bool m_vsyncCheck = false;

	int m_videoCardMemory = 0;
	char m_videoCardDescription[128] = { 0, };

	IDXGISwapChain*				m_SwapChain = nullptr;
	ID3D11Device*				m_Device = nullptr;
	ID3D11DeviceContext*		m_DeviceContext = nullptr;

	ID3D11RenderTargetView*		m_RenderTargetView = nullptr;
	ID3D11DepthStencilView*		m_DepthStencilView = nullptr;

	ID3D11Texture2D*			m_DepthStencilBuffer = nullptr;

	ID3D11DepthStencilState*	m_DepthStencilState = nullptr;
	ID3D11RasterizerState*		m_RasterState = nullptr;

	XMMATRIX m_projection;
	XMMATRIX m_world;
	XMMATRIX m_ortho;

	ID3D11DepthStencilState* m_DepthDisabledStencilState = nullptr;
};

