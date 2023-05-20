#pragma once

#include "Event.h"

namespace Explorer
{
	/// <summary>
	/// 窗口改变大小事件
	/// </summary>
	class WindowResizeEvent :public Event
	{
	private:
		unsigned int m_Width;	//被调整到的窗口宽
		unsigned int m_Height;	//被调整到的窗口高
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
		/// 返回该事件的类型
		/// </summary>
		/// <returns>WindowResize事件</returns>
		static EventType GetStaticType() 
		{
			return EventType::WindowResize; 
		}

		/// <summary>
		/// 返回该事件类型
		/// </summary>
		/// <returns>事件类型</returns>
		virtual EventType GetEventType() const override 
		{ 
			return GetStaticType(); 
		}

		/// <summary>
		/// 返回该事件名
		/// </summary>
		/// <returns>"WindowResize"</returns>
		virtual const char* GetName() const override 
		{ 
			return "WindowResize"; 
		}

		/// <summary>
		/// 返回事件种类标志
		/// </summary>
		/// <returns>EventCategoryApplication事件</returns>
		virtual int GetCategoryFlags() const override 
		{
			return EventCategoryApplication; 
		}
	};

	/// <summary>
	/// 窗口关闭事件
	/// </summary>
	class WindowCloseEvent :public Event
	{
	public:
		WindowCloseEvent() {}

		/// <summary>
		/// 返回该事件的类型
		/// </summary>
		/// <returns>WindowClose事件</returns>
		static EventType GetStaticType()
		{
			return EventType::WindowClose;
		}

		/// <summary>
		/// 返回该事件类型
		/// </summary>
		/// <returns>事件类型</returns>
		virtual EventType GetEventType() const override
		{
			return GetStaticType();
		}

		/// <summary>
		/// 返回该事件名
		/// </summary>
		/// <returns>"WindowClose"</returns>
		virtual const char* GetName() const override
		{
			return "WindowClose";
		}

		/// <summary>
		/// 返回事件种类标志
		/// </summary>
		/// <returns>EventCategoryApplication事件</returns>
		virtual int GetCategoryFlags() const override
		{
			return EventCategoryApplication;
		}
	};
}