#pragma once

#include "Explorer/Core.h"
#include "Explorer/Events/Event.h"

namespace Explorer
{
	/// <summary>
	/// ��
	/// </summary>
	class EXPLORER_API Layer
	{
	protected:
		std::string m_DebugName;	//Layer��Debugģʽ�е�����
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer();

		/// <summary>
		/// ���ò�̶�����ջ
		/// </summary>
		virtual void OnAttach() {}

		/// <summary>
		/// ���ò�Ӳ�ջ����
		/// </summary>
		virtual void OnDetach() {}

		/// <summary>
		/// �ò����ʱ���ã�ÿ֡����
		/// </summary>
		virtual void OnUpdate() {}

		/// <summary>
		/// �¼������������¼�
		/// </summary>
		/// <param name="event">�¼�</param>
		virtual void OnEvent(Event& event) {}

		inline const std::string& GetName() const { return m_DebugName; }
	};
}