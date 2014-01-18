
//////////////////////////////////////////////////////////
//文件名: Bumpmap.vs		日期: 创建于:2014/1/18
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
    

    // 设定为齐次坐标方便计算
    input.position.w = 1.0f;

    // 根据三个矩阵算出顶点所呈现的位置
    output.position = mul(input.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    
    // 将纹理坐标保存
    output.tex = input.tex;
    
    // 计算法相向量
    output.normal = mul(input.normal,(float3x3)worldMatrix);
    output.normal = normalize(output.normal);
    
    // 计算切向量
    output.tangent = mul(input.tangent,(float3x3)worldMatrix);
    output.tangent = normalize(output.tangent);
    
    // 计算副法线向量
    output.binormal = mul(input.binormal,(float3x3)worldMatrix);
    output.binormal = normalize(output.binormal);
    
    return output;
}