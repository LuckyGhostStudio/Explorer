#pragma once

#include "Core.h"
#include "Events/Event.h"
#include "Events/ApplicationEvent.h"
#include "Windows/Window.h"

namespace Explorer
{
	/// <summary>
	/// Ӧ�ó����ࣺ�������
	/// </summary>
	class EXPLORER_API Application
	{
	private:
		std::unique_ptr<Window> m_Window;	//����ָ��
		bool m_Running = true;				//�Ƿ���������

		/// <summary>
		/// ���ڹرջص�����
		/// </summary>
		/// <param name="e">���ڹر��¼�</param>
		/// <returns>�Ƿ��ѹر�</returns>
		bool OnWindowClose(WindowCloseEvent& e);
	public:
		Application();
		virtual ~Application();

		/// <summary>
		/// �¼��ص�����
		/// </summary>
		/// <param name="e">�¼�</param>
		void OnEvent(Event& e);

		/// <summary>
		/// ����
		/// </summary>
		void Run();
	};

	/// <summary>
	/// ����Ӧ�ó�����SandboxApp����
	/// </summary>
	/// <returns>Ӧ�ó���ָ��</returns>
	Application* CreateApplication();
}