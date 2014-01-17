
//////////////////////////////////////////////////////////
//�ļ���: alphamapshaderclass.h		����: ������:2014/1/17
//////////////////////////////////////////////////////////
#ifndef _ALPHAMAPSHADERCLASS_H_
#define _ALPHAMAPSHADERCLASS_H_

//////////////
// FE CLASS INCLUDES //
//////////////
#include "shaderClass.h"

////////////////////////////////////////////////////////////////////////////////
// ����: AlphaMapShaderClass
////////////////////////////////////////////////////////////////////////////////
class AlphaMapShaderClass : public ShaderClass
{
public:
	AlphaMapShaderClass();
	AlphaMapShaderClass(const AlphaMapShaderClass&);
	~AlphaMapShaderClass();

	bool Initialize(ID3D11Device*, HWND);
	void ShutDown();
	bool Render(ID3D11DeviceContext*, int, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, ID3D11ShaderResourceView**);

};

#endif