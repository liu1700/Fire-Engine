
//////////////////////////////////////////////////////////
//�ļ���: inputclass.cpp		����: ������:2014/1/8
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

	// ��ʼ�����м�Ϊ�ͷ�״̬
	for (i = 0; i < 256; i++)
	{
		m_keys[i] = false;
	}

	return;
}

void InputClass::keyDown(unsigned int input)
{
	// ����м�������Ѱ���״̬����key������
	m_keys[input] = true;

	return;
}

void InputClass::keyUp(unsigned int input)
{
	// �������̧�������key�����Ӧ�İ���״̬
	m_keys[input] = false;
}

bool InputClass::isKeyDown(unsigned int key)
{
	// ��ⰴ��״̬
	return m_keys[key];
}