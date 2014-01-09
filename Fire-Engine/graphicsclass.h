
//////////////////////////////////////////////////////////
//文件名: graphicsclass.h		日期: 创建于:2014/1/8
//////////////////////////////////////////////////////////
#ifndef _GRAPHICSCLASS_H_
#define _GRAPHICSCLASS_H_

//////////////
//INCLUDES
//////////////

//#include <Windows.h>
#include "d3dclass.h"

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
	void Shutdown();
	bool Frame();

private:
	bool Render();

private:
	D3DClass* m_D3D;

};


#endif