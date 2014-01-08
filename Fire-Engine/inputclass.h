
//////////////////////////////////////////////////////////
//�ļ���: inputclass.h		����: ������:2014/1/8
//////////////////////////////////////////////////////////
#ifndef _INPUTCLASS_H_
#define _INPUTCLASS_H_

////////////////////////////////////
//����: InputClass
////////////////////////////////////
class InputClass
{
public:
	InputClass();
	InputClass(const InputClass&);
	~InputClass();

	void Initialize();
	void keyDown(unsigned int);
	void keyUp(unsigned int);

	bool isKeyDown(unsigned int);
private:
	bool m_keys[256];
};

#endif