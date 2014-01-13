
//////////////////////////////////////////////////////////
//�ļ���: modelclass.cpp		����: ������:2014/1/10
//////////////////////////////////////////////////////////
#include "modelclass.h"

ModelClass::ModelClass()
{
	m_vertexBuffer = NULL;
	m_indexBuffer = NULL;
	m_Texture = NULL;
	m_inModel = NULL;
	m_ModelInfo = NULL;
}

ModelClass::ModelClass(const ModelClass& other)
{

}

ModelClass::~ModelClass()
{

}

bool ModelClass::Initialze(ID3D11Device* device, WCHAR* textureFilename, WCHAR* modelFilename)
{
	// ����ģ������
	if((m_ModelInfo = m_inModel->LoadModelFromFile(modelFilename, m_vertexCount, m_indexCount)) == NULL)
		return false;
	
	// ��ʼ����������������
	if(!InitialzeBuffers(device))
		return false;

	// Ϊģ�ͼ�������
	if(!LoadTexture(device, textureFilename))
		return false;

	return true;
}

void ModelClass::ShutDown()
{
	ReleaseTexture();

	ShutdownBuffers();

	return;
}

void ModelClass::Render(ID3D11DeviceContext* deviceContext)
{
	// �������������������ڻ�ͼ������
	RenderBuffers(deviceContext);

	return;
}

int ModelClass::GetIndexCount()
{
	// ���������������ṩ��shader
	return m_indexCount;
}

ID3D11ShaderResourceView* ModelClass::GetTexture()
{
	return m_Texture->GetTexture();
}

bool ModelClass::InitialzeBuffers(ID3D11Device* device)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;

	////  ��������ֱ��趨������������������
	//m_vertexCount = 6;
	//m_indexCount = 6;

	vertices = new VertexType[m_vertexCount];
	indices = new unsigned long[m_indexCount];

	if(!vertices || !indices)
		return false;

	// ˳ʱ�����㣬�ᱻ��Ϊ�����棬��ʱ�뱻��Ϊ���棬�����˳�����Ҫ

	for (int i = 0; i < m_vertexCount; i++)
	{
		vertices[i].position = D3DXVECTOR3(m_ModelInfo[i][0], m_ModelInfo[i][1], m_ModelInfo[i][2]);
		vertices[i].texture = D3DXVECTOR2(m_ModelInfo[i][3], m_ModelInfo[i][4]);
		vertices[i].normal = D3DXVECTOR3(m_ModelInfo[i][5], m_ModelInfo[i][6], m_ModelInfo[i][7]);

		indices[i] = i;
	}

	//vertices[0].position = D3DXVECTOR3(-1.0f, -1.0f, 0.0f);
	//vertices[0].texture = D3DXVECTOR2(0.0f, 1.0f);
	//vertices[0].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

	//vertices[1].position = D3DXVECTOR3(-1.0f, 1.0f, 0.0f);
	//vertices[1].texture = D3DXVECTOR2(0.0f, 0.0f);
	//vertices[1].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

	//vertices[2].position = D3DXVECTOR3(1.0f, 1.0f, 0.0f);
	//vertices[2].texture = D3DXVECTOR2(1.0f, 0.0f);
	//vertices[2].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

	//vertices[3].position = D3DXVECTOR3(1.0f, 1.0f, 0.0f);
	//vertices[3].texture = D3DXVECTOR2(1.0f, 0.0f);
	//vertices[3].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

	//vertices[4].position = D3DXVECTOR3(1.0f, -1.0f, 0.0f);
	//vertices[4].texture = D3DXVECTOR2(1.0f, 1.0f);
	//vertices[4].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

	//vertices[5].position = D3DXVECTOR3(-1.0f, -1.0f, 0.0f);
	//vertices[5].texture = D3DXVECTOR2(0.0f, 1.0f);
	//vertices[5].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

	//// ����
	//indices[0] = 0;
	//indices[1] = 1;
	//indices[2] = 2;
	//indices[3] = 3;
	//indices[4] = 4;
	//indices[5] = 5;

	// ���þ�̬���㻺�����Ϣ
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Ϊ�����������ָ��
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// �������㻺��
	if(FAILED(device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer)))
		return false;

	// ���þ�̬�����������Ϣ
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned int) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Ϊ�����������ָ��
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// ������������
	if(FAILED(device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer)))
		return false;

	// ɾ��vertexbuffer��indexbuffer����Ϊ�����Ѿ�ȡ����������buffer����Ϣ
	delete []vertices;
	vertices = NULL;

	delete []indices;
	indices = NULL;

	return true;
}

void ModelClass::ShutdownBuffers()
{
	if (m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = NULL;
	}
	if (m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = NULL;
	}
	if (m_ModelInfo)
	{
		for(int i = 0; i < m_vertexCount; i++)
		{
			delete []m_ModelInfo[i];
		}
		delete []m_ModelInfo;
		m_ModelInfo = NULL;
	}

	return;
}

void ModelClass::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride;
	unsigned int offset;

	// �趨���㻺��Ĳ���ֵ��ƫ����
	stride = sizeof(VertexType);
	offset = 0;

	// ����㻺��
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	// ������������
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// �趨���ֶ��㻺�������Ƶ�ͼԪ
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}

bool ModelClass::LoadTexture(ID3D11Device* device, WCHAR* filename)
{
	m_Texture = new TextureClass;
	if(!m_Texture)
		return false;

	if(!m_Texture->Initialze(device, filename))
		return false;

	return true;
}

void ModelClass::ReleaseTexture()
{
	if (m_Texture)
	{
		m_Texture->ShutDown();
		delete m_Texture;
		m_Texture = NULL;
	}
	return;
}