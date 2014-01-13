
//////////////////////////////////////////////////////////
//�ļ���: Light.vs		����: ������:2014/1/11
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

cbuffer CameraBuffer
{
	float3 cameraPosition;
	float padding;
};

////////
// TYPEDEFS //
////////
struct VertexInputType
{
	float4 position : POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float3 viewDirection : TEXCOORD1;
};

//////////////////////////////////////
// Vertex Shader
//////////////////////////////////////
PixelInputType LightVertexShader(VertexInputType input)
{
	PixelInputType output;
	float4 worldPosition;
	
	// ���������w��Ϊ1����ʾһ����ά����
	input.position.w = 1.0f;
	
	// �����λ�ó���world��view��projection�������ó��ĵ��λ��Ϊ2D��Ļ��Ⱦ��λ��
	output.position = mul(input.position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);
	
	// �洢��ɫΪpixel shader׼��
	output.tex = input.tex;
	
	// ��������������ȫ�ֹ���
	output.normal = mul(input.normal, (float3x3)worldMatrix);
	
	// ��normal�������
	output.normal = normalize(output.normal);
	
	// ���㶥�������������е�λ��
	worldPosition = mul(input.position, worldMatrix);
	
	// ����cameraλ�������������������۲췽��
	output.viewDirection = cameraPosition.xyz - worldPosition.xyz;
	
	// ���۲췽����
	output.viewDirection = normalize(output.viewDirection);
	
	return output;
}
