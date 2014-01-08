
//////////////////////////////////////////////////////////
//�ļ���: systemclass.cpp		����: ������:2014/1/8
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
	
	// ��ʼ������
	InitialzeWindows(screenHeight, screenWidth);

	// ��ʼ��Input����
	m_Input = new InputClass;
	if(!m_Input)
		return false;

	m_Input->Initialize();

	// ��ʼ��Graphics����
	m_Graphics = new GraphicsClass;
	if (!(m_Graphics->Initialze(screenHeight, screenWidth, m_hwnd)))
		return false;

	return true;
	
}

void SystemClass::Shutdown()
{
	// �ͷ�Graphic����
	if (m_Graphics)
	{
		m_Graphics->Shutdown();
		delete m_Graphics;
		m_Graphics = NULL;
	}

	// �ͷ�Input����
	if (m_Input)
	{
		delete m_Input;
		m_Input = NULL;
	}

	// �������
	ShutdownWindows();

	return;
}

void SystemClass::Run()
{
	HANDLE phWait = CreateWaitableTimer(NULL, false, NULL);
	LARGE_INTEGER liDueTime;
	liDueTime.QuadPart = -1i64;		// 1s ��ʼ��ʱ
	
	SetWaitableTimer(phWait, &liDueTime, 20, NULL, NULL, 0);	// 20msΪһ����
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

	// ��Ӧ���̰���
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
			// ��¼������Ϊ
			m_Input->keyDown((unsigned int)wParam);
			return 0;
		}
	case WM_KEYUP:
		{
			// �����ͷ�
			m_Input->keyUp((unsigned int)wParam);
			return 0;
		}
	default:
		{
			// ������ϢתΪĬ����Ϣ����
			return DefWindowProc(hWnd, uMsg, wParam, lParam);
		}
	}
}

void SystemClass::InitialzeWindows(int& screenWidth, int& screenHeight)
{
	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;
	int posX, posY;

	// ��ָ��ָ��ApplicationHandle
	ApplicationHandle = this;

	// ��ø�Ӧ�õ�ʵ��
	m_hinstance = GetModuleHandle(NULL);

	// �趨Ӧ������
	m_applicationName = L"Fire Engine";

	//  �趨����������
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

	// ע�ᴰ����
	RegisterClassEx(&wc);

	// ����������Ļ��������
	screenWidth = GetSystemMetrics(SM_CXSCREEN);
	screenHeight = GetSystemMetrics(SM_CYSCREEN);

	if (FULL_SCREEN)
	{
		// ���ѡ��ȫ������Ļ�趨�����ֱ���ͬʱ�趨Ϊ32λ
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize			= sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth	= (unsigned long)screenWidth;
		dmScreenSettings.dmPelsHeight	= (unsigned long)screenHeight;
		dmScreenSettings.dmBitsPerPel	= 32;
		dmScreenSettings.dmFields		= DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// ��Ϊȫ����ʾ
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		// �趨�������Ͻ�λ��
		posX = posY = 0;
	}
	else
	{
		// ����Ǵ�����ʾĬ���趨Ϊ 800x600
		screenWidth		= 800;
		screenHeight	= 600;

		// �������趨����Ļ�м�
		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;

	}

	// ��������
	m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_applicationName, m_applicationName,
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
		posX, posY, screenWidth, screenHeight, NULL, NULL, m_hinstance, NULL);

	// ��ʾ���ڲ���ȡ����
	ShowWindow(m_hwnd, SW_SHOW);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);

	// ����ָ����
	ShowCursor(false);

	return;
}

void SystemClass::ShutdownWindows()
{
	ShowCursor(true);

	// ��ԭ��ʾ����
	if (FULL_SCREEN)
	{
		ChangeDisplaySettings(NULL, 0);
	}

	// ɾ������
	DestroyWindow(m_hwnd);
	m_hwnd = NULL;

	// ɾ�����
	UnregisterClass(m_applicationName, m_hinstance);
	m_hinstance = NULL;

	// �ͷ�ָ��
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
			return ApplicationHandle->MessageHandler(hWnd, uMsg, wParam, lParam);		// ���رմ��ڵ���Ϣ֮��������Ϣ����systemclass�еĻص������������ִ�������
		}
	}
}