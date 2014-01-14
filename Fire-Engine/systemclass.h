
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
//FE CLASS INCLUDES
///////////////////
#include "inputclass.h"
#include "graphicsclass.h"
#include "soundclass.h"

#include "fpsclass.h"
#include "cpuclass.h"
#include "timerclass.h"

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
	LARGE_INTEGER m_liDueTime;
	DWORD m_dwRet;

	HANDLE m_keyboardEvent;
	HANDLE m_mouseEvent;
	HANDLE m_phWait;

	//HANDLE m_ah[3];

	InputClass* m_Input;
	GraphicsClass* m_Graphics;
	SoundClass* m_Sound;

	FpsClass* m_Fps;
	CpuClass* m_Cpu;
	TimerClass* m_Timer;

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