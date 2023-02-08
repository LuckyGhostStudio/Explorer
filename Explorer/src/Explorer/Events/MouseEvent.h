#pragma once

#include "Event.h"

namespace Explorer
{
	/// <summary>
	/// ����ƶ��¼�
	/// </summary>
	class EXPLORER_API MouseMovedEvent :public Event
	{
	private:
		float m_MouseX;		//��� x ����
		float m_MouseY;		//��� y ����
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
		/// ���ظ��¼�������
		/// </summary>
		/// <returns>MouseMoved�¼�</returns>
		static EventType GetStaticType()
		{
			return EventType::MouseMoved;
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
		/// <returns>"MouseMoved"</returns>
		virtual const char* GetName() const override
		{
			return "MouseMoved";
		}

		/// <summary>
		/// �����¼������־
		/// </summary>
		/// <returns>EventCategoryMouse | EventCategoryInput�¼�</returns>
		virtual int GetCategoryFlags() const override
		{
			return EventCategoryMouse | EventCategoryInput;
		}
	};

	/// <summary>
	/// �����ֹ����¼�
	/// </summary>
	class EXPLORER_API MouseScrolledEvent :public Event
	{
	private:
		float m_XOffset;	//���ˮƽ����ƫ����
		float m_YOffset;	//�����ֱ����ƫ����
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
		/// ���ظ��¼�������
		/// </summary>
		/// <returns>MouseScrolled�¼�</returns>
		static EventType GetStaticType()
		{
			return EventType::MouseScrolled;
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
		/// <returns>"MouseScrolled"</returns>
		virtual const char* GetName() const override
		{
			return "MouseScrolled";
		}

		/// <summary>
		/// �����¼������־
		/// </summary>
		/// <returns>EventCategoryMouse | EventCategoryInput�¼�</returns>
		virtual int GetCategoryFlags() const override
		{
			return EventCategoryMouse | EventCategoryInput;
		}
	};

	/// <summary>
	/// ��갴ť�¼�
	/// </summary>
	class EXPLORER_API MouseButtonEvent :public Event
	{
	protected:
		int m_Button;	//��갴ť

		MouseButtonEvent(int button) :m_Button(button) {}
	public:
		inline int GetMouseButton() const { return m_Button; }

		/// <summary>
		/// �����¼������־
		/// </summary>
		/// <returns>EventCategoryMouse | EventCategoryInput�¼�</returns>
		virtual int GetCategoryFlags() const override
		{
			return EventCategoryMouse | EventCategoryInput;
		}
	};

	/// <summary>
	/// ��갴ť�����¼�
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
		/// ���ظ��¼�������
		/// </summary>
		/// <returns>MouseButtonPressed�¼�</returns>
		static EventType GetStaticType()
		{
			return EventType::MouseButtonPressed;
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
		/// <returns>"MouseButtonPressed"</returns>
		virtual const char* GetName() const override
		{
			return "MouseButtonPressed";
		}
	};

	/// <summary>
	/// ��갴ţ̌���¼�
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
		/// ���ظ��¼�������
		/// </summary>
		/// <returns>MouseButtonReleased�¼�</returns>
		static EventType GetStaticType()
		{
			return EventType::MouseButtonReleased;
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
		/// <returns>"MouseButtonReleased"</returns>
		virtual const char* GetName() const override
		{
			return "MouseButtonReleased";
		}
	};
}