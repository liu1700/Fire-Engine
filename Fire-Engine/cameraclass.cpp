
//////////////////////////////////////////////////////////
//�ļ���: cameraclass.cpp		����: ������:2014/1/10
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

	// ָ���Ϸ�������
	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;
	
	// �趨�۲���������е�λ��
	position.x = m_positionX;
	position.y = m_positionY;
	position.z = m_positionZ;

	// �趨Ĭ�Ϲ۲��۲�ķ���
	lookAt.x = 0.0f;
	lookAt.y = 0.0f;
	lookAt.z = 1.0f;

	// �趨�Ի���Ϊ��λ���趨����Y��yaw���� X��pitch���� Z��roll�������ת 1 �� = 0.0174532925����
	pitch = m_rotationX * onedegree;
	yaw = m_rotationY * onedegree;
	roll = m_rotationZ * onedegree;

	// ������ת����
	D3DXMatrixRotationYawPitchRoll(&rotationMatrix, yaw, pitch, roll);

	// ������ת���������������任
	D3DXVec3TransformCoord(&lookAt, &lookAt, &rotationMatrix);
	D3DXVec3TransformCoord(&up, &up, &rotationMatrix);

	// ��ת��ı�۲�������ֵ
	lookAt = position + lookAt;

	// �����۲����
	D3DXMatrixLookAtLH(&m_viewMatrix, &position, &lookAt, &up);

	return;

}

void CameraClass::GetViewMatrix(D3DXMATRIX& viewMatrix)
{
	viewMatrix = m_viewMatrix;

	return;
}