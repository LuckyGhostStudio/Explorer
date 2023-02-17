#pragma once

#include "Core.h"

#include "Windows/Window.h"
#include "Explorer/LayerStack.h"
#include "Explorer/Events/Event.h"
#include "Explorer/Events/ApplicationEvent.h"

#include "Explorer/ImGui/ImGuiLayer.h"

#include "Explorer/Renderer/Shader.h"
#include "Explorer/Renderer/Buffer.h"
#include "Explorer/Renderer/VertexArray.h"

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
		ImGuiLayer* m_ImGuiLayer;			//ImGui��ָ��

		bool m_Running = true;				//�Ƿ���������
		LayerStack m_LayerStack;			//��ջ

		std::shared_ptr<Shader> m_Shader;				//��ɫ��
		std::shared_ptr<VertexArray> m_VertexArray;		//��������

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
		/// ����
		/// </summary>
		void Run();

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