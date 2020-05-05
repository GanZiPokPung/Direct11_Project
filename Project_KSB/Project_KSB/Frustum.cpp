#include "pch.h"
#include "Frustum.h"

Frustum::Frustum()
{
}

Frustum::~Frustum()
{
}

void Frustum::ConstructFrustum(float depth, XMMATRIX proj, XMMATRIX view)
{
	XMFLOAT4X4 pMatrix;
	XMStoreFloat4x4(&pMatrix, proj);

	// 절두체에서 최소 Z거리를 계산
	float minimumZ = -pMatrix._43 / pMatrix._33;
	float r = depth / (depth - minimumZ);

	// 갱신한 값을 다시 투영 행렬에 설정
	pMatrix._33 = r;
	pMatrix._43 = -r * minimumZ;
	proj = XMLoadFloat4x4(&pMatrix);

	// 동차좌표계로 변환
	XMMATRIX finalMatrix = XMMatrixMultiply(view, proj);

	XMFLOAT4X4 matrix;
	XMStoreFloat4x4(&matrix, finalMatrix);

	// 가까운 평면
	float x = (float)(matrix._14 + matrix._13);
	float y = (float)(matrix._24 + matrix._23);
	float z = (float)(matrix._34 + matrix._33);
	float w = (float)(matrix._44 + matrix._43);
	m_Planes[0] = XMVectorSet(x, y, z, w);
	m_Planes[0] = XMPlaneNormalize(m_Planes[0]);

	// 먼 평면
	x = (float)(matrix._14 - matrix._13);
	y = (float)(matrix._24 - matrix._23);
	z = (float)(matrix._34 - matrix._33);
	w = (float)(matrix._44 - matrix._43);
	m_Planes[1] = XMVectorSet(x, y, z, w);
	m_Planes[1] = XMPlaneNormalize(m_Planes[1]);

	// 왼쪽 평면
	x = (float)(matrix._14 + matrix._11);
	y = (float)(matrix._24 + matrix._21);
	z = (float)(matrix._34 + matrix._31);
	w = (float)(matrix._44 + matrix._41);
	m_Planes[2] = XMVectorSet(x, y, z, w);
	m_Planes[2] = XMPlaneNormalize(m_Planes[2]);

	// 오른쪽 평면
	x = (float)(matrix._14 - matrix._11);
	y = (float)(matrix._24 - matrix._21);
	z = (float)(matrix._34 - matrix._31);
	w = (float)(matrix._44 - matrix._41);
	m_Planes[3] = XMVectorSet(x, y, z, w);
	m_Planes[3] = XMPlaneNormalize(m_Planes[3]);

	// 윗 평면
	x = (float)(matrix._14 - matrix._12);
	y = (float)(matrix._24 - matrix._22);
	z = (float)(matrix._34 - matrix._32);
	w = (float)(matrix._44 - matrix._42);
	m_Planes[4] = XMVectorSet(x, y, z, w);
	m_Planes[4] = XMPlaneNormalize(m_Planes[4]);

	// 아래 평면
	x = (float)(matrix._14 + matrix._12);
	y = (float)(matrix._24 + matrix._22);
	z = (float)(matrix._34 + matrix._32);
	w = (float)(matrix._44 + matrix._42);
	m_Planes[5] = XMVectorSet(x, y, z, w);
	m_Planes[5] = XMPlaneNormalize(m_Planes[5]);
}

bool Frustum::CheckPoint(float x, float y, float z)
{
	for (int i = 0; i < 6; ++i)
	{
		// 포인트가 뷰 frustum의 6개 평면 모두 안에 있는지 확인한다.
		if (XMVectorGetX(XMPlaneDotCoord(m_Planes[i], XMVectorSet(x, y, z, 1.f))) < 0.f)
			return false;
	}

	return true;
}

bool Frustum::CheckCube(float centerX, float centerY, float centerZ, float radius)
{
	// 뷰 프러스텀에 큐브의 한 점이 있는지 확인한다.
	for (int i = 0; i < 6; ++i)
	{
		// 가까운 평면 밑쪽 두 점
		if (XMVectorGetX(XMPlaneDotCoord(m_Planes[i], XMVectorSet((centerX - radius), (centerY - radius), 
			(centerZ - radius), 1.f))) >= 0.f)
			continue;

		if (XMVectorGetX(XMPlaneDotCoord(m_Planes[i], XMVectorSet((centerX + radius), (centerY - radius),
			(centerZ - radius), 1.f))) >= 0.f)
			continue;

		// 가까운 평면 위쪽 두 점
		if (XMVectorGetX(XMPlaneDotCoord(m_Planes[i], XMVectorSet((centerX - radius), (centerY + radius),
			(centerZ - radius), 1.f))) >= 0.f)
			continue;

		if (XMVectorGetX(XMPlaneDotCoord(m_Planes[i], XMVectorSet((centerX + radius), (centerY + radius),
			(centerZ - radius), 1.f))) >= 0.f)
			continue;

		// 먼 평면 밑쪽 두 점
		if (XMVectorGetX(XMPlaneDotCoord(m_Planes[i], XMVectorSet((centerX - radius), (centerY - radius),
			(centerZ + radius), 1.f))) >= 0.f)
			continue;

		if (XMVectorGetX(XMPlaneDotCoord(m_Planes[i], XMVectorSet((centerX + radius), (centerY - radius),
			(centerZ + radius), 1.f))) >= 0.f)
			continue;

		// 먼 평면 위쪽 두 점
		if (XMVectorGetX(XMPlaneDotCoord(m_Planes[i], XMVectorSet((centerX - radius), (centerY + radius),
			(centerZ + radius), 1.f))) >= 0.f)
			continue;

		if (XMVectorGetX(XMPlaneDotCoord(m_Planes[i], XMVectorSet((centerX + radius), (centerY + radius),
			(centerZ + radius), 1.f))) >= 0.f)
			continue;

		return false;
	}

	return true;
}

bool Frustum::CheckSphere(float centerX, float centerY, float centerZ, float radius)
{
	for (int i = 0; i < 6; ++i)
	{
		// 구의 반경 뷰 frustum의 안에 있는지 확인한다.
		if (XMVectorGetX(XMPlaneDotCoord(m_Planes[i], XMVectorSet(centerX, centerY, centerZ, 1.f))) < -radius)
			return false;
	}

	return true;
}

bool Frustum::CheckRectangle(float centerX, float centerY, float centerZ, float sizeX, float sizeY, float sizeZ)
{
	// 뷰 프러스텀에 사각형이 한 점이 있는지 확인한다.
	for (int i = 0; i < 6; ++i)
	{
		// 가까운 평면 밑쪽 두 점
		if (XMVectorGetX(XMPlaneDotCoord(m_Planes[i], XMVectorSet((centerX - sizeX), (centerY - sizeY),
			(centerZ - sizeZ), 1.f))) >= 0.f)
			continue;

		if (XMVectorGetX(XMPlaneDotCoord(m_Planes[i], XMVectorSet((centerX + sizeX), (centerY - sizeY),
			(centerZ - sizeZ), 1.f))) >= 0.f)
			continue;

		// 가까운 평면 위쪽 두 점
		if (XMVectorGetX(XMPlaneDotCoord(m_Planes[i], XMVectorSet((centerX - sizeX), (centerY + sizeY),
			(centerZ - sizeZ), 1.f))) >= 0.f)
			continue;

		if (XMVectorGetX(XMPlaneDotCoord(m_Planes[i], XMVectorSet((centerX + sizeX), (centerY + sizeY),
			(centerZ - sizeZ), 1.f))) >= 0.f)
			continue;

		// 먼 평면 밑쪽 두 점
		if (XMVectorGetX(XMPlaneDotCoord(m_Planes[i], XMVectorSet((centerX - sizeX), (centerY - sizeY),
			(centerZ + sizeZ), 1.f))) >= 0.f)
			continue;

		if (XMVectorGetX(XMPlaneDotCoord(m_Planes[i], XMVectorSet((centerX + sizeX), (centerY - sizeY),
			(centerZ + sizeZ), 1.f))) >= 0.f)
			continue;

		// 먼 평면 위쪽 두 점
		if (XMVectorGetX(XMPlaneDotCoord(m_Planes[i], XMVectorSet((centerX - sizeX), (centerY + sizeY),
			(centerZ + sizeZ), 1.f))) >= 0.f)
			continue;

		if (XMVectorGetX(XMPlaneDotCoord(m_Planes[i], XMVectorSet((centerX + sizeX), (centerY + sizeY),
			(centerZ + sizeZ), 1.f))) >= 0.f)
			continue;

		return false;
	}

	return true;
}
