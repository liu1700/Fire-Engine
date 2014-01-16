
//////////////////////////////////////////////////////////
//文件名: bitmapclass.h		日期: 创建于:2014/1/12
//////////////////////////////////////////////////////////
#ifndef _BITMAPCLASS_H_
#define _BITMAPCLASS_H_

//////////////
//FE CLASS INCLUDES //
//////////////
#include "texturerenderClass.h"

////////////////////////////////////
//类名: BitmapClass
////////////////////////////////////
class BitmapClass
{
public:
	BitmapClass();
	BitmapClass(const BitmapClass&);
	~BitmapClass();

	bool Initialze(ID3D11Device*, int, int, WCHAR*, int, int);
	void ShutDown();
	bool Render(ID3D11DeviceContext*, int, int);

	int GetIndexCount();

	ID3D11ShaderResourceView** GetTexture();

private:
	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
	};

private:
	bool InitialzeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	bool UpdateBuffer(ID3D11DeviceContext*, int, int);
	void RenderBuffers(ID3D11DeviceContext*);

	bool LoadTexture(ID3D11Device*, WCHAR*);
	void ReleaseTexture();

private:
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount;

	TextureArrayClass* m_TextureArray;
	
	int m_screenWidth, m_screenHeight;
	int m_bitmapWidth, m_bitmapHeight;
	int m_previousPosX, m_previousPosY;
};

#endif