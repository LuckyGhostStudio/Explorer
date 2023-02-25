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
		virtual void OnAttach() override;

		/// <summary>
		/// ���ò�Ӳ�ջ�Ƴ�ʱ����
		/// </summary>
		virtual void OnDetach() override;

		/// <summary>
		/// ��ʼ������Ⱦ
		/// </summary>
		void Begin();

		/// <summary>
		/// ����������Ⱦ
		/// </summary>
		void End();
	};
}