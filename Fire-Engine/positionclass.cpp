
//////////////////////////////////////////////////////////
//文件名: positionclass.cpp		日期: 创建于:2014/1/15
//////////////////////////////////////////////////////////
#include "positionclass.h"

PositionClass::PositionClass()
{
	m_frameTime = 0.0f;
	m_rotationY = 0.0f;
	m_rotationX = 0.0f;
	m_headUpSpeed = 0.0f;
	m_headDownSpeed = 0.0f;
	m_leftTurnSpeed = 0.0f;
	m_rightTurnSpeed = 0.0f;
}

PositionClass::PositionClass(const PositionClass& other)
{

}

PositionClass::~PositionClass()
{

}

void PositionClass::SetFrameTimer(float time)
{
	// 根据帧速率来计算视角变换的速度
	m_frameTime = time;
	return;
}

void PositionClass::GetRotation(float& x, float& y)
{
	x = m_rotationX;
	y = m_rotationY;
	return;
}

void PositionClass::GetTransfer(float& x, float& z)
{
	x = m_transferX;
	z = m_transferZ;
}

void PositionClass::TurnLeft(bool keydown)
{
	// 平滑的移动镜头，有缓冲感
	if(keydown)
	{
		m_leftTurnSpeed += m_frameTime * 0.01f;

		if(m_leftTurnSpeed > (m_frameTime * 0.15f))
		{
			m_leftTurnSpeed = m_frameTime * 0.15f;
		}
	}
	else
	{
		m_leftTurnSpeed -= m_frameTime* 0.005f;

		if(m_leftTurnSpeed < 0.0f)
		{
			m_leftTurnSpeed = 0.0f;
		}
	}

	// 更新旋转速度
	m_rotationY -= m_leftTurnSpeed;
	if(m_rotationY < 0.0f)
	{
		m_rotationY += 360.0f;
	}

	return;
}

void PositionClass::TurnRight(bool keydown)
{
	// 和左转相似
	if(keydown)
	{
		m_rightTurnSpeed += m_frameTime * 0.01f;

		if(m_rightTurnSpeed > (m_frameTime * 0.15f))
		{
			m_rightTurnSpeed = m_frameTime * 0.15f;
		}
	}
	else
	{
		m_rightTurnSpeed -= m_frameTime* 0.005f;

		if(m_rightTurnSpeed < 0.0f)
		{
			m_rightTurnSpeed = 0.0f;
		}
	}

	m_rotationY += m_rightTurnSpeed;
	if(m_rotationY > 360.0f)
	{
		m_rotationY -= 360.0f;
	}

	return;
}

void PositionClass::LookUp(bool keydown)
{
	// 平滑的移动镜头，有缓冲感
	if(keydown)
	{
		m_headUpSpeed += m_frameTime * 0.01f;

		if(m_headUpSpeed > (m_frameTime * 0.15f))
		{
			m_headUpSpeed = m_frameTime * 0.15f;
		}
	}
	else
	{
		m_headUpSpeed -= m_frameTime* 0.005f;

		if(m_headUpSpeed < 0.0f)
		{
			m_headUpSpeed = 0.0f;
		}
	}

	// 更新旋转速度
	m_rotationX -= m_headUpSpeed;
	if(m_rotationX < 0.0f)
	{
		m_rotationX += 360.0f;
	}

	return;
}

void PositionClass::LookDown(bool keydown)
{
	// 平滑的移动镜头，有缓冲感
	if(keydown)
	{
		m_headDownSpeed += m_frameTime * 0.01f;

		if(m_headDownSpeed > (m_frameTime * 0.15f))
		{
			m_headDownSpeed = m_frameTime * 0.15f;
		}
	}
	else
	{
		m_headDownSpeed -= m_frameTime* 0.005f;

		if(m_headDownSpeed < 0.0f)
		{
			m_headDownSpeed = 0.0f;
		}
	}

	// 更新旋转速度
	m_rotationX += m_headDownSpeed;
	if(m_rotationX < 0.0f)
	{
		m_rotationX += 360.0f;
	}

	return;
}

void PositionClass::MoveForward( bool keydown )
{
	// 平滑的移动镜头，有缓冲感
	if(keydown)
	{
		m_moveForwardSpeed += m_frameTime * 0.01f;

		if(m_moveForwardSpeed > (m_frameTime * 0.15f))
		{
			m_moveForwardSpeed = m_frameTime * 0.15f;
		}
	}
	else
	{
		m_moveForwardSpeed -= m_frameTime* 0.005f;

		if(m_moveForwardSpeed < 0.0f)
		{
			m_moveForwardSpeed = 0.0f;
		}
	}
	m_transferZ += m_moveForwardSpeed;

	return;
}

void PositionClass::MoveBack( bool keydown )
{
	// 平滑的移动镜头，有缓冲感
	if(keydown)
	{
		m_moveBackSpeed += m_frameTime * 0.01f;

		if(m_moveBackSpeed > (m_frameTime * 0.15f))
		{
			m_moveBackSpeed = m_frameTime * 0.15f;
		}
	}
	else
	{
		m_moveBackSpeed -= m_frameTime* 0.005f;

		if(m_moveBackSpeed < 0.0f)
		{
			m_moveBackSpeed = 0.0f;
		}
	}
	m_transferZ -= m_moveBackSpeed;

	return;
}

void PositionClass::MoveLeft( bool keydown )
{
	// 平滑的移动镜头，有缓冲感
	if(keydown)
	{
		m_moveLeftSpeed += m_frameTime * 0.01f;

		if(m_moveLeftSpeed > (m_frameTime * 0.15f))
		{
			m_moveLeftSpeed = m_frameTime * 0.15f;
		}
	}
	else
	{
		m_moveLeftSpeed -= m_frameTime* 0.005f;

		if(m_moveLeftSpeed < 0.0f)
		{
			m_moveLeftSpeed = 0.0f;
		}
	}
	m_transferX -= m_moveLeftSpeed;

	return;
}

void PositionClass::MoveRight( bool keydown )
{
	// 平滑的移动镜头，有缓冲感
	if(keydown)
	{
		m_moveRightSpeed += m_frameTime * 0.01f;

		if(m_moveRightSpeed > (m_frameTime * 0.15f))
		{
			m_moveRightSpeed = m_frameTime * 0.15f;
		}
	}
	else
	{
		m_moveRightSpeed -= m_frameTime* 0.005f;

		if(m_moveRightSpeed < 0.0f)
		{
			m_moveRightSpeed = 0.0f;
		}
	}
	m_transferX += m_moveRightSpeed;

	return;
}
