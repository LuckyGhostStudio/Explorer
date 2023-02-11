#pragma once

#include "Explorer/Layer.h"

namespace Explorer
{
	/// <summary>
	/// ImGui��
	/// </summary>
	class EXPLORER_API ImGuiLayer :public Layer
	{
	private:
		float m_Time = 0.0f;	//��ǰ֡ʱ��
	public:
		ImGuiLayer();
		~ImGuiLayer();

		/// <summary>
		/// ���ò���ӵ���ջʱ����
		/// </summary>
		void OnAttach();

		/// <summary>
		/// ���ò�Ӳ�ջ�Ƴ�ʱ����
		/// </summary>
		void OnDetach();

		/// <summary>
		/// �ò����ʱ���ã�ÿ֡����
		/// </summary>
		void OnUpdate();

		/// <summary>
		/// �¼������������¼�
		/// </summary>
		/// <param name="event">�¼�</param>
		void OnEvent(Event& event);
	};
}