#include "EnginePCH.h"
#include "TimeManager.h"

EngineModule::TimeManager::TimeManager()
	:m_deltaTime(0.)
	, m_frequency()
	, m_prevCount()
	, m_curentCount()
	, m_elapsedTime(0.)
	, m_fps(0)
	, m_updateCount(0)
{

}

EngineModule::TimeManager::~TimeManager()
{

}

void EngineModule::TimeManager::Initialize()
{
	QueryPerformanceFrequency(&m_frequency); // ���μ����� ī���� ���� ��
	QueryPerformanceCounter(&m_prevCount); // ���� �������� ī���� �� 
	QueryPerformanceCounter(&m_curentCount);
}

float EngineModule::TimeManager::Update()
{
	QueryPerformanceCounter(&m_curentCount);

	m_deltaTime = (m_curentCount.QuadPart - m_prevCount.QuadPart) / (double)m_frequency.QuadPart;

	m_prevCount = m_curentCount;

	++m_updateCount;
	m_elapsedTime += m_deltaTime;
	if (m_elapsedTime >= 1.)
	{
		m_elapsedTime -= 1.;
		m_fps = m_updateCount;
		m_updateCount = 0;
	}

	return static_cast<float>(m_deltaTime);
}

void EngineModule::TimeManager::Finalize()
{

}
