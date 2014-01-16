
//////////////////////////////////////////////////////////
//�ļ���: textureshaderclass.h		����: ������:2014/1/12
//////////////////////////////////////////////////////////
#ifndef _TEXTURESHADERCLASS_H_
#define _TEXTURESHADERCLASS_H_

//////////////
// FE INCLUDES //
//////////////
#include "shaderClass.h"

////////////////////////////////////////////////////////////////////////////////
// ����: TextureShaderClass
////////////////////////////////////////////////////////////////////////////////
class TextureShaderClass : public ShaderClass
{
public:
	TextureShaderClass();
	TextureShaderClass(const TextureShaderClass&);
	~TextureShaderClass();

	bool Initialize(ID3D11Device*, HWND);
	void ShutDown();
	bool Render(ID3D11DeviceContext*, int, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, ID3D11ShaderResourceView*);

};

#endif