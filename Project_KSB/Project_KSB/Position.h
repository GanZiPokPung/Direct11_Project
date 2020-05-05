#pragma once
class Position
{
public:
	Position();
	~Position();

public:
	void SetFrameTime(float frameTime);
	void GetRotation(float& rotation);

	void TurnLeft(bool keyDown);
	void TurnRight(bool keyDown);

private:
	float m_frameTime = 0.f;
	float m_rotationY = 0.f;
	float m_leftTurnSpeed = 0.f;
	float m_rightTurnSpeed = 0.f;

};

