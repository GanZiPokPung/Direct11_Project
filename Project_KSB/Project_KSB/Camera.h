#pragma once
class Camera : public AlignedAllocationPolicy<16>
{
public:
	Camera();
	Camera(const Camera& other);
	~Camera();

public:
	XMFLOAT3 GetPosition() { return m_pos; }
	XMFLOAT3 GetRotation() { return m_rot; }

	void SetPosition(float x, float y, float z);
	void SetRotation(float x, float y, float z);

	void GetViewMatrix(XMMATRIX& view) { view = m_view; }

public:
	void Render();

private:
	XMFLOAT3 m_pos;
	XMFLOAT3 m_rot;
	XMMATRIX m_view;

};

