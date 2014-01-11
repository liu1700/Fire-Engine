
//////////////////////////////////////////////////////////
//文件名: lightclass.h		日期: 创建于:2014/1/11
//////////////////////////////////////////////////////////
#ifndef _LIGHTCLASS_H_
#define _LIGHTCLASS_H_

/////////
//INCLUDES //
/////////
#include <D3DX10math.h>

////////////////////////////////////
//类名: LightClass
////////////////////////////////////
class LightClass
{
public:
	LightClass();
	LightClass(const LightClass&);
	~LightClass();

	void SetDiffuseColor(float, float, float, float);
	void SetDirection(float, float, float);

	D3DXVECTOR4 GetDiffuseColor();
	D3DXVECTOR3 GetDirection();

private:
	D3DXVECTOR4 m_diffuseColor;
	D3DXVECTOR3 m_direction;
};


#endif