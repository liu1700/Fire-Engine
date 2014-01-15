
//////////////////////////////////////////////////////////
//文件名: systemclass.cpp		日期: 创建于:2014/1/8
//////////////////////////////////////////////////////////
#include "systemclass.h"

SystemClass::SystemClass()
{
	m_Input = NULL;
	m_Graphics = NULL;
	m_Sound = NULL;

	m_Fps = NULL;
	m_Cpu = NULL;
	m_Timer = NULL;

	m_Position = NULL;
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

	m_phWait = CreateWaitableTimer(NULL, false, NULL);
	m_liDueTime.QuadPart = -1i64;	// 1s 后开始计时
	SetWaitableTimer(m_phWait, &m_liDueTime, 20, NULL, NULL, 0);	// 20ms为一周期
	
	// 初始化窗口
	InitialzeWindows(screenHeight, screenWidth);

	// 初始化Input对象
	m_Input = new InputClass;
	if(!m_Input)
		return false;

	if(!m_Input->Initialize(m_hinstance, m_hwnd, screenWidth, screenHeight, m_mouseEvent, m_keyboardEvent))
	{
		MessageBox(m_hwnd, L"无法初始化输入对象", L"Error", MB_OK);
		return false;
	}

	// 初始化Graphics对象
	m_Graphics = new GraphicsClass;
	if (!(m_Graphics->Initialze(screenHeight, screenWidth, m_hwnd)))
		return false;

	// 初始化Sound对象
	m_Sound = new SoundClass;
	if(!m_Sound)
		return false;

	if(!m_Sound->Initialize(m_hwnd))
	{
		MessageBox(m_hwnd, L"无法初始化DirectSound",L"Error", MB_OK);
		return false;
	}

	// 初始化Fps对象
	m_Fps = new FpsClass;
	if(!m_Fps)
		return false;
	m_Fps->Initialize();

	// 初始化Cpu对象
	m_Cpu = new CpuClass;
	if(!m_Cpu)
		return false;
	m_Cpu->Initialize();

	// 初始化Timer对象
	m_Timer = new TimerClass;
	if(!m_Timer)
		return false;

	if (!m_Timer->Initialize())
	{
		MessageBox(m_hwnd, L"不能初始化Timer对象", L"Error", MB_OK);
		return false;
	}

	// 初始化Position对象
	m_Position = new PositionClass;
	if(!m_Position)
		return false;

	// 初始化handle数组	
	//m_ah[0] = m_mouseEvent;
	//m_ah[1] = m_keyboardEvent;
	//m_ah[2] = m_phWait;

	return true;
	
}

void SystemClass::Shutdown()
{
	// 释放位置对象
	if (m_Position)
	{
		delete m_Position;
		m_Position = NULL;
	}

	//释放Timer，CPU，FPS对象
	if(m_Timer)
	{
		delete m_Timer;
		m_Timer = NULL;
	}

	if(m_Cpu)
	{
		delete m_Cpu;
		m_Cpu = NULL;
	}

	if(m_Fps)
	{
		delete m_Fps;
		m_Fps = NULL;
	}

	// 释放Sound对象
	if (m_Sound)
	{
		m_Sound->ShutDown();
		delete m_Sound;
		m_Sound = NULL;
	}

	// 释放Graphic对象
	if (m_Graphics)
	{
		m_Graphics->ShutDown();
		delete m_Graphics;
		m_Graphics = NULL;
	}

	// 释放Input对象
	if (m_Input)
	{
		m_Input->ShutDown();
		delete m_Input;
		m_Input = NULL;
	}

	// 清除窗口
	ShutdownWindows();

	return;
}

void SystemClass::Run()
{
	bool bExit = false;

	while(!bExit)
	{
		MSG msg;
		m_dwRet = MsgWaitForMultipleObjects(1, &m_phWait, false, INFINITE, QS_ALLINPUT);

		switch(m_dwRet - WAIT_OBJECT_0)
		{
		case 0:
			{
				if(!m_Input->ReadMouse())
					bExit = true;
				if(!Frame())
					bExit = true;
				if(!m_Input->ReadKeyboard())
					bExit = true;

				if (m_Input->IsEscapePressed() == true)
				{
					bExit = true;
				}
			}
			break;
		//case 1:
		//	{
		//		if(!m_Input->ReadKeyboard())
		//			bExit = true;

		//		if (m_Input->IsEscapePressed() == true)
		//		{
		//			bExit = true;
		//		}
		//	}
		//	break;
		//case 2:
		//	{
		//		if(!Frame())
		//			bExit = true;
		//	}
		//	break;

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
	int mouseX, mouseY;
	bool keyDown;
	float rotationY;

	// 更新系统状态
	m_Timer->Frame();
	m_Cpu->Frame();
	m_Fps->Frame();

	// 输入循环
	m_Input->Frame();
	// 获取鼠标位置
	m_Input->GetMouseLocation(mouseX, mouseY);

	// 设定更新速度
	m_Position->SetFrameTimer(m_Timer->GetTime());

	// 检测左右键是否被按
	keyDown = m_Input->IsLeftArrowPressed();
	m_Position->TurnLeft(keyDown);

	keyDown = m_Input->IsRightArrowPressed();
	m_Position->TurnRight(keyDown);

	m_Position->GetRotation(rotationY);

	// 为图形对象进行帧处理
	if (!m_Graphics->Frame(mouseX, mouseY, m_Fps->GetFps(), m_Cpu->GetCpuPercentage(), m_Timer->GetTime(), rotationY))
		return false;

	return true;
}

LRESULT CALLBACK SystemClass::MessageHandler(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// 其他消息转为默认消息处理
	return DefWindowProc(hWnd, uMsg, wParam, lParam);

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