#pragma once

#include "Event.h"
#include "Explorer/Core/KeyCodes.h"

namespace Explorer
{
	/// <summary>
	/// ���̰����¼������м����¼��Ļ���
	/// </summary>
	class KeyEvent :public Event
	{
	protected:
		KeyCode m_KeyCode;	//��������

		KeyEvent(const KeyCode keycode) :m_KeyCode(keycode) {}

	public:
		inline KeyCode GetKeyCode() const { return m_KeyCode; }

		/// <summary>
		/// �����¼������־
		/// </summary>
		/// <returns>EventCategoryKeyboard | EventCategoryInput�¼�</returns>
		virtual int GetCategoryFlags() const override
		{
			return EventCategoryKeyboard | EventCategoryInput;
		}
	};

	/// <summary>
	/// ���������¼�
	/// </summary>
	class KeyPressedEvent :public KeyEvent
	{
	private:
		int m_RepeatCount;	//�����ظ�����
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
		/// ���ظ��¼�������
		/// </summary>
		/// <returns>KeyPressed�¼�</returns>
		static EventType GetStaticType()
		{
			return EventType::KeyPressed;
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
		/// <returns>"KeyPressed"</returns>
		virtual const char* GetName() const override
		{
			return "KeyPressed";
		}
	};

	/// <summary>
	/// ����̧���¼�
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
		/// ���ظ��¼�������
		/// </summary>
		/// <returns>KeyReleased�¼�</returns>
		static EventType GetStaticType()
		{
			return EventType::KeyReleased;
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
		/// <returns>"KeyReleased"</returns>
		virtual const char* GetName() const override
		{
			return "KeyReleased";
		}
	};

	/// <summary>
	/// ���������ַ��¼�
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
		/// ���ظ��¼�������
		/// </summary>
		/// <returns>KeyTyped�¼�</returns>
		static EventType GetStaticType()
		{
			return EventType::keyTyped;
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
		/// <returns>"KeyTyped"</returns>
		virtual const char* GetName() const override
		{
			return "KeyTyped";
		}
	};
}