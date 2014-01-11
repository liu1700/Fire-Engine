
//////////////////////////////////////////////////////////
//�ļ���: fileIO.h		����: ������:2014/1/9
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
//����: FileIOClass
////////////////////////////////////
class FileIOClass
{
public:
	FileIOClass();
	FileIOClass(const FileIOClass&);
	~FileIOClass();

public:
	void WriteTextFile(const wchar_t*, const char*);
	void WriteTextFile(const wchar_t*, int);

public:
	float** LoadModelFromFile(const wchar_t*, int&, int&);

private:
	ofstream m_textFile;

};


#endif