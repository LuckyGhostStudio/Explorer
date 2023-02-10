#pragma once

#include "Core.h"
#include "Events/Event.h"

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
	public:
		Application();
		virtual ~Application();

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