
//////////////////////////////////////////////////////////
//文件名: soundclass.cpp		日期: 创建于:2014/1/14
//////////////////////////////////////////////////////////
#include "soundclass.h"

SoundClass::SoundClass()
{
	m_DirectSound = NULL;
	m_primaryBuffer = NULL;
	m_secondaryBuffer1 = NULL;
}

SoundClass::SoundClass(const SoundClass& other)
{

}

SoundClass::~SoundClass()
{

}

bool SoundClass::Initialize(HWND hwnd)
{
	// 初始化DSound与主声音缓冲
	if(!InitializeDirectSound(hwnd))
		return false;

	// 加载波形声音到第二缓冲
	if(!LoadWaveFile("../Fire-Engine/Sounds/songbird1.WAV", &m_secondaryBuffer1))
		return false;

	// 加载之后立刻播放
	if(!PlayWaveFile())
		return false;

	return true;
}

void SoundClass::ShutDown()
{
	ShutdownWaveFile(&m_secondaryBuffer1);

	ShutdownDirectSound();

	return;
}

bool SoundClass::InitializeDirectSound(HWND hwnd)
{
	DSBUFFERDESC bufferDesc;
	WAVEFORMATEX waveFormat;

	// 初始化Dsound接口
	if(FAILED(DirectSoundCreate8(NULL, &m_DirectSound, NULL)))
		return false;

	// 设定合作级别
	if(FAILED(m_DirectSound->SetCooperativeLevel(hwnd, DSSCL_PRIORITY)))
		return false;

	// 设置主缓冲的描述
	bufferDesc.dwSize = sizeof(DSBUFFERDESC);
	bufferDesc.dwFlags = DSBCAPS_PRIMARYBUFFER | DSBCAPS_CTRLVOLUME;
	bufferDesc.dwBufferBytes = 0;
	bufferDesc.dwReserved = 0;
	bufferDesc.lpwfxFormat = NULL;
	bufferDesc.guid3DAlgorithm = GUID_NULL;

	// 控制主缓冲
	if(FAILED(m_DirectSound->CreateSoundBuffer(&bufferDesc, &m_primaryBuffer, NULL)))
		return false;

	// 设置音频格式以及详细信息
	waveFormat.wFormatTag = WAVE_FORMAT_PCM;
	waveFormat.nSamplesPerSec = 44100;
	waveFormat.wBitsPerSample = 16;
	waveFormat.nChannels = 2;
	waveFormat.nBlockAlign = (waveFormat.wBitsPerSample / 8) * waveFormat.nChannels;
	waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
	waveFormat.cbSize = 0;

	// 设置主缓冲的格式
	if(FAILED(m_primaryBuffer->SetFormat(&waveFormat)))
		return false;

	return true;
}

void SoundClass::ShutdownDirectSound()
{
	if(m_primaryBuffer)
	{
		m_primaryBuffer->Release();
		m_primaryBuffer = 0;
	}

	if(m_DirectSound)
	{
		m_DirectSound->Release();
		m_DirectSound = 0;
	}

	return;
}

bool SoundClass::LoadWaveFile(char* filename, IDirectSoundBuffer8** secondaryBuffer)
{
	FILE* filePtr;
	unsigned int count;
	WaveHeaderType waveFileHeader;
	WAVEFORMATEX waveFormat;
	DSBUFFERDESC bufferDesc;
	IDirectSoundBuffer* tempBuffer;
	unsigned char* waveData;
	unsigned char *bufferPtr;
	unsigned long bufferSize;

	// 以二进制形式打开文件
	if(fopen_s(&filePtr, filename, "rb") != 0)
		return false;

	// 开始读取文件
	count = fread(&waveFileHeader, sizeof(waveFileHeader), 1, filePtr);
	if(count != 1)
	{
		return false;
	}

	// 检查格式
	if((waveFileHeader.chunkId[0] != 'R') || (waveFileHeader.chunkId[1] != 'I') || 
		(waveFileHeader.chunkId[2] != 'F') || (waveFileHeader.chunkId[3] != 'F'))
	{
		return false;
	}

	// 检查是否为波形文件
	if((waveFileHeader.format[0] != 'W') || (waveFileHeader.format[1] != 'A') ||
		(waveFileHeader.format[2] != 'V') || (waveFileHeader.format[3] != 'E'))
	{
		return false;
	}

	// 检查是否为fmt 格式
	if((waveFileHeader.subChunkId[0] != 'f') || (waveFileHeader.subChunkId[1] != 'm') ||
		(waveFileHeader.subChunkId[2] != 't') || (waveFileHeader.subChunkId[3] != ' '))
	{
		return false;
	}

	// 检查音频格式是否为WAVE_FORMAT_PCM.
	if(waveFileHeader.audioFormat != WAVE_FORMAT_PCM)
	{
		return false;
	}

	// 检查音频是否以stereo格式记录
	if(waveFileHeader.numChannels != 1)
	{
		return false;
	}

	// 检查Hz
	if(waveFileHeader.sampleRate != 44100)
	{
		return false;
	}

	// 确保为16位
	if(waveFileHeader.bitsPerSample != 16)
	{
		return false;
	}

	// 检查数据头
	if((waveFileHeader.dataChunkId[0] != 'd') || (waveFileHeader.dataChunkId[1] != 'a') ||
		(waveFileHeader.dataChunkId[2] != 't') || (waveFileHeader.dataChunkId[3] != 'a'))
	{
		return false;
	}

	// 设定第二缓冲区的音频格式
	waveFormat.wFormatTag = WAVE_FORMAT_PCM;
	waveFormat.nSamplesPerSec = 44100;
	waveFormat.wBitsPerSample = 16;
	waveFormat.nChannels = 2;
	waveFormat.nBlockAlign = (waveFormat.wBitsPerSample / 8) * waveFormat.nChannels;
	waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
	waveFormat.cbSize = 0;

	// 设置第二缓冲区的描述信息
	bufferDesc.dwSize = sizeof(DSBUFFERDESC);
	bufferDesc.dwFlags = DSBCAPS_CTRLVOLUME;
	bufferDesc.dwBufferBytes = waveFileHeader.dataSize;
	bufferDesc.dwReserved = 0;
	bufferDesc.lpwfxFormat = &waveFormat;
	bufferDesc.guid3DAlgorithm = GUID_NULL;

	// 创建临时声音缓存
	if(FAILED(m_DirectSound->CreateSoundBuffer(&bufferDesc, &tempBuffer, NULL)))
		return false;

	// 检查缓存格式并创建第二缓冲区
	if(FAILED(tempBuffer->QueryInterface(IID_IDirectSoundBuffer8, (void**)&*secondaryBuffer)))
		return false;

	// 释放临时缓存
	tempBuffer->Release();
	tempBuffer = NULL;

	// 移动到音乐文件头
	fseek(filePtr, sizeof(WaveHeaderType), SEEK_SET);

	// 创建临时缓存保存音乐文件
	waveData = new unsigned char[waveFileHeader.dataSize];
	if(!waveData)
		return false;

	count = fread(waveData, 1, waveFileHeader.dataSize, filePtr);
	if(count != waveFileHeader.dataSize)
		return false;

	// 读完后关闭文件
	if(fclose(filePtr) != 0)
		return false;

	// 锁定第二缓冲去方便写入
	if(FAILED((*secondaryBuffer)->Lock(0, waveFileHeader.dataSize, (void**)&bufferPtr, (DWORD*)&bufferSize, NULL, 0, 0)))
		return false;

	// 将声音文件拷贝到内存中
	memcpy(bufferPtr, waveData, waveFileHeader.dataSize);

	// 文件写入后解锁
	if(FAILED((*secondaryBuffer)->Unlock((void*)bufferPtr, bufferSize, NULL, 0)))
		return false;

	// 释放文件
	delete [] waveData;
	waveData = NULL;

	return true;
}

void SoundClass::ShutdownWaveFile(IDirectSoundBuffer8** secondaryBuffer)
{
	if(*secondaryBuffer)
	{
		(*secondaryBuffer)->Release();
		*secondaryBuffer = 0;
	}

	return;
}

bool SoundClass::PlayWaveFile()
{

	// 在声音缓冲头设置位置
	if(FAILED(m_secondaryBuffer1->SetCurrentPosition(0)))
		return false;

	// 设定音量
	if(FAILED(m_secondaryBuffer1->SetVolume(DSBVOLUME_MAX)))
		return false;

	// 播放第二缓存的内容
	if(FAILED(m_secondaryBuffer1->Play(0, 0, 0)))
		return false;

	return true;
}