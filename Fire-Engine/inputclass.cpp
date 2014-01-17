
//////////////////////////////////////////////////////////
//�ļ���: inputclass.cpp		����: ������:2014/1/8
//////////////////////////////////////////////////////////
#include "inputclass.h"

InputClass::InputClass()
{
	m_directInput = NULL;
	m_keyboard = NULL;
	m_mouse = NULL;

	m_hMouseEvent = CreateEvent(NULL, false, false, NULL);
	m_hKeyboardEvent = CreateEvent(NULL, false, false, NULL);
}

InputClass::InputClass(const InputClass& other)
{

}

InputClass::~InputClass()
{

}

bool InputClass::Initialize(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, 
							HANDLE& hMouseEvent, HANDLE& hKeyboardEvent)
{
	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;

	// ��ʼ�����λ��
	m_mouseX = 0;
	m_mouseY = 0;

	// ��ʼ��������ӿ�
	if(FAILED(DirectInput8Create(hinstance, DIRECTINPUT_VERSION, IID_IDirectInput8, 
		(void**)&m_directInput, NULL)))
		return false;

	// ��ʼ����������ӿ�
	if(FAILED(m_directInput->CreateDevice(GUID_SysKeyboard, &m_keyboard, NULL)))
		return false;

	// �������ݸ�ʽ
	if(FAILED(m_keyboard->SetDataFormat(&c_dfDIKeyboard)))
		return false;

	// �趨�������������������������̵�����
	if(FAILED(m_keyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE)))
		return false;

	// ռ�ݼ���
	if(FAILED(m_keyboard->Acquire()))
		return false;

	// Ϊ������ýӿ�
	if(FAILED(m_directInput->CreateDevice(GUID_SysMouse, &m_mouse, NULL)))
		return false;
	if(FAILED(m_mouse->SetDataFormat(&c_dfDIMouse)))
		return false;
	if(FAILED(m_mouse->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE)))
		return false;

	if(FAILED(m_mouse->Acquire()))
		return false;

	// Ҫ���豸Acquire����֮ǰ��ȡhandle������Acquired���豸���ܱ���ȡhandle
	// ���handle�¼�
	m_keyboard->SetEventNotification(m_hKeyboardEvent);
	m_mouse->SetEventNotification(m_hMouseEvent);
	hMouseEvent = m_hMouseEvent;
	hKeyboardEvent = m_hKeyboardEvent;

	return true;
}

void InputClass::ShutDown()
{
	if (m_mouse)
	{
		m_mouse->Unacquire();
		m_mouse->Release();
		m_mouse = NULL;
	}
	if (m_keyboard)
	{
		m_keyboard->Unacquire();
		m_keyboard->Release();
		m_keyboard = NULL;
	}
	if (m_directInput)
	{
		m_directInput->Release();
		m_directInput = NULL;
	}

	return;
}

bool InputClass::Frame()
{
	//// ��ȡ���̵�״̬
	//if(!ReadKeyboard())
	//	return false;
	//
	//// ��ȡ����״̬
	//if(!ReadMouse())
	//	return false;

	ProcessInput();

	return true;

}

bool InputClass::ReadKeyboard()
{
	HRESULT result;

	// ��ȡ����״̬
	result = m_keyboard->GetDeviceState(sizeof(m_keyboardState), (LPVOID)&m_keyboardState);
	if(FAILED(result))
	{
		// ���ʧȥ�����������»�ȡ
		if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
		{
			m_keyboard->Acquire();
		} 
		else
		{
			return false;
		}
	}
	return true;

}

bool InputClass::ReadMouse()
{
	HRESULT result;

	// ��ȡ����״̬
	result = m_mouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&m_mouseState);
	if(FAILED(result))
	{
		// ���ʧȥ�����������»�ȡ
		if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
		{
			m_mouse->Acquire();
		} 
		else
		{
			return false;
		}
	}
	return true;

}

void InputClass::ProcessInput()
{
	// ����λ��
	m_mouseX += m_mouseState.lX;
	m_mouseY += m_mouseState.lY;

	// ��ֹ����Ƴ���Ļ
	if(m_mouseX < 0) {m_mouseX = 0;}
	if(m_mouseY < 0) {m_mouseY = 0;}

	if(m_mouseX > m_screenWidth) {m_mouseX = m_screenWidth;}
	if(m_mouseY > m_screenHeight) {m_mouseY = m_screenHeight;}

	return;
}

bool InputClass::IsEscapePressed()
{
	// ����Ƿ���esc��
	if(m_keyboardState[DIK_ESCAPE]&0x80)
	{
		return true;
	}

	return false;
}

bool InputClass::IsLeftArrowPressed()
{
	if (m_keyboardState[DIK_LEFTARROW]&0x80)
	{
		return true;
	}
	return false;
}

bool InputClass::IsRightArrowPressed()
{
	if (m_keyboardState[DIK_RIGHTARROW]&0x80)
	{
		return true;
	}
	return false;
}

bool InputClass::IsUpArrowPressed()
{
	if (m_keyboardState[DIK_UPARROW]&0x80)
	{
		return true;
	}
	return false;
}

bool InputClass::IsDownArrowPressed()
{
	if (m_keyboardState[DIK_DOWNARROW]&0x80)
	{
		return true;
	}
	return false;
}

bool InputClass::IsMoveForward()
{
	if (m_keyboardState[DIK_W]&0x80)
	{
		return true;
	}
	return false;
}

bool InputClass::IsMoveBack()
{
	if (m_keyboardState[DIK_S]&0x80)
	{
		return true;
	}
	return false;
}

bool InputClass::IsMoveLeft()
{
	if (m_keyboardState[DIK_A]&0x80)
	{
		return true;
	}
	return false;
}

bool InputClass::IsMoveRight()
{
	if (m_keyboardState[DIK_D]&0x80)
	{
		return true;
	}
	return false;
}

void InputClass::GetMouseLocation(int& mouseX, int& mouseY)
{
	mouseX = m_mouseX;
	mouseY = m_mouseY;

	return;
}


