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


};

