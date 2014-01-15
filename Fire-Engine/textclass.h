
//////////////////////////////////////////////////////////
//文件名: textclass.h		日期: 创建于:2014/1/13
//////////////////////////////////////////////////////////
#ifndef _TEXTCLASS_H_
#define _TEXTCLASS_H_

////////////
//LINKING
////////////
#pragma comment(lib, "FW1FontWrapper.lib")

//////////////
//INCLUDES
//////////////
#include <FW1FontWrapper.h>


////////////////////////////////////
//类名: TextClass
////////////////////////////////////
class TextClass
{
public:
	TextClass();
	TextClass(const TextClass&);
	~TextClass();

	bool Initialze(ID3D11Device*, ID3D11DeviceContext*);
	void ShutDown();
	void Render(ID3D11DeviceContext* deviceContext, const wchar_t* text, int positionX, int positionY, int flags = FW1_NOGEOMETRYSHADER);

	void SetMousePosition(int, int, ID3D11DeviceContext*);
	bool SetFps(int, ID3D11DeviceContext*);
	bool SetCpu(int, ID3D11DeviceContext*);
	bool SetRenerCount(int, ID3D11DeviceContext*);

private:
	struct FontType
	{
		float fontSize;
		UINT32 textColor;
	};

private:
	bool InitialzeSentence(ID3D11Device* device, float fontsize, UINT32 textcolor, const wchar_t* texttype);

private:
	IFW1Factory* m_pFW1Factory;
	IFW1FontWrapper* m_pFontWrapper;

	FontType* m_FontType;
	
};

#endif