#pragma once
#include "exppch.h"	//Ԥ����ͷ�ļ�

namespace Explorer
{
	/*
	* Events in Explorer are currently blocking,meaning when an event occurs
	* itimmediately gets dispatched and must be dealt with right then an there.
	* For the future, a better strategy might be to buffer events in an event
	* bus and process them during the "event" part of the update stage.
	*
	* Explorer�е��¼���ǰ��������״̬������ζ���¼�����ʱ
	* �����������ͣ�������������
	* δ�������õĲ��Կ��������¼��л����¼�
	* ���ߣ����ڸ��½׶εġ��¼������ִ������ǡ�
	*/

	/// <summary>
	/// �¼�����
	/// </summary>
	enum class EventType
	{
		None = 0,

		WindowClose,			//���ڹر��¼�
		WindowResize,			//���ڴ�С�ı��¼�
		WindowFocus,			//���ھ۽��¼�
		WindowLostFocus,		//����ʧ���¼�
		WindowMoved,			//�����ƶ��¼�

		AppTick,				//Appʱ���¼�
		AppUpdate,				//Appÿ֡�����¼�
		AppRender,				//App��Ⱦ�¼�

		KeyPressed,				//���������¼�
		KeyReleased,			//����̧���¼�
		keyTyped,				//���������ַ��¼�

		MouseButtonPressed,		//��갴ť�����¼�
		MouseButtonReleased,	//��갴ţ̌���¼�
		MouseMoved, 			//����ƶ��¼�
		MouseScrolled			//�����ֹ����¼�
	};

	/// <summary>
	/// �¼�����
	/// </summary>
	enum EventCategory
	{
		None = 0,
		EventCategoryApplication = BIT(0),	// 1  App���¼�
		EventCategoryInput = BIT(1),		// 2  �������¼�
		EventCategoryKeyboard = BIT(2),		// 4  �������¼�
		EventCategoryMouse = BIT(3),		// 8  ������¼�
		EventCategoryMouseButton = BIT(4)	// 16 ��갴ť���¼�
	};

	/// <summary>
	/// �¼���
	/// </summary>
	class EXPLORER_API Event
	{
		friend class EventDispatcher;	//�¼�������
	public:
		/// <summary>
		/// �����¼�����
		/// </summary>
		/// <returns>�¼�����</returns>
		virtual EventType GetEventType() const = 0;

		/// <summary>
		/// �����¼���
		/// </summary>
		/// <returns>�¼���</returns>
		virtual const char* GetName() const = 0;

		/// <summary>
		/// �����¼�����־
		/// </summary>
		/// <returns>�¼�����־</returns>
		virtual int GetCategoryFlags() const = 0;

		/// <summary>
		/// �����¼������ַ���
		/// </summary>
		/// <returns>�¼���</returns>
		virtual std::string ToString() const { return GetName(); }

		/// <summary>
		/// ��ǰ�¼��Ƿ���Ŀ���¼�������
		/// </summary>
		/// <param name="category">Ŀ���¼�����</param>
		/// <returns>���</returns>
		inline bool IsInCategory(EventCategory category)
		{
			return GetCategoryFlags() & category;	//�¼�����־�� Ŀ���¼���� ��λ��
		}

		/// <summary>
		/// ��ȡ�¼�������
		/// </summary>
		/// <returns></returns>
		inline bool GetHandled() const { return m_Handled; }
	protected:
		bool m_Handled = false;		//�Ƿ��Ѵ���
	};

	/// <summary>
	/// �¼�������
	/// </summary>
	class EventDispatcher
	{
		template<typename T>
		using EventFn = std::function<bool(T&)>;	//�¼����� bool func(T&)
	private:
		Event& m_Event;		//��ǰ���յ��¼�
	public:
		EventDispatcher(Event& event) :m_Event(event) {}

		/// <summary>
		/// �¼�����
		/// </summary>
		/// <typeparam name="T">����</typeparam>
		/// <param name="func">�¼�����</param>
		/// <returns>���Ƚ��</returns>
		template<typename T>
		bool Dispatch(EventFn<T> func)
		{
			if (m_Event.GetEventType() == T::GetStaticType()) {		//���յ����¼����� == �¼���������
				m_Event.m_Handled = func(*(T*)&m_Event);			//ִ���¼����� func(Event& e)����m_Eventת��ΪT
				return true;
			}
			return false;
		}
	};

	/// <summary>
	/// �������������������¼�
	/// </summary>
	/// <param name="os">���������</param>
	/// <param name="e">�¼�</param>
	/// <returns>����¼�</returns>
	inline std::ostream& operator<<(std::ostream& os, const Event& e)
	{
		return os << e.ToString();
	}
}