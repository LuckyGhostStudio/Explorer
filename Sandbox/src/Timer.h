#pragma once

#include <chrono>

/// <summary>
/// ��ʱ��
/// </summary>
/// <typeparam name="Fn">��ʱ��ֹͣʱ�ص�����</typeparam>
template<typename Fn>
class Timer
{
public:
	Timer(const char* name, Fn&& func)
		: m_Name(name), m_Func(func), m_Stopped(false)
	{
		m_StartTimepoint = std::chrono::high_resolution_clock::now();	//��ʼʱ���
	}

	~Timer()
	{
		if (!m_Stopped)
			Stop();
	}

	void Stop()
	{
		auto endTimepoint = std::chrono::high_resolution_clock::now();	//����ʱ���

		//��ʼ����ʱ�䣨΢�
		long long start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).time_since_epoch().count();
		long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();

		m_Stopped = true;

		float duration = (end - start) * 0.001f;	//����ʱ�䣨���룩
		m_Func({ m_Name, duration });				//�����Խ����ӵ��б�
	}
private:
	const char* m_Name;		//���Գ������
	Fn m_Func;
	std::chrono::time_point<std::chrono::steady_clock> m_StartTimepoint;	//���Կ�ʼʱ���
	bool m_Stopped;
};

//����name�����������ʱ �������Խ��profileResult��ӵ��б�m_ProfileResults
#define PROFILE_SCOPE(name) Timer timer##__LINE__(name, [&](ProfileResult profileResult) { m_ProfileResults.push_back(profileResult); })
