
//////////////////////////////////////////////////////////
//文件名: systemclass.cpp		日期: 创建于:2014/1/8
//////////////////////////////////////////////////////////
#include "systemclass.h"

SystemClass::SystemClass()
{
	m_Input = NULL;
	m_Graphics = NULL;
}

SystemClass::SystemClass(const SystemClass&)
{

}

SystemClass::~SystemClass()
{

}

bool SystemClass::Initialze()
{
	int screenWidth = 0, 
		screenHeight = 0;
	
	// 初始化窗口
	InitialzeWindows(screenHeight, screenWidth);

	// 初始化Input对象
	m_Input = new InputClass;
	if(!m_Input)
		return false;

	m_Input->Initialize();

	// 初始化Graphics对象
	m_Graphics = new GraphicsClass;
	if (!(m_Graphics->Initialze(screenHeight, screenWidth, m_hwnd)))
		return false;

	return true;
	
}

void SystemClass::Shutdown()
{
	// 释放Graphic对象
	if (m_Graphics)
	{
		m_Graphics->Shutdown();
		delete m_Graphics;
		m_Graphics = NULL;
	}

	// 释放Input对象
	if (m_Input)
	{
		delete m_Input;
		m_Input = NULL;
	}

	// 清除窗口
	ShutdownWindows();

	return;
}

void SystemClass::Run()
{
	HANDLE phWait = CreateWaitableTimer(NULL, false, NULL);
	LARGE_INTEGER liDueTime;
	liDueTime.QuadPart = -1i64;		// 1s 后开始计时
	
	SetWaitableTimer(phWait, &liDueTime, 20, NULL, NULL, 0);	// 20ms为一周期
	DWORD dwRet = 0;
	bool bExit = false;

	while(!bExit)
	{
		MSG msg;
		dwRet = MsgWaitForMultipleObjects(1, &phWait, false, INFINITE, QS_ALLINPUT);

		switch(dwRet - WAIT_OBJECT_0)
		{
		case 0:
			{
				if(!Frame())
					bExit = true;
			}
			break;

		case 1:
			{
				while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
				{
					if (msg.message == WM_QUIT)
					{
						bExit = true;
					}
					else
					{
						TranslateMessage(&msg);
						DispatchMessage(&msg);
					}
				}
			}
			break;
		default:
			break;
		}	
	}
}

bool SystemClass::Frame()
{

	// 相应键盘按键
	if (m_Input->isKeyDown(VK_ESCAPE))
		return false;

	if (!m_Graphics->Frame())
		return false;

	return true;
}

LRESULT CALLBACK SystemClass::MessageHandler(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_KEYDOWN:
		{
			// 记录按键行为
			m_Input->keyDown((unsigned int)wParam);
			return 0;
		}
	case WM_KEYUP:
		{
			// 按键释放
			m_Input->keyUp((unsigned int)wParam);
			return 0;
		}
	default:
		{
			// 其他消息转为默认消息处理
			return DefWindowProc(hWnd, uMsg, wParam, lParam);
		}
	}
}

void SystemClass::InitialzeWindows(int& screenWidth, int& screenHeight)
{
	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;
	int posX, posY;

	// 将指针指向到ApplicationHandle
	ApplicationHandle = this;

	// 获得该应用的实例
	m_hinstance = GetModuleHandle(NULL);

	// 设定应用名称
	m_applicationName = L"Fire Engine";

	//  设定窗口类详情
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc		= WndProc;
	wc.cbClsExtra		= 0;
	wc.cbWndExtra		= 0;
	wc.hInstance		= m_hinstance;
	wc.hIcon			= LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm			= wc.hIcon;
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground	= (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName		= NULL;
	wc.lpszClassName	= m_applicationName;
	wc.cbSize			= sizeof(WNDCLASSEX);

	// 注册窗口类
	RegisterClassEx(&wc);

	// 根据桌面屏幕决定长宽
	screenWidth = GetSystemMetrics(SM_CXSCREEN);
	screenHeight = GetSystemMetrics(SM_CYSCREEN);

	if (FULL_SCREEN)
	{
		// 如果选择全屏则将屏幕设定到最大分辨率同时设定为32位
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize			= sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth	= (unsigned long)screenWidth;
		dmScreenSettings.dmPelsHeight	= (unsigned long)screenHeight;
		dmScreenSettings.dmBitsPerPel	= 32;
		dmScreenSettings.dmFields		= DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// 改为全屏显示
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		// 设定窗口左上角位置
		posX = posY = 0;
	}
	else
	{
		// 如果是窗口显示默认设定为 800x600
		screenWidth		= 800;
		screenHeight	= 600;

		// 将窗口设定在屏幕中间
		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;

	}

	// 创建窗口
	m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_applicationName, m_applicationName,
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
		posX, posY, screenWidth, screenHeight, NULL, NULL, m_hinstance, NULL);

	// 显示窗口并获取焦点
	ShowWindow(m_hwnd, SW_SHOW);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);

	// 隐藏指针光标
	ShowCursor(false);

	return;
}

void SystemClass::ShutdownWindows()
{
	ShowCursor(true);

	// 还原显示设置
	if (FULL_SCREEN)
	{
		ChangeDisplaySettings(NULL, 0);
	}

	// 删除窗口
	DestroyWindow(m_hwnd);
	m_hwnd = NULL;

	// 删除句柄
	UnregisterClass(m_applicationName, m_hinstance);
	m_hinstance = NULL;

	// 释放指针
	ApplicationHandle = NULL;

	return;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}
	case WM_CLOSE:
		{
			PostQuitMessage(0);
			return 0;
		}
	default:
		{
			return ApplicationHandle->MessageHandler(hWnd, uMsg, wParam, lParam);		// 除关闭窗口的消息之外其他消息交由systemclass中的回调函数处理，保持代码整洁
		}
	}
}