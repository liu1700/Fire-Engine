
//////////////////////////////////////////////////////////
//文件名: graphicsclass.cpp		日期: 创建于:2014/1/8
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

	// 创建D3D对象
	m_D3D = new D3DClass;
	if(!m_D3D)
		return false;

	// 初始化D3D对象
	if(!m_D3D->Initialze(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR))
	{
		MessageBox(hwnd, L"Could not initialize Direct3D", L"Error", MB_OK);
		return false;
	}

	// 获取显卡信息并
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
	// BackBuffer清除为灰色
	m_D3D->BeginScene(0.5f, 0.5f, 0.5f, 1.0f);

	// 呈现
	m_D3D->EndScene();

	return true;
}
