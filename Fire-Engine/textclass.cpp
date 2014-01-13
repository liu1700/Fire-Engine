
//////////////////////////////////////////////////////////
//文件名: textclass.cpp		日期: 创建于:2014/1/13
//////////////////////////////////////////////////////////
#include "textclass.h"

TextClass::TextClass()
{
	m_pFW1Factory = NULL;
	m_pFontWrapper = NULL;
	m_FontType = NULL;
}

TextClass::TextClass(const TextClass& other)
{

}

TextClass::~TextClass()
{

}

bool TextClass::Initialze(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	if(FAILED(FW1CreateFactory(FW1_VERSION, &m_pFW1Factory)))
		return false;

	if(!InitialzeSentence(device, 64.0f, 0xff0099ff, L"Arial"))
		return false;		

	return true;
}

void TextClass::ShutDown()
{
	m_pFontWrapper->Release();
	m_pFW1Factory->Release();

	if (m_FontType)
	{
		delete m_FontType;
		m_FontType = NULL;
	}

	return;
}

bool TextClass::Render(ID3D11DeviceContext* deviceContext, const wchar_t* text,
					   int positionX, int positionY, int flags)
{
	if(!UpdateSentence(deviceContext, text, positionX, positionY, flags))
		return false;

	return true;
}

bool TextClass::InitialzeSentence(ID3D11Device* device, float fontsize, UINT32 textcolor, const wchar_t* texttype)
{
	m_FontType = new FontType;
	if(!m_FontType)
		return false;

	m_FontType->fontSize = fontsize;
	m_FontType->textColor = textcolor;

	if(FAILED(m_pFW1Factory->CreateFontWrapper(device, texttype, &m_pFontWrapper)))
		return false;

	return true;
}

bool TextClass::UpdateSentence(ID3D11DeviceContext* deviceContext, const wchar_t* text,
							   int positionX, int positionY, int flags)
{
	m_pFontWrapper->DrawString(deviceContext, text, m_FontType->fontSize, 
		(float)positionX, (float)positionY, m_FontType->textColor, flags);

	return true;
}