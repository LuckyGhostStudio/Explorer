#pragma once

#include "Explorer/Core/Layer.h"

#include "Explorer/Events/ApplicationEvent.h"
#include "Explorer/Events/KeyEvent.h"
#include "Explorer/Events/MouseEvent.h"

namespace Explorer
{
	/// <summary>
	/// ImGui��
	/// </summary>
	class ImGuiLayer :public Layer
	{
	private:
		float m_Time = 0.0f;		//��ǰ֡ʱ��
		bool m_BlockEvents = true;	//�Ƿ���ֹ�����¼�
	public:
		ImGuiLayer();
		~ImGuiLayer();

		/// <summary>
		/// ���ò���ӵ���ջʱ����
		/// </summary>
		virtual void OnAttach() override;

		/// <summary>
		/// ���ò�Ӳ�ջ�Ƴ�ʱ����
		/// </summary>
		virtual void OnDetach() override;

		/// <summary>
		/// �¼��ص�����
		/// </summary>
		/// <param name="e">�¼�</param>
		virtual void OnEvent(Event& e) override;

		/// <summary>
		/// ��ʼ������Ⱦ
		/// </summary>
		void Begin();

		/// <summary>
		/// ����������Ⱦ
		/// </summary>
		void End();

		/// <summary>
		/// ��ֹ�¼�����ֹ�����¼�
		/// </summary>
		/// <param name="block">�Ƿ���ֹ</param>
		void BlockEvents(bool block) { m_BlockEvents = block; }

		/// <summary>
		/// ������ɫ����
		/// </summary>
		void SetDarkThemeColors();
	};
}