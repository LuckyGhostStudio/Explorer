#pragma once

#include "Explorer/Layer.h"

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

		/// <summary>
		/// ��갴ť����
		/// </summary>
		/// <param name="event">��갴ť�����¼�</param>
		bool OnMouseButtonPressedEvent(MouseButtonPressedEvent& event);

		/// <summary>
		/// ��갴ţ̌��
		/// </summary>
		/// <param name="event">��갴ţ̌���¼�</param>
		bool OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& event);

		/// <summary>
		/// ����ƶ�
		/// </summary>
		/// <param name="event">����ƶ��¼�</param>
		bool OnMouseMovedEvent(MouseMovedEvent& event);

		/// <summary>
		/// �����ֹ���
		/// </summary>
		/// <param name="event">�����ֹ����¼�</param>
		bool OnMouseScrolledEvent(MouseScrolledEvent& event);

		/// <summary>
		/// ���̰�������
		/// </summary>
		/// <param name="event">���̰��������¼�</param>
		bool OnKeyPressedEvent(KeyPressedEvent& event);

		/// <summary>
		/// ���̰���̧��
		/// </summary>
		/// <param name="event">���̰���̧���¼�</param>
		bool OnKeyReleasedEvent(KeyReleasedEvent& event);

		/// <summary>
		/// �������밴���ַ�
		/// </summary>
		/// <param name="event">���������ַ��¼�</param>
		/// <returns></returns>
		bool OnKeyTypedEvent(KeyTypedEvent& event);

		/// <summary>
		/// ���ڴ�С�ı�
		/// </summary>
		/// <param name="event">���ڴ�С�ı��¼�</param>
		bool OnWindowResizeEvent(WindowResizeEvent& event);
	};
}