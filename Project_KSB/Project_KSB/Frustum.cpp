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

	// ����ü���� �ּ� Z�Ÿ��� ���
	float minimumZ = -pMatrix._43 / pMatrix._33;
	float r = depth / (depth - minimumZ);

	// ������ ���� �ٽ� ���� ��Ŀ� ����
	pMatrix._33 = r;
	pMatrix._43 = -r * minimumZ;
	proj = XMLoadFloat4x4(&pMatrix);

	// ������ǥ��� ��ȯ
	XMMATRIX finalMatrix = XMMatrixMultiply(view, proj);

	XMFLOAT4X4 matrix;
	XMStoreFloat4x4(&matrix, finalMatrix);

	// ����� ���
	float x = (float)(matrix._14 + matrix._13);
	float y = (float)(matrix._24 + matrix._23);
	float z = (float)(matrix._34 + matrix._33);
	float w = (float)(matrix._44 + matrix._43);
	m_Planes[0] = XMVectorSet(x, y, z, w);
	m_Planes[0] = XMPlaneNormalize(m_Planes[0]);

	// �� ���
	x = (float)(matrix._14 - matrix._13);
	y = (float)(matrix._24 - matrix._23);
	z = (float)(matrix._34 - matrix._33);
	w = (float)(matrix._44 - matrix._43);
	m_Planes[1] = XMVectorSet(x, y, z, w);
	m_Planes[1] = XMPlaneNormalize(m_Planes[1]);

	// ���� ���
	x = (float)(matrix._14 + matrix._11);
	y = (float)(matrix._24 + matrix._21);
	z = (float)(matrix._34 + matrix._31);
	w = (float)(matrix._44 + matrix._41);
	m_Planes[2] = XMVectorSet(x, y, z, w);
	m_Planes[2] = XMPlaneNormalize(m_Planes[2]);

	// ������ ���
	x = (float)(matrix._14 - matrix._11);
	y = (float)(matrix._24 - matrix._21);
	z = (float)(matrix._34 - matrix._31);
	w = (float)(matrix._44 - matrix._41);
	m_Planes[3] = XMVectorSet(x, y, z, w);
	m_Planes[3] = XMPlaneNormalize(m_Planes[3]);

	// �� ���
	x = (float)(matrix._14 - matrix._12);
	y = (float)(matrix._24 - matrix._22);
	z = (float)(matrix._34 - matrix._32);
	w = (float)(matrix._44 - matrix._42);
	m_Planes[4] = XMVectorSet(x, y, z, w);
	m_Planes[4] = XMPlaneNormalize(m_Planes[4]);

	// �Ʒ� ���
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
		// ����Ʈ�� �� frustum�� 6�� ��� ��� �ȿ� �ִ��� Ȯ���Ѵ�.
		if (XMVectorGetX(XMPlaneDotCoord(m_Planes[i], XMVectorSet(x, y, z, 1.f))) < 0.f)
			return false;
	}

	return true;
}

bool Frustum::CheckCube(float centerX, float centerY, float centerZ, float radius)
{
	// �� �������ҿ� ť���� �� ���� �ִ��� Ȯ���Ѵ�.
	for (int i = 0; i < 6; ++i)
	{
		// ����� ��� ���� �� ��
		if (XMVectorGetX(XMPlaneDotCoord(m_Planes[i], XMVectorSet((centerX - radius), (centerY - radius), 
			(centerZ - radius), 1.f))) >= 0.f)
			continue;

		if (XMVectorGetX(XMPlaneDotCoord(m_Planes[i], XMVectorSet((centerX + radius), (centerY - radius),
			(centerZ - radius), 1.f))) >= 0.f)
			continue;

		// ����� ��� ���� �� ��
		if (XMVectorGetX(XMPlaneDotCoord(m_Planes[i], XMVectorSet((centerX - radius), (centerY + radius),
			(centerZ - radius), 1.f))) >= 0.f)
			continue;

		if (XMVectorGetX(XMPlaneDotCoord(m_Planes[i], XMVectorSet((centerX + radius), (centerY + radius),
			(centerZ - radius), 1.f))) >= 0.f)
			continue;

		// �� ��� ���� �� ��
		if (XMVectorGetX(XMPlaneDotCoord(m_Planes[i], XMVectorSet((centerX - radius), (centerY - radius),
			(centerZ + radius), 1.f))) >= 0.f)
			continue;

		if (XMVectorGetX(XMPlaneDotCoord(m_Planes[i], XMVectorSet((centerX + radius), (centerY - radius),
			(centerZ + radius), 1.f))) >= 0.f)
			continue;

		// �� ��� ���� �� ��
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
		// ���� �ݰ� �� frustum�� �ȿ� �ִ��� Ȯ���Ѵ�.
		if (XMVectorGetX(XMPlaneDotCoord(m_Planes[i], XMVectorSet(centerX, centerY, centerZ, 1.f))) < -radius)
			return false;
	}

	return true;
}

bool Frustum::CheckRectangle(float centerX, float centerY, float centerZ, float sizeX, float sizeY, float sizeZ)
{
	// �� �������ҿ� �簢���� �� ���� �ִ��� Ȯ���Ѵ�.
	for (int i = 0; i < 6; ++i)
	{
		// ����� ��� ���� �� ��
		if (XMVectorGetX(XMPlaneDotCoord(m_Planes[i], XMVectorSet((centerX - sizeX), (centerY - sizeY),
			(centerZ - sizeZ), 1.f))) >= 0.f)
			continue;

		if (XMVectorGetX(XMPlaneDotCoord(m_Planes[i], XMVectorSet((centerX + sizeX), (centerY - sizeY),
			(centerZ - sizeZ), 1.f))) >= 0.f)
			continue;

		// ����� ��� ���� �� ��
		if (XMVectorGetX(XMPlaneDotCoord(m_Planes[i], XMVectorSet((centerX - sizeX), (centerY + sizeY),
			(centerZ - sizeZ), 1.f))) >= 0.f)
			continue;

		if (XMVectorGetX(XMPlaneDotCoord(m_Planes[i], XMVectorSet((centerX + sizeX), (centerY + sizeY),
			(centerZ - sizeZ), 1.f))) >= 0.f)
			continue;

		// �� ��� ���� �� ��
		if (XMVectorGetX(XMPlaneDotCoord(m_Planes[i], XMVectorSet((centerX - sizeX), (centerY - sizeY),
			(centerZ + sizeZ), 1.f))) >= 0.f)
			continue;

		if (XMVectorGetX(XMPlaneDotCoord(m_Planes[i], XMVectorSet((centerX + sizeX), (centerY - sizeY),
			(centerZ + sizeZ), 1.f))) >= 0.f)
			continue;

		// �� ��� ���� �� ��
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
