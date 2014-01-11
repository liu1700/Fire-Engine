
//////////////////////////////////////////////////////////
//文件名: fileIO.cpp		日期: 创建于:2014/1/10
//////////////////////////////////////////////////////////
#include "fileIO.h"

FileIOClass::FileIOClass()
{

}

FileIOClass::FileIOClass(const FileIOClass& other)
{

}

FileIOClass::~FileIOClass()
{

}

void FileIOClass::WriteTextFile(const wchar_t* filepath, const char* texts)
{
	m_textFile.open(filepath, ios::app);
	if(m_textFile.fail())
		return;

	m_textFile<<texts<<"\n"<<endl;
	m_textFile.close();
}

void FileIOClass::WriteTextFile(const wchar_t* filepath, int texts)
{
	m_textFile.open(filepath, ios::app);
	if(m_textFile.fail())
		return;

	m_textFile<<texts<<"\n"<<endl;
	m_textFile.close();
}

float** FileIOClass::LoadModelFromFile(const wchar_t* filepath, int& vertexcount, int& indexcount)
{
	ifstream inModelFile;
	float** modelInfo = NULL;
	char input;
	
	inModelFile.open(filepath);
	if(inModelFile.fail())
		return NULL;

	inModelFile.get(input);
	while(input != ':')
	{
		inModelFile.get(input);
	}

	// 读入顶点数目
	inModelFile>>vertexcount;
	indexcount = vertexcount;

	// 准备读入模型信息,申请内存
	modelInfo  = new float*[vertexcount];
	for (int i = 0; i < vertexcount; i++)
	{
		modelInfo[i] = new float[8];
	}

	if(!modelInfo)
		return NULL;

	// 开始读到文件头
	inModelFile.get(input);
	while(input != ':')
	{
		inModelFile.get(input);
	}
	inModelFile.get(input);
	inModelFile.get(input);

	for (int i = 0; i < vertexcount; i++)
	{
		// 读入信息顺序为 x,y,z,tu,tv,nx,ny,nz
		inModelFile>>modelInfo[i][0]>>modelInfo[i][1]>>modelInfo[i][2];
		inModelFile>>modelInfo[i][3]>>modelInfo[i][4];
		inModelFile>>modelInfo[i][5]>>modelInfo[i][6]>>modelInfo[i][7];
	}

	inModelFile.close();

	return modelInfo;

}