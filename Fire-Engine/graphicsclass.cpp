
//////////////////////////////////////////////////////////
//�ļ���: graphicsclass.cpp		����: ������:2014/1/8
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

	// ����Camera����
	m_Camera = new CameraClass;
	if(!m_Camera)
		return false;

	// �趨Camera��λ��
	m_Camera->SetPosition(0.0f, 0.0f, -20.0f);

	// ����model����
	m_Model = new ModelClass;
	if(!m_Model)
		return false;

	// ��ʼ��model����
	if(!m_Model->Initialze(m_D3D->GetDevice()))
	{
		MessageBox(hwnd, L"�޷���ʼ��model",L"Error", MB_OK);
		return false;
	}

	// ����shader����
	m_ColorShader = new ColorShaderClass;
	if(!m_ColorShader)
		return false;

	// ��ʼ��shader
	if(!m_ColorShader->Initialze(m_D3D->GetDevice(), hwnd))
	{
		MessageBox(hwnd, L"��ʼ��Shader�������", L"Error", MB_OK);
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

	// BackBuffer���Ϊ��ɫ
	m_D3D->BeginScene(0.7f, 0.7f, 0.7f, 1.0f);

	// ����Cameraλ�����ViewMatrix
	m_Camera->Render();

	// ��ȡ����Matrix
	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetWorldMatrix(worldMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);

	// ��model��vertex��index buffer �ŵ�ͼ�λ��ƹ�����
	m_Model->Render(m_D3D->GetDeviceContext());

	// ����shader��Ⱦmodel
	if(!m_ColorShader->Render(m_D3D->GetDeviceContext(), m_Model->GetIndexCount(),
		worldMatrix, viewMatrix, projectionMatrix))
		return false;

	// ����
	m_D3D->EndScene();

	return true;
}
