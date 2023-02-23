#pragma once

#include "Explorer/Core/Core.h"
#include "Explorer/Core/DeltaTime.h"
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
		/// ���ò���ӵ���ջʱ����
		/// </summary>
		virtual void OnAttach() {}

		/// <summary>
		/// ���ò�Ӳ�ջ�Ƴ�ʱ����
		/// </summary>
		virtual void OnDetach() {}

		/// <summary>
		/// �ò����ʱ���ã�ÿ֡����
		/// </summary>
		/// <param name="dt">֡���</param>
		virtual void OnUpdate(DeltaTime dt) {}

		/// <summary>
		/// ��ȾImGui
		/// </summary>
		virtual void OnImGuiRender() {}

		/// <summary>
		/// �¼������������¼�
		/// </summary>
		/// <param name="event">�¼�</param>
		virtual void OnEvent(Event& event) {}

		inline const std::string& GetName() const { return m_DebugName; }
	};
}