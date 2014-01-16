
//////////////////////////////////////////////////////////
//文件名: bitmapclass.cpp		日期: 创建于:2014/1/12
//////////////////////////////////////////////////////////
#include "bitmapclass.h"

BitmapClass::BitmapClass()
{
	m_vertexBuffer = NULL;
	m_indexBuffer = NULL;
	m_TextureArray = NULL;
}

BitmapClass::BitmapClass(const BitmapClass& other)
{

}

BitmapClass::~BitmapClass()
{

}

bool BitmapClass::Initialze(ID3D11Device* device, int screenWidth, int screenHeight, WCHAR* textureFilename, int bitmapWidth, int bitmapHeight)
{
	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;

	m_bitmapWidth = bitmapWidth;
	m_bitmapHeight = bitmapHeight;

	m_previousPosX = -1;
	m_previousPosY = -1;

	if(!InitialzeBuffers(device))
		return false;

	if(!LoadTexture(device, textureFilename))
		return false;

	return true;
}

void BitmapClass::ShutDown()
{
	ReleaseTexture();
	ShutdownBuffers();

	return;
}

bool BitmapClass::Render(ID3D11DeviceContext* deviceContext, int positionX, int positionY)
{
	if(!UpdateBuffer(deviceContext, positionX, positionY))
		return false;

	// 将顶点与索引缓存置于绘图管线中
	RenderBuffers(deviceContext);

	return true;
}

int BitmapClass::GetIndexCount()
{
	// 返回索引个数，提供给shader
	return m_indexCount;
}

ID3D11ShaderResourceView** BitmapClass::GetTexture()
{
	return m_TextureArray->GetTextureArray();
}

bool BitmapClass::InitialzeBuffers(ID3D11Device* device)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;

	////  两个数组分别设定三个顶点与三个索引
	m_vertexCount = 6;
	m_indexCount = m_vertexCount;

	vertices = new VertexType[m_vertexCount];
	indices = new unsigned long[m_indexCount];

	if(!vertices || !indices)
		return false;

	memset(vertices, 0, (sizeof(VertexType) * m_vertexCount));
	for(int i = 0; i < m_indexCount; i++)
	{
		indices[i] = i;
	}


	// 设置静态顶点缓冲的信息
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
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

void BitmapClass::ShutdownBuffers()
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

	return;
}

bool BitmapClass::UpdateBuffer(ID3D11DeviceContext* deviceContext, int positionX, int positionY)
{
	float left, right, top, bottom;
	VertexType* vertices;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	VertexType* verticesPtr;

	// 判断图片是否移动
	if ((positionX == m_previousPosX)&&(positionY == m_previousPosY))
	{
		return true;
	}

	// 如果改变了则更新位置
	m_previousPosX = positionX;
	m_previousPosY = positionY;

	// 根据位置计算位图正常的显示位置
	left = (float)((m_screenWidth / 2) * -1) + (float)positionX;
	right = left + (float)m_bitmapWidth;
	top = (float)(m_screenHeight / 2) - (float)positionY;
	bottom = top - (float)m_bitmapHeight;

	// 创建顶点数组
	vertices = new VertexType[m_vertexCount];
	if(!vertices)
		return false;

	vertices[0].position = D3DXVECTOR3(right, top, 0.0f);
	vertices[0].texture = D3DXVECTOR2(1.0f, 0.0f);

	vertices[1].position = D3DXVECTOR3(right, bottom, 0.0f);
	vertices[1].texture = D3DXVECTOR2(1.0f, 1.0f);

	vertices[2].position = D3DXVECTOR3(left, bottom, 0.0f);
	vertices[2].texture = D3DXVECTOR2(0.0f, 1.0f);

	vertices[3].position = D3DXVECTOR3(left, bottom, 0.0f);
	vertices[3].texture = D3DXVECTOR2(0.0f, 1.0f);

	vertices[4].position = D3DXVECTOR3(left, top, 0.0f);
	vertices[4].texture = D3DXVECTOR2(0.0f, 0.0f);

	vertices[5].position = D3DXVECTOR3(right, top, 0.0f);
	vertices[5].texture = D3DXVECTOR2(1.0f, 0.0f);

	if(FAILED(deviceContext->Map(m_vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource)))
		return false;

	verticesPtr = (VertexType*)mappedResource.pData;

	// 将数据拷贝到顶点缓存中
	memcpy(verticesPtr, (void*)vertices, (sizeof(VertexType) * m_vertexCount));

	deviceContext->Unmap(m_vertexBuffer, 0);

	delete []vertices;
	vertices = NULL;

	return true;
}

void BitmapClass::RenderBuffers(ID3D11DeviceContext* deviceContext)
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

bool BitmapClass::LoadTexture(ID3D11Device* device, WCHAR* filename)
{
	m_TextureArray = new TextureClass;
	if(!m_TextureArray)
		return false;

	if(!m_TextureArray->Initialze(device, filename))
		return false;

	return true;
}

void BitmapClass::ReleaseTexture()
{
	if (m_TextureArray)
	{
		m_TextureArray->ShutDown();
		delete m_TextureArray;
		m_TextureArray = NULL;
	}
	return;
}