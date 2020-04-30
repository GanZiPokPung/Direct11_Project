#pragma once

class Font;
class FontShader;
class Text : public AlignedAllocationPolicy<16>
{
private:
	struct SentenceType
	{
		ID3D11Buffer* vertexBuffer;
		ID3D11Buffer* indexBuffer;
		int vertexCount, indexCount, maxLength;
		float r, g, b;
	};

	struct VertexType
	{
		XMFLOAT3 pos;
		XMFLOAT2 tex;
	};

public:
	Text();
	Text(const Text& other);
	~Text();

public:
	bool Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, HWND hwnd,
		int screenWidth, int screenHeight, XMMATRIX view);
	void Shutdown();
	bool Render(ID3D11DeviceContext* deviceContext, XMMATRIX world, XMMATRIX ortho);

	bool SetMousePosition(int x, int y, ID3D11DeviceContext* deviceContext);
	bool SetKeyState(char key, ID3D11DeviceContext* deviceContext);

	bool SetFps(int fps, ID3D11DeviceContext* deviceContext);
	bool SetCpu(int cpu, ID3D11DeviceContext* deviceContext);
private:
	bool InitializeSentence(SentenceType** sentence, int maxLength, ID3D11Device* device);
	bool UpdateSentence(SentenceType* sentence, char* text, int posX, int posY, 
		float r, float g, float b, ID3D11DeviceContext* deviceContext);
	void ReleaseSentence(SentenceType** sentence);
	bool RenderSentence(ID3D11DeviceContext* deviceContext, SentenceType* sentence,
		XMMATRIX world, XMMATRIX ortho);

private:
	Font* m_Font = nullptr;
	FontShader* m_FontShader = nullptr;
	int m_screenWidth = 0;
	int m_screenHeight = 0;
	XMMATRIX m_view;
	SentenceType* m_Sentence1 = nullptr;
	SentenceType* m_Sentence2 = nullptr;
	SentenceType* m_Sentence3 = nullptr;
};

