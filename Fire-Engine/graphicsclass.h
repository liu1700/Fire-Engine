
//////////////////////////////////////////////////////////
//文件名: graphicsclass.h		日期: 创建于:2014/1/8
//////////////////////////////////////////////////////////
#ifndef _GRAPHICSCLASS_H_
#define _GRAPHICSCLASS_H_

//////////////
//FE CLASS INCLUDES //
//////////////
#include "d3dclass.h"
#include "cameraclass.h"
#include "modelclass.h"
#include "lightshaderclass.h"
#include "lightclass.h"
#include "bitmapclass.h"
#include "textureshaderclass.h"
#include "textclass.h"

//////////////
//GLOBALS
//////////////
const bool FULL_SCREEN		= false;
const bool VSYNC_ENABLED	= true;
const float SCREEN_DEPTH	= 1000.0f;
const float SCREEN_NEAR		= 0.1f;

////////////////////////////////////
//类名: GraphicsClass
////////////////////////////////////
class GraphicsClass
{
public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass&);
	~GraphicsClass();

	bool Initialze(int, int, HWND);
	void ShutDown();
	bool Frame(int, int, int, int, float);

	bool Render(float, int, int, int, int, float);

private:
	D3DClass* m_D3D;
	CameraClass* m_Camera;
	ModelClass* m_Model;
	LightShaderClass* m_LightShader;
	TextureShaderClass* m_TextureShader;
	LightClass* m_Light;
	BitmapClass* m_Bitmap;
	TextClass* m_Text;

};


#endif