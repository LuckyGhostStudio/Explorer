#pragma once

#include "Event.h"

namespace Explorer
{
	/// <summary>
	/// 鼠标移动事件
	/// </summary>
	class EXPLORER_API MouseMovedEvent :public Event
	{
	private:
		float m_MouseX;		//鼠标 x 坐标
		float m_MouseY;		//鼠标 y 坐标
	public:
		MouseMovedEvent(float x, float y) :m_MouseX(x), m_MouseY(y) {}

		inline float GetX() const { return m_MouseX; }
		inline float GetY() const { return m_MouseY; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseMovedEvent: " << m_MouseX << ", " << m_MouseY;
			return ss.str();
		}

		/// <summary>
		/// 返回该事件的类型
		/// </summary>
		/// <returns>MouseMoved事件</returns>
		static EventType GetStaticType()
		{
			return EventType::MouseMoved;
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
		/// <returns>"MouseMoved"</returns>
		virtual const char* GetName() const override
		{
			return "MouseMoved";
		}

		/// <summary>
		/// 返回事件种类标志
		/// </summary>
		/// <returns>EventCategoryMouse | EventCategoryInput事件</returns>
		virtual int GetCategoryFlags() const override
		{
			return EventCategoryMouse | EventCategoryInput;
		}
	};

	/// <summary>
	/// 鼠标滚轮滚动事件
	/// </summary>
	class EXPLORER_API MouseScrolledEvent :public Event
	{
	private:
		float m_XOffset;	//鼠标水平滚动偏移量
		float m_YOffset;	//鼠标竖直滚动偏移量
	public:
		MouseScrolledEvent(float xOffset, float yOffset) :m_XOffset(xOffset), m_YOffset(yOffset) {}

		inline float GetXOffset() const { return m_XOffset; }
		inline float GetYOffset() const { return m_YOffset; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseScrolledEvent: " << m_XOffset << ", " << m_YOffset;
			return ss.str();
		}

		/// <summary>
		/// 返回该事件的类型
		/// </summary>
		/// <returns>MouseScrolled事件</returns>
		static EventType GetStaticType()
		{
			return EventType::MouseScrolled;
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
		/// <returns>"MouseScrolled"</returns>
		virtual const char* GetName() const override
		{
			return "MouseScrolled";
		}

		/// <summary>
		/// 返回事件种类标志
		/// </summary>
		/// <returns>EventCategoryMouse | EventCategoryInput事件</returns>
		virtual int GetCategoryFlags() const override
		{
			return EventCategoryMouse | EventCategoryInput;
		}
	};

	/// <summary>
	/// 鼠标按钮事件
	/// </summary>
	class EXPLORER_API MouseButtonEvent :public Event
	{
	protected:
		int m_Button;	//鼠标按钮

		MouseButtonEvent(int button) :m_Button(button) {}
	public:
		inline int GetMouseButton() const { return m_Button; }

		/// <summary>
		/// 返回事件种类标志
		/// </summary>
		/// <returns>EventCategoryMouse | EventCategoryInput事件</returns>
		virtual int GetCategoryFlags() const override
		{
			return EventCategoryMouse | EventCategoryInput;
		}
	};

	/// <summary>
	/// 鼠标按钮按下事件
	/// </summary>
	class EXPLORER_API MouseButtonPressedEvent :public MouseButtonEvent
	{
	public:
		MouseButtonPressedEvent(int button) :MouseButtonEvent(button) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseButtonPressedEvent: " << m_Button;
			return ss.str();
		}

		/// <summary>
		/// 返回该事件的类型
		/// </summary>
		/// <returns>MouseButtonPressed事件</returns>
		static EventType GetStaticType()
		{
			return EventType::MouseButtonPressed;
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
		/// <returns>"MouseButtonPressed"</returns>
		virtual const char* GetName() const override
		{
			return "MouseButtonPressed";
		}
	};

	/// <summary>
	/// 鼠标按钮抬起事件
	/// </summary>
	class EXPLORER_API MouseButtonReleasedEvent :public MouseButtonEvent
	{
	public:
		MouseButtonReleasedEvent(int button) :MouseButtonEvent(button) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseButtonReleasedEvent: " << m_Button;
			return ss.str();
		}

		/// <summary>
		/// 返回该事件的类型
		/// </summary>
		/// <returns>MouseButtonReleased事件</returns>
		static EventType GetStaticType()
		{
			return EventType::MouseButtonReleased;
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
		/// <returns>"MouseButtonReleased"</returns>
		virtual const char* GetName() const override
		{
			return "MouseButtonReleased";
		}
	};
}