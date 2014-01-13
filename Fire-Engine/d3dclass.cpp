
//////////////////////////////////////////////////////////
//�ļ���: d3dclass.cpp		����: ������:2014/1/9
//////////////////////////////////////////////////////////
#include "d3dclass.h"
#include "fileIO.h"

D3DClass::D3DClass()
{
	m_swapChain = NULL;
	m_device = NULL;
	m_deviceContext = NULL;
	m_renderTargetView = NULL;
	m_depthStencilBuffer = NULL;
	m_depthStencilState = NULL;
	m_depthStencilView = NULL;
	m_rasterState = NULL;
	m_depthDisabledStencilState = NULL;

	m_alphaEnableBlendingState = NULL;
	m_depthDisabledStencilState = NULL;
}

D3DClass::D3DClass(const D3DClass& other)
{

}

D3DClass::~D3DClass()
{

}

bool D3DClass::Initialze(int screenWidth, int screenHeight, bool vsync, HWND hwnd, 
						 bool fullscreen, float screenDepth, float screenNear)
{
	IDXGIFactory* factory;
	IDXGIAdapter* adapter;
	IDXGIOutput* adapterOutput;
	unsigned int numModes, i, numerator, denominator, stringLength;
	DXGI_MODE_DESC* displayModeList;
	DXGI_ADAPTER_DESC adapterDesc;
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	D3D_FEATURE_LEVEL featureLevel;
	ID3D11Texture2D* backBufferPtr;
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	D3D11_RASTERIZER_DESC rasterDesc;
	D3D11_VIEWPORT viewport;
	float fieldOfView, screenAspect;
	D3D11_DEPTH_STENCIL_DESC depthDisabledStencilDesc;

	D3D11_BLEND_DESC blendStateDesc;

	// ���ô�ֱͬ��
	m_vsync_enabled = vsync;

	// ����dxͼ�ι���
	if(FAILED(CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory)))
		return false;

	// ����������
	if(FAILED(factory->EnumAdapters(0, &adapter)))
		return false;

	// ö�ٻ�����������
	if(FAILED(adapter->EnumOutputs(0, &adapterOutput)))
		return false;

	// ����ʺ�DXGI_FORMAT_R8G8B8A8_UNORM��ʾģʽ��������
	if(FAILED(adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL)))
		return false;

	// �����ʺϿͻ����Կ�����ʾģʽ�б�
	displayModeList = new DXGI_MODE_DESC[numModes];
	if(!displayModeList)
		return false;

	// ����displayModeList���ݽṹ����
	if(FAILED(adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList)))
		return false;

	// �Զ�ƥ���ʺ���Ļ����ʾģʽ
	for (i = 0; i < numModes; i++)
	{
		if (displayModeList[i].Width == (unsigned int)screenWidth)
		{
			if (displayModeList[i].Height == (unsigned int)screenHeight)
			{
				numerator = displayModeList[i].RefreshRate.Numerator;
				denominator = displayModeList[i].RefreshRate.Denominator;
			}
		}
	}

	// ��ȡ�Կ�����
	if(FAILED(adapter->GetDesc(&adapterDesc)))
		return false;

	// ��MBΪ��λ�洢�Կ��ڴ�
	m_videoCardMemory = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);

	// �洢�Կ�����
	if(wcstombs_s(&stringLength, m_videoCardDescription, 128, adapterDesc.Description, 128) != 0)
		return false;
	
	// �ͷŸղ����ڻ�ȡ�Կ���Ϣ�Լ�ˢ���ʵı���,��Ϊ�����Ѿ���������Ϣ���洢��
	delete[] displayModeList;
	displayModeList = NULL;

	adapterOutput->Release();
	adapterOutput = NULL;

	adapter->Release();
	adapter = NULL;

	factory->Release();
	factory = NULL;

	// ���ý����� //
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

	// �趨BufferCountΪ1��������ֻ����һ������
	swapChainDesc.BufferCount = 1;

	// �趨�����л���ĳ���
	swapChainDesc.BufferDesc.Width = screenWidth;
	swapChainDesc.BufferDesc.Height = screenHeight;

	// �趨Ϊ32λɫ
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	// �趨backbuffer��ˢ����
	if (m_vsync_enabled)
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = numerator;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = denominator;
	}
	else
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	}

	// �趨backbuffer���÷�
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	// �趨Ҫ��Ⱦ�Ĵ���
	swapChainDesc.OutputWindow = hwnd;

	// �رն��ز���
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;

	// �趨Ϊȫ���򴰿���ʾ
	if (fullscreen)
	{
		swapChainDesc.Windowed = false;
	}
	else
	{
		swapChainDesc.Windowed = true;
	}

	// �趨ͼ����ʾ����Ϊδָ��(��Ļ���ص����ɷ�ʽ)
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	// ͼ�����֮�����backbuffer
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	swapChainDesc.Flags = 0;

	// �趨��ʾ����Ҳ��������Ҫ��DX��
	featureLevel = D3D_FEATURE_LEVEL_11_0;

	// ������������D3D device��D3Ddevice������
	if(FAILED(D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &featureLevel, 1, D3D11_SDK_VERSION,
		&swapChainDesc, &m_swapChain, &m_device, NULL, &m_deviceContext)))
		return false;

	// ��backbuffer�����ݹ�����������
	if(FAILED(m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferPtr)))
		return false;

	// ��backbufferָ�봴����ȾĿ��
	if(FAILED(m_device->CreateRenderTargetView(backBufferPtr, NULL, &m_renderTargetView)))
		return false;

	// �ͷ�backbuffer��ָ��
	backBufferPtr->Release();
	backBufferPtr = NULL;

	// ��Ȼ���������������� //
	// ��ʼ����Ȼ���
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

	// �趨��Ȼ�����Ϣ
	depthBufferDesc.Width				= screenWidth;
	depthBufferDesc.Height				= screenHeight;
	depthBufferDesc.MipLevels			= 1;
	depthBufferDesc.ArraySize			= 1;
	depthBufferDesc.Format				= DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count	= 1;
	depthBufferDesc.SampleDesc.Quality	= 0;
	depthBufferDesc.Usage				= D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags			= D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags		= 0;
	depthBufferDesc.MiscFlags			= 0;

	// Ϊ��Ȼ��洴������
	if(FAILED(m_device->CreateTexture2D(&depthBufferDesc, NULL, &m_depthStencilBuffer)))
		return false;

	// �������ģ��
	// ��ʼ�����״̬
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	// �趨���״̬
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	// ������ǰ��
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// �����ں���
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// �������ģ��
	if(FAILED(m_device->CreateDepthStencilState(&depthStencilDesc, &m_depthStencilState)))
		return false;

	// �趨���ģ��
	m_deviceContext->OMSetDepthStencilState(m_depthStencilState, 1);

	// ������Ȼ�����ͼ
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	// �������ģ����ͼ
	if(FAILED(m_device->CreateDepthStencilView(m_depthStencilBuffer, &depthStencilViewDesc, &m_depthStencilView)))
		return false;

	// ����Ȼ�����ͼ����ȾĿ����ͼ�󶨵���ͼ����
	m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);

	// �趨��դ����Ϣ
	rasterDesc.AntialiasedLineEnable	= false;
	rasterDesc.CullMode					= D3D11_CULL_BACK;
	rasterDesc.DepthBias				= 0;
	rasterDesc.DepthBiasClamp			= 0.0f;
	rasterDesc.DepthClipEnable			= true;
	rasterDesc.FillMode					= D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise	= false;
	rasterDesc.MultisampleEnable		= false;
	rasterDesc.ScissorEnable			= false;
	rasterDesc.SlopeScaledDepthBias		= 0.0f;

	// ������դ��
	if(FAILED(m_device->CreateRasterizerState(&rasterDesc, &m_rasterState)))
		return false;

	m_deviceContext->RSSetState(m_rasterState);

	// �趨�Ӵ���
	viewport.Width = (float)screenWidth;
	viewport.Height = (float)screenHeight;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;

	// �����Ӵ���
	m_deviceContext->RSSetViewports(1, &viewport);

	// ����ͶӰ���󣨽�3D����ת��Ϊ2D���棩
	// �趨ͶӰ����
	fieldOfView = (float)D3DX_PI / 4.0f;
	screenAspect = (float)screenWidth / (float)screenHeight;

	// Ϊ3D��Ⱦ����ͶӰ����
	D3DXMatrixPerspectiveFovLH(&m_projectionMatrix, fieldOfView, screenAspect,
		screenNear, screenDepth);
	
	// ����������󣨽����������ת������Ⱦ��3D�����е����꣩
	// ����������ʼ��Ϊ��λ��
	D3DXMatrixIdentity(&m_worldMatrix);

	// �����۲����
	// NULL //

	// Ϊ2D��Ⱦ������������
	D3DXMatrixOrthoLH(&m_orthoMatrix, (float)screenWidth, (float)screenHeight, screenNear, screenDepth);

	// ����2Dģ��
	// �����ڴ�
	ZeroMemory(&depthDisabledStencilDesc, sizeof(depthDisabledStencilDesc));

	// �趨���״̬
	depthDisabledStencilDesc.DepthEnable = false;
	depthDisabledStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthDisabledStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthDisabledStencilDesc.StencilEnable = true;
	depthDisabledStencilDesc.StencilReadMask = 0xFF;
	depthDisabledStencilDesc.StencilWriteMask = 0xFF;

	// ������ǰ��
	depthDisabledStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthDisabledStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// �����ں���
	depthDisabledStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthDisabledStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// �������ģ��
	if(FAILED(m_device->CreateDepthStencilState(&depthDisabledStencilDesc, &m_depthDisabledStencilState)))
		return false;
	
	// ��ʼ�����״̬
	ZeroMemory(&blendStateDesc, sizeof(D3D11_BLEND_DESC));

	// ����͸���Ȼ����Ϣ
	blendStateDesc.RenderTarget[0].BlendEnable		= TRUE;
	blendStateDesc.RenderTarget[0].SrcBlend			= D3D11_BLEND_ONE;
	blendStateDesc.RenderTarget[0].DestBlend		= D3D11_BLEND_INV_SRC_ALPHA;
	blendStateDesc.RenderTarget[0].BlendOp			= D3D11_BLEND_OP_ADD;
	blendStateDesc.RenderTarget[0].SrcBlendAlpha	= D3D11_BLEND_ONE;
	blendStateDesc.RenderTarget[0].DestBlendAlpha	= D3D11_BLEND_ZERO;
	blendStateDesc.RenderTarget[0].BlendOpAlpha		= D3D11_BLEND_OP_ADD;
	blendStateDesc.RenderTarget[0].RenderTargetWriteMask = 0x0f;

	if(FAILED(m_device->CreateBlendState(&blendStateDesc, &m_alphaEnableBlendingState)))
		return false;

	// �ر�͸���Ȼ��
	blendStateDesc.RenderTarget[0].BlendEnable = FALSE;
	if(FAILED(m_device->CreateBlendState(&blendStateDesc, &m_alphaDisableBlendingState)))
		return false;

	// ��ȡ�Կ���Ϣ��������ı��ļ���
	char videoCardName[128];
	int videoMenmory;
	FileIOClass* writeCardName;

	GetVideoCardInfo(videoCardName, videoMenmory);

	writeCardName = new FileIOClass;
	writeCardName->WriteTextFile(L"VideoCardInfo.txt", videoCardName);
	writeCardName->WriteTextFile(L"VideoCardInfo.txt", videoMenmory);

	return true;
}

void D3DClass::ShutDown()
{
	// ���ͷ���Դ֮ǰ�����趨Ϊ����ģʽ�������ͷŽ�����ʱ���������
	if (m_swapChain)
	{
		m_swapChain->SetFullscreenState(false, NULL);
	}

	if (m_alphaEnableBlendingState)
	{
		m_alphaEnableBlendingState->Release();
		m_alphaEnableBlendingState = NULL;
	}

	if (m_alphaDisableBlendingState)
	{
		m_alphaDisableBlendingState->Release();
		m_alphaDisableBlendingState = NULL;
	}

	if (m_rasterState)
	{
		m_rasterState->Release();
		m_rasterState = NULL;
	}

	if (m_depthDisabledStencilState)
	{
		m_depthDisabledStencilState->Release();
		m_depthDisabledStencilState = NULL;
	}

	if (m_depthStencilView)
	{
		m_depthStencilView->Release();
		m_depthStencilView = NULL;
	}

	if (m_depthStencilState)
	{
		m_depthStencilState->Release();
		m_depthStencilState = NULL;
	}

	if (m_depthStencilBuffer)
	{
		m_depthStencilBuffer->Release();
		m_depthStencilBuffer = NULL;
	}

	if (m_renderTargetView)
	{
		m_renderTargetView->Release();
		m_renderTargetView = NULL;
	}

	if (m_deviceContext)
	{
		m_deviceContext->Release();
		m_deviceContext = NULL;
	}

	if (m_device)
	{
		m_device->Release();
		m_device = NULL;
	}

	if (m_swapChain)
	{
		m_swapChain->Release();
		m_swapChain = NULL;
	}

	return;
}

void D3DClass::BeginScene(float red, float green, float blue, float alpha)
{
	float color[4];

	color[0] = red;
	color[1] = green;
	color[2] = blue;
	color[3] = alpha;

	// ��color���backbuffer
	m_deviceContext->ClearRenderTargetView(m_renderTargetView, color);
	
	// �����Ȼ���
	m_deviceContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	return;
}

void D3DClass::EndScene()
{
	// �����ƺõ�backbuffer���ֳ���
	if (m_vsync_enabled)
	{
		// ����Ϊ��Ļˢ����
		m_swapChain->Present(1, 0);
	}
	else
	{
		// �����ٶ�������
		m_swapChain->Present(0, 0);
	}

	return;
}

ID3D11Device* D3DClass::GetDevice()
{
	return m_device;
}

ID3D11DeviceContext* D3DClass::GetDeviceContext()
{
	return m_deviceContext;
}

void D3DClass::GetProjectionMatrix(D3DXMATRIX& projectionMatrix)
{
	projectionMatrix = m_projectionMatrix;
	return;
}

void D3DClass::GetWorldMatrix(D3DXMATRIX& worldMatrix)
{
	worldMatrix = m_worldMatrix;
	return;
}

void D3DClass::GetOrthoMatrix(D3DXMATRIX& orthoMatrix)
{
	orthoMatrix = m_orthoMatrix;
	return;
}

void D3DClass::GetVideoCardInfo(char* cardName, int& memory)
{
	strcpy_s(cardName, 128, m_videoCardDescription);
	memory = m_videoCardMemory;
	return;
}

void D3DClass::TurnZBufferOn()
{
	m_deviceContext->OMSetDepthStencilState(m_depthStencilState, 1);
	return;
}

void D3DClass::TurnZBufferOff()
{
	m_deviceContext->OMSetDepthStencilState(m_depthDisabledStencilState, 1);
	return;
}

void D3DClass::TurnOnAlphaBlending()
{
	float blendFactor[4];

	blendFactor[0] = 0.0f;
	blendFactor[1] = 0.0f;
	blendFactor[2] = 0.0f;
	blendFactor[3] = 0.0f;

	m_deviceContext->OMSetBlendState(m_alphaEnableBlendingState, blendFactor, 0xffffffff);

	return;
}

void D3DClass::TurnOffAlphaBlending()
{
	float blendFactor[4];

	blendFactor[0] = 0.0f;
	blendFactor[1] = 0.0f;
	blendFactor[2] = 0.0f;
	blendFactor[3] = 0.0f;

	m_deviceContext->OMSetBlendState(m_alphaDisableBlendingState, blendFactor, 0xffffffff);

	return;
}