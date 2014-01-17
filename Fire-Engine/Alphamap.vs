
//////////////////////////////////////////////////////////
//文件名: Multitexture.vs		日期: 创建于:2014/1/16
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
    

    // 设定为齐次坐标方便计算
    input.position.w = 1.0f;

    // 根据三个矩阵算出顶点所呈现的位置
    output.position = mul(input.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    
    // 将纹理坐标保存
    output.tex = input.tex;
    
    return output;
}