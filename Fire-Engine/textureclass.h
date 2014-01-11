
//////////////////////////////////////////////////////////
//�ļ���: textureclass.h		����: ������:2014/1/11
//////////////////////////////////////////////////////////
#ifndef _TEXTURECLASS_H_
#define _TEXTURECLASS_H_

//////////////
//INCLUDES
//////////////
#include <D3D11.h>
#include <D3DX11tex.h>

////////////////////////////////////
//����: TextureClass
////////////////////////////////////
class TextureClass
{
public:
	TextureClass();
	TextureClass(const TextureClass&);
	~TextureClass();

	bool Initialze(ID3D11Device*, WCHAR*);
	void ShutDown();

	ID3D11ShaderResourceView* GetTexture();

private:
	ID3D11ShaderResourceView* m_texture;
};

#endif