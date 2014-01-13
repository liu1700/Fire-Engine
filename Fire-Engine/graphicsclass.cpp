
//////////////////////////////////////////////////////////
//�ļ���: graphicsclass.cpp		����: ������:2014/1/8
//////////////////////////////////////////////////////////
#include "graphicsclass.h"

GraphicsClass::GraphicsClass()
{
	m_D3D = NULL;
	m_Camera = NULL;
	m_Model = NULL;
	m_LightShader = NULL;
	m_Light = NULL;
	m_Bitmap = NULL;
	m_TextureShader = NULL;
	m_Text = NULL;
}

GraphicsClass::GraphicsClass(const GraphicsClass& other)
{

}

GraphicsClass::~GraphicsClass()
{

}

bool GraphicsClass::Initialze(int screenWidth, int screenHeight, HWND hwnd)
{
	D3DXMATRIX baseViewMatrix;

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
	m_Camera->SetPosition(0.0f, 0.0f, -10.0f);
	m_Camera->Render();
	m_Camera->GetViewMatrix(baseViewMatrix);

	m_Text = new TextClass;
	if(!m_Text)
		return false;

	if(!m_Text->Initialze(m_D3D->GetDevice(), m_D3D->GetDeviceContext()))
	{
		MessageBox(hwnd, L"�޷���ʼ��text",L"Error",MB_OK);
		return false;
	}

	// ����model����
	m_Model = new ModelClass;
	if(!m_Model)
		return false;

	// ��ʼ��model����
	if(!m_Model->Initialze(m_D3D->GetDevice(), L"../Fire-Engine/Textures/test.dds", L"../Fire-Engine/Models/Cube.txt"))
	{
		MessageBox(hwnd, L"�޷���ʼ��model",L"Error", MB_OK);
		return false;
	}

	// ����Lightshader����
	m_LightShader = new LightShaderClass;
	if(!m_LightShader)
		return false;

	// ��ʼ��shader
	if(!m_LightShader->Initialze(m_D3D->GetDevice(), hwnd))
	{
		MessageBox(hwnd, L"��ʼ��LightShader�������", L"Error", MB_OK);
		return false;
	}

	// ����Textureshader����
	m_TextureShader = new TextureShaderClass;
	if(!m_TextureShader)
		return false;

	// ��ʼ��shader
	if(!m_TextureShader->Initialize(m_D3D->GetDevice(), hwnd))
	{
		MessageBox(hwnd, L"��ʼ��TextureShader�������", L"Error", MB_OK);
		return false;
	}

	// ����2Dͼ�����
	m_Bitmap = new BitmapClass;
	if(!m_Bitmap)
		return false;

	// ��ʼ��bitmap
	if(!m_Bitmap->Initialze(m_D3D->GetDevice(), screenWidth, screenHeight, L"../Fire-Engine/Textures/test.dds", 256,256))
	{
		MessageBox(hwnd, L"�޷���ʼ��bitmap����", L"Error", MB_OK);
		return false;
	}

	// �������ն���
	m_Light = new LightClass;
	if(!m_Light)
		return false;

	// ��ʼ�����ն���
	m_Light->SetAmbientColor(0.2f, 0.2f, 0.2f, 1.0f);
	m_Light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetDirection(1.0f, 0.0f, 1.0f);
	m_Light->SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetSpecularPower(32.0f);

	return true;
}

void GraphicsClass::Shutdown()
{
	if (m_Text)
	{
		m_Text->ShutDown();
		delete m_Text;
		m_Text = NULL;
	}

	if (m_TextureShader)
	{
		m_TextureShader->Shutdown();
		delete m_TextureShader;
		m_TextureShader = NULL;
	}

	if (m_Bitmap)
	{
		m_Bitmap->ShutDown();
		delete m_Bitmap;
		m_Bitmap = NULL;
	}

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

	//  ÿ֡������ת
	rotation += (float)D3DX_PI * 0.005f;
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
	D3DXMATRIX viewMatrix, projectionMatrix, worldMatrix, orthoMatrix;

	// BackBuffer���Ϊ��ɫ
	m_D3D->BeginScene(0.7f, 0.7f, 0.7f, 1.0f);

	// ����Cameraλ�����ViewMatrix
	m_Camera->Render();

	// ��ȡ����Matrix
	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetWorldMatrix(worldMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);

	// ��ȡ��������
	m_D3D->GetOrthoMatrix(orthoMatrix);

	// �ر�Z buffer
	m_D3D->TurnZBufferOff();
	// ����͸�����
	m_D3D->TurnOnAlphaBlending();

	// ��Ⱦtext
	if(!m_Text->Render(m_D3D->GetDeviceContext(), L"OK", 200, 400))
		return false;

	m_D3D->TurnOffAlphaBlending();
	
	// ��Bitmap��vertex��index buffer �ŵ�ͼ�λ��ƹ�����
	if(!m_Bitmap->Render(m_D3D->GetDeviceContext(), 1, 1))
		return false;

	 //��ȾBitmap
	if(!m_TextureShader->Render(m_D3D->GetDeviceContext(), m_Bitmap->GetIndexCount(), 
		worldMatrix, viewMatrix, orthoMatrix, m_Bitmap->GetTexture()))
		return false;

	// ����Z buffer
	m_D3D->TurnZBufferOn();

	// ��ת�������
	D3DXMatrixRotationY(&worldMatrix, rotation);

	// ��model��vertex��index buffer �ŵ�ͼ�λ��ƹ�����
	m_Model->Render(m_D3D->GetDeviceContext());

	// ����shader��Ⱦmodel
	if(!m_LightShader->Render(m_D3D->GetDeviceContext(), m_Model->GetIndexCount(),
		worldMatrix, viewMatrix, projectionMatrix, m_Model->GetTexture(), m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
		m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower()))
		return false;

	// ����
	m_D3D->EndScene();

	return true;
}
