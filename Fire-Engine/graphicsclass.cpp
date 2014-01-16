
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
	m_Bitmap = NULL;
	m_TextureShader = NULL;
	m_Text = NULL;
	m_ModelList = NULL;
	m_Frustum = NULL;
	m_MultiTextureShader = NULL;
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
	m_Camera->SetPosition(0.0f, 0.0f, -10.0f);
	m_Camera->Render();
	m_Camera->GetViewMatrix(baseViewMatrix);

	m_Text = new TextClass;
	if(!m_Text)
		return false;

	if(!m_Text->Initialze(m_D3D->GetDevice(), m_D3D->GetDeviceContext()))
	{
		MessageBox(hwnd, L"无法初始化text",L"Error",MB_OK);
		return false;
	}

	// 创建model对象
	m_Model = new ModelClass;
	if(!m_Model)
		return false;

	// 初始化model对象
	if(!m_Model->Initialze(m_D3D->GetDevice(), L"../Fire-Engine/Models/Cube.txt", L"../Fire-Engine/Textures/stone01.dds", 
		L"../Fire-Engine/Textures/dirt01.dds"))
	{
		MessageBox(hwnd, L"无法初始化model",L"Error", MB_OK);
		return false;
	}

	// 创建Lightshader对象
	m_LightShader = new LightShaderClass;
	if(!m_LightShader)
		return false;

	// 初始化shader
	if(!m_LightShader->Initialze(m_D3D->GetDevice(), hwnd))
	{
		MessageBox(hwnd, L"初始化LightShader对象错误", L"Error", MB_OK);
		return false;
	}

	// 创建Textureshader对象
	m_TextureShader = new TextureShaderClass;
	if(!m_TextureShader)
		return false;

	// 初始化shader
	if(!m_TextureShader->Initialize(m_D3D->GetDevice(), hwnd))
	{
		MessageBox(hwnd, L"初始化TextureShader对象错误", L"Error", MB_OK);
		return false;
	}

	// 创建MultiTextureshader对象
	m_MultiTextureShader = new MultiTextureShaderClass;
	if(!m_MultiTextureShader)
		return false;

	// 初始化shader
	if(!m_MultiTextureShader->Initialize(m_D3D->GetDevice(), hwnd))
	{
		MessageBox(hwnd, L"初始化MultiTextureShader对象错误", L"Error", MB_OK);
		return false;
	}

	// 创建2D图像对象
	m_Bitmap = new BitmapClass;
	if(!m_Bitmap)
		return false;

	// 初始化bitmap
	if(!m_Bitmap->Initialze(m_D3D->GetDevice(), screenWidth, screenHeight, L"../Fire-Engine/Textures/test.dds", 256,256))
	{
		MessageBox(hwnd, L"无法初始化bitmap对象", L"Error", MB_OK);
		return false;
	}

	// 创建ModelList对象
	m_ModelList = new ModelListClass;
	if(!m_ModelList)
		return false;

	// 初始化25个模型对象
	if(!m_ModelList->Initialze(25))
	{
		MessageBox(hwnd, L"无法初始化ModelList对象", L"Error", MB_OK);
		return false;
	}

	// 创建视锥对象
	m_Frustum = new FrustumClass;
	if(!m_Frustum)
		return false;

	// 创建光照对象
	m_Light = new LightClass;
	if(!m_Light)
		return false;

	// 初始化光照对象
	m_Light->SetAmbientColor(0.2f, 0.2f, 0.2f, 1.0f);
	m_Light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetDirection(1.0f, 0.0f, 1.0f);
	m_Light->SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetSpecularPower(32.0f);

	return true;
}

void GraphicsClass::ShutDown()
{
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

bool GraphicsClass::Frame(int mouseX, int mouseY, int fps, int cpu, float frameTime, float rotationY)
{
	m_Text->SetMousePosition(mouseX, mouseY, m_D3D->GetDeviceContext());
	m_Text->SetFps(fps, m_D3D->GetDeviceContext());
	m_Text->SetCpu(cpu, m_D3D->GetDeviceContext());

	if(!Render( mouseX, mouseY, fps, cpu, frameTime))
		return false;

	// 设定Camera的信息
	m_Camera->SetPosition(0.0f, 0.0f, -10.0f);
	m_Camera->SetRotation(0.0f, rotationY, 0.0f);

	return true;
}

bool GraphicsClass::Render( int mouseX, int mouseY, int fps, int cpu, float frameTime)
{
	D3DXMATRIX viewMatrix, projectionMatrix, worldMatrix, orthoMatrix;
	int modelCount, renderCount;
	float positionX, positionY, positionZ, radius;
	D3DXVECTOR4 color;
	bool renderModel;

	// BackBuffer清除为灰色
	m_D3D->BeginScene(0.7f, 0.7f, 0.7f, 1.0f);

	// 基于Camera位置算出ViewMatrix
	m_Camera->Render();

	// 获取三个Matrix
	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetWorldMatrix(worldMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);
	// 获取正交矩阵
	m_D3D->GetOrthoMatrix(orthoMatrix);

	// 建立视锥体
	m_Frustum->ConstructFrustum(SCREEN_DEPTH, projectionMatrix, viewMatrix);
	modelCount = m_ModelList->GetModelCount();

	// 初始化渲染模型的数量为0
	renderCount = 0;

	for(int index=0; index<modelCount; index++)
	{
		// 获取模型数据
		m_ModelList->GetData(index, positionX, positionY, positionZ, color);

		// 设定半径
		radius = 1.73205081f;

		// 开始检测
		renderModel = m_Frustum->CheckCube(positionX, positionY, positionZ, radius);

		// 是否开始渲染
		if(renderModel)
		{
			// 将模型移动至相应位置
			D3DXMatrixTranslation(&worldMatrix, positionX, positionY, positionZ); 

			// 将model的vertex与index buffer 放到图形绘制管线上
			if(!m_Bitmap->Render(m_D3D->GetDeviceContext(), 400, 400))
				return false;

			// 利用shader渲染model
			if(!m_LightShader->Render(m_D3D->GetDeviceContext(), m_Bitmap->GetIndexCount(),
				worldMatrix, viewMatrix, projectionMatrix, m_Bitmap->GetTexture(), m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
				m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower()))
				return false;
			// 重新设定至世界矩阵
			m_D3D->GetWorldMatrix(worldMatrix);

			renderCount++;
		}
	}

	// 关闭Z buffer
	m_D3D->TurnZBufferOff();

	// 渲染text
	//m_Text->Render(m_D3D->GetDeviceContext(), L"OK", 200, 400);
	m_Text->SetMousePosition(mouseX, mouseY, m_D3D->GetDeviceContext());
	m_Text->SetFps(fps, m_D3D->GetDeviceContext());
	m_Text->SetCpu(cpu, m_D3D->GetDeviceContext());
	m_Text->SetRenerCount(renderCount, m_D3D->GetDeviceContext());

	
	// 将Bitmap的vertex与index buffer 放到图形绘制管线上
	//if(!m_Bitmap->Render(m_D3D->GetDeviceContext(), 400, 400))
	//	return false;

	 //渲染Bitmap
	//if(!m_TextureShader->Render(m_D3D->GetDeviceContext(), m_Bitmap->GetIndexCount(), 
	//	worldMatrix, viewMatrix, orthoMatrix, m_Bitmap->GetTexture()))
	//	return false;
	m_MultiTextureShader->Render(m_D3D->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
		m_Model->GetTextureArray());

	// 开启Z buffer
	m_D3D->TurnZBufferOn();

	// 呈现
	m_D3D->EndScene();

	return true;
}
