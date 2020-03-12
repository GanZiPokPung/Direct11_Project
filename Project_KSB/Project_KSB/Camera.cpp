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
	// 카메라의 좌표와 회전을 이용하여 view, proj 행렬을 계산한다.

	XMFLOAT3 up, pos, lookAt;
	XMVECTOR vUp, vPos, vLookAt;
	float yaw, pitch, roll;
	XMMATRIX rotationMatrix;

	up.x = 0.f;
	up.y = 1.f;
	up.z = 0.f;

	vUp = XMLoadFloat3(&up);
	
	pos = m_pos;
	vPos = XMLoadFloat3(&pos);

	lookAt.x = 0.f;
	lookAt.y = 0.f;
	lookAt.z = 1.f;

	vLookAt = XMLoadFloat3(&lookAt);

	pitch = XMConvertToRadians(m_rot.x);
	yaw = XMConvertToRadians(m_rot.y);
	roll = XMConvertToRadians(m_rot.z);

	// yaw pitch roll을 이용하여 회전행렬을 구함
	rotationMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

	// look벡터, up벡터
	vLookAt = XMVector3TransformCoord(vLookAt, rotationMatrix);
	vUp = XMVector3TransformCoord(vUp, rotationMatrix);

	//vLookAt = XMVectorAdd(vPos, vLookAt);

	// 구한 벡터들을 사용하여 view 행렬 구함
	m_view = XMMatrixLookAtLH(vPos, vLookAt, vUp);
}
