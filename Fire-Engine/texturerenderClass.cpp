
TectureRenderClass::TectureRenderClass()
{
	m_vertexBuffer = NULL;
	m_indexBuffer = NULL;
	m_TextureArray = NULL;
}

TectureRenderClass::TectureRenderClass(const TectureRenderClass& other)
{

}

TectureRenderClass::~TectureRenderClass()
{

}

int TectureRenderClass::GetIndexCount()
{
	// 返回索引个数，提供给shader
	return m_indexCount;
}

ID3D11ShaderResourceView** TectureRenderClass::GetTexture()
{
	return m_TextureArray->GetTextureArray();
}

bool TectureRenderClass::InitialzeBuffers(ID3D11Device* device)
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

void TectureRenderClass::ShutdownBuffers()
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

void TectureRenderClass::RenderBuffers(ID3D11DeviceContext* deviceContext)
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

void TectureRenderClass::ReleaseTexture()
{
	if (m_TextureArray)
	{
		m_TextureArray->ShutDown();
		delete m_TextureArray;
		m_TextureArray = NULL;
	}
	return;
}