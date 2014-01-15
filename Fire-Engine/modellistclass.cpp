
//////////////////////////////////////////////////////////
//文件名: modellistclass.cpp		日期: 创建于:2014/1/15
//////////////////////////////////////////////////////////
#include "modellistclass.h"

ModelListClass::ModelListClass()
{
	m_ModelInfoList = NULL;
}

ModelListClass::ModelListClass(const ModelListClass& other)
{

}

ModelListClass::~ModelListClass()
{

}

bool ModelListClass::Initialze(int numModels)
{
	float red, green, blue;

	// 存储模型数量
	m_modelCount = numModels;

	// 创建模型信息的数组
	m_ModelInfoList = new ModelInfoType[m_modelCount];
	if(!m_ModelInfoList)
		return false;

	// 随机生成颜色与位置
	srand((unsigned int)time(NULL));

	// 随机赋值
	for(int i=0; i<m_modelCount; i++)
	{
		red = (float)rand() / RAND_MAX;
		green = (float)rand() / RAND_MAX;
		blue = (float)rand() / RAND_MAX;

		m_ModelInfoList[i].color = D3DXVECTOR4(red, green, blue, 1.0f);

		m_ModelInfoList[i].positionX = (((float)rand()-(float)rand())/RAND_MAX) * 10.0f;
		m_ModelInfoList[i].positionY = (((float)rand()-(float)rand())/RAND_MAX) * 10.0f;
		m_ModelInfoList[i].positionZ = ((((float)rand()-(float)rand())/RAND_MAX) * 10.0f) + 5.0f;
	}

	return true;
}

void ModelListClass::ShutDown()
{
	if (m_ModelInfoList)
	{
		delete []m_ModelInfoList;
		m_ModelInfoList = NULL;
	}

	return;
}

int ModelListClass::GetModelCount()
{
	return m_modelCount;
}

void ModelListClass::GetData(int index, float& positionX, float& positionY, float& positionZ, D3DXVECTOR4& color)
{
	// 获取模型组中模型的信息
	positionX = m_ModelInfoList[index].positionX;
	positionY = m_ModelInfoList[index].positionY;
	positionZ = m_ModelInfoList[index].positionZ;

	color = m_ModelInfoList[index].color;

	return;
}