
//////////////////////////////////////////////////////////
//�ļ���: inputclass.cpp		����: ������:2014/1/8
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

	// ��ʼ�����λ��
	m_mouseX = 0;
	m_mouseY = 0;

	// ��ʼ��������ӿ�
	if(FAILED(DirectInput8Create(hinstance, DIRECT3D_VERSION, IID_IDirectInput8, 
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
	// ��ȡ���̵�״̬
	if(!ReadKeyboard())
		return false;
	
	// ��ȡ����״̬
	if(!ReadMouse())
		return false;

	ProcessInput();

	return true;

}