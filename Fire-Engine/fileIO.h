
//////////////////////////////////////////////////////////
//文件名: fileIO.h		日期: 创建于:2014/1/9
//////////////////////////////////////////////////////////
#ifndef _FILEIO_H_
#define _FILEIO_H_

//////////////
//INCLUDES
//////////////
#include <iomanip>
#include <fstream>

using namespace std;

////////////////////////////////////
//类名: FileIOClass
////////////////////////////////////
class FileIOClass
{
public:
	FileIOClass();
	FileIOClass(const FileIOClass&);
	~FileIOClass();

	void WriteTextFile(const wchar_t* filepath, const char* texts);
	void WriteTextFile(const wchar_t* filepath, int texts);

private:
	ofstream m_file;
};



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


#endif