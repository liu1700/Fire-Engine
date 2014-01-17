
//////////////////////////////////////////////////////////
//文件名: positionclass.h		日期: 创建于:2014/1/15
//////////////////////////////////////////////////////////
#ifndef _POSITIONCLASS_H_
#define _POSITIONCLASS_H_

/////////
//INCLUDES //
/////////
#include <math.h>

////////////////////////////////////
//类名: PositionClass
////////////////////////////////////
class PositionClass
{
public:
	PositionClass();
	PositionClass(const PositionClass&);
	~PositionClass();

	void SetFrameTimer(float);
	void GetRotation(float&, float&);
	void GetTransfer(float&, float&);

	void TurnLeft(bool);
	void TurnRight(bool);
	void LookUp(bool);
	void LookDown(bool);

	void MoveForward(bool);
	void MoveBack(bool);
	void MoveLeft(bool);
	void MoveRight(bool);

private:
	float m_frameTime;
	float m_transferZ, m_transferX;
	float m_rotationY, m_rotationX;
	float m_leftTurnSpeed, m_rightTurnSpeed,
			m_headUpSpeed, m_headDownSpeed;
	float m_moveForwardSpeed, m_moveBackSpeed,
			m_moveLeftSpeed, m_moveRightSpeed;
};

#endif