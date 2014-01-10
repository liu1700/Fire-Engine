
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
	m_file.open(filepath, ios::app);
	m_file<<texts<<"\n"<<endl;
	m_file.close();
}

void FileIOClass::WriteTextFile(const wchar_t* filepath, int texts)
{
	m_file.open(filepath, ios::app);
	m_file<<texts<<"\n"<<endl;
	m_file.close();
}