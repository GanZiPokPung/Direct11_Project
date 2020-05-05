#include "pch.h"
#include "Camera.h"

Camera::Camera()
{
	m_pos = XMFLOAT3(0.f, 0.f, 0.f);
	m_rot = XMFLOAT3(0.f, 0.f, 0.f);
}

Camera::Camera(const Camera& other)
{
}

Camera::~Camera()
{
}

void Camera::SetPosition(float x, float y, float z)
{
	m_pos.x = x;
	m_pos.y = y;
	m_pos.z = z;
}

void Camera::SetRotation(float x, float y, float z)
{
	m_rot.x = x;
	m_rot.y = y;
	m_rot.z = z;
}

void Camera::Render()
{
	// ī�޶��� ��ǥ�� ȸ���� �̿��Ͽ� view, proj ����� ����Ѵ�.

	XMFLOAT3 up, pos, lookAt;
	XMVECTOR vUp, vPos, vLookAt;

	up.x = 0.f;
	up.y = 1.f;
	up.z = 0.f;

	vUp = XMLoadFloat3(&up);
	
	pos = m_pos;
	vPos = XMLoadFloat3(&pos);

	float radians = m_rot.y * 0.0174532925f;

	lookAt.x = sinf(radians) + m_pos.x;
	lookAt.y = m_pos.y;
	lookAt.z = cosf(radians) + m_pos.z;

	vLookAt = XMLoadFloat3(&lookAt);

	// ���� ���͵��� ����Ͽ� view ��� ����
	m_view = XMMatrixLookAtLH(vPos, vLookAt, vUp);
}
