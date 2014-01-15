
//////////////////////////////////////////////////////////
//�ļ���: systemclass.cpp		����: ������:2014/1/8
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
	m_liDueTime.QuadPart = -1i64;	// 1s ��ʼ��ʱ
	SetWaitableTimer(m_phWait, &m_liDueTime, 20, NULL, NULL, 0);	// 20msΪһ����
	
	// ��ʼ������
	InitialzeWindows(screenHeight, screenWidth);

	// ��ʼ��Input����
	m_Input = new InputClass;
	if(!m_Input)
		return false;

	if(!m_Input->Initialize(m_hinstance, m_hwnd, screenWidth, screenHeight, m_mouseEvent, m_keyboardEvent))
	{
		MessageBox(m_hwnd, L"�޷���ʼ���������", L"Error", MB_OK);
		return false;
	}

	// ��ʼ��Graphics����
	m_Graphics = new GraphicsClass;
	if (!(m_Graphics->Initialze(screenHeight, screenWidth, m_hwnd)))
		return false;

	// ��ʼ��Sound����
	m_Sound = new SoundClass;
	if(!m_Sound)
		return false;

	if(!m_Sound->Initialize(m_hwnd))
	{
		MessageBox(m_hwnd, L"�޷���ʼ��DirectSound",L"Error", MB_OK);
		return false;
	}

	// ��ʼ��Fps����
	m_Fps = new FpsClass;
	if(!m_Fps)
		return false;
	m_Fps->Initialize();

	// ��ʼ��Cpu����
	m_Cpu = new CpuClass;
	if(!m_Cpu)
		return false;
	m_Cpu->Initialize();

	// ��ʼ��Timer����
	m_Timer = new TimerClass;
	if(!m_Timer)
		return false;

	if (!m_Timer->Initialize())
	{
		MessageBox(m_hwnd, L"���ܳ�ʼ��Timer����", L"Error", MB_OK);
		return false;
	}

	// ��ʼ��Position����
	m_Position = new PositionClass;
	if(!m_Position)
		return false;

	// ��ʼ��handle����	
	//m_ah[0] = m_mouseEvent;
	//m_ah[1] = m_keyboardEvent;
	//m_ah[2] = m_phWait;

	return true;
	
}

void SystemClass::Shutdown()
{
	// �ͷ�λ�ö���
	if (m_Position)
	{
		delete m_Position;
		m_Position = NULL;
	}

	//�ͷ�Timer��CPU��FPS����
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

	// �ͷ�Sound����
	if (m_Sound)
	{
		m_Sound->ShutDown();
		delete m_Sound;
		m_Sound = NULL;
	}

	// �ͷ�Graphic����
	if (m_Graphics)
	{
		m_Graphics->ShutDown();
		delete m_Graphics;
		m_Graphics = NULL;
	}

	// �ͷ�Input����
	if (m_Input)
	{
		m_Input->ShutDown();
		delete m_Input;
		m_Input = NULL;
	}

	// �������
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

	// ����ϵͳ״̬
	m_Timer->Frame();
	m_Cpu->Frame();
	m_Fps->Frame();

	// ����ѭ��
	m_Input->Frame();
	// ��ȡ���λ��
	m_Input->GetMouseLocation(mouseX, mouseY);

	// �趨�����ٶ�
	m_Position->SetFrameTimer(m_Timer->GetTime());

	// ������Ҽ��Ƿ񱻰�
	keyDown = m_Input->IsLeftArrowPressed();
	m_Position->TurnLeft(keyDown);

	keyDown = m_Input->IsRightArrowPressed();
	m_Position->TurnRight(keyDown);

	m_Position->GetRotation(rotationY);

	// Ϊͼ�ζ������֡����
	if (!m_Graphics->Frame(mouseX, mouseY, m_Fps->GetFps(), m_Cpu->GetCpuPercentage(), m_Timer->GetTime(), rotationY))
		return false;

	return true;
}

LRESULT CALLBACK SystemClass::MessageHandler(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// ������ϢתΪĬ����Ϣ����
	return DefWindowProc(hWnd, uMsg, wParam, lParam);

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