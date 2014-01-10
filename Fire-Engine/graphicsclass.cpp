
//////////////////////////////////////////////////////////
//文件名: graphicsclass.cpp		日期: 创建于:2014/1/8
//////////////////////////////////////////////////////////
#include "graphicsclass.h"

GraphicsClass::GraphicsClass()
{
	m_D3D = NULL;
	m_Camera = NULL;
	m_Model = NULL;
	m_ColorShader = NULL;
}

GraphicsClass::GraphicsClass(const GraphicsClass& other)
{

}

GraphicsClass::~GraphicsClass()
{

}

bool GraphicsClass::Initialze(int screenWidth, int screenHeight, HWND hwnd)
{

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

	// 创建Camera对象
	m_Camera = new CameraClass;
	if(!m_Camera)
		return false;

	// 设定Camera的位置
	m_Camera->SetPosition(0.0f, 0.0f, -20.0f);

	// 创建model对象
	m_Model = new ModelClass;
	if(!m_Model)
		return false;

	// 初始化model对象
	if(!m_Model->Initialze(m_D3D->GetDevice()))
	{
		MessageBox(hwnd, L"无法初始化model",L"Error", MB_OK);
		return false;
	}

	// 创建shader对象
	m_ColorShader = new ColorShaderClass;
	if(!m_ColorShader)
		return false;

	// 初始化shader
	if(!m_ColorShader->Initialze(m_D3D->GetDevice(), hwnd))
	{
		MessageBox(hwnd, L"初始化Shader对象错误", L"Error", MB_OK);
		return false;
	}

	return true;
}

void GraphicsClass::Shutdown()
{
	if (m_ColorShader)
	{
		m_ColorShader->ShutDown();
		delete m_ColorShader;
		m_ColorShader = NULL;
	}

	if (m_Model)
	{
		m_Model->ShutDown();
		delete m_Model;
		m_Model = NULL;
	}

	if (m_Camera)
	{
		delete m_Camera;
		m_Camera = NULL;
	}

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
	D3DXMATRIX viewMatrix, projectionMatrix, worldMatrix;

	// BackBuffer清除为灰色
	m_D3D->BeginScene(0.7f, 0.7f, 0.7f, 1.0f);

	// 基于Camera位置算出ViewMatrix
	m_Camera->Render();

	// 获取三个Matrix
	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetWorldMatrix(worldMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);

	// 将model的vertex与index buffer 放到图形绘制管线上
	m_Model->Render(m_D3D->GetDeviceContext());

	// 利用shader渲染model
	if(!m_ColorShader->Render(m_D3D->GetDeviceContext(), m_Model->GetIndexCount(),
		worldMatrix, viewMatrix, projectionMatrix))
		return false;

	// 呈现
	m_D3D->EndScene();

	return true;
}
