
///////TEXTURERENDER_CLASS_H_/////////
#ifndef _TEXTURERENDERCLASS_H_
#define _TEXTURERENDERCLASS_H_

//////////////
//INCLUDES
//////////////
#include <D3D11.h>
#include <D3DX10math.h>

//////////////
//FE CLASS INCLUDES //
//////////////
#include "texturearrayclass.h"
#include "fileIO.h"

////////////////////////////////////
//����: TectureRenderClass
////////////////////////////////////
class TectureRenderClass
{
public:
	TectureRenderClass();
	TectureRenderClass(const TectureRenderClass&);
	virtual ~TectureRenderClass();

protected:
	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
	};

protected:
	bool InitialzeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

	bool LoadTexture(ID3D11Device*, WCHAR*);
	void ReleaseTexture();

protected:
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount;

	TextureArrayClass* m_TextureArray;
};


#endif