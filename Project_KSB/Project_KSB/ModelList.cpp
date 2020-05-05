#include "pch.h"
#include "ModelList.h"
#include <time.h>

ModelList::ModelList()
{
}

ModelList::~ModelList()
{
}

int ModelList::GetModelCount()
{
	return m_modelCount;
}

void ModelList::GetData(int index, float& posX, float& posY, float& posZ, XMFLOAT4& color)
{
	posX = m_ModelInfoList[index].posX;
	posY = m_ModelInfoList[index].posY;
	posZ = m_ModelInfoList[index].posZ;

	color = m_ModelInfoList[index].color;
}

bool ModelList::Initialize(int number)
{
	float r = 0.f, g = 0.f, b = 0.f;

	m_modelCount = number;

	m_ModelInfoList = new ModelInfo[m_modelCount];

	srand((unsigned int)time(NULL));

	for (int i = 0; i < m_modelCount; ++i)
	{
		r = (float)rand() / RAND_MAX;
		g = (float)rand() / RAND_MAX;
		b = (float)rand() / RAND_MAX;

		m_ModelInfoList[i].color = XMFLOAT4{ r, g, b, 1.f };

		m_ModelInfoList[i].posX = (((float)rand() - (float)rand()) / RAND_MAX) * 10.f;
		m_ModelInfoList[i].posY = (((float)rand() - (float)rand()) / RAND_MAX) * 10.f;
		m_ModelInfoList[i].posZ = (((float)rand() - (float)rand()) / RAND_MAX) * 10.f + 5.f;
	}

	return true;
}

void ModelList::Shutdown()
{
	SAFE_DELETE_ARRAY(m_ModelInfoList);
}
