
//////////////////////////////////////////////////////////
//�ļ���: positionclass.h		����: ������:2014/1/15
//////////////////////////////////////////////////////////
#ifndef _POSITIONCLASS_H_
#define _POSITIONCLASS_H_

/////////
//INCLUDES //
/////////
#include <math.h>

////////////////////////////////////
//����: PositionClass
////////////////////////////////////
class PositionClass
{
public:
	PositionClass();
	PositionClass(const PositionClass&);
	~PositionClass();

	void SetFrameTimer(float);
	void GetRotation(float&);

	void TurnLeft(bool);
	void TurnRight(bool);

private:
	float m_frameTime;
	float m_rotationY;
	float m_leftTurnSpeed, m_rightTurnSpeed;
};

#endif