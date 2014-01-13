
//////////////////////////////////////////////////////////
//文件名: d3dclass.cpp		日期: 创建于:2014/1/9
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

	// 设置垂直同步
	m_vsync_enabled = vsync;

	// 创建dx图形工厂
	if(FAILED(CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory)))
		return false;

	// 创建适配器
	if(FAILED(factory->EnumAdapters(0, &adapter)))
		return false;

	// 枚举基本的适配器
	if(FAILED(adapter->EnumOutputs(0, &adapterOutput)))
		return false;

	// 获得适合DXGI_FORMAT_R8G8B8A8_UNORM显示模式的适配器
	if(FAILED(adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL)))
		return false;

	// 创建适合客户端显卡的显示模式列表
	displayModeList = new DXGI_MODE_DESC[numModes];
	if(!displayModeList)
		return false;

	// 设置displayModeList数据结构内容
	if(FAILED(adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList)))
		return false;

	// 自动匹配适合屏幕的显示模式
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

	// 获取显卡详情
	if(FAILED(adapter->GetDesc(&adapterDesc)))
		return false;

	// 以MB为单位存储显卡内存
	m_videoCardMemory = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);

	// 存储显卡名称
	if(wcstombs_s(&stringLength, m_videoCardDescription, 128, adapterDesc.Description, 128) != 0)
		return false;
	
	// 释放刚才用于获取显卡信息以及刷新率的变量,因为我们已经获得相关信息并存储了
	delete[] displayModeList;
	displayModeList = NULL;

	adapterOutput->Release();
	adapterOutput = NULL;

	adapter->Release();
	adapter = NULL;

	factory->Release();
	factory = NULL;

	// 设置交换链 //
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

	// 设定BufferCount为1，缓存中只绘制一幅画面
	swapChainDesc.BufferCount = 1;

	// 设定缓存中画面的长宽
	swapChainDesc.BufferDesc.Width = screenWidth;
	swapChainDesc.BufferDesc.Height = screenHeight;

	// 设定为32位色
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	// 设定backbuffer的刷新率
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

	// 设定backbuffer的用法
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	// 设定要渲染的窗口
	swapChainDesc.OutputWindow = hwnd;

	// 关闭多重采样
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;

	// 设定为全屏或窗口显示
	if (fullscreen)
	{
		swapChainDesc.Windowed = false;
	}
	else
	{
		swapChainDesc.Windowed = true;
	}

	// 设定图像显示命令为未指定(屏幕像素点生成方式)
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	// 图像呈现之后清空backbuffer
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	swapChainDesc.Flags = 0;

	// 设定显示级别，也就是我们要用DX几
	featureLevel = D3D_FEATURE_LEVEL_11_0;

	// 创建交换链，D3D device和D3Ddevice上下文
	if(FAILED(D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &featureLevel, 1, D3D11_SDK_VERSION,
		&swapChainDesc, &m_swapChain, &m_device, NULL, &m_deviceContext)))
		return false;

	// 将backbuffer的内容关联到交换链
	if(FAILED(m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferPtr)))
		return false;

	// 用backbuffer指针创建渲染目标
	if(FAILED(m_device->CreateRenderTargetView(backBufferPtr, NULL, &m_renderTargetView)))
		return false;

	// 释放backbuffer的指针
	backBufferPtr->Release();
	backBufferPtr = NULL;

	// 深度缓存会带给我们立体感 //
	// 初始化深度缓存
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

	// 设定深度缓存信息
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

	// 为深度缓存创建纹理
	if(FAILED(m_device->CreateTexture2D(&depthBufferDesc, NULL, &m_depthStencilBuffer)))
		return false;

	// 设置深度模板
	// 初始化深度状态
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	// 设定深度状态
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	// 像素在前面
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// 像素在后面
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// 创建深度模板
	if(FAILED(m_device->CreateDepthStencilState(&depthStencilDesc, &m_depthStencilState)))
		return false;

	// 设定深度模板
	m_deviceContext->OMSetDepthStencilState(m_depthStencilState, 1);

	// 创建深度缓存视图
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	// 创建深度模板视图
	if(FAILED(m_device->CreateDepthStencilView(m_depthStencilBuffer, &depthStencilViewDesc, &m_depthStencilView)))
		return false;

	// 将深度缓存视图与渲染目标视图绑定到绘图管线
	m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);

	// 设定光栅化信息
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

	// 创建光栅化
	if(FAILED(m_device->CreateRasterizerState(&rasterDesc, &m_rasterState)))
		return false;

	m_deviceContext->RSSetState(m_rasterState);

	// 设定视窗口
	viewport.Width = (float)screenWidth;
	viewport.Height = (float)screenHeight;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;

	// 创建视窗口
	m_deviceContext->RSSetViewports(1, &viewport);

	// 创建投影矩阵（将3D场景转化为2D画面）
	// 设定投影矩阵
	fieldOfView = (float)D3DX_PI / 4.0f;
	screenAspect = (float)screenWidth / (float)screenHeight;

	// 为3D渲染创建投影矩阵
	D3DXMatrixPerspectiveFovLH(&m_projectionMatrix, fieldOfView, screenAspect,
		screenNear, screenDepth);
	
	// 创建世界矩阵（将物体的坐标转换到渲染的3D场景中的坐标）
	// 将世界矩阵初始化为单位阵
	D3DXMatrixIdentity(&m_worldMatrix);

	// 创建观察矩阵
	// NULL //

	// 为2D渲染创建正交矩阵
	D3DXMatrixOrthoLH(&m_orthoMatrix, (float)screenWidth, (float)screenHeight, screenNear, screenDepth);

	// 设置2D模板
	// 分配内存
	ZeroMemory(&depthDisabledStencilDesc, sizeof(depthDisabledStencilDesc));

	// 设定深度状态
	depthDisabledStencilDesc.DepthEnable = false;
	depthDisabledStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthDisabledStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthDisabledStencilDesc.StencilEnable = true;
	depthDisabledStencilDesc.StencilReadMask = 0xFF;
	depthDisabledStencilDesc.StencilWriteMask = 0xFF;

	// 像素在前面
	depthDisabledStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthDisabledStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// 像素在后面
	depthDisabledStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthDisabledStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// 创建深度模板
	if(FAILED(m_device->CreateDepthStencilState(&depthDisabledStencilDesc, &m_depthDisabledStencilState)))
		return false;
	
	// 初始化混合状态
	ZeroMemory(&blendStateDesc, sizeof(D3D11_BLEND_DESC));

	// 创建透明度混合信息
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

	// 关闭透明度混合
	blendStateDesc.RenderTarget[0].BlendEnable = FALSE;
	if(FAILED(m_device->CreateBlendState(&blendStateDesc, &m_alphaDisableBlendingState)))
		return false;

	// 获取显卡信息并输出到文本文件中
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
	// 在释放资源之前，先设定为窗口模式，否则释放交换链时会出现问题
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

	// 用color清除backbuffer
	m_deviceContext->ClearRenderTargetView(m_renderTargetView, color);
	
	// 清除深度缓存
	m_deviceContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	return;
}

void D3DClass::EndScene()
{
	// 将绘制好的backbuffer呈现出来
	if (m_vsync_enabled)
	{
		// 锁定为屏幕刷新率
		m_swapChain->Present(1, 0);
	}
	else
	{
		// 呈现速度无限制
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