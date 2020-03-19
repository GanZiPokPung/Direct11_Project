#pragma once

//#define COLOR_MODE
//#define TEXTURE_MODE
#define LIGHT_MODE

class Direct3D;
class Camera;
class Model;
class ColorShader;
class ModelTexture;
class TextureShader;
class ModelLight;
class LightShader;
class Light;
class Input;
class Graphic
{
public:
	Graphic();
	Graphic(const Graphic& other);
	~Graphic();

	bool Initialize(int width, int height, HWND hwnd);
	void Shutdown();
	bool Frame();

	// 임시 코드
	bool UpdateInput(Input* input);

private:
	bool Render(float rotation);

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

#ifdef LIGHT_MODE
	// light
	ModelLight* m_ModelLight = nullptr;
	LightShader* m_LightShader = nullptr;
	Light* m_Light = nullptr;
#endif
};

