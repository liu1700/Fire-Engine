
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

//////////////
//FE CLASS INCLUDES //
//////////////
#include "textureclass.h"
#include "fileIO.h"

////////////////////////////////////
//类名: ModelClass
////////////////////////////////////
class ModelClass
{
public:
	ModelClass();
	ModelClass(const ModelClass&);
	~ModelClass();

	bool Initialze(ID3D11Device*, WCHAR*, WCHAR*);
	void ShutDown();
	void Render(ID3D11DeviceContext*);

	int GetIndexCount();

	ID3D11ShaderResourceView* GetTexture();

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
	void RenderBuffers(ID3D11DeviceContext*);

	bool LoadTexture(ID3D11Device*, WCHAR*);
	void ReleaseTexture();

private:
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount;
	float** m_ModelInfo;

	TextureClass* m_Texture;
	FileIOClass* m_Model;
};

#endif