
//////////////////////////////////////////////////////////
//�ļ���: cpuclass.cpp		����: ������:2014/1/14
//////////////////////////////////////////////////////////
#include "cpuclass.h"

CpuClass::CpuClass()
{

}

CpuClass::CpuClass(const CpuClass& other)
{

}

CpuClass::~CpuClass()
{

}

void CpuClass::Initialize()
{
	PDH_STATUS status;


	// ��ʼ��flag�����ȡcpuʹ�ö�
	m_canReadCpu = true;

	// �������ж����ȡcpuʹ�ö�
	status = PdhOpenQuery(NULL, 0, &m_queryHandle);
	if(status != ERROR_SUCCESS)
	{
		m_canReadCpu = false;
	}

	// �趨�����ȡ����CPU
	status = PdhAddCounter(m_queryHandle, TEXT("\\Processor(_Total)\\% processor time"), 0, &m_counterHandle);
	if(status != ERROR_SUCCESS)
	{
		m_canReadCpu = false;
	}

	m_lastSampleTime = GetTickCount(); 

	m_cpuUsage = 0;

	return;
}

void CpuClass::Shutdown()
{
	if(m_canReadCpu)
	{
		PdhCloseQuery(m_queryHandle);
	}

	return;
}

void CpuClass::Frame()
{
	PDH_FMT_COUNTERVALUE value; 

	if(m_canReadCpu)
	{
		if((m_lastSampleTime + 1000) < GetTickCount())
		{
			m_lastSampleTime = GetTickCount(); 

			PdhCollectQueryData(m_queryHandle);

			PdhGetFormattedCounterValue(m_counterHandle, PDH_FMT_LONG, NULL, &value);

			m_cpuUsage = value.longValue;
		}
	}

	return;
}

int CpuClass::GetCpuPercentage()
{
	int usage;

	if(m_canReadCpu)
	{
		usage = (int)m_cpuUsage;
	}
	else
	{
		usage = 0;
	}

	return usage;
}