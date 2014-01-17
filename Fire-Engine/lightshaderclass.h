
//////////////////////////////////////////////////////////
//文件名: lightshaderclass.h		日期: 创建于:2014/1/11 //
//////////////////////////////////////////////////////////

#ifndef _LIGHTSHADERCLASS_H_
#define _LIGHTSHADERCLASS_H_

//////////////
// FE CLASS INCLUDES //
//////////////
#include "shaderClass.h"

////////////////////////////////////
//类名: LightShaderClass
////////////////////////////////////
class LightShaderClass : public ShaderClass
{
public:
	LightShaderClass();
	LightShaderClass(const LightShaderClass&);
	~LightShaderClass();

	bool Initialze(ID3D11Device*, HWND);
	void ShutDown();
	bool Render(ID3D11DeviceContext*, int, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, ID3D11ShaderResourceView**, 
		D3DXVECTOR3, D3DXVECTOR4, D3DXVECTOR4, D3DXVECTOR3, D3DXVECTOR4, float);

private:
	struct CameraBufferType
	{
		D3DXVECTOR3 cameraPosition;
		float padding;
	};

	struct LightBufferType
	{
		D3DXVECTOR4 ambientColor;
		D3DXVECTOR4 diffuseColor;
		D3DXVECTOR3 lightDirection;
		float specularPower;
		D3DXVECTOR4 specularColor;
	};

private:
	bool InitialzeShader(ID3D11Device*, HWND, WCHAR*, WCHAR*, char*, char*);
	void ShutdownShader();

	bool SetShaderParameters(ID3D11DeviceContext*, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, ID3D11ShaderResourceView**, 
		D3DXVECTOR3, D3DXVECTOR4, D3DXVECTOR4, D3DXVECTOR3, D3DXVECTOR4, float);

private:
	ID3D11Buffer* m_lightBuffer;
	ID3D11Buffer* m_cameraBuffer;
};

#endif