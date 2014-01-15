
//////////////////////////////////////////////////////////
//�ļ���: modellistclass.h		����: ������:2014/1/15
//////////////////////////////////////////////////////////
#ifndef _MODELLISTCLASS_H_
#define _MODELLISTCLASS_H_

/////////
//INCLUDES //
/////////
#include <D3DX10math.h>
#include <stdlib.h>
#include <time.h>

////////////////////////////////////
//����: ModelListClass
////////////////////////////////////
class ModelListClass
{
public:
	ModelListClass();
	ModelListClass(const ModelListClass&);
	~ModelListClass();

	bool Initialze(int);
	void ShutDown();

	int GetModelCount();
	void GetData(int, float&, float&, float&, D3DXVECTOR4&);

private:
	struct ModelInfoType
	{
		D3DXVECTOR4 color;
		float positionX, positionY, positionZ; 
	};

private:
	int m_modelCount;
	ModelInfoType* m_ModelInfoList;
};

#endif