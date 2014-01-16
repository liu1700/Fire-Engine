
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
	// ���������������ṩ��shader
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

	////  ��������ֱ��趨������������������
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


	// ���þ�̬���㻺�����Ϣ
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
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