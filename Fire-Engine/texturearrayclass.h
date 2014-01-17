
//////////////////////////////////////////////////////////
//文件名: texturearrayclass.h		日期: 创建于:2014/1/11
//////////////////////////////////////////////////////////
#ifndef _TEXTUREARRAYCLASS_H_
#define _TEXTUREARRAYCLASS_H_

//////////////
//INCLUDES
//////////////
#include <D3D11.h>
#include <D3DX11tex.h>

////////////////////////////////////
//类名: TextureArrayClass
////////////////////////////////////
class TextureArrayClass
{
public:
	TextureArrayClass();
	TextureArrayClass(const TextureArrayClass&);
	~TextureArrayClass();

	bool Initialze(ID3D11Device*, WCHAR*, WCHAR*, WCHAR*);
	bool Initialze(ID3D11Device*, WCHAR*, WCHAR*);
	bool Initialze(ID3D11Device*, WCHAR*);
	void ShutDown();

	ID3D11ShaderResourceView** GetTextureArray();

private:
	ID3D11ShaderResourceView* m_textures[3];
};

#endif