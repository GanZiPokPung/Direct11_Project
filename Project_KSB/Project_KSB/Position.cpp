#include "pch.h"
#include "Position.h"

Position::Position()
{
}

Position::~Position()
{
}

void Position::SetFrameTime(float frameTime)
{
	m_frameTime = frameTime;
}

void Position::GetRotation(float& rotation)
{
	rotation = m_rotationY;
}

void Position::TurnLeft(bool keyDown)
{
	if (keyDown)
	{
		m_leftTurnSpeed += m_frameTime * 0.01f;

		if (m_leftTurnSpeed > (m_frameTime * 0.15f))
			m_leftTurnSpeed = m_frameTime * 0.15f;
	}
	else
	{
		m_leftTurnSpeed -= m_frameTime * 0.005f;

		if (m_leftTurnSpeed < 0.f)
			m_leftTurnSpeed = 0.f;
	}

	m_rotationY -= m_leftTurnSpeed;
	if (m_rotationY < 0.f)
		m_rotationY += 360.f;
}

void Position::TurnRight(bool keyDown)
{
	if (keyDown)
	{
		m_rightTurnSpeed += m_frameTime * 0.01f;

		if (m_rightTurnSpeed > (m_frameTime * 0.15f))
			m_rightTurnSpeed = m_frameTime * 0.15f;
	}
	else
	{
		m_rightTurnSpeed -= m_frameTime * 0.005f;

		if (m_rightTurnSpeed < 0.f)
			m_rightTurnSpeed = 0.f;
	}

	m_rotationY += m_rightTurnSpeed;
	if (m_rotationY > 360.f)
		m_rotationY -= 360.f;
}
