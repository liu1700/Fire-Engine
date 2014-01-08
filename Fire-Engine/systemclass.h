
//////////////////////////////////////////////////////////
//�ļ���: systemclass.h		����: ������:2014/1/8
//////////////////////////////////////////////////////////
#ifndef _SYSTEMCLASS_H_
#define _SYSTEMCLASS_H_

///////////////////
//Ԥ����			
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
//����: SystemClass
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