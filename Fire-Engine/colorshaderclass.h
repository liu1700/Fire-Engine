
//////////////////////////////////////////////////////////
//文件名: colorshaderclass.h		日期: 创建于:2014/1/10
//////////////////////////////////////////////////////////

#ifndef _COLORSHADERCLASS_H_
#define _COLORSHADERCLASS_H_

/////////
//INCLUDES //
/////////
#include <D3D11.h>
#include <D3DX10math.h>
#include <D3DX11async.h>

////////////////////////////////////
//类名: ColorShaderClass
////////////////////////////////////
class ColorShaderClass
{
public:
	ColorShaderClass();
	ColorShaderClass(const ColorShaderClass&);
	~ColorShaderClass();

	bool Initialze(ID3D11Device*, HWND);
	void ShutDown();
	bool Render(ID3D11DeviceContext*, int, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX);

private:
	struct MatrixBufferType
	{
		D3DXMATRIX world;
		D3DXMATRIX view;
		D3DXMATRIX projection;
	};

private:
	bool InitialzeShader(ID3D11Device*, HWND, WCHAR*, WCHAR*);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR*);

	bool SetShaderParameters(ID3D11DeviceContext*, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX);
	void RenderShader(ID3D11DeviceContext*, int);

private:
	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11InputLayout* m_layout;
	ID3D11Buffer* m_matrixBuffer;
};

#endif