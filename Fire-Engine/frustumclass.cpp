
//////////////////////////////////////////////////////////
//文件名: frustumclass.cpp		日期: 创建于:2014/1/15
//////////////////////////////////////////////////////////
#include "frustumclass.h"

FrustumClass::FrustumClass()
{

}

FrustumClass::FrustumClass(const FrustumClass& other)
{

}

FrustumClass::~FrustumClass()
{

}

void FrustumClass::ConstructFrustum(float screenDepth, D3DXMATRIX projectionMatrix, D3DXMATRIX viewMatrix)
{
	float zMinimun, r;
	D3DXMATRIX matrix;

	// 计算视锥最小的Z轴深度
	zMinimun = -projectionMatrix._43 / projectionMatrix._33;
	r = screenDepth / (screenDepth - zMinimun);
	projectionMatrix._33 = r;
	projectionMatrix._43 = -r * zMinimun;

	// 创建视锥矩阵
	D3DXMatrixMultiply(&matrix, &viewMatrix, &projectionMatrix);
	
	// 计算靠near视锥的平面
	m_planes[0].a = matrix._14 + matrix._13;
	m_planes[0].b = matrix._24 + matrix._23;
	m_planes[0].c = matrix._34 + matrix._33;
	m_planes[0].d = matrix._44 + matrix._43;
	D3DXPlaneNormalize(&m_planes[0], &m_planes[0]);

	// 计算靠far视锥的平面
	m_planes[1].a = matrix._14 - matrix._13;
	m_planes[1].b = matrix._24 - matrix._23;
	m_planes[1].c = matrix._34 - matrix._33;
	m_planes[1].d = matrix._44 - matrix._43;
	D3DXPlaneNormalize(&m_planes[1], &m_planes[1]);

	// 计算左视锥的平面
	m_planes[2].a = matrix._14 + matrix._13;
	m_planes[2].b = matrix._24 + matrix._23;
	m_planes[2].c = matrix._34 + matrix._33;
	m_planes[2].d = matrix._44 + matrix._43;
	D3DXPlaneNormalize(&m_planes[2], &m_planes[2]);

	// 计算右视锥的平面
	m_planes[3].a = matrix._14 - matrix._13;
	m_planes[3].b = matrix._24 - matrix._23;
	m_planes[3].c = matrix._34 - matrix._33;
	m_planes[3].d = matrix._44 - matrix._43;
	D3DXPlaneNormalize(&m_planes[3], &m_planes[3]);

	// 计算视锥的顶部平面
	m_planes[4].a = matrix._14 - matrix._13;
	m_planes[4].b = matrix._24 - matrix._23;
	m_planes[4].c = matrix._34 - matrix._33;
	m_planes[4].d = matrix._44 - matrix._43;
	D3DXPlaneNormalize(&m_planes[4], &m_planes[4]);

	// 计算靠视锥底部的平面
	m_planes[5].a = matrix._14 + matrix._13;
	m_planes[5].b = matrix._24 + matrix._23;
	m_planes[5].c = matrix._34 + matrix._33;
	m_planes[5].d = matrix._44 + matrix._43;
	D3DXPlaneNormalize(&m_planes[5], &m_planes[5]);

	return;

}

bool FrustumClass::CheckPoint(float x, float y, float z)
{
	// 检测点是否在视锥体内
	for (int i = 0; i < 6; i++)
	{
		if(D3DXPlaneDotCoord(&m_planes[i], &D3DXVECTOR3(x, y, z)) < 0.0f)
		{
			return false;
		}
	}
	return true;
}

bool FrustumClass::CheckCube(float xCenter, float yCenter, float zCenter, float radius)
{
	// 检测正方体上的点是否在视锥体内
	for(int i = 0; i < 6; i++)
	{
		if(D3DXPlaneDotCoord(&m_planes[i], &D3DXVECTOR3((xCenter - radius), (yCenter - radius), (zCenter - radius))) >= 0.0f)
		{
			continue;
		}

		if(D3DXPlaneDotCoord(&m_planes[i], &D3DXVECTOR3((xCenter + radius), (yCenter - radius), (zCenter - radius))) >= 0.0f)
		{
			continue;
		}

		if(D3DXPlaneDotCoord(&m_planes[i], &D3DXVECTOR3((xCenter - radius), (yCenter + radius), (zCenter - radius))) >= 0.0f)
		{
			continue;
		}

		if(D3DXPlaneDotCoord(&m_planes[i], &D3DXVECTOR3((xCenter + radius), (yCenter + radius), (zCenter - radius))) >= 0.0f)
		{
			continue;
		}

		if(D3DXPlaneDotCoord(&m_planes[i], &D3DXVECTOR3((xCenter - radius), (yCenter - radius), (zCenter + radius))) >= 0.0f)
		{
			continue;
		}

		if(D3DXPlaneDotCoord(&m_planes[i], &D3DXVECTOR3((xCenter + radius), (yCenter - radius), (zCenter + radius))) >= 0.0f)
		{
			continue;
		}

		if(D3DXPlaneDotCoord(&m_planes[i], &D3DXVECTOR3((xCenter - radius), (yCenter + radius), (zCenter + radius))) >= 0.0f)
		{
			continue;
		}

		if(D3DXPlaneDotCoord(&m_planes[i], &D3DXVECTOR3((xCenter + radius), (yCenter + radius), (zCenter + radius))) >= 0.0f)
		{
			continue;
		}

		return false;
	}
}

bool FrustumClass::CheckSphere(float xCenter, float yCenter, float zCenter, float radius)
{
	// 检测球体是否在视锥内
	for (int i = 0; i < 6; i++)
	{
		if (D3DXPlaneDotCoord(&m_planes[i], &D3DXVECTOR3(xCenter, yCenter, zCenter)) < -radius)
		{
			return false;
		}
	}
}

bool FrustumClass::CheckRectangle(float xCenter, float yCenter, float zCenter, float xSize, float ySize, float zSize)
{
	for(int i=0; i<6; i++)
	{
		if(D3DXPlaneDotCoord(&m_planes[i], &D3DXVECTOR3((xCenter - xSize), (yCenter - ySize), (zCenter - zSize))) >= 0.0f)
		{
			continue;
		}

		if(D3DXPlaneDotCoord(&m_planes[i], &D3DXVECTOR3((xCenter + xSize), (yCenter - ySize), (zCenter - zSize))) >= 0.0f)
		{
			continue;
		}

		if(D3DXPlaneDotCoord(&m_planes[i], &D3DXVECTOR3((xCenter - xSize), (yCenter + ySize), (zCenter - zSize))) >= 0.0f)
		{
			continue;
		}

		if(D3DXPlaneDotCoord(&m_planes[i], &D3DXVECTOR3((xCenter - xSize), (yCenter - ySize), (zCenter + zSize))) >= 0.0f)
		{
			continue;
		}

		if(D3DXPlaneDotCoord(&m_planes[i], &D3DXVECTOR3((xCenter + xSize), (yCenter + ySize), (zCenter - zSize))) >= 0.0f)
		{
			continue;
		}

		if(D3DXPlaneDotCoord(&m_planes[i], &D3DXVECTOR3((xCenter + xSize), (yCenter - ySize), (zCenter + zSize))) >= 0.0f)
		{
			continue;
		}

		if(D3DXPlaneDotCoord(&m_planes[i], &D3DXVECTOR3((xCenter - xSize), (yCenter + ySize), (zCenter + zSize))) >= 0.0f)
		{
			continue;
		}

		if(D3DXPlaneDotCoord(&m_planes[i], &D3DXVECTOR3((xCenter + xSize), (yCenter + ySize), (zCenter + zSize))) >= 0.0f)
		{
			continue;
		}

		return false;
	}

	return true;
}