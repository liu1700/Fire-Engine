
//////////////////////////////////////////////////////////
//文件名: inputclass.h		日期: 创建于:2014/1/8
//////////////////////////////////////////////////////////
#ifndef _INPUTCLASS_H_
#define _INPUTCLASS_H_

////////////////////////
// 预处理
////////////////////////
#define DIRECTINPUT_VERSION 0x0800

/////////
//LINKING //
/////////
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

/////////
//INCLUDES //
/////////
#include <dinput.h>

////////////////////////////////////
//类名: InputClass
////////////////////////////////////
class InputClass
{
public:
	InputClass();
	InputClass(const InputClass&);
	~InputClass();

	bool Initialize(HINSTANCE, HWND, int, int, HANDLE&, HANDLE&);
	void ShutDown();
	bool Frame();

	bool IsEscapePressed();
	void GetMouseLocation(int&, int&);


	bool ReadKeyboard();
	bool ReadMouse();
	void ProcessInput();

private:
	IDirectInput8* m_directInput;
	IDirectInputDevice8* m_keyboard;
	IDirectInputDevice8* m_mouse;

	unsigned char m_keyboardState[256];
	DIMOUSESTATE m_mouseState;

	HANDLE m_hMouseEvent, m_hKeyboardEvent;

	int m_screenWidth, m_screenHeight;
	int m_mouseX, m_mouseY;

};

#endif