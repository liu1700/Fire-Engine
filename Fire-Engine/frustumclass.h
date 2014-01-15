
//////////////////////////////////////////////////////////
//�ļ���: frustumclass.h		����: ������:2014/1/15
//////////////////////////////////////////////////////////
#ifndef _FURSTUMCLASS_H_
#define _FURSTUMCLASS_H_

/////////
//INCLUDES //
/////////
#include <D3DX10math.h>

////////////////////////////////////
//����: FrustumClass
////////////////////////////////////
class FrustumClass
{
public:
	FrustumClass();
	FrustumClass(const FrustumClass&);
	~FrustumClass();

	void ConstructFrustum(float, D3DXMATRIX, D3DXMATRIX);

	bool CheckPoint(float, float, float);
	bool CheckCube(float, float, float, float);
	bool CheckSphere(float, float, float, float);
	bool CheckRectangle(float, float, float, float, float, float);

private:
	D3DXPLANE m_planes[6];

};

#endif