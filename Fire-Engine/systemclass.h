
//////////////////////////////////////////////////////////
//文件名: systemclass.h		日期: 创建于:2014/1/8
//////////////////////////////////////////////////////////
#ifndef _SYSTEMCLASS_H_
#define _SYSTEMCLASS_H_

///////////////////
//预处理			
///////////////////
#define WIN32_LEAN_AND_MEAN

//////////////
//INCLUDES
//////////////
#include <Windows.h>

///////////////////
//CLASS INCLUDES
///////////////////
#include "inputclass.h"
#include "graphicsclass.h"

////////////////////////////////////
//类名: SystemClass
////////////////////////////////////
class SystemClass
{
public:
	SystemClass();
	SystemClass(const SystemClass&);
	~SystemClass();

	bool Initialze();
	void Shutdown();
	void Run();

	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

private:
	bool Frame();
	void InitialzeWindows(int&, int&);
	void ShutdownWindows();

private:
	LPCWSTR m_applicationName;
	HINSTANCE m_hinstance;
	HWND m_hwnd;

	InputClass* m_Input;
	GraphicsClass* m_Graphics;

};

//////////////
//FUNCTION PROTOTYPES //
//////////////
static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

//////////////
//GLOBALS
//////////////
static SystemClass* ApplicationHandle = NULL;

#endif