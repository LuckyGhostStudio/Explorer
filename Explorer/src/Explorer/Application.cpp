#include "exppch.h"	//Ԥ����ͷ�ļ�
#include "Application.h"

#include "Explorer/Log.h"

#include "glad/glad.h"

namespace Explorer
{
#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)		//�󶨺���x ���غ�������

	Application* Application::Instance = nullptr;	//����

	Application::Application()
	{
		EXP_CORE_ASSERT(!Instance, "Application already exisit!");	//Application�Ѵ���
		Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());	//��������
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));		//���ûص�����
	}

	Application::~Application()
	{

	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* layer)
	{
		m_LayerStack.PushOverlay(layer);
		layer->OnAttach();
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);	//�¼�������
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));	//���� ���ڹر��¼�

		EXP_CORE_TRACE("{0}", e);	//�¼�������־

		//��������±�����ջ
		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();) {
			(*--it)->OnEvent(e);		//���ȡ�������¼�
			if (e.GetHandled()) break;	//�¼��Ѵ��� ����
		}
	}

	void Application::Run()
	{
		while (m_Running) {
			glClearColor(0.3, 0.5, 0.8, 1.0);
			glClear(GL_COLOR_BUFFER_BIT);

			//���²�ջ�����в�
			for (Layer* layer : m_LayerStack) {
				layer->OnUpdate();
			}

			m_Window->OnUpdate();		//��OnUpdate����ѯ������յ��¼�
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;	//��������
		return true;
	}
}