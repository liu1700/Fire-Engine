
//////////////////////////////////////////////////////////
//文件名: textureshaderclass.cpp		日期: 创建于:2014/1/12
//////////////////////////////////////////////////////////
#include "textureshaderclass.h"

TextureShaderClass::TextureShaderClass()
{

}


TextureShaderClass::TextureShaderClass(const TextureShaderClass& other)
{

}

TextureShaderClass::~TextureShaderClass()
{

}

bool TextureShaderClass::Initialize(ID3D11Device* device, HWND hwnd)
{
	if(!InitializeShader(device, hwnd, L"../Fire-Engine/Textures.vs", L"../Fire-Engine/Textures.ps", "TextureVertexShader", "TexturePixelShader"))
		return false;

	return true;
}

void TextureShaderClass::ShutDown()
{
	ShutdownShader();

	return;
}

bool TextureShaderClass::Render(ID3D11DeviceContext* deviceContext, int indexCount, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, 
								D3DXMATRIX projectionMatrix, ID3D11ShaderResourceView* texture)
{
	if(!SetShaderParameters(deviceContext, worldMatrix, viewMatrix, projectionMatrix, &texture, 1))
		return false;

	RenderShader(deviceContext, indexCount);

	return true;
}