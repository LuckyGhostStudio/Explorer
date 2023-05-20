#pragma once

#include "Event.h"

namespace Explorer
{
	/// <summary>
	/// ���ڸı��С�¼�
	/// </summary>
	class WindowResizeEvent :public Event
	{
	private:
		unsigned int m_Width;	//���������Ĵ��ڿ�
		unsigned int m_Height;	//���������Ĵ��ڸ�
	public:
		WindowResizeEvent(unsigned int width, unsigned int height) :m_Width(width), m_Height(height) {}

		inline unsigned int GetWidth() const { return m_Width; }
		inline unsigned int GetHeight() const { return m_Height; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "WindowResizeEvent: " << m_Width << ", " << m_Height;
			return ss.str();
		}

		/// <summary>
		/// ���ظ��¼�������
		/// </summary>
		/// <returns>WindowResize�¼�</returns>
		static EventType GetStaticType() 
		{
			return EventType::WindowResize; 
		}

		/// <summary>
		/// ���ظ��¼�����
		/// </summary>
		/// <returns>�¼�����</returns>
		virtual EventType GetEventType() const override 
		{ 
			return GetStaticType(); 
		}

		/// <summary>
		/// ���ظ��¼���
		/// </summary>
		/// <returns>"WindowResize"</returns>
		virtual const char* GetName() const override 
		{ 
			return "WindowResize"; 
		}

		/// <summary>
		/// �����¼������־
		/// </summary>
		/// <returns>EventCategoryApplication�¼�</returns>
		virtual int GetCategoryFlags() const override 
		{
			return EventCategoryApplication; 
		}
	};

	/// <summary>
	/// ���ڹر��¼�
	/// </summary>
	class WindowCloseEvent :public Event
	{
	public:
		WindowCloseEvent() {}

		/// <summary>
		/// ���ظ��¼�������
		/// </summary>
		/// <returns>WindowClose�¼�</returns>
		static EventType GetStaticType()
		{
			return EventType::WindowClose;
		}

		/// <summary>
		/// ���ظ��¼�����
		/// </summary>
		/// <returns>�¼�����</returns>
		virtual EventType GetEventType() const override
		{
			return GetStaticType();
		}

		/// <summary>
		/// ���ظ��¼���
		/// </summary>
		/// <returns>"WindowClose"</returns>
		virtual const char* GetName() const override
		{
			return "WindowClose";
		}

		/// <summary>
		/// �����¼������־
		/// </summary>
		/// <returns>EventCategoryApplication�¼�</returns>
		virtual int GetCategoryFlags() const override
		{
			return EventCategoryApplication;
		}
	};
}