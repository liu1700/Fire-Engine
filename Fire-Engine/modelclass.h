
//////////////////////////////////////////////////////////
//文件名: modelclass.h		日期: 创建于:2014/1/10
//////////////////////////////////////////////////////////
#ifndef _MODELCLASS_H_
#define _MODELCLASS_H_

//////////////
//INCLUDES
//////////////
#include <D3D11.h>
#include <D3DX10math.h>

////////////////////////////////////
//类名: ModelClass
////////////////////////////////////
class ModelClass
{
public:
	ModelClass();
	ModelClass(const ModelClass&);
	~ModelClass();

	bool Initialze(ID3D11Device*);
	void ShutDown();
	void Render(ID3D11DeviceContext*);

	int GetIndexCount();

private:
	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR4 color;
	};

private:
	bool InitialzeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

private:
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount;
};

#endif