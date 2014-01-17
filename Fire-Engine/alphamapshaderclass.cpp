
//////////////////////////////////////////////////////////
//文件名: alphamapshaderclass.cpp		日期: 创建于:2014/1/17
//////////////////////////////////////////////////////////
#include "alphamapshaderclass.h"

AlphaMapShaderClass::AlphaMapShaderClass()
{

}


AlphaMapShaderClass::AlphaMapShaderClass(const AlphaMapShaderClass& other)
{
}


AlphaMapShaderClass::~AlphaMapShaderClass()
{
}


bool AlphaMapShaderClass::Initialize(ID3D11Device* device, HWND hwnd)
{
	if(!InitializeShader(device, hwnd, L"../Fire-Engine/Alphamap.vs", L"../Fire-Engine/Alphamap.ps", "AlphaMapVertexShader", "AlphaMapPixelShader" ))
		return false;

	return true;
}

void AlphaMapShaderClass::ShutDown()
{
	ShutdownShader();

	return;
}

bool AlphaMapShaderClass::Render(ID3D11DeviceContext* deviceContext, int indexCount, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, 
								D3DXMATRIX projectionMatrix, ID3D11ShaderResourceView** textureArray)
{
	if(!SetShaderParameters(deviceContext, worldMatrix, viewMatrix, projectionMatrix, textureArray, 3))
		return false;

	RenderShader(deviceContext, indexCount);

	return true;
}

