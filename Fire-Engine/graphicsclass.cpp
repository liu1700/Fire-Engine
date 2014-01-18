
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
	m_ModelList = NULL;
	m_Frustum = NULL;
	m_MultiTextureShader = NULL;
	m_AlphaMapShader = NULL;
	m_BumpMapShader = NULL;
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

	// ����MultiTextureshader����
	m_MultiTextureShader = new MultiTextureShaderClass;
	if(!m_MultiTextureShader)
		return false;

	// ��ʼ��shader
	if(!m_MultiTextureShader->Initialize(m_D3D->GetDevice(), hwnd))
	{
		MessageBox(hwnd, L"��ʼ��MultiTextureShader�������", L"Error", MB_OK);
		return false;
	}

	// ��������ʼ��AlphaMapShader����
	m_AlphaMapShader = new AlphaMapShaderClass;
	if(!m_AlphaMapShader)
		return false;

	if (!m_AlphaMapShader->Initialize(m_D3D->GetDevice(), hwnd))
	{
		MessageBox(hwnd, L"��ʼ��AlphaMapShader�������", L"Error", MB_OK);
		return false;
	}

	// ��������ʼ��BumpMapShader����
	m_BumpMapShader = new BumpMapShaderClass;
	if(!m_BumpMapShader)
		return false;

	if (!m_BumpMapShader->Initialize(m_D3D->GetDevice(), hwnd))
	{
		MessageBox(hwnd, L"��ʼ��BumpMapShader�������", L"Error", MB_OK);
		return false;
	}

	// ����2Dͼ�����
	m_Bitmap = new BitmapClass;
	if(!m_Bitmap)
		return false;

	// ��ʼ��bitmap
	if(!m_Bitmap->Initialze(m_D3D->GetDevice(), screenWidth, screenHeight, L"../Fire-Engine/Textures/stone01.dds", 
							L"../Fire-Engine/Textures/dirt01.dds", L"../Fire-Engine/Textures/alpha01.dds", 256,256))
	{
		MessageBox(hwnd, L"�޷���ʼ��bitmap����", L"Error", MB_OK);
		return false;
	}

	// ����model����
	m_Model = new ModelClass;
	if(!m_Model)
		return false;

	// ��ʼ��model����
	if(!m_Model->Initialize(m_D3D->GetDevice(), "../Fire-Engine/Models/Cube.txt", L"../Fire-Engine/Textures/stone01.dds", 
		L"../Fire-Engine/Textures/bump01.dds"))
	{
		MessageBox(hwnd, L"�޷���ʼ��model",L"Error", MB_OK);
		return false;
	}

	// ����ModelList����
	m_ModelList = new ModelListClass;
	if(!m_ModelList)
		return false;

	// ��ʼ��25��ģ�Ͷ���
	if(!m_ModelList->Initialze(25))
	{
		MessageBox(hwnd, L"�޷���ʼ��ModelList����", L"Error", MB_OK);
		return false;
	}

	// ������׶����
	m_Frustum = new FrustumClass;
	if(!m_Frustum)
		return false;

	// �������ն���
	m_Light = new LightClass;
	if(!m_Light)
		return false;

	// ��ʼ�����ն���
	//m_Light->SetAmbientColor(0.2f, 0.2f, 0.2f, 1.0f);
	m_Light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetDirection(0.0f, 0.0f, 1.0f);
	//m_Light->SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	//m_Light->SetSpecularPower(32.0f);

	return true;
}

void GraphicsClass::ShutDown()
{
	if (m_BumpMapShader)
	{
		m_BumpMapShader->Shutdown();
		delete m_BumpMapShader;
		m_BumpMapShader = NULL;
	}

	if (m_AlphaMapShader)
	{
		m_AlphaMapShader->ShutDown();
		delete m_AlphaMapShader;
		m_AlphaMapShader = NULL;
	}

	if (m_MultiTextureShader)
	{
		m_MultiTextureShader->ShutDown();
		delete m_MultiTextureShader;
		m_MultiTextureShader = NULL;
	}

	if (m_Frustum)
	{
		delete m_Frustum;
		m_Frustum = NULL;
	}

	if (m_ModelList)
	{
		m_ModelList->ShutDown();
		delete m_ModelList;
		m_ModelList = NULL;
	}

	if (m_Text)
	{
		m_Text->ShutDown();
		delete m_Text;
		m_Text = NULL;
	}

	if (m_TextureShader)
	{
		m_TextureShader->ShutDown();
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
		m_Model->Shutdown();
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

bool GraphicsClass::Frame(int mouseX, int mouseY, int fps, int cpu, float frameTime, D3DXVECTOR3 rotation)
{
	m_Text->SetMousePosition(mouseX, mouseY, m_D3D->GetDeviceContext());
	m_Text->SetFps(fps, m_D3D->GetDeviceContext());
	m_Text->SetCpu(cpu, m_D3D->GetDeviceContext());

	static float modelrotation = 0.0f;

	modelrotation += (float)D3DX_PI * 0.01f;
	if(modelrotation > 360.0f)
	{
		modelrotation -= 360.0f;
	}

	if(!Render( mouseX, mouseY, fps, cpu, frameTime, modelrotation))
		return false;

	// �趨Camera����Ϣ
	m_Camera->SetPosition(0.0f, 0.0f, -10.0f);
	m_Camera->SetRotation(rotation.x, rotation.y, 0.0f);

	return true;
}

bool GraphicsClass::Render( int mouseX, int mouseY, int fps, int cpu, float frameTime, float rotation)
{
	D3DXMATRIX viewMatrix, projectionMatrix, worldMatrix, orthoMatrix;
	int modelCount, renderCount;
	float positionX, positionY, positionZ, radius;
	D3DXVECTOR4 color;
	bool renderModel;

	// BackBuffer���Ϊ��ɫ
	m_D3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// ����Cameraλ�����ViewMatrix
	m_Camera->Render();

	// ��ȡ����Matrix
	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetWorldMatrix(worldMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);
	// ��ȡ��������
	m_D3D->GetOrthoMatrix(orthoMatrix);

	// ������׶��
	m_Frustum->ConstructFrustum(SCREEN_DEPTH, projectionMatrix, viewMatrix);
	modelCount = m_ModelList->GetModelCount();

	// ��ʼ����Ⱦģ�͵�����Ϊ0
	renderCount = 0;

	for(int index=0; index<modelCount; index++)
	{
		// ��ȡģ������
		m_ModelList->GetData(index, positionX, positionY, positionZ, color);

		// �趨�뾶
		radius = 1.73205081f;

		// ��ʼ���
		renderModel = m_Frustum->CheckCube(positionX, positionY, positionZ, radius);

		// �Ƿ�ʼ��Ⱦ
		if(renderModel = false)
		{

			// ��ģ���ƶ�����Ӧλ��
			D3DXMatrixTranslation(&worldMatrix, positionX, positionY, positionZ); 

			// ��model��vertex��index buffer �ŵ�ͼ�λ��ƹ�����
			m_Model->Render(m_D3D->GetDeviceContext());

			m_BumpMapShader->Render(m_D3D->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
				m_Model->GetTextureArray(), m_Light->GetDirection(), m_Light->GetDiffuseColor());

			// �����趨���������
			m_D3D->GetWorldMatrix(worldMatrix);

			renderCount++;
		}
	}

	D3DXMatrixRotationY(&worldMatrix, rotation);

	// ��model��vertex��index buffer �ŵ�ͼ�λ��ƹ�����
	m_Model->Render(m_D3D->GetDeviceContext());

	m_BumpMapShader->Render(m_D3D->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
		m_Model->GetTextureArray(), m_Light->GetDirection(), m_Light->GetDiffuseColor());

	// ��Ⱦtext
	m_Text->SetMousePosition(mouseX, mouseY, m_D3D->GetDeviceContext());
	m_Text->SetFps(fps, m_D3D->GetDeviceContext());
	m_Text->SetCpu(cpu, m_D3D->GetDeviceContext());
	m_Text->SetRenerCount(renderCount, m_D3D->GetDeviceContext());

	
	// ��Bitmap��vertex��index buffer �ŵ�ͼ�λ��ƹ�����
	if(!m_Bitmap->Render(m_D3D->GetDeviceContext(), 400, 400))
		return false;

	 //��ȾBitmap
	if(!m_AlphaMapShader->Render(m_D3D->GetDeviceContext(), m_Bitmap->GetIndexCount(), 
		worldMatrix, viewMatrix, orthoMatrix, m_Bitmap->GetTextureArray()))
		return false;

	// ����
	m_D3D->EndScene();

	return true;
}
