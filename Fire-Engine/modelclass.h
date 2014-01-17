
//////////////////////////////////////////////////////////
//文件名: modelclass.h		日期: 创建于:2014/1/10
//////////////////////////////////////////////////////////
#ifndef _MODELCLASS_H_
#define _MODELCLASS_H_

//////////////
//FE CLASS INCLUDES //
//////////////
#include "texturerenderClass.h"

////////////////////////////////////
//类名: ModelClass
////////////////////////////////////
class ModelClass : public TectureRenderClass
{
public:
	ModelClass();
	ModelClass(const ModelClass&);
	~ModelClass();

	bool Initialze(ID3D11Device*,WCHAR*,WCHAR*,WCHAR*);
	void ShutDown();
	void Render(ID3D11DeviceContext*);

private:
	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
		D3DXVECTOR3 normal;
	};

private:
	bool InitialzeBuffers(ID3D11Device*);
	void ShutdownBuffers();

	bool LoadTexture(ID3D11Device*, WCHAR*, WCHAR*);

private:
	float** m_ModelInfo;
	FileIOClass* m_inModel;
};

#endif