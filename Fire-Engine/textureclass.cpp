
//////////////////////////////////////////////////////////
//�ļ���: textureclass.cpp		����: ������:2014/1/11
//////////////////////////////////////////////////////////
#include "textureclass.h"

TextureClass::TextureClass()
{
	m_texture = NULL;
}

TextureClass::TextureClass(const TextureClass& other)
{

}

TextureClass::~TextureClass()
{

}

bool TextureClass::Initialze(ID3D11Device* device, WCHAR* filename)
{
	if(FAILED(D3DX11CreateShaderResourceViewFromFile(device, filename, NULL, NULL, &m_texture, NULL)))
		return false;

	return true;
}

void TextureClass::ShutDown()
{
	if (m_texture)
	{
		m_texture->Release();
		m_texture = NULL;
	}

	return;
}

ID3D11ShaderResourceView* TextureClass::GetTexture()
{
	return m_texture;
}