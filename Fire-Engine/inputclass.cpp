
//////////////////////////////////////////////////////////
//文件名: inputclass.cpp		日期: 创建于:2014/1/8
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

	// 初始化鼠标位置
	m_mouseX = 0;
	m_mouseY = 0;

	// 初始化主输入接口
	if(FAILED(DirectInput8Create(hinstance, DIRECTINPUT_VERSION, IID_IDirectInput8, 
		(void**)&m_directInput, NULL)))
		return false;

	// 初始化键盘输入接口
	if(FAILED(m_directInput->CreateDevice(GUID_SysKeyboard, &m_keyboard, NULL)))
		return false;

	// 设置数据格式
	if(FAILED(m_keyboard->SetDataFormat(&c_dfDIKeyboard)))
		return false;

	// 设定合作级别放置于其他程序共享键盘的现象
	if(FAILED(m_keyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE)))
		return false;

	// 占据键盘
	if(FAILED(m_keyboard->Acquire()))
		return false;

	// 为鼠标设置接口
	if(FAILED(m_directInput->CreateDevice(GUID_SysMouse, &m_mouse, NULL)))
		return false;
	if(FAILED(m_mouse->SetDataFormat(&c_dfDIMouse)))
		return false;
	if(FAILED(m_mouse->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE)))
		return false;

	if(FAILED(m_mouse->Acquire()))
		return false;

	// 要在设备Acquire（）之前获取handle，否则被Acquired的设备不能被获取handle
	// 输出handle事件
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
	//// 获取键盘的状态
	//if(!ReadKeyboard())
	//	return false;
	//
	//// 获取鼠标的状态
	//if(!ReadMouse())
	//	return false;

	ProcessInput();

	return true;

}

bool InputClass::ReadKeyboard()
{
	HRESULT result;

	// 获取键盘状态
	result = m_keyboard->GetDeviceState(sizeof(m_keyboardState), (LPVOID)&m_keyboardState);
	if(FAILED(result))
	{
		// 如果失去焦点则尝试重新获取
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

	// 获取键盘状态
	result = m_mouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&m_mouseState);
	if(FAILED(result))
	{
		// 如果失去焦点则尝试重新获取
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
	// 更新位置
	m_mouseX += m_mouseState.lX;
	m_mouseY += m_mouseState.lY;

	// 防止鼠标移出屏幕
	if(m_mouseX < 0) {m_mouseX = 0;}
	if(m_mouseY < 0) {m_mouseY = 0;}

	if(m_mouseX > m_screenWidth) {m_mouseX = m_screenWidth;}
	if(m_mouseY > m_screenHeight) {m_mouseY = m_screenHeight;}

	return;
}

bool InputClass::IsEscapePressed()
{
	// 检测是否按下esc键
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


