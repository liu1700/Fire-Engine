
//////////////////////////////////////////////////////////
//文件名: inputclass.cpp		日期: 创建于:2014/1/8
//////////////////////////////////////////////////////////
#include "inputclass.h"

InputClass::InputClass()
{
	m_directInput = NULL;
	m_keyboard = NULL;
	m_mouse = NULL;
}

InputClass::InputClass(const InputClass& other)
{

}

InputClass::~InputClass()
{

}

bool InputClass::Initialize(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight)
{
	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;

	// 初始化鼠标位置
	m_mouseX = 0;
	m_mouseY = 0;

	// 初始化主输入接口
	if(FAILED(DirectInput8Create(hinstance, DIRECT3D_VERSION, IID_IDirectInput8, 
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
	if(FAILED(m_mouse->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE)))
		return false;
	if(FAILED(m_mouse->Acquire()))
		return false;

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
	// 获取键盘的状态
	if(!ReadKeyboard())
		return false;
	
	// 获取鼠标的状态
	if(!ReadMouse())
		return false;

	ProcessInput();

	return true;

}