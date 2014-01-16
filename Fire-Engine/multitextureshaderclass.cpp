
//////////////////////////////////////////////////////////
//文件名: multitextureshaderclass.cpp		日期: 创建于:2014/1/16
//////////////////////////////////////////////////////////
#include "multitextureshaderclass.h"

MultiTextureShaderClass::MultiTextureShaderClass()
{

}


MultiTextureShaderClass::MultiTextureShaderClass(const MultiTextureShaderClass& other)
{
}


MultiTextureShaderClass::~MultiTextureShaderClass()
{
}


bool MultiTextureShaderClass::Initialize(ID3D11Device* device, HWND hwnd)
{
	if(!InitializeShader(device, hwnd, L"../Fire-Engine/Multitexture.vs", L"../Fire-Engine/Multitexture.ps", "MultiTextureVertexShader", "MultiTexturePixelShader" ))
		return false;

	return true;
}

void MultiTextureShaderClass::ShutDown()
{
	ShutdownShader();

	return;
}

bool MultiTextureShaderClass::Render(ID3D11DeviceContext* deviceContext, int indexCount, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, 
								D3DXMATRIX projectionMatrix, ID3D11ShaderResourceView** textureArray)
{
	if(!SetShaderParameters(deviceContext, worldMatrix, viewMatrix, projectionMatrix, textureArray, 2))
		return false;

	RenderShader(deviceContext, indexCount);

	return true;
}

