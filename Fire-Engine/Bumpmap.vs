
//////////////////////////////////////////////////////////
//�ļ���: Bumpmap.vs		����: ������:2014/1/18
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
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 binormal : BINORMAL;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 binormal : BINORMAL;
};

//////////////////////////////////////
// Vertex Shader
//////////////////////////////////////
PixelInputType BumpMapVertexShader(VertexInputType input)
{
	PixelInputType output;
    

    // �趨Ϊ������귽�����
    input.position.w = 1.0f;

    // ������������������������ֵ�λ��
    output.position = mul(input.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    
    // ���������걣��
    output.tex = input.tex;
    
    // ���㷨������
    output.normal = mul(input.normal,(float3x3)worldMatrix);
    output.normal = normalize(output.normal);
    
    // ����������
    output.tangent = mul(input.tangent,(float3x3)worldMatrix);
    output.tangent = normalize(output.tangent);
    
    // ���㸱��������
    output.binormal = mul(input.binormal,(float3x3)worldMatrix);
    output.binormal = normalize(output.binormal);
    
    return output;
}