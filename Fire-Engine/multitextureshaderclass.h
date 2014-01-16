
//////////////////////////////////////////////////////////
//文件名: multitextureshaderclass.h		日期: 创建于:2014/1/16
//////////////////////////////////////////////////////////
#ifndef _MULTITEXTURESHADERCLASS_H_
#define _MULTITEXTURESHADERCLASS_H_

//////////////
// FE INCLUDES //
//////////////
#include "shaderClass.h"

////////////////////////////////////////////////////////////////////////////////
// 类名: MultiTextureShaderClass
////////////////////////////////////////////////////////////////////////////////
class MultiTextureShaderClass : public ShaderClass
{
public:
	MultiTextureShaderClass();
	MultiTextureShaderClass(const MultiTextureShaderClass&);
	~MultiTextureShaderClass();

	bool Initialize(ID3D11Device*, HWND);
	void ShutDown();
	bool Render(ID3D11DeviceContext*, int, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, ID3D11ShaderResourceView**);

};

#endif