#pragma once

#include "Event.h"
#include "Explorer/Core/KeyCodes.h"

namespace Explorer
{
	/// <summary>
	/// 键盘按键事件：所有键盘事件的基类
	/// </summary>
	class KeyEvent :public Event
	{
	protected:
		KeyCode m_KeyCode;	//按键代码

		KeyEvent(const KeyCode keycode) :m_KeyCode(keycode) {}

	public:
		inline KeyCode GetKeyCode() const { return m_KeyCode; }

		/// <summary>
		/// 返回事件种类标志
		/// </summary>
		/// <returns>EventCategoryKeyboard | EventCategoryInput事件</returns>
		virtual int GetCategoryFlags() const override
		{
			return EventCategoryKeyboard | EventCategoryInput;
		}
	};

	/// <summary>
	/// 按键按下事件
	/// </summary>
	class KeyPressedEvent :public KeyEvent
	{
	private:
		int m_RepeatCount;	//按键重复次数
	public:
		KeyPressedEvent(const KeyCode keycode, int repeatCount) :KeyEvent(keycode), m_RepeatCount(repeatCount) {}

		inline int GetRepeatCount() const { return m_RepeatCount; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyPressedEvent: " << m_KeyCode << " (" << m_RepeatCount << " repeats)";
			return ss.str();
		}

		/// <summary>
		/// 返回该事件的类型
		/// </summary>
		/// <returns>KeyPressed事件</returns>
		static EventType GetStaticType()
		{
			return EventType::KeyPressed;
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
		/// <returns>"KeyPressed"</returns>
		virtual const char* GetName() const override
		{
			return "KeyPressed";
		}
	};

	/// <summary>
	/// 按键抬起事件
	/// </summary>
	class KeyReleasedEvent :public KeyEvent
	{
	public:
		KeyReleasedEvent(const KeyCode keycode) : KeyEvent(keycode) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyReleasedEvent: " << m_KeyCode;
			return ss.str();
		}

		/// <summary>
		/// 返回该事件的类型
		/// </summary>
		/// <returns>KeyReleased事件</returns>
		static EventType GetStaticType()
		{
			return EventType::KeyReleased;
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
		/// <returns>"KeyReleased"</returns>
		virtual const char* GetName() const override
		{
			return "KeyReleased";
		}
	};

	/// <summary>
	/// 按键输入字符事件
	/// </summary>
	class KeyTypedEvent :public KeyEvent
	{
	public:
		KeyTypedEvent(const KeyCode keycode) :KeyEvent(keycode) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyTypedEvent: " << m_KeyCode;
			return ss.str();
		}

		/// <summary>
		/// 返回该事件的类型
		/// </summary>
		/// <returns>KeyTyped事件</returns>
		static EventType GetStaticType()
		{
			return EventType::keyTyped;
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
		/// <returns>"KeyTyped"</returns>
		virtual const char* GetName() const override
		{
			return "KeyTyped";
		}
	};
}