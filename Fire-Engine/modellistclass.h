
//////////////////////////////////////////////////////////
//文件名: modellistclass.h		日期: 创建于:2014/1/15
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
//类名: ModelListClass
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