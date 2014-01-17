
///////SHADER_CLASS_H_/////////
#ifndef _SHADERCLASS_H_
#define _SHADERCLASS_H_

//////////////
// INCLUDES //
//////////////
#include <tchar.h>
#include <atlconv.h>
#include <d3d11.h>
#include <d3dx10math.h>
#include <d3dx11async.h>
#include "fileIO.h"

// 类名：ShaderClass
// 其他shader的基类

class ShaderClass
{
public:
	ShaderClass();
	ShaderClass(const ShaderClass&);
	virtual ~ShaderClass() {};

protected:
	virtual bool InitializeShader(ID3D11Device*, HWND, WCHAR*, WCHAR*, char*, char*);
	virtual void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR*);

	virtual void RenderShader(ID3D11DeviceContext*, int);
	virtual bool SetShaderParameters(ID3D11DeviceContext*, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, ID3D11ShaderResourceView**, UINT);

protected:
	struct MatrixBufferType
	{
		D3DXMATRIX world;
		D3DXMATRIX view;
		D3DXMATRIX projection;
	};

	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11InputLayout* m_layout;
	ID3D11Buffer* m_matrixBuffer;
	ID3D11SamplerState* m_sampleState;
};

#endif