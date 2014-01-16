
//////////////////////////////////////////////////////////
//文件名: textureshaderclass.h		日期: 创建于:2014/1/12
//////////////////////////////////////////////////////////
#ifndef _TEXTURESHADERCLASS_H_
#define _TEXTURESHADERCLASS_H_

//////////////
// FE INCLUDES //
//////////////
#include "shaderClass.h"

////////////////////////////////////////////////////////////////////////////////
// 类名: TextureShaderClass
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