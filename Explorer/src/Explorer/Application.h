#pragma once

#include "Core.h"

#include "Windows/Window.h"
#include "Explorer/LayerStack.h"
#include "Explorer/Events/Event.h"
#include "Explorer/Events/ApplicationEvent.h"

namespace Explorer
{
	/// <summary>
	/// Ӧ�ó����ࣺ�������
	/// </summary>
	class EXPLORER_API Application
	{
	private:
		static Application* Instance;		//��̬ʵ��

		std::unique_ptr<Window> m_Window;	//����ָ��
		bool m_Running = true;				//�Ƿ���������
		LayerStack m_LayerStack;	

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

		/// <summary>
		/// �����ͨ�㵽��ջ
		/// </summary>
		/// <param name="layer">��</param>
		void PushLayer(Layer* layer);

		/// <summary>
		/// ��Ӹ��ǲ㵽��ջ
		/// </summary>
		/// <param name="layer">��</param>
		void PushOverlay(Layer* layer);

		/// <summary>
		/// ����Application��ʵ��
		/// </summary>
		/// <returns>ʵ��</returns>
		inline static Application& GetInstance() { return *Instance; }

		/// <summary>
		/// ����Application�Ĵ���
		/// </summary>
		/// <returns></returns>
		inline Window& GetWindow() { return *m_Window; }
	};

	/// <summary>
	/// ����Ӧ�ó�����SandboxApp����
	/// </summary>
	/// <returns>Ӧ�ó���ָ��</returns>
	Application* CreateApplication();
}