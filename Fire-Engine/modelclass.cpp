
//////////////////////////////////////////////////////////
//�ļ���: modelclass.cpp		����: ������:2014/1/10
//////////////////////////////////////////////////////////
#include "modelclass.h"

ModelClass::ModelClass()
{
	m_inModel = NULL;
	m_ModelInfo = NULL;
}

ModelClass::ModelClass(const ModelClass& other)
{

}

ModelClass::~ModelClass()
{

}

bool ModelClass::Initialze(ID3D11Device* device, WCHAR* modelFilename, WCHAR* textureFilename1, WCHAR* textureFilename2)
{
	// ����ģ������
	if((m_ModelInfo = m_inModel->LoadModelFromFile(modelFilename, m_vertexCount, m_indexCount)) == NULL)
		return false;
	
	// ��ʼ����������������
	if(!InitialzeBuffers(device))
		return false;

	// Ϊģ�ͼ�������
	if(!LoadTexture(device, textureFilename1, textureFilename2))
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

bool ModelClass::InitialzeBuffers(ID3D11Device* device)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;

	//  ��������ֱ��趨������������������

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

bool ModelClass::LoadTexture(ID3D11Device* device, WCHAR* filename1, WCHAR* filename2)
{
	m_TextureArray = new TextureArrayClass;
	if(!m_TextureArray)
		return false;

	if(!m_TextureArray->Initialze(device, filename1, filename2))
		return false;

	return true;
}