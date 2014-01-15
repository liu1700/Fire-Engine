
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

	if(!InitialzeSentence(device, 16.0f, 0xff0099ff, L"Arial"))
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

void TextClass::Render(ID3D11DeviceContext* deviceContext, const wchar_t* text,
					   int positionX, int positionY, int flags)
{
	m_pFontWrapper->DrawString(deviceContext, text, m_FontType->fontSize, 
		(float)positionX, (float)positionY, m_FontType->textColor, flags);

	return;
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

void TextClass::SetMousePosition(int mouseX, int mouseY, ID3D11DeviceContext* deviceContext)
{
	char tempString[16];
	char mouseString[16];

	// 将mouseX转换为字符串格式
	_itoa_s(mouseX, tempString, 10);
	strcpy_s(mouseString, "Mouse X:");
	strcat_s(mouseString, tempString);

	size_t stringSize1 = strlen(mouseString) + 1;
	const size_t newsize1 = 100;
	size_t convertedChars1 = 0;
	wchar_t wcstring1[newsize1];
	mbstowcs_s(&convertedChars1, wcstring1, stringSize1, mouseString, _TRUNCATE);

	// 渲染
	Render(deviceContext, (const wchar_t*)wcstring1, 200, 400);

	// 将mouseY转换为字符串格式
	_itoa_s(mouseY, tempString, 10);
	strcpy_s(mouseString, "Mouse Y:");
	strcat_s(mouseString, tempString);

	size_t stringSize2 = strlen(mouseString) + 1;
	const size_t newsize2 = 100;
	size_t convertedChars2 = 0;
	wchar_t wcstring2[newsize2];
	mbstowcs_s(&convertedChars2, wcstring2, stringSize2, mouseString, _TRUNCATE);

	// 渲染
	Render(deviceContext, (const wchar_t*)wcstring2, 200, 420);

}

bool TextClass::SetFps(int fps, ID3D11DeviceContext* deviceContext)
{
	char tempString[16];
	char fpsString[16];
	float red, green, blue;

	// 限定上限
	if(fps > 9999)
	{
		fps = 9999;
	}

	// 转变为字符串格式
	_itoa_s(fps, tempString, 10);

	// 设置字符
	strcpy_s(fpsString, "Fps: ");
	strcat_s(fpsString, tempString);

	// 高于60fps为绿色
	if(fps >= 60)
	{
		red = 0.0f;
		green = 1.0f;
		blue = 0.0f;
	}

	// 低于60fps为黄色
	if(fps < 60)
	{
		red = 1.0f;
		green = 1.0f;
		blue = 0.0f;
	}

	// 低于30为红色
	if(fps < 30)
	{
		red = 1.0f;
		green = 0.0f;
		blue = 0.0f;
	}

	size_t stringSize1 = strlen(fpsString) + 1;
	const size_t newsize1 = 100;
	size_t convertedChars1 = 0;
	wchar_t wcstring1[newsize1];
	mbstowcs_s(&convertedChars1, wcstring1, stringSize1, fpsString, _TRUNCATE);

	// 渲染
	Render(deviceContext, (const wchar_t*)wcstring1, 10, 20);

	return true;
}

bool TextClass::SetCpu(int cpu, ID3D11DeviceContext* deviceContext)
{
	char tempString[16];
	char cpuString[16];

	_itoa_s(cpu, tempString, 10);

	strcpy_s(cpuString, "Cpu: ");
	strcat_s(cpuString, tempString);
	strcat_s(cpuString, "%");

	size_t stringSize1 = strlen(cpuString) + 1;
	const size_t newsize1 = 100;
	size_t convertedChars1 = 0;
	wchar_t wcstring1[newsize1];
	mbstowcs_s(&convertedChars1, wcstring1, stringSize1, cpuString, _TRUNCATE);

	// 渲染
	Render(deviceContext, (const wchar_t*)wcstring1, 10, 40);

	return true;
}

bool TextClass::SetRenerCount(int rendercount, ID3D11DeviceContext* deviceContext)
{
	char tempString[32];
	char rcString[32];

	_itoa_s(rendercount, tempString, 10);

	strcpy_s(rcString, "Render Count: ");
	strcat_s(rcString, tempString);

	size_t stringSize1 = strlen(rcString) + 1;
	const size_t newsize1 = 100;
	size_t convertedChars1 = 0;
	wchar_t wcstring1[newsize1];
	mbstowcs_s(&convertedChars1, wcstring1, stringSize1, rcString, _TRUNCATE);

	// 渲染
	Render(deviceContext, (const wchar_t*)wcstring1, 10, 60);

	return true;
}