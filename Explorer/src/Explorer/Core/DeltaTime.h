#pragma once

namespace Explorer
{
	/// <summary>
	/// ֡���
	/// </summary>
	class DeltaTime
	{
	private:
		float m_Time;	//֡���ʱ�䣨�룩
	public:
		/// <summary>
		/// ֡���
		/// </summary>
		/// <param name="time">֡���ʱ�䣨�룩</param>
		DeltaTime(float time = 0.0f) :m_Time(time)
		{

		}

		/// <summary>
		/// Ĭ��ǿתΪfloat
		/// </summary>
		operator float() const { return m_Time; }

		/// <summary>
		/// ����֡���ʱ�䣨�룩
		/// </summary>
		/// <returns>��</returns>
		float GetSeconds() const { return m_Time; }

		/// <summary>
		/// ����֡���ʱ�䣨���룩
		/// </summary>
		/// <returns>����</returns>
		float GetMilliseconds() const { return m_Time * 1000.0f; }
	};
}