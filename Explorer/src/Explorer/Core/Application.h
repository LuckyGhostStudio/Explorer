#pragma once

#include "Core.h"

#include "Explorer/Core/Window.h"
#include "Explorer/Core/LayerStack.h"
#include "Explorer/Events/Event.h"
#include "Explorer/Events/ApplicationEvent.h"

#include "Explorer/Core/DeltaTime.h"

#include "Explorer/ImGui/ImGuiLayer.h"

namespace Explorer
{
	/// <summary>
	/// Ӧ�ó����ࣺ�������
	/// </summary>
	class Application
	{
	private:
		static Application* Instance;		//��̬ʵ��

		std::unique_ptr<Window> m_Window;	//����ָ��
		ImGuiLayer* m_ImGuiLayer;			//ImGui��ָ��

		bool m_Running = true;				//�Ƿ���������
		bool m_Minimized = false;			//�Ƿ���С��
		LayerStack m_LayerStack;			//��ջ

		float m_LastFrameTime = 0.0f;		//��һ֡ʱ��

		/// <summary>
		/// ���ڹرջص�����
		/// </summary>
		/// <param name="e">���ڹر��¼�</param>
		/// <returns>�Ƿ��ѹر�</returns>
		bool OnWindowClose(WindowCloseEvent& e);

		/// <summary>
		/// ��������ʱ����
		/// </summary>
		/// <param name="e">���������¼�</param>
		/// <returns>�¼�������</returns>
		bool OnWindowResize(WindowResizeEvent& e);
	public:
		Application();
		virtual ~Application();

		/// <summary>
		/// ����
		/// </summary>
		void Run();

		/// <summary>
		/// �رճ���
		/// </summary>
		void Close();

		/// <summary>
		/// �¼��ص�����
		/// </summary>
		/// <param name="e">�¼�</param>
		void OnEvent(Event& e);

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