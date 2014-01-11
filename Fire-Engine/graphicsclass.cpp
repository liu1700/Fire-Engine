
//////////////////////////////////////////////////////////
//文件名: graphicsclass.cpp		日期: 创建于:2014/1/8
//////////////////////////////////////////////////////////
#include "graphicsclass.h"

GraphicsClass::GraphicsClass()
{
	m_D3D = NULL;
	m_Camera = NULL;
	m_Model = NULL;
	m_LightShader = NULL;
	m_Light = NULL;
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
	m_Camera->SetPosition(0.0f, 0.0f, -5.0f);

	// 创建model对象
	m_Model = new ModelClass;
	if(!m_Model)
		return false;

	// 初始化model对象
	if(!m_Model->Initialze(m_D3D->GetDevice(), L"../Fire-Engine/Textures/test.dds", L"../Fire-Engine/Models/Cube.txt"))
	{
		MessageBox(hwnd, L"无法初始化model",L"Error", MB_OK);
		return false;
	}

	// 创建shader对象
	m_LightShader = new LightShaderClass;
	if(!m_LightShader)
		return false;

	// 初始化shader
	if(!m_LightShader->Initialze(m_D3D->GetDevice(), hwnd))
	{
		MessageBox(hwnd, L"初始化Shader对象错误", L"Error", MB_OK);
		return false;
	}

	// 创建光照对象
	m_Light = new LightClass;
	if(!m_Light)
		return false;

	// 初始化光照对象
	m_Light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetDirection(0.0f, 0.0f, 1.0f);

	return true;
}

void GraphicsClass::Shutdown()
{
	if (m_Model)
	{
		delete m_Model;
		m_Model = NULL;
	}
	if (m_Light)
	{
		delete m_Light;
		m_Light = NULL;
	}

	if (m_LightShader)
	{
		m_LightShader->ShutDown();
		delete m_LightShader;
		m_LightShader = NULL;
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
	static float rotation = 0.0f;

	//  每帧更新旋转
	rotation += (float)D3DX_PI * 0.01f;
	if (rotation > 360.0f)
	{
		rotation -= 360.0f;
	}

	if(!Render(rotation))
		return false;

	return true;
}

bool GraphicsClass::Render(float rotation)
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

	// 旋转世界矩阵
	D3DXMatrixRotationY(&worldMatrix, rotation);

	// 将model的vertex与index buffer 放到图形绘制管线上
	m_Model->Render(m_D3D->GetDeviceContext());

	// 利用shader渲染model
	if(!m_LightShader->Render(m_D3D->GetDeviceContext(), m_Model->GetIndexCount(),
		worldMatrix, viewMatrix, projectionMatrix, m_Model->GetTexture(), m_Light->GetDirection(), m_Light->GetDiffuseColor()))
		return false;

	// 呈现
	m_D3D->EndScene();

	return true;
}
