#pragma once

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
};

