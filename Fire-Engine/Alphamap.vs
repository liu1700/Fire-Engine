
//////////////////////////////////////////////////////////
//�ļ���: Multitexture.vs		����: ������:2014/1/16
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
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
};

//////////////////////////////////////
// Vertex Shader
//////////////////////////////////////
PixelInputType AlphaMapVertexShader(VertexInputType input)
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
    
    return output;
}