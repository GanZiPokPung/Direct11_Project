#pragma once

#define TEXTURE_MODE

class Direct3D;
class Camera;
class Model;
class ColorShader;
class ModelTexture;
class TextureShader;
class Graphic
{
public:
	Graphic();
	Graphic(const Graphic& other);
	~Graphic();

	bool Initialize(int width, int height, HWND hwnd);
	void Shutdown();
	bool Frame();

private:
	bool Render();

private:
	Direct3D* m_Direct3D = nullptr;
	Camera* m_Camera = nullptr;

#ifdef COLOR_MODE
	// color
	Model* m_Model = nullptr;
	ColorShader* m_ColorShader = nullptr;
#endif

#ifdef TEXTURE_MODE
	// texture
	ModelTexture* m_ModelTexture = nullptr;
	TextureShader* m_TextureShader = nullptr;
#endif
};

