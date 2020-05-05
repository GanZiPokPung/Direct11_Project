#pragma once
class ModelList
{
private:
	struct ModelInfo
	{
		XMFLOAT4 color;
		float posX, posY, posZ;
	};

public:
	ModelList();
	~ModelList();

public:
	int GetModelCount();
	void GetData(int index, float& posX, float& posY, float& posZ, XMFLOAT4& color);

public:
	bool Initialize(int number);
	void Shutdown();

private:
	int m_modelCount = 0;
	ModelInfo* m_ModelInfoList = nullptr;
};

