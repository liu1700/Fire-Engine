
//////////////////////////////////////////////////////////
//�ļ���: FireMain.cpp		����: ������:2014/1/8
//////////////////////////////////////////////////////////
#include "systemclass.h"

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd )
{
	SystemClass* System;

	// ����System��
	System = new SystemClass;

	if (!System)
	{
		return 0;
	}

	// ��ʼ��System��,��ѭ��
	if (System->Initialze())
	{
		System->Run();
	}

	// ɾ��System��
	System->Shutdown();
	delete System;
	System = NULL;

	return 0;

}
