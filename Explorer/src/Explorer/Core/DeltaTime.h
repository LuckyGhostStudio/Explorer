#pragma once

namespace Explorer
{
	/// <summary>
	/// 帧间隔
	/// </summary>
	class DeltaTime
	{
	private:
		float m_Time;	//帧间隔时间（秒）
	public:
		/// <summary>
		/// 帧间隔
		/// </summary>
		/// <param name="time">帧间隔时间（秒）</param>
		DeltaTime(float time = 0.0f) :m_Time(time)
		{

		}

		/// <summary>
		/// 默认强转为float
		/// </summary>
		operator float() const { return m_Time; }

		/// <summary>
		/// 返回帧间隔时间（秒）
		/// </summary>
		/// <returns>秒</returns>
		float GetSeconds() const { return m_Time; }

		/// <summary>
		/// 返回帧间隔时间（毫秒）
		/// </summary>
		/// <returns>毫秒</returns>
		float GetMilliseconds() const { return m_Time * 1000.0f; }
	};
}