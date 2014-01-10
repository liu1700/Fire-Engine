
//////////////////////////////////////////////////////////
//�ļ���: color.vs		����: ������:2014/1/10
//////////////////////////////////////////////////////////

////////
// GLOBALS //
////////
cbuffer MatrixBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

////////
// TYPEDEFS //
////////
struct VertexInputType
{
	float4 position : POSITION;
	float4 color : COLOR;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
};

//////////////////////////////////////
// Vertex Shader
//////////////////////////////////////
PixelInputType ColorVertexShader(VertexInputType input)
{
	PixelInputType output;
	
	// ���������w��Ϊ1����ʾһ����ά����
	input.position.w = 1.0f;
	
	// �����λ�ó���world��view��projection�������ó��ĵ��λ��Ϊ2D��Ļ��Ⱦ��λ��
	output.position = mul(input.position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);
	
	// �洢��ɫΪpixel shader׼��
	output.color = input.color;
	
	return output;
}
