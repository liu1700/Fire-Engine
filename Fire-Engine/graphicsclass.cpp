
//////////////////////////////////////////////////////////
//�ļ���: graphicsclass.cpp		����: ������:2014/1/8
//////////////////////////////////////////////////////////
#include "graphicsclass.h"
#include "fileIO.h"

GraphicsClass::GraphicsClass()
{
	m_D3D = NULL;
}

GraphicsClass::GraphicsClass(const GraphicsClass& other)
{

}

GraphicsClass::~GraphicsClass()
{

}

bool GraphicsClass::Initialze(int screenWidth, int screenHeight, HWND hwnd)
{
	char videoCardName[128];
	int videoMenmory;
	FileIOClass* writeCardName;

	// ����D3D����
	m_D3D = new D3DClass;
	if(!m_D3D)
		return false;

	// ��ʼ��D3D����
	if(!m_D3D->Initialze(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR))
	{
		MessageBox(hwnd, L"Could not initialize Direct3D", L"Error", MB_OK);
		return false;
	}

	// ��ȡ�Կ���Ϣ��
	m_D3D->GetVideoCardInfo(videoCardName, videoMenmory);

	writeCardName = new FileIOClass;
	writeCardName->WriteTextFile(L"VideoCardInfo.txt", videoCardName);
	writeCardName->WriteTextFile(L"VideoCardInfo.txt", videoMenmory);

	return true;
}

void GraphicsClass::Shutdown()
{
	if (m_D3D)
	{
		m_D3D->ShutDown();
		delete m_D3D;
		m_D3D = NULL;
	}

	return;
}

bool GraphicsClass::Frame()
{
	if(!Render())
		return false;

	return true;
}

bool GraphicsClass::Render()
{
	// BackBuffer���Ϊ��ɫ
	m_D3D->BeginScene(0.5f, 0.5f, 0.5f, 1.0f);

	// ����
	m_D3D->EndScene();

	return true;
}
