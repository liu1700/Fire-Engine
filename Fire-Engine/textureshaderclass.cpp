
//////////////////////////////////////////////////////////
//�ļ���: textureshaderclass.cpp		����: ������:2014/1/12
//////////////////////////////////////////////////////////
#include "textureshaderclass.h"

TextureShaderClass::TextureShaderClass()
{
	m_vertexShader = NULL;
	m_pixelShader = NULL;
	m_layout = NULL;
	m_matrixBuffer = NULL;

	m_sampleState = NULL;
}


TextureShaderClass::TextureShaderClass(const TextureShaderClass& other)
{
}


TextureShaderClass::~TextureShaderClass()
{
}


bool TextureShaderClass::Initialize(ID3D11Device* device, HWND hwnd)
{
	if(!InitializeShader(device, hwnd, L"../Fire-Engine/Textures.vs", L"../Fire-Engine/Textures.ps"))
		return false;

	return true;
}

void TextureShaderClass::Shutdown()
{
	ShutdownShader();

	return;
}

bool TextureShaderClass::Render(ID3D11DeviceContext* deviceContext, int indexCount, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, 
								D3DXMATRIX projectionMatrix, ID3D11ShaderResourceView* texture)
{
	if(!SetShaderParameters(deviceContext, worldMatrix, viewMatrix, projectionMatrix, texture))
		return false;

	RenderShader(deviceContext, indexCount);

	return true;
}

bool TextureShaderClass::InitializeShader(ID3D11Device* device, HWND hwnd, WCHAR* vsFilename, WCHAR* psFilename)
{
	ID3D10Blob* errorMessage;
	ID3D10Blob* vertexShaderBuffer;
	ID3D10Blob* pixelShaderBuffer;
	D3D11_INPUT_ELEMENT_DESC polygonLayout[2];
	unsigned int numElements;
	D3D11_BUFFER_DESC matrixBufferDesc;

	D3D11_SAMPLER_DESC samplerDesc;

	// ��ʼ��ָ��Ϊnull
	errorMessage = NULL;
	vertexShaderBuffer = NULL;
	pixelShaderBuffer = NULL;

	// ����Shader
	if (FAILED(D3DX11CompileFromFile(vsFilename, NULL, NULL, "TextureVertexShader", "vs_5_0",
		D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, &vertexShaderBuffer, &errorMessage, NULL)))
	{
		if (errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, hwnd, vsFilename);
		}
		else
		{
			MessageBox(hwnd, vsFilename, L"δ�ҵ�Shader�ļ�", MB_OK);
		}

		return false;
	}

	if (FAILED(D3DX11CompileFromFile(psFilename, NULL, NULL, "TexturePixelShader", "ps_5_0",
		D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, &pixelShaderBuffer, &errorMessage, NULL)))
	{
		if (errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, hwnd, psFilename);
		}
		else
		{
			MessageBox(hwnd, psFilename, L"δ�ҵ�Shader�ļ�", MB_OK);
		}

		return false;
	}

	// �ӻ����д���shader
	if(FAILED(device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &m_vertexShader)))
		return false;

	if(FAILED(device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &m_pixelShader)))
		return false;

	// �趨����shader�ļ������ݵĸ�ʽ
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

	// �õ�������Ԫ�ص�����
	numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	// ��������
	if(FAILED(device->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(),
		vertexShaderBuffer->GetBufferSize(), &m_layout)))
		return false;

	vertexShaderBuffer->Release();
	vertexShaderBuffer = NULL;

	pixelShaderBuffer->Release();
	pixelShaderBuffer = NULL;

	// �趨��̬�ľ��󻺳�
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	if(FAILED(device->CreateBuffer(&matrixBufferDesc, NULL, &m_matrixBuffer)))
		return false;
	
	// �趨�����������
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

	// ���������ȡ״̬
	if(FAILED(device->CreateSamplerState(&samplerDesc, &m_sampleState)))
		return false;

	return true;
}

void TextureShaderClass::ShutdownShader()
{
	if(m_sampleState)
	{
		m_sampleState->Release();
		m_sampleState = NULL;
	}

	if(m_matrixBuffer)
	{
		m_matrixBuffer->Release();
		m_matrixBuffer = NULL;
	}

	if(m_layout)
	{
		m_layout->Release();
		m_layout = NULL;
	}

	if(m_pixelShader)
	{
		m_pixelShader->Release();
		m_pixelShader = NULL;
	}

	if(m_vertexShader)
	{
		m_vertexShader->Release();
		m_vertexShader = NULL;
	}

	return;
}

void TextureShaderClass::OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, WCHAR* shaderFilename)
{
	FileIOClass* compileErrors;

	compileErrors = new FileIOClass;
	if(!compileErrors)
		return;

	// ��ȡ������Ϣ���ı�ָ��,��д��txt
	compileErrors->WriteTextFile(L"shader-error.txt",(char*)errorMessage->GetBufferPointer());

	// �ͷ�errorMessage
	errorMessage->Release();
	errorMessage = NULL;

	MessageBox(hwnd, L"����Shader���ִ��󣬲鿴shader-error.txt��ȡ������Ϣ��", shaderFilename, MB_OK);

	return;
}

bool TextureShaderClass::SetShaderParameters(ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, 
											 D3DXMATRIX projectionMatrix, ID3D11ShaderResourceView* texture)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType* dataPtr;
	unsigned int bufferNumber;

	// ������ת��Ϊshader��׼��
	D3DXMatrixTranspose(&worldMatrix, &worldMatrix);
	D3DXMatrixTranspose(&viewMatrix, &viewMatrix);
	D3DXMatrixTranspose(&projectionMatrix, &projectionMatrix);

	// �������󻺳�����ʹ���д
	if(FAILED(deviceContext->Map(m_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0,
		&mappedResource)))
		return false;

	// ��ȡ���󻺳��ָ��
	dataPtr = (MatrixBufferType*)mappedResource.pData;

	// ��ת�ú�ľ���д��data
	dataPtr->world = worldMatrix;
	dataPtr->view = viewMatrix;
	dataPtr->projection = projectionMatrix;

	// ����
	deviceContext->Unmap(m_matrixBuffer, 0);

	bufferNumber = 0;
	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_matrixBuffer);

	// �趨������Դ
	deviceContext->PSSetShaderResources(0, 1, &texture);

	return true;
}

void TextureShaderClass::RenderShader(ID3D11DeviceContext* deviceContext, int indexCount)
{
	// �趨�������벼��
	deviceContext->IASetInputLayout(m_layout);

	// �趨vertex��pixel shader
	deviceContext->VSSetShader(m_vertexShader, NULL, 0);
	deviceContext->PSSetShader(m_pixelShader, NULL, 0);

	// ��pixel shader���趨����״̬
	deviceContext->PSSetSamplers(0, 1, &m_sampleState);

	// ��Ⱦͼ��
	deviceContext->DrawIndexed(indexCount, 0, 0);

	return;
}