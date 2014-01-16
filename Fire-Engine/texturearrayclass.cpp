
//////////////////////////////////////////////////////////
//文件名: texturearrayclass.cpp		日期: 创建于:2014/1/11
//////////////////////////////////////////////////////////
#include "texturearrayclass.h"

TextureArrayClass::TextureArrayClass()
{
	m_textures[0] = NULL;
	m_textures[1] = NULL;
}

TextureArrayClass::TextureArrayClass(const TextureArrayClass& other)
{

}

TextureArrayClass::~TextureArrayClass()
{

}

bool TextureArrayClass::Initialze(ID3D11Device* device, WCHAR* filename1, WCHAR* filename2)
{
	if(FAILED(D3DX11CreateShaderResourceViewFromFile(device, filename1, NULL, NULL, &m_textures[0], NULL)
		||D3DX11CreateShaderResourceViewFromFile(device, filename2, NULL, NULL, &m_textures[1], NULL)))
		return false;

	return true;
}

void TextureArrayClass::ShutDown()
{
	if (m_textures[0])
	{
		m_textures[0]->Release();
		m_textures[0] = NULL;
	}

	if (m_textures[1])
	{
		m_textures[1]->Release();
		m_textures[1] = NULL;
	}

	return;
}

ID3D11ShaderResourceView** TextureArrayClass::GetTextureArray()
{
	return m_textures;
}