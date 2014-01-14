
//////////////////////////////////////////////////////////
//文件名: timerclass.cpp		日期: 创建于:2014/1/14
//////////////////////////////////////////////////////////
#include "timerclass.h"

TimerClass::TimerClass()
{

}

TimerClass::TimerClass(const TimerClass& other)
{

}

TimerClass::~TimerClass()
{

}

bool TimerClass::Initialize()
{
	// 检查系统是否支持高精度计时器
	QueryPerformanceFrequency((LARGE_INTEGER*)&m_frequency);
	if(m_frequency == 0)
	{
		return false;
	}

	// 计算出每毫秒的计算次数
	m_ticksPerMs = (float)(m_frequency / 1000);

	QueryPerformanceCounter((LARGE_INTEGER*)&m_startTime);

	return true;
}

void TimerClass::Frame()
{
	INT64 currentTime;
	float timeDifference;


	QueryPerformanceCounter((LARGE_INTEGER*)& currentTime);

	timeDifference = (float)(currentTime - m_startTime);

	m_frameTime = timeDifference / m_ticksPerMs;

	m_startTime = currentTime;

	return;
}

float TimerClass::GetTime()
{
	return m_frameTime;
}