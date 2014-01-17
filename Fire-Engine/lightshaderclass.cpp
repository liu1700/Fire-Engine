
//////////////////////////////////////////////////////////
//文件名: lightshaderclass.cpp		日期: 创建于:2014/1/11 //
//////////////////////////////////////////////////////////
#include "lightshaderclass.h"

LightShaderClass::LightShaderClass()
{
	m_lightBuffer = NULL;
	m_cameraBuffer = NULL;
}

LightShaderClass::LightShaderClass(const LightShaderClass& other)
{

}

LightShaderClass::~LightShaderClass()
{
	
}

bool LightShaderClass::Initialze(ID3D11Device* device, HWND hwnd)
{
	// 初始化shader
	if(!InitialzeShader(device, hwnd, L"../Fire-Engine/light.vs", L"../Fire-Engine/light.ps", "LightVertexShader", "LightPixelShader"))
		return false;

	return true;
}

void LightShaderClass::ShutDown()
{
	ShutdownShader();

	return;
}

bool LightShaderClass::Render(ID3D11DeviceContext* deviceContext, int indexCount, 
							  D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix,
							  ID3D11ShaderResourceView** textureArray, 
							  D3DXVECTOR3 lightDirection, D3DXVECTOR4 ambientColor,D3DXVECTOR4 diffuseColor,
							  D3DXVECTOR3 cameraPosition, D3DXVECTOR4 specularColor, float specularPower)
{
	if(!SetShaderParameters(deviceContext, worldMatrix, viewMatrix, projectionMatrix, textureArray, lightDirection, ambientColor, diffuseColor,
						cameraPosition, specularColor, specularPower))
		return false;
	
	// 开始渲染
	RenderShader(deviceContext, indexCount);

	return true;
}

bool LightShaderClass::InitialzeShader(ID3D11Device* device, HWND hwnd, WCHAR* vsFilename, WCHAR* psFilename, char* vsFuncname, char* psFuncname)
{ 
	// 这里边的DESC全部用来设定接口的描述信息
	ID3D10Blob* errorMessage;
	ID3D10Blob* vertexShaderBuffer;
	ID3D10Blob* pixelShaderBuffer;
	D3D11_INPUT_ELEMENT_DESC polygonLayout[3];
	unsigned int numElements;
	D3D11_BUFFER_DESC matrixBufferDesc;
	D3D11_BUFFER_DESC lightBufferDesc;
	D3D11_BUFFER_DESC cameraBufferDesc;
	D3D11_SAMPLER_DESC samplerDesc;

	// 初始化指针为null
	errorMessage = NULL;
	vertexShaderBuffer = NULL;
	pixelShaderBuffer = NULL;

	// 编译Shader
	if (FAILED(D3DX11CompileFromFile(vsFilename, NULL, NULL, vsFuncname, "vs_5_0",
		D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, &vertexShaderBuffer, &errorMessage, NULL)))
	{
		if (errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, hwnd, vsFilename);
		}
		else
		{
			MessageBox(hwnd, vsFilename, L"未找到Shader文件", MB_OK);
		}

		return false;
	}

	if (FAILED(D3DX11CompileFromFile(psFilename, NULL, NULL, psFuncname, "ps_5_0",
		D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, &pixelShaderBuffer, &errorMessage, NULL)))
	{
		if (errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, hwnd, psFilename);
		}
		else
		{
			MessageBox(hwnd, psFilename, L"未找到Shader文件", MB_OK);
		}

		return false;
	}

	// 从缓存中创建shader
	if(FAILED(device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &m_vertexShader)))
		return false;

	if(FAILED(device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &m_pixelShader)))
		return false;

	// 设定传到shader文件的数据的格式
	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "TEXCOORD";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	polygonLayout[2].SemanticName = "NORMAL";
	polygonLayout[2].SemanticIndex = 0;
	polygonLayout[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[2].InputSlot = 0;
	polygonLayout[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[2].InstanceDataStepRate = 0;

	// 得到布局中元素的数量
	numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	// 创建布局
	if(FAILED(device->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(),
		vertexShaderBuffer->GetBufferSize(), &m_layout)))
		return false;

	vertexShaderBuffer->Release();
	vertexShaderBuffer = NULL;

	pixelShaderBuffer->Release();
	pixelShaderBuffer = NULL;

	// 设定动态的矩阵缓冲
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	if(FAILED(device->CreateBuffer(&matrixBufferDesc, NULL, &m_matrixBuffer)))
		return false;

	// 设定纹理抽象描述
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// 创建纹理抽取状态
	if(FAILED(device->CreateSamplerState(&samplerDesc, &m_sampleState)))
		return false;

	// 设定camera的动态缓存信息
	cameraBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	cameraBufferDesc.ByteWidth = sizeof(CameraBufferType);
	cameraBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cameraBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cameraBufferDesc.MiscFlags = 0;
	cameraBufferDesc.StructureByteStride = 0;

	// 创建指向camera的指针
	if(FAILED(device->CreateBuffer(&cameraBufferDesc, NULL, &m_cameraBuffer)))
		return false;

	// 设置动态光照的描述信息
	lightBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	lightBufferDesc.ByteWidth = sizeof(LightBufferType);
	lightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	lightBufferDesc.MiscFlags = 0;
	lightBufferDesc.StructureByteStride = 0;

	// 创建光照缓存
	if(FAILED(device->CreateBuffer(&lightBufferDesc, NULL, &m_lightBuffer)))
		return false;

	return true;

}

void LightShaderClass::ShutdownShader()
{
	if (m_matrixBuffer)
	{
		m_matrixBuffer->Release();
		m_matrixBuffer = NULL;
	}
	if (m_layout)
	{
		m_layout->Release();
		m_layout = NULL;
	}
	if (m_pixelShader)
	{
		m_pixelShader->Release();
		m_pixelShader = NULL;
	}
	if (m_vertexShader)
	{
		m_vertexShader->Release();
		m_vertexShader = NULL;
	}
	if (m_sampleState)
	{
		m_sampleState->Release();
		m_sampleState = NULL;
	}
	if (m_lightBuffer)
	{
		m_lightBuffer->Release();
		m_lightBuffer = NULL;
	}
	if (m_cameraBuffer)
	{
		m_cameraBuffer->Release();
		m_cameraBuffer = NULL;
	}

	return;
}

bool LightShaderClass::SetShaderParameters(ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix, 
										   D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix,
										   ID3D11ShaderResourceView** textureArray, D3DXVECTOR3 lightDirection, D3DXVECTOR4 ambientColor, D3DXVECTOR4 diffuseColor,
										   D3DXVECTOR3 cameraPosition, D3DXVECTOR4 specularColor, float specularPower)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType* dataPtr;
	LightBufferType* dataPtr2;
	CameraBufferType* dataPtr3;
	unsigned int bufferNumber;

	// 将矩阵转置为shader做准备
	D3DXMatrixTranspose(&worldMatrix, &worldMatrix);
	D3DXMatrixTranspose(&viewMatrix, &viewMatrix);
	D3DXMatrixTranspose(&projectionMatrix, &projectionMatrix);

	// 锁定矩阵缓冲区，使其可写
	if(FAILED(deviceContext->Map(m_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0,
		&mappedResource)))
		return false;

	// 获取矩阵缓冲的指针
	dataPtr = (MatrixBufferType*)mappedResource.pData;

	// 将转置后的矩阵写入data
	dataPtr->world = worldMatrix;
	dataPtr->view = viewMatrix;
	dataPtr->projection = projectionMatrix;

	// 解锁
	deviceContext->Unmap(m_matrixBuffer, 0);

	bufferNumber = 0;
	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_matrixBuffer);

	// 锁定camera缓冲区，使其可写
	if(FAILED(deviceContext->Map(m_cameraBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, 
		&mappedResource)))
		return false;

	// 获取camera指针
	dataPtr3 = (CameraBufferType*)mappedResource.pData;

	// copy指针
	dataPtr3->cameraPosition = cameraPosition;
	dataPtr3->padding = 0.0f;

	// 解锁
	deviceContext->Unmap(m_cameraBuffer, 0);

	// 因为前边的matrix buffernumber为0，所以这里为1
	bufferNumber = 1;
	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_cameraBuffer);

	// 设定纹理资源
	deviceContext->PSSetShaderResources(0, 1, textureArray);

	// 锁定光照缓冲区，使其可写
	if(FAILED(deviceContext->Map(m_lightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, 
		&mappedResource)))
		return false;

	// 获取矩阵缓冲的指针
	dataPtr2 = (LightBufferType*)mappedResource.pData;

	// 将转置后的矩阵写入data
	dataPtr2->ambientColor = ambientColor;
	dataPtr2->diffuseColor = diffuseColor;
	dataPtr2->lightDirection = lightDirection;
	dataPtr2->specularColor = specularColor;
	dataPtr2->specularPower = specularPower;

	// 解锁
	deviceContext->Unmap(m_lightBuffer, 0);

	bufferNumber = 0;
	deviceContext->PSSetConstantBuffers(bufferNumber, 1, &m_lightBuffer);

	return true;
}