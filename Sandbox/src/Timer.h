#pragma once

#include <chrono>

/// <summary>
/// 计时器
/// </summary>
/// <typeparam name="Fn">计时器停止时回调函数</typeparam>
template<typename Fn>
class Timer
{
public:
	Timer(const char* name, Fn&& func)
		: m_Name(name), m_Func(func), m_Stopped(false)
	{
		m_StartTimepoint = std::chrono::high_resolution_clock::now();	//开始时间点
	}

	~Timer()
	{
		if (!m_Stopped)
			Stop();
	}

	void Stop()
	{
		auto endTimepoint = std::chrono::high_resolution_clock::now();	//结束时间点

		//开始结束时间（微妙）
		long long start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).time_since_epoch().count();
		long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();

		m_Stopped = true;

		float duration = (end - start) * 0.001f;	//持续时间（毫秒）
		m_Func({ m_Name, duration });				//将测试结果添加到列表
	}
private:
	const char* m_Name;		//测试程序段名
	Fn m_Func;
	std::chrono::time_point<std::chrono::steady_clock> m_StartTimepoint;	//测试开始时间点
	bool m_Stopped;
};

//测试name程序段运行用时 并将测试结果profileResult添加到列表m_ProfileResults
#define PROFILE_SCOPE(name) Timer timer##__LINE__(name, [&](ProfileResult profileResult) { m_ProfileResults.push_back(profileResult); })
