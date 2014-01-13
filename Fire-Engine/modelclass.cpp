
//////////////////////////////////////////////////////////
//文件名: modelclass.cpp		日期: 创建于:2014/1/10
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
	// 加载模型数据
	if((m_ModelInfo = m_inModel->LoadModelFromFile(modelFilename, m_vertexCount, m_indexCount)) == NULL)
		return false;
	
	// 初始化顶点与索引缓存
	if(!InitialzeBuffers(device))
		return false;

	// 为模型加载纹理
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
	// 将顶点与索引缓存置于绘图管线中
	RenderBuffers(deviceContext);

	return;
}

int ModelClass::GetIndexCount()
{
	// 返回索引个数，提供给shader
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

	////  两个数组分别设定三个顶点与三个索引
	//m_vertexCount = 6;
	//m_indexCount = 6;

	vertices = new VertexType[m_vertexCount];
	indices = new unsigned long[m_indexCount];

	if(!vertices || !indices)
		return false;

	// 顺时针填充点，会被认为是正面，逆时针被认为反面，填充点的顺序很重要

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

	//// 索引
	//indices[0] = 0;
	//indices[1] = 1;
	//indices[2] = 2;
	//indices[3] = 3;
	//indices[4] = 4;
	//indices[5] = 5;

	// 设置静态顶点缓冲的信息
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// 为顶点数据添加指针
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// 创建顶点缓存
	if(FAILED(device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer)))
		return false;

	// 设置静态索引缓冲的信息
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned int) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// 为索引数据添加指针
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// 创建索引缓冲
	if(FAILED(device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer)))
		return false;

	// 删掉vertexbuffer与indexbuffer，因为我们已经取得了这两个buffer的信息
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

	// 设定顶点缓冲的步进值与偏移量
	stride = sizeof(VertexType);
	offset = 0;

	// 激活顶点缓存
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	// 激活索引缓存
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// 设定这种顶点缓存所绘制的图元
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