
//////////////////////////////////////////////////////////
//文件名: cameraclass.cpp		日期: 创建于:2014/1/10
//////////////////////////////////////////////////////////
#define onedegree	0.0174532925f

#include "cameraclass.h"

CameraClass::CameraClass()
{
	m_positionX = 0.0f;
	m_positionY = 0.0f;
	m_positionZ = 0.0f;

	m_rotationX = 0.0f;
	m_rotationY = 0.0f;
	m_rotationZ = 0.0f;
}

CameraClass::CameraClass(const CameraClass& other)
{

}

CameraClass::~CameraClass()
{

}

void CameraClass::SetPosition(float x, float y, float z)
{
	m_positionX = x;
	m_positionY = y;
	m_positionZ = z;

	return;
}

void CameraClass::SetRotation(float x, float y, float z)
{
	m_rotationX = x;
	m_rotationY = y;
	m_rotationZ = z;

	return;
}

void CameraClass::UpdatePosition(float x, float y, float z)
{
	m_positionX = x;
	m_positionY = y;
	m_positionZ = z;

	return;
}

void CameraClass::UpdateRotation(float x, float y, float z)
{
	m_rotationX = x;
	m_rotationY = y;
	m_rotationZ = z;

	return;
}

D3DXVECTOR3 CameraClass::GetPosition()
{
	return D3DXVECTOR3(m_positionX, m_positionY, m_positionZ);
}

D3DXVECTOR3 CameraClass::GetRotation()
{
	return D3DXVECTOR3(m_rotationX, m_rotationY, m_rotationZ);
}

void CameraClass::Render()
{
	D3DXVECTOR3 up, position, lookAt;
	float yaw, pitch, roll;
	D3DXMATRIX rotationMatrix;

	// 指向上方的向量
	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;
	
	// 设定观察点在世界中的位置
	position.x = m_positionX;
	position.y = m_positionY;
	position.z = m_positionZ;

	// 设定默认观察点观察的方向
	lookAt.x = 0.0f;
	lookAt.y = 0.0f;
	lookAt.z = 1.0f;

	// 设定以弧度为单位，设定关于Y（yaw）， X（pitch）， Z（roll）轴的旋转 1 度 = 0.0174532925弧度
	pitch = m_rotationX * onedegree;
	yaw = m_rotationY * onedegree;
	roll = m_rotationZ * onedegree;

	// 创建旋转矩阵
	D3DXMatrixRotationYawPitchRoll(&rotationMatrix, yaw, pitch, roll);

	// 根据旋转矩阵将其他矩阵伴随变换
	D3DXVec3TransformCoord(&lookAt, &lookAt, &rotationMatrix);
	D3DXVec3TransformCoord(&up, &up, &rotationMatrix);

	// 旋转后改变观察向量的值
	lookAt = position + lookAt;

	// 创建观察矩阵
	D3DXMatrixLookAtLH(&m_viewMatrix, &position, &lookAt, &up);

	return;

}

void CameraClass::GetViewMatrix(D3DXMATRIX& viewMatrix)
{
	viewMatrix = m_viewMatrix;

	return;
}