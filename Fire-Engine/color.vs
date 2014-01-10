
//////////////////////////////////////////////////////////
//文件名: color.vs		日期: 创建于:2014/1/10
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
	
	// 将齐次坐标w设为1，表示一个三维坐标
	input.position.w = 1.0f;
	
	// 将点的位置乘以world，view，projection矩阵最后得出的点的位置为2D屏幕渲染的位置
	output.position = mul(input.position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);
	
	// 存储颜色为pixel shader准备
	output.color = input.color;
	
	return output;
}
