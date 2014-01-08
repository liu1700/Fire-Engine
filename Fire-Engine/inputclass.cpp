
//////////////////////////////////////////////////////////
//文件名: inputclass.cpp		日期: 创建于:2014/1/8
//////////////////////////////////////////////////////////
#include "inputclass.h"

InputClass::InputClass()
{

}

InputClass::InputClass(const InputClass& other)
{

}

InputClass::~InputClass()
{

}

void InputClass::Initialize()
{
	int i;

	// 初始化所有键为释放状态
	for (i = 0; i < 256; i++)
	{
		m_keys[i] = false;
	}

	return;
}

void InputClass::keyDown(unsigned int input)
{
	// 如果有键按下则把按下状态存在key数组里
	m_keys[input] = true;

	return;
}

void InputClass::keyUp(unsigned int input)
{
	// 如果按键抬起则清除key数组对应的按下状态
	m_keys[input] = false;
}

bool InputClass::isKeyDown(unsigned int key)
{
	// 检测按键状态
	return m_keys[key];
}