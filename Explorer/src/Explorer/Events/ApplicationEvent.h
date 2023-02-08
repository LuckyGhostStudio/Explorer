#pragma once

#include "Event.h"

namespace Explorer
{
	/// <summary>
	/// ���ڸı��С�¼�
	/// </summary>
	class EXPLORER_API WindowResizeEvent :public Event
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
	class EXPLORER_API WindowCloseEvent :public Event
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

	/// <summary>
	/// Appʱ���¼�
	/// </summary>
	class EXPLORER_API AppTickEvent :public Event
	{
	public:
		AppTickEvent() {}

		/// <summary>
		/// ���ظ��¼�������
		/// </summary>
		/// <returns>AppTick�¼�</returns>
		static EventType GetStaticType()
		{
			return EventType::AppTick;
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
		/// <returns>"AppTick"</returns>
		virtual const char* GetName() const override
		{
			return "AppTick";
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
	/// App�����¼�
	/// </summary>
	class EXPLORER_API AppUpdateEvent :public Event
	{
	public:
		AppUpdateEvent() {}

		/// <summary>
		/// ���ظ��¼�������
		/// </summary>
		/// <returns>AppUpdate�¼�</returns>
		static EventType GetStaticType()
		{
			return EventType::AppUpdate;
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
		/// <returns>"AppUpdate"</returns>
		virtual const char* GetName() const override
		{
			return "AppUpdate";
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
	/// App��Ⱦ�¼�
	/// </summary>
	class EXPLORER_API AppRenderEvent :public Event
	{
	public:
		AppRenderEvent() {}

		/// <summary>
		/// ���ظ��¼�������
		/// </summary>
		/// <returns>AppRender�¼�</returns>
		static EventType GetStaticType()
		{
			return EventType::AppRender;
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
		/// <returns>"AppRender"</returns>
		virtual const char* GetName() const override
		{
			return "AppRender";
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