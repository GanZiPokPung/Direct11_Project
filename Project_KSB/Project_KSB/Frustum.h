#pragma once
class Frustum : public AlignedAllocationPolicy<16>
{
public:
	Frustum();
	~Frustum();

public:
	void ConstructFrustum(float depth, XMMATRIX proj, XMMATRIX view);

	bool CheckPoint(float x, float y, float z);
	bool CheckCube(float centerX, float centerY, float centerZ, float radius);
	bool CheckSphere(float centerX, float centerY, float centerZ, float radius);
	bool CheckRectangle(float centerX, float centerY, float centerZ, float sizeX, float sizeY, float sizeZ);

private:
	XMVECTOR m_Planes[6];
};

